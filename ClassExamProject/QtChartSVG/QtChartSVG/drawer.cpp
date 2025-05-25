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
	m_result->setPen(m_params->getPen());
	m_result->setBrush(m_params->getBrushColor());
	m_result->setRect(m_rect);

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
	int penwidth = m_params->getPenwidth();
	QSize spacesize = m_params->getSpacesize();

	double scalex = spacesize.width() / (rect.width() + penwidth);
	double scaley = spacesize.height() / (rect.height() + penwidth);
	double scale = m_params->getScale();

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
		|| params->getType() != myqtsvg::ShapeType::Circle)
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

	QPointF targetpoint = calcuRealCenter(m_params->getCenter(), m_params->getCenterHOffset(), m_params->getCenterVOffset());
	QTransform translatetransform = calcuTranslateTransform(center, targetpoint);
	m_circle = m_circle * translatetransform;

	m_scale = calcuscaleFactor(m_circle.boundingRect());
	QTransform scaletransform = calcuscaleTransform(targetpoint);
	m_circle = m_circle * scaletransform;
}

void DiagramDrawerCircle::draw(QPainter& painter)
{
	build();

	QPen pen = m_params->getPen();
	double realpenwidth = pen.widthF() / m_scale;
	pen.setWidthF(realpenwidth);

	painter.setPen(pen);
	painter.setBrush(m_params->getBrushColor());

	painter.translate(calcuRealCenter(m_params->getCenter(), m_params->getCenterHOffset(), m_params->getCenterVOffset()));
	painter.rotate(m_params->getRotate());
	if (m_scale < 1e-6)
		throw std::runtime_error("error");
	painter.scale(m_scale, m_scale);

	double radio = m_params->getRadio();
	QSizeF size = QSizeF(m_initheight * radio, m_initheight);
	painter.drawEllipse(QPointF(0, 0), size.width() / 2, size.height() / 2);
	painter.resetTransform();

}

std::shared_ptr<DrawResult> DiagramDrawerCircle::getResult()
{
	build();
	QPen pen = m_params->getPen();
	int realpenwidth = static_cast<int>(pen.width() / m_scale);
	pen.setWidth(realpenwidth);
	m_result->setPen(pen);
	m_result->setBrush(m_params->getBrushColor());
	m_result->setCircle(m_circle);
	return m_result;
}

QPolygonF DiagramDrawerCircle::calcuBasicalCircle()
{
	qreal height = m_initheight;
	double radio = m_params->getRadio();
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
	rotatetransform.rotate(m_params->getRotate());
	rotatetransform.translate(-x, -y);

	return rotatetransform;
}

qreal DiagramDrawerCircle::calcuscaleFactor(QRectF bound)
{
	int penwidth = m_params->getPenwidth();
	QSize spacesize = m_params->getSpacesize();
	qreal scalex = spacesize.width() / (bound.width() + penwidth);
	qreal scaley = spacesize.height() / (bound.height() + penwidth);

	qreal scale = qMin(scalex, scaley) * m_params->getScale();

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
	if (params == nullptr || params.get() == nullptr || params->getType() != myqtsvg::ShapeType::Triangle)
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
	painter.setPen(m_params->getPen());
	painter.setBrush(m_params->getBrushColor());
	painter.drawPolygon(m_triangle);
}

std::shared_ptr<DrawResult> DiagramDrawerTriangle::getResult()
{
	build();
	m_result->setPen(m_params->getPen());
	m_result->setBrush(m_params->getBrushColor());
	m_result->setTriangle(m_triangle);
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

	auto radios = m_params->getRadios();
	int bottom = radios.getBottom();
	int left = radios.getLeft();
	int right = radios.getRight();

	QPointF bottomleft = QPointF(0, 0);
	QPointF bottomright = QPointF(bottom, 0);
	double constheta = (left * left + bottom * bottom - right * right) * 1.0 / (2 * left * bottom) * 1.0;
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

	auto radios = m_params->getRadios();
	int bottom = radios.getBottom();
	int left = radios.getLeft();
	int right = radios.getRight();
	qreal targetAngle = m_params->getRotate() * 1.0;

	double cosA = (left * left + bottom * bottom - right * right) / (2.0 * left * bottom);
	double sinA = std::sqrt(1 - cosA * cosA);
	QPointF C(left * cosA, -left * sinA); 

	QPointF A(0, 0);
	QPointF B(bottom, 0);
	QPointF D = C; 

	QPointF center = (A + B + D) / 3.0;

	QPointF direction;
	DiagramDrawParamsTriangle::EdgeType edgetype = m_params->getEdgeType();

	switch (edgetype)
	{
	case DiagramDrawParamsTriangle::EdgeType::Bottom:
		direction = B - A; 
		break;
	case DiagramDrawParamsTriangle::EdgeType::Left:
		direction = D - A; 
		break;
	case DiagramDrawParamsTriangle::EdgeType::Right:
		direction = D - B; 
		break;
	default:
		throw std::runtime_error("error");
	}
	double currentAngle = std::atan2(direction.y(), direction.x());
	double currentAngleDegrees = qRadiansToDegrees(currentAngle);
	double rotationAngle = targetAngle - currentAngleDegrees;

	QTransform transform;
	transform.translate(center.x(), center.y());    
	transform.rotate(rotationAngle);                
	transform.translate(-center.x(), -center.y());  

	return transform;
}

QTransform DiagramDrawerTriangle::calcuTranslateTransfrom(QPointF trianglecenter)
{
	QTransform translateTransform;

	QPoint realcenter = calcuRealCenter(m_params->getCenter(), m_params->getCenterHOffset(), m_params->getCenterVOffset());
	QPointF newpoint = realcenter - trianglecenter;
	translateTransform.translate(newpoint.x(), newpoint.y());
	return translateTransform;
}

QTransform DiagramDrawerTriangle::calcuScaleTransform(QRectF trianglerect)
{
	QTransform scaleTransform;
	int penwidth = m_params->getPenwidth();
	QSize spacesize = m_params->getSpacesize();

	qreal scalex = spacesize.width() / (trianglerect.width() + penwidth);
	qreal scaley = spacesize.height() / (trianglerect.height() + penwidth);
	qreal scale = qMin(scalex, scaley) * m_params->getScale();

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
	if (params == nullptr || params.get() == nullptr || params->getType() != myqtsvg::ShapeType::Line)
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
	painter.setPen(m_params->getPen());
	painter.setBrush(m_params->getBrushColor());
	painter.drawLine(m_line);
}

std::shared_ptr<DrawResult> DiagramDrawerLine::getResult()
{
	build();
	m_result->setPen(m_params->getPen());
	m_result->setBrush(m_params->getBrushColor());
	m_result->setLine(m_line);
	return m_result;
}

QLineF DiagramDrawerLine::calcuLine()
{
	int penwidth = m_params->getPenwidth();
	QSize spacesize = m_params->getSpacesize();
	double halfwidth = (spacesize.width() - penwidth) / 2.0;
	double halfheight = (spacesize.height() - penwidth) / 2.0;

	int angle = m_params->getRotate();
	double radians = qDegreesToRadians(static_cast<double>(angle));

	double dx = std::cos(radians);
	double dy = std::sin(radians);
	if (dx == 0 && dy == 0) {
		throw std::runtime_error("Direction vector is undefined.");
	}

	double tWidth = (dx != 0) ? std::abs(halfwidth / dx) : std::numeric_limits<double>::max();
	double tHeight = (dy != 0) ? std::abs(halfheight / dy) : std::numeric_limits<double>::max();
	double t = std::min(tWidth, tHeight) * m_params->getScale();

	QPoint realcenter = calcuRealCenter(m_params->getCenter(), m_params->getCenterHOffset(), m_params->getCenterVOffset());
	QPointF start = realcenter - QPointF(t * dx, t * dy);
	QPointF end = realcenter + QPointF(t * dx, t * dy);

	return QLineF(start, end);
}

DiagramDrawerMouse::DiagramDrawerMouse(std::shared_ptr<IDidgramDrawParams> params)
	:m_params(nullptr)
	, m_result(std::make_shared<DrawResultMouse>())
{
	if (params == nullptr || params.get() == nullptr || params->getType() != myqtsvg::ShapeType::Mouse)
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
	if (m_params->getIsDrawInCanvas())
	{
		painter.save();
		painter.setPen(m_params->getPen());
		painter.translate(m_params->getCenterHOffset(), m_params->getCenterVOffset());
		painter.drawPath(*m_params->getPaht());
		painter.restore();
	}
	else
	{
		QString text = "笔";
		QFont font;
		int basesize = qMin(m_params->getSpacesize().width(), m_params->getSpacesize().height()) / 2;
		font.setPointSize(basesize);
		painter.setFont(font);
		int x = m_params->getCenter().x() - m_params->getSpacesize().width() / 2;
		int y = m_params->getCenter().y() - m_params->getSpacesize().height() / 2;
		QRect rect(x, y, m_params->getSpacesize().width(), m_params->getSpacesize().height());

		painter.drawText(rect, Qt::AlignCenter, text);
	}


}

std::shared_ptr<DrawResult> DiagramDrawerMouse::getResult()
{
	m_result->setPen(m_params->getPen());
	m_result->setBrush(m_params->getBrushColor());
	auto path = m_params->getPaht();
	if (path == nullptr)
		path = std::make_shared<QPainterPath>();
	path->translate(m_params->getCenterHOffset(), m_params->getCenterVOffset());
	m_result->setPath(*path);

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
	painter.setPen(m_params->getPen());
	painter.setFont(m_params->getFont());
	if (!m_params->getIsDrawInCanvas())
	{
		QString text = "文本";
		QFontMetrics metrics(m_params->getFont());
		int width = metrics.horizontalAdvance(text) + 10;
		int height = metrics.height() + 5;
		int x = m_params->getCenter().x() - width / 2;
		int y = m_params->getCenter().y() - height / 2;
		QRect rect(x, y, width, height);

		painter.drawText(rect, Qt::AlignCenter, text);
	}
	else
	{
		if (m_params->getTextEdit() == nullptr)
			throw std::runtime_error("error");
		//auto newpos = m_params->m_textedit->pos() + QPoint(m_params->m_centerHoffset, m_params->m_centerVoffset);
		//m_params->m_textedit->move(newpos);

		auto textedit = m_params->getTextEdit();
		textedit->setFont(m_params->getFont());
		textedit->setTextColor(m_params->getPenColor());
		textedit->adjustsize();
		QSize size = textedit->size();
		auto newcenter = m_params->getCenter() + QPoint(m_params->getCenterHOffset(), m_params->getCenterVOffset());
		QPoint topleft(newcenter.x() - size.width() / 2, newcenter.y() - size.height() / 2);
		textedit->move(topleft);

		auto rect = textedit->geometry();
		auto text = textedit->text();

		painter.drawText(rect, Qt::AlignCenter, text);
	}
}

std::shared_ptr<DrawResult> DiagramDrawerText::getResult()
{
	m_result->setPen(m_params->getPen());
	m_result->setFont(m_params->getFont());
	m_result->setRect(m_params->getTextEdit()->geometry());
	m_result->setText(m_params->getTextEdit()->text());
	return m_result;
}

DiagramDrawerChoose::DiagramDrawerChoose(std::shared_ptr<IDidgramDrawParams> params)
{
	if (params == nullptr || params.get() == nullptr || params->getType() != myqtsvg::ShapeType::choose)
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
	int basesize = qMin(m_params->getSpacesize().width(), m_params->getSpacesize().height()) / 2;
	font.setPointSize(basesize);
	painter.setFont(font);
	int x = m_params->getCenter().x() - m_params->getSpacesize().width() / 2;
	int y = m_params->getCenter().y() - m_params->getSpacesize().height() / 2;
	QRect rect(x, y, m_params->getSpacesize().width(), m_params->getSpacesize().height());

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
	myqtsvg::ShapeType type = params->getType();
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
			})
		.addDrawerCreator(myqtsvg::ShapeType::Pentagon, [](std::shared_ptr<IDidgramDrawParams> params) {
		return std::make_shared<DiagramDrawerPentagon>(params);
			})
		.addDrawerCreator(myqtsvg::ShapeType::Hexagon, [](std::shared_ptr<IDidgramDrawParams> params) {
		return std::make_shared<DiagramdrawerHexagon>(params);
			})
		.addDrawerCreator(myqtsvg::ShapeType::Star, [](std::shared_ptr<IDidgramDrawParams> params) {
		return std::make_shared<DiagramDrawerStar>(params);
			});
}

DiagramDrawerPentagon::DiagramDrawerPentagon(std::shared_ptr<IDidgramDrawParams> params)
	:m_params(nullptr)
	, m_result(std::make_shared<DrawResultPentagon>())
{
	if (params == nullptr)
		throw std::runtime_error("error");
	auto p = std::dynamic_pointer_cast<DiagramDrawParamsPentagon>(params);
	if (p == nullptr)
		throw std::runtime_error("error");
	m_params = p;
}

void DiagramDrawerPentagon::build()
{
	QPoint center = calcuRealCenter(m_params->getCenter(), m_params->getCenterHOffset(), m_params->getCenterVOffset());
	int cx = center.x();
	int cy = center.y();

	QSize spacesize = m_params->getSpacesize();
	qreal radius = qMin(spacesize.width(), spacesize.height()) / 2.0 * 0.9;

	// 创建多边形
	QPolygonF polygon;

	for (int i = 0; i < 5; ++i)
	{
		double angle = 2 * M_PI * i / 5 - M_PI / 2;
		qreal x = cx + radius * cos(angle);
		qreal y = cy + radius * sin(angle);
		polygon.append(QPointF(x, y));
	}

	QTransform transfrom;
	transfrom.translate(center.x(), center.y());
	transfrom.rotate(m_params->getRotate());
	transfrom.scale(m_params->getScale(), m_params->getScale());
	transfrom.translate(-center.x(), -center.y());
	polygon = polygon * transfrom;
	

	m_pentagon = polygon;
}

void DiagramDrawerPentagon::draw(QPainter& painter)
{
	build();
	painter.setPen(m_params->getPen());
	painter.setBrush(m_params->getBrushColor());
	painter.drawPolygon(m_pentagon);
}

std::shared_ptr<DrawResult> DiagramDrawerPentagon::getResult()
{
	build();
	m_result->setPen(m_params->getPen());
	m_result->setBrush(m_params->getBrushColor());
	m_result->setPentagon(m_pentagon);
	return m_result;
}

DiagramdrawerHexagon::DiagramdrawerHexagon(std::shared_ptr<IDidgramDrawParams> params)
	:m_params(nullptr)
	, m_result(std::make_shared<DrawResultHexagon>())
{
	if (params == nullptr)
		throw std::runtime_error("error");
	auto p = std::dynamic_pointer_cast<DiagramDrawParamsHexagon>(params);
	if (p == nullptr)
		throw std::runtime_error("error");
	m_params = p;
}

void DiagramdrawerHexagon::build()
{
	QPoint center = calcuRealCenter(m_params->getCenter(), m_params->getCenterHOffset(), m_params->getCenterVOffset());
	int cx = center.x();
	int cy = center.y();

	QSize spacesize = m_params->getSpacesize();
	qreal radius = qMin(spacesize.width(), spacesize.height()) / 2.0 * 0.9;

	QPolygonF polygon;

	for (int i = 0; i < 6; ++i)
	{
		double angle = 2 * M_PI * i / 6 - M_PI / 6;
		qreal x = cx + radius * cos(angle);
		qreal y = cy + radius * sin(angle);
		polygon.append(QPointF(x, y));
	}

	QTransform transfrom;
	transfrom.translate(center.x(), center.y());
	transfrom.rotate(m_params->getRotate());
	transfrom.scale(m_params->getScale(), m_params->getScale());
	transfrom.translate(-center.x(), -center.y());
	polygon = polygon * transfrom;

	m_hexagon = polygon;
}

void DiagramdrawerHexagon::draw(QPainter& painter)
{
	build();
	painter.setPen(m_params->getPen());
	painter.setBrush(m_params->getBrushColor());
	painter.drawPolygon(m_hexagon);
}

std::shared_ptr<DrawResult> DiagramdrawerHexagon::getResult()
{
	build();
	m_result->setPen(m_params->getPen());
	m_result->setBrush(m_params->getBrushColor());
	m_result->setHexagon(m_hexagon);
	return m_result;
}

DiagramDrawerStar::DiagramDrawerStar(std::shared_ptr<IDidgramDrawParams> params)
	:m_params(nullptr)
	, m_result(std::make_shared<DrawResultStar>())
{
	if (params == nullptr)
		throw std::runtime_error("error");
	auto p = std::dynamic_pointer_cast<DiagramDrawParamsStar>(params);
	if (p == nullptr)
		throw std::runtime_error("error");
	m_params = p;
}

void DiagramDrawerStar::build()
{
	QPoint center = calcuRealCenter(m_params->getCenter(), m_params->getCenterHOffset(), m_params->getCenterVOffset());
	int cx = center.x();
	int cy = center.y();

	QSize spacesize = m_params->getSpacesize();
	double radius = qMin(spacesize.width(), spacesize.height()) / 2.0 * 0.9;

	QPolygonF star;

	double innerRadius = radius * 0.382;

	for (int i = 0; i < 10; ++i) 
	{
		double angle = 2 * M_PI * i / 10 - M_PI / 2;

		qreal currentRadius = (i % 2 == 0) ? radius : innerRadius;

		qreal x = cx + currentRadius * cos(angle);
		qreal y = cy + currentRadius * sin(angle);

		star.append(QPointF(x, y));
	}

	QTransform transfrom;
	transfrom.translate(center.x(), center.y());
	transfrom.rotate(m_params->getRotate());
	transfrom.scale(m_params->getScale(), m_params->getScale());
	transfrom.translate(-center.x(), -center.y());
	star = star * transfrom;

	m_star = star;
}

void DiagramDrawerStar::draw(QPainter& painter)
{
	build();
	painter.setPen(m_params->getPen());
	painter.setBrush(m_params->getBrushColor());
	painter.drawPolygon(m_star);
}

std::shared_ptr<DrawResult> DiagramDrawerStar::getResult()
{
	build();
	m_result->setPen(m_params->getPen());
	m_result->setBrush(m_params->getBrushColor());
	m_result->setStar(m_star);
	return m_result;
}
