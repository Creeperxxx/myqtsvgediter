#include "drawer.h"
#include <qmath.h>

QPoint IDiagramDrawer::calcuRealCenter(QPoint center, int hmove, int vmove)
{
	return center + QPoint(hmove, vmove);
}

DiagramDrawerRect::DiagramDrawerRect(std::shared_ptr<IDidgramDrawParams> params)
	:m_params(nullptr)
	, m_result(std::make_shared<DrawResultRect>())
{
	if (params == nullptr || params.get() == nullptr)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsRect>(params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	m_params = castparams;
}

void DiagramDrawerRect::build()
{
	m_rect = calcuRect();
}

void DiagramDrawerRect::draw(QPainter& painter)
{
	build();

	QPen pen = m_params->getPen();
	QBrush brush = m_params->getBrushColor();
	painter.setPen(pen);
	painter.setBrush(brush);
	painter.drawPolygon(m_rect);

}

std::shared_ptr<DrawResult> DiagramDrawerRect::getResult()
{
	build();
	m_result->m_painterpen = m_params->getPen();
	m_result->m_painterbrush = m_params->getBrushColor();
	m_result->m_rect = m_rect;
	return m_result;
}

QPolygonF DiagramDrawerRect::calcuRect()
{
	QPolygonF rect = calcuBasicalRect();
	rect = rect * calcuRotateTransform(rect.boundingRect().center());
	rect = rect * calcuTranslateTransform(rect.boundingRect().center());
	rect = rect * calcuScaleTransform(rect.boundingRect());
	return rect;
}

QPolygonF DiagramDrawerRect::calcuBasicalRect()
{
	double height = 50;

	double radio = m_params->getRadio();
	if (radio < 1e-10)
		throw std::runtime_error("error");
	double width = height * radio;


	QPointF lefttop(0.0, 0.0);
	QPointF leftbottom(0.0, height - 1);
	QPointF righttop(width - 1, 0.0);
	QPointF rightbottom(width - 1, height - 1);

	QPolygonF basicalrect;
	basicalrect << lefttop << leftbottom << rightbottom << righttop;
	return basicalrect;
}

QTransform DiagramDrawerRect::calcuRotateTransform(QPointF center)
{
	QTransform rotatetransform;
	qreal x = center.x();
	qreal y = center.y();
	rotatetransform.translate(x, y);
	rotatetransform.rotate(m_params->getRotate());
	rotatetransform.translate(-x, -y);
	return rotatetransform;
}

QTransform DiagramDrawerRect::calcuTranslateTransform(QPointF center)
{
	QTransform translateTransform;
	QPoint realcenter = calcuRealCenter(m_params->getCenter(), m_params->getCenterHOffset(), m_params->getCenterVOffset());

	QPointF finalpoint = realcenter - center;
	translateTransform.translate(finalpoint.x(), finalpoint.y());
	return translateTransform;
}

QTransform DiagramDrawerRect::calcuScaleTransform(QRectF rect)
{
	QTransform scaletransform;
	int penwidth = m_params->m_pen.width();
	QSize spacesize = m_params->m_spacesize;

	double scalex = spacesize.width() / (rect.width() + penwidth);
	double scaley = spacesize.height() / (rect.height() + penwidth);
	double scale = m_params->m_scale;

	double finalscale = qMin(scalex, scaley) * scale;
	if (finalscale < 1e-10)
		throw std::runtime_error("error");
	qreal x = rect.center().x();
	qreal y = rect.center().y();
	scaletransform.translate(x, y);
	scaletransform.scale(finalscale, finalscale);
	scaletransform.translate(-x, -y);
	return scaletransform;
}

DiagramDrawerCircle::DiagramDrawerCircle(std::shared_ptr<IDidgramDrawParams> params)
	:m_params(nullptr)
	, m_result(std::make_shared<DrawResultCircle>())
	, m_scale(0)
	, m_initheight(10)
	, m_circle(QPolygonF())

{
	if (params == nullptr
		|| params.get() == nullptr
		|| params->m_type != myqtsvg::ShapeType::Circle)
		throw std::runtime_error("error");

	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsCircle>(params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	m_params = castparams;

}

void DiagramDrawerCircle::build()
{
	m_circle = calcuBasicalCircle();

	QPointF center = m_circle.boundingRect().center();
	QTransform rotatetransform = calcurotatetransform(center);
	m_circle = m_circle * rotatetransform;

	QPointF targetpoint = calcuRealCenter(m_params->m_center, m_params->m_centerHoffset, m_params->m_centerVoffset);
	QTransform translatetransform = calcuTranslateTransform(center, targetpoint);
	m_circle = m_circle * translatetransform;

	m_scale = calcuscaleFactor(m_circle.boundingRect());
	QTransform scaletransform = calcuscaleTransform(targetpoint);
	m_circle = m_circle * scaletransform;
}

void DiagramDrawerCircle::draw(QPainter& painter)
{
	build();

	QPen pen = m_params->m_pen;
	double realpenwidth = pen.widthF() / m_scale;
	pen.setWidthF(realpenwidth);

	painter.setPen(pen);
	painter.setBrush(m_params->m_brush);

	painter.translate(calcuRealCenter(m_params->m_center, m_params->m_centerHoffset, m_params->m_centerVoffset));
	painter.rotate(m_params->m_rotate);
	if (m_scale < 1e-6)
		throw std::runtime_error("error");
	painter.scale(m_scale, m_scale);

	double radio = m_params->m_radio;
	QSizeF size = QSizeF(m_initheight * radio, m_initheight);
	painter.drawEllipse(QPointF(0, 0), size.width() / 2, size.height() / 2);
	painter.resetTransform();

}

std::shared_ptr<DrawResult> DiagramDrawerCircle::getResult()
{
	build();
	QPen pen = m_params->m_pen;
	int realpenwidth = static_cast<int>(pen.width() / m_scale);
	pen.setWidth(realpenwidth);
	m_result->m_painterpen = pen;
	m_result->m_painterbrush = m_params->m_brush;
	m_result->m_circle = m_circle;
	return m_result;
}

QPolygonF DiagramDrawerCircle::calcuBasicalCircle()
{
	qreal height = m_initheight;
	double radio = m_params->m_radio;
	qreal width = height * radio;

	QPointF lefttop(0, 0);
	QPointF leftbottom(0, height - 1);
	QPointF righttop(width - 1, 0);
	QPointF rightbottom(width - 1, height - 1);

	QPolygonF circle;
	circle << lefttop << leftbottom << rightbottom << righttop;

	return circle;
}

QTransform DiagramDrawerCircle::calcurotatetransform(QPointF center)
{
	QTransform rotatetransform;
	qreal x = center.x();
	qreal y = center.y();

	rotatetransform.translate(x, y);
	rotatetransform.rotate(m_params->m_rotate);
	rotatetransform.translate(-x, -y);

	return rotatetransform;
}

qreal DiagramDrawerCircle::calcuscaleFactor(QRectF bound)
{
	int penwidth = m_params->m_pen.width();
	QSize spacesize = m_params->m_spacesize;
	qreal scalex = spacesize.width() / (bound.width() + penwidth);
	qreal scaley = spacesize.height() / (bound.height() + penwidth);

	qreal scale = qMin(scalex, scaley) * m_params->m_scale;

	return scale;
}

QTransform DiagramDrawerCircle::calcuscaleTransform(QPointF center)
{
	qreal x = center.x();
	qreal y = center.y();
	QTransform scaletransform;

	scaletransform.translate(x, y);
	scaletransform.scale(m_scale, m_scale);
	scaletransform.translate(-x, -y);

	return scaletransform;
}

QTransform DiagramDrawerCircle::calcuTranslateTransform(QPointF mycenter, QPointF targetpoint)
{
	QTransform translatetransform;

	QPointF move = targetpoint - mycenter;
	translatetransform.translate(move.x(), move.y());
	return translatetransform;
}

DiagramDrawerTriangle::DiagramDrawerTriangle(std::shared_ptr<IDidgramDrawParams> params)
	:m_params(nullptr)
	, m_result(std::make_shared<DrawResultTriangle>())
	, m_triangle(QPolygonF())
{
	if (params == nullptr || params.get() == nullptr || params->m_type != myqtsvg::ShapeType::Triangle)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	m_params = castparams;
}

void DiagramDrawerTriangle::build()
{
	m_triangle = calcuTriangle();
}

void DiagramDrawerTriangle::draw(QPainter& painter)
{
	build();
	painter.setPen(m_params->m_pen);
	painter.setBrush(m_params->m_brush);
	painter.drawPolygon(m_triangle);
}

std::shared_ptr<DrawResult> DiagramDrawerTriangle::getResult()
{
	build();
	m_result->m_painterpen = m_params->m_pen;
	m_result->m_painterbrush = m_params->m_brush;
	m_result->m_triangle = m_triangle;
	return m_result;
}

QPolygonF DiagramDrawerTriangle::calcuTriangle()
{
	QPolygonF triangle = calcuBasicalTriangle();

	QTransform rotateTransform = calcuRotateTransform();
	triangle = triangle * rotateTransform;

	QTransform translateTransform = calcuTranslateTransfrom(triangle.boundingRect().center());
	triangle = triangle * translateTransform;

	QTransform scaleTransform = calcuScaleTransform(triangle.boundingRect());
	triangle = triangle * scaleTransform;
	return triangle;
}

QPolygonF DiagramDrawerTriangle::calcuBasicalTriangle()
{

	auto radios = m_params->m_sideRadios;
	double bottom = radios.m_bottom;
	double left = radios.m_left;
	double right = radios.m_right;

	QPointF bottomleft = QPointF(0, 0);
	QPointF bottomright = QPointF(bottom, 0);
	qreal constheta = (left * left + bottom * bottom - right * right) / (2 * left * bottom);
	constheta = qBound(-1.0, constheta, 1.0);
	qreal theta = acos(constheta);
	qreal x = left * cos(theta);
	qreal y = left * sin(theta);
	QPointF toppoint(x, -y);

	QPolygonF triangle;
	triangle << bottomleft << bottomright << toppoint;
	return triangle;
}

QTransform DiagramDrawerTriangle::calcuRotateTransform()
{
	auto radios = m_params->m_sideRadios;
	double bottom = radios.m_bottom;
	double left = radios.m_left;
	double right = radios.m_right;

	// 计算顶点C的坐标
	double cosA = (left * left + bottom * bottom - right * right) / (2 * left * bottom);
	double sinA = std::sqrt(1 - cosA * cosA);
	QPointF C(left * cosA, -left * sinA);

	qreal angle = m_params->m_rotate;

	DiagramDrawParamsTriangle::EdgeType edgetype = m_params->m_edgetype;

	// 计算各边的方向
	double targetsin = std::sin(qDegreesToRadians(angle));
	double targetcos = std::cos(qDegreesToRadians(angle));

	double newsin = 0;
	double newcos = 1; // 默认为无旋转

	switch (edgetype)
	{
	case DiagramDrawParamsTriangle::EdgeType::Bottom:
	{ // 边AB (A→B)
		double cosAB = 1.0;  // 方向 (a,0)
		double sinAB = 0.0;
		newsin = targetsin * cosAB - targetcos * sinAB;
		newcos = targetcos * cosAB + targetsin * sinAB;
		break;
	}
	case DiagramDrawParamsTriangle::EdgeType::Left:
	{ // 边AC (A→C)
		double cosAC = cosA;
		double sinAC = sinA;
		newsin = targetsin * cosAC - targetcos * sinAC;
		newcos = targetcos * cosAC + targetsin * sinAC;
		break;
	}
	case DiagramDrawParamsTriangle::EdgeType::Right:
	{ // 边BC (B→C)
		double bcX = C.x() - bottom; // B→C 的向量
		double bcY = C.y();
		double lengthBC = qSqrt(bcX * bcX + bcY * bcY);
		double cosBC = bcX / lengthBC;
		double sinBC = bcY / lengthBC;
		newsin = targetsin * cosBC - targetcos * sinBC;
		newcos = targetcos * cosBC + targetsin * sinBC;
		break;
	}
	default:
		throw std::runtime_error("Invalid edge type");
		break; // 无效输入，保持无旋转
	}

	QTransform transform;
	transform.setMatrix(newcos, -newsin, 0,
		newsin, newcos, 0,
		0, 0, 1);
	return transform;
}

QTransform DiagramDrawerTriangle::calcuTranslateTransfrom(QPointF trianglecenter)
{
	QTransform translateTransform;

	QPoint realcenter = calcuRealCenter(m_params->m_center, m_params->m_centerHoffset, m_params->m_centerVoffset);
	QPointF newpoint = realcenter - trianglecenter;
	translateTransform.translate(newpoint.x(), newpoint.y());
	return translateTransform;
}

QTransform DiagramDrawerTriangle::calcuScaleTransform(QRectF trianglerect)
{
	QTransform scaleTransform;
	int penwidth = m_params->m_pen.width();
	QSize spacesize = m_params->m_spacesize;

	qreal scalex = spacesize.width() / (trianglerect.width() + penwidth);
	qreal scaley = spacesize.height() / (trianglerect.height() + penwidth);
	qreal scale = qMin(scalex, scaley) * m_params->m_scale;

	qreal x = trianglerect.center().x();
	qreal y = trianglerect.center().y();
	scaleTransform.translate(x, y);
	scaleTransform.scale(scale, scale);
	scaleTransform.translate(-x, -y);

	return scaleTransform;
}

DiagramDrawerLine::DiagramDrawerLine(std::shared_ptr<IDidgramDrawParams> params)
	:m_params(nullptr)
	, m_result(std::make_shared<DrawResultLine>())
	, m_line(QLineF())
{
	if (params == nullptr || params.get() == nullptr || params->m_type != myqtsvg::ShapeType::Line)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsLine>(params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	m_params = castparams;
}

void DiagramDrawerLine::build()
{
	m_line = calcuLine();

}

void DiagramDrawerLine::draw(QPainter& painter)
{
	build();
	painter.setPen(m_params->m_pen);
	painter.setBrush(m_params->m_brush);
	painter.drawLine(m_line);
}

std::shared_ptr<DrawResult> DiagramDrawerLine::getResult()
{
	build();
	m_result->m_painterpen = m_params->m_pen;
	m_result->m_painterbrush = m_params->m_brush;
	m_result->m_line = m_line;
	return m_result;
}

QLineF DiagramDrawerLine::calcuLine()
{
	int penwidth = m_params->m_pen.width();
	QSize spacesize = m_params->m_spacesize;
	double halfwidth = (spacesize.width() - penwidth) / 2.0;
	double halfheight = (spacesize.height() - penwidth) / 2.0;

	// Convert rotation angle to radians
	int angle = m_params->m_rotate;
	double radians = qDegreesToRadians(static_cast<double>(angle));

	// Calculate direction vector
	double dx = std::cos(radians);
	double dy = std::sin(radians);

	// Avoid division by zero and handle extreme angles
	if (dx == 0 && dy == 0) {
		throw std::runtime_error("Direction vector is undefined.");
	}

	// Calculate scaling factor t for line length based on space size
	double tWidth = (dx != 0) ? std::abs(halfwidth / dx) : std::numeric_limits<double>::max();
	double tHeight = (dy != 0) ? std::abs(halfheight / dy) : std::numeric_limits<double>::max();
	double t = std::min(tWidth, tHeight) * m_params->m_scale;

	QPoint realcenter = calcuRealCenter(m_params->m_center, m_params->m_centerHoffset, m_params->m_centerVoffset);
	QPointF start = realcenter - QPointF(t * dx, t * dy);
	QPointF end = realcenter + QPointF(t * dx, t * dy);

	return QLineF(start, end);
}

DiagramDrawerMouse::DiagramDrawerMouse(std::shared_ptr<IDidgramDrawParams> params)
	:m_params(nullptr)
	, m_result(std::make_shared<DrawResultMouse>())
{
	if (params == nullptr || params.get() == nullptr || params->m_type != myqtsvg::ShapeType::Mouse)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsMouse>(params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	m_params = castparams;
}

void DiagramDrawerMouse::build()
{

}

void DiagramDrawerMouse::draw(QPainter& painter)
{
	if (m_params->m_isdrawInHuabu)
	{
		painter.setPen(m_params->m_pen);
		painter.translate(m_params->m_centerHoffset, m_params->m_centerVoffset);
		painter.drawPath(*m_params->m_path);
		painter.resetTransform();
	}
	else
	{
		QString text = "笔";
		QFont font;
		int basesize = qMin(m_params->m_spacesize.width(), m_params->m_spacesize.height()) / 2;
		font.setPointSize(basesize);
		painter.setFont(font);
		int x = m_params->m_center.x() - m_params->m_spacesize.width() / 2;
		int y = m_params->m_center.y() - m_params->m_spacesize.height() / 2;
		QRect rect(x, y, m_params->m_spacesize.width(), m_params->m_spacesize.height());

		painter.drawText(rect, Qt::AlignCenter, text);
	}


}

std::shared_ptr<DrawResult> DiagramDrawerMouse::getResult()
{
	m_result->m_painterpen = m_params->m_pen;

	m_result->m_path = *m_params->m_path;
	m_result->m_path.translate(m_params->m_centerHoffset, m_params->m_centerVoffset);

	return m_result;
}

DiagramDrawerText::DiagramDrawerText(std::shared_ptr<IDidgramDrawParams> params)
	: m_result(std::make_shared<DrawResultText>())
{
	if (params == nullptr || params.get() == nullptr)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsText>(params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	m_params = castparams;
}

void DiagramDrawerText::build()
{
}

void DiagramDrawerText::draw(QPainter& painter)
{
	painter.setPen(m_params->m_pen);
	painter.setFont(m_params->m_font);
	if (!m_params->m_isdrawInHuabu)
	{
		QString text = "文本";
		QFontMetrics metrics(m_params->m_font);
		int width = metrics.horizontalAdvance(text) + 10;
		int height = metrics.height() + 5;
		int x = m_params->m_center.x() - width / 2;
		int y = m_params->m_center.y() - height / 2;
		QRect rect(x, y, width, height);

		painter.drawText(rect, Qt::AlignCenter, text);
	}
	else
	{
		if (m_params->m_textedit == nullptr)
			throw std::runtime_error("error");
		//auto newpos = m_params->m_textedit->pos() + QPoint(m_params->m_centerHoffset, m_params->m_centerVoffset);
		//m_params->m_textedit->move(newpos);

		m_params->m_textedit->setFont(m_params->m_font);
		m_params->m_textedit->setTextColor(m_params->m_pen.color());
		m_params->m_textedit->adjustsize();
		QSize size = m_params->m_textedit->size();
		auto newcenter = m_params->m_center + QPoint(m_params->m_centerHoffset, m_params->m_centerVoffset);
		QPoint topleft(newcenter.x() - size.width() / 2, newcenter.y() - size.height() / 2);
		m_params->m_textedit->move(topleft);

		auto rect = m_params->m_textedit->geometry();
		auto text = m_params->m_textedit->text();

		painter.drawText(rect, Qt::AlignCenter, text);
	}
}

std::shared_ptr<DrawResult> DiagramDrawerText::getResult()
{
	m_result->m_painterpen = m_params->m_pen;
	m_result->m_font = m_params->m_font;
	m_result->m_rect = m_params->m_textedit->geometry();
	m_result->m_text = m_params->m_textedit->text();
	return m_result;
}

DiagramDrawerChoose::DiagramDrawerChoose(std::shared_ptr<IDidgramDrawParams> params)
{
	if (params == nullptr || params.get() == nullptr || params->m_type != myqtsvg::ShapeType::choose)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsChoose>(params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	m_params = castparams;
}

void DiagramDrawerChoose::build()
{
}

void DiagramDrawerChoose::draw(QPainter& painter)
{
	QString text = "选择";
	QFont font;
	int basesize = qMin(m_params->m_spacesize.width(), m_params->m_spacesize.height()) / 2;
	font.setPointSize(basesize);
	painter.setFont(font);
	int x = m_params->m_center.x() - m_params->m_spacesize.width() / 2;
	int y = m_params->m_center.y() - m_params->m_spacesize.height() / 2;
	QRect rect(x, y, m_params->m_spacesize.width(), m_params->m_spacesize.height());

	painter.drawText(rect, Qt::AlignCenter, text);
}

std::shared_ptr<DrawResult> DiagramDrawerChoose::getResult()
{
	return nullptr;
}

DiagramDrawInterface& DiagramDrawInterface::getInstance()
{
	static DiagramDrawInterface instance;
	return instance;
}

DiagramDrawInterface& DiagramDrawInterface::addDrawerCreator(myqtsvg::ShapeType type, std::function<std::shared_ptr<IDiagramDrawer>(std::shared_ptr<IDidgramDrawParams>)> drawer)
{
	m_drawerMap[type] = drawer;
	return *this;
}

std::shared_ptr<IDiagramDrawer> DiagramDrawInterface::getDrawer(std::shared_ptr<IDidgramDrawParams> params)
{
	myqtsvg::ShapeType type = params->m_type;
	if (m_drawerMap.find(type) != m_drawerMap.end())
	{
		return m_drawerMap[type](params);
	}
	else
	{
		throw std::runtime_error("error");
	}

}

DiagramDrawInterface::DiagramDrawInterface()
{
	defaultinit();
}

void DiagramDrawInterface::defaultinit()
{
	addDrawerCreator(myqtsvg::ShapeType::Rect, [](std::shared_ptr<IDidgramDrawParams> params) {
		return std::make_shared<DiagramDrawerRect>(params);
		})
		.addDrawerCreator(myqtsvg::ShapeType::Circle, [](std::shared_ptr<IDidgramDrawParams> params) {
		return std::make_shared<DiagramDrawerCircle>(params);
			})
		.addDrawerCreator(myqtsvg::ShapeType::Triangle, [](std::shared_ptr<IDidgramDrawParams> params) {
		return std::make_shared<DiagramDrawerTriangle>(params);
			})
		.addDrawerCreator(myqtsvg::ShapeType::Line, [](std::shared_ptr<IDidgramDrawParams> params) {
		return std::make_shared<DiagramDrawerLine>(params);
			})
		.addDrawerCreator(myqtsvg::ShapeType::choose, [](std::shared_ptr<IDidgramDrawParams> params) {
		return std::make_shared<DiagramDrawerChoose>(params);
			})
		.addDrawerCreator(myqtsvg::ShapeType::Mouse, [](std::shared_ptr<IDidgramDrawParams> params) {
		return std::make_shared<DiagramDrawerMouse>(params);
			})
		.addDrawerCreator(myqtsvg::ShapeType::Text, [](std::shared_ptr<IDidgramDrawParams> params) {
		return std::make_shared<DiagramDrawerText>(params);
			});
}


