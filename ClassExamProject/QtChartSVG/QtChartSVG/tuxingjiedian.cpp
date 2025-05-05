#include "tuxingjiedian.h"
#include <cmath>
#include <qmath.h>


DiagramDrawerRect::DiagramDrawerRect(std::shared_ptr<IDidgramDrawParams> params)
	:m_params(nullptr)
	, m_result(std::make_shared<DrawResultRect>())
	, m_rect(QPolygonF())
{
	if (params == nullptr)
		throw std::runtime_error("error");

	m_params = std::dynamic_pointer_cast<DiagramDrawParamsRect>(params);
	if(m_params == nullptr)
		throw std::runtime_error("error");
	build();

}


void DiagramDrawerRect::build()
{
	if (m_params == nullptr)
		throw std::runtime_error("error");
	if (m_params->m_paramChanged)
	{
		m_rect = calcuRect();

		m_params->m_paramChanged = false;
	}
}

void DiagramDrawerRect::draw(QPainter& painter)
{
	build();

	painter.setPen(m_params->m_pen);
	painter.setBrush(m_params->m_brush);
	painter.drawPolygon(m_rect);

}

std::shared_ptr<DrawResult> DiagramDrawerRect::getResult()
{
	build();
	m_result->m_painterpen = m_params->m_pen;
	m_result->m_painterbrush = m_params->m_brush;
	m_result->m_rect = m_rect;
	return m_result;
}

std::shared_ptr<IDidgramDrawParams> DiagramDrawerRect::getParams()
{
	return m_params;
}


QPolygonF DiagramDrawerCircle::calcuBasicalCircle()
{
	qreal height = 10;
	qreal width = height * m_params->m_boundingrectradio;

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
	qreal penwidth = m_params->m_pen.widthF();
	qreal scalex = m_params->m_spacesize.width() / (bound.width() + penwidth);
	qreal scaley = m_params->m_spacesize.height() / (bound.height() + penwidth);

	qreal scale = qMin(scalex, scaley) * m_params->m_scale;

	return scale;
}

QTransform DiagramDrawerCircle::calcuscaleTransform()
{
	qreal x = m_params->m_center.x();
	qreal y = m_params->m_center.y();
	QTransform scaletransform;
	scaletransform.translate(x, y);
	scaletransform.scale(m_scale, m_scale);
	scaletransform.translate(-x, -y);

	return scaletransform;
}

QTransform DiagramDrawerCircle::calcuTranslateTransform(QPointF mycenter)
{
	QTransform translatetransform;
	QPointF move = m_params->m_center - mycenter;
	translatetransform.translate(move.x(), move.y());
	return translatetransform;
}




DiagramDrawerCircle::DiagramDrawerCircle(std::shared_ptr<IDidgramDrawParams> params)
	:m_params(nullptr)
	, m_result(std::make_shared<DrawResultCircle>())
	, m_scale(0)
	, m_initheight(10)
	, m_circle(QPolygonF())
{
	if (params == nullptr)
		throw std::runtime_error("error");
	m_params = std::dynamic_pointer_cast<DiagramDrawParamsCircle>(params);
	if(m_params == nullptr)
		throw std::runtime_error("error");
	build();
}


void DiagramDrawerCircle::build()
{
	if (m_params == nullptr)
		throw std::runtime_error("error");
	if (m_params->m_paramChanged)
	{
		m_circle = calcuBasicalCircle();

		QPointF center = m_circle.boundingRect().center();
		QTransform rotatetransform = calcurotatetransform(center);
		m_circle = m_circle * rotatetransform;

		center = m_circle.boundingRect().center();
		QTransform translatetransform = calcuTranslateTransform(center);
		m_circle = m_circle * translatetransform;

		m_scale = calcuscaleFactor(m_circle.boundingRect());
		QTransform scaletransform = calcuscaleTransform();
		m_circle = m_circle * scaletransform;
	}
}

void DiagramDrawerCircle::draw(QPainter& painter)
{
	build();

	QPen pen = m_params->m_pen;
	qreal realpenwidth = m_params->m_pen.widthF() / m_scale;
	pen.setWidthF(realpenwidth);

	painter.setPen(pen);
	painter.setBrush(m_params->m_brush);
	if (m_scale < 1e-6)
		throw std::runtime_error("error");
	painter.translate(m_params->m_center);
	painter.rotate(m_params->m_rotate);
	painter.scale(m_scale, m_scale);

	QSizeF size = QSizeF(m_initheight * m_params->m_boundingrectradio, m_initheight);
	painter.drawEllipse(QPointF(0, 0), size.width() / 2, size.height() / 2);
	painter.resetTransform();
}

std::shared_ptr<DrawResult> DiagramDrawerCircle::getResult()
{
	build();
	m_result->m_painterpen = m_params->m_pen;
	m_result->m_painterbrush = m_params->m_brush;
	m_result->m_circle = m_circle;
	return m_result;
}


std::shared_ptr<IDidgramDrawParams> DiagramDrawerCircle::getParams()
{
	return m_params;
}

QPolygonF DiagramDrawerRect::calcuBasicalRect()
{
	double height = 50; //这里很有意思，如果设置的很小，比如小于1，最终缩放会出问题，会缩放的很小
	//越小，缩放得到的越小。但设置很大不会出问题

	if (m_params->m_boundingrectradio < 1e-10)
		throw std::runtime_error("error");
	double width = height * m_params->m_boundingrectradio;


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
	rotatetransform.translate(center.x(), center.y());
	rotatetransform.rotate(m_params->m_rotate);
	rotatetransform.translate(-center.x(), -center.y());
	return rotatetransform;
}

QTransform DiagramDrawerRect::calcuTranslateTransform(QPointF center)
{
	QTransform translateTransform;
	QPointF finalpoint = m_params->m_center - center;
	translateTransform.translate(finalpoint.x(), finalpoint.y());
	return translateTransform;
}

QTransform DiagramDrawerRect::calcuScaleTransform(QRectF rect)
{
	QTransform scaletransform;
	qreal penwidth = m_params->m_pen.widthF();
	qreal scalex = m_params->m_spacesize.width() / (rect.width() + penwidth);
	qreal scaley = m_params->m_spacesize.height() / (rect.height() + penwidth);
	qreal scale = qMin(scalex, scaley) * m_params->m_scale;
	if (scale < 1e-10)
		throw std::runtime_error("error");
	qreal x = rect.center().x();
	qreal y = rect.center().y();
	scaletransform.translate(x, y);
	scaletransform.scale(scale, scale);
	scaletransform.translate(-x, -y);
	return scaletransform;
}

QPolygonF DiagramDrawerRect::calcuRect()
{
	QPolygonF rect = calcuBasicalRect();
	rect = rect * calcuRotateTransform(rect.boundingRect().center());
	rect = rect * calcuTranslateTransform(rect.boundingRect().center());
	rect = rect * calcuScaleTransform(rect.boundingRect());
	return rect;
}






DiagramDrawInterface& DiagramDrawInterface::getInstance()
{
	static DiagramDrawInterface instance;
	return instance;
}

DiagramDrawInterface& DiagramDrawInterface::addDrawerCreator(ShapeType type, std::function<std::shared_ptr<IDiagramDrawer>(std::shared_ptr<IDidgramDrawParams>)> drawer)
{
	m_drawerMap[type] = drawer;
	return *this;
}

std::shared_ptr<IDiagramDrawer> DiagramDrawInterface::getDrawer(std::shared_ptr<IDidgramDrawParams> params)
{
	if (m_drawerMap.find(params->m_type) != m_drawerMap.end())
	{
		return m_drawerMap[params->m_type](params);
	}
	else
	{
		throw std::runtime_error("error");
	}

}






DiagramDrawerTriangle::DiagramDrawerTriangle(std::shared_ptr<IDidgramDrawParams> params)
	:m_params(nullptr)
	, m_result(std::make_unique<DrawResultTriangle>())
	, m_triangle(QPolygonF())
{
	if (params == nullptr)
		throw std::runtime_error("error");
	m_params = std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(params);
	if(m_params == nullptr)
		throw std::runtime_error("error");
	build();
}

void DiagramDrawerTriangle::build()
{
	if (m_params == nullptr)
		throw std::runtime_error("error");
	if (m_params->m_paramChanged)
	{
		m_triangle = calcuTriangle();
		m_params->m_paramChanged = false;
	}
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



std::shared_ptr<IDidgramDrawParams> DiagramDrawerTriangle::getParams()
{
	return m_params;
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

QTransform DiagramDrawerTriangle::calcuTranslateTransfrom(QPointF trianglecenter)
{
	QTransform translateTransform;
	QPointF newpoint = m_params->m_center - trianglecenter;
	translateTransform.translate(newpoint.x(), newpoint.y());
	return translateTransform;
}

QTransform DiagramDrawerTriangle::calcuScaleTransform(QRectF trianglerect)
{
	QTransform scaleTransform;
	qreal penwidth = m_params->m_pen.widthF();
	qreal scalex = m_params->m_spacesize.width() / (trianglerect.width() + penwidth);
	qreal scaley = m_params->m_spacesize.height() / (trianglerect.height() + penwidth);
	qreal scale = qMin(scalex, scaley) * m_params->m_scale;

	qreal x = trianglerect.center().x();
	qreal y = trianglerect.center().y();
	scaleTransform.translate(x, y);
	scaleTransform.scale(scale, scale);
	scaleTransform.translate(-x, -y);

	return scaleTransform;
}


QPolygonF DiagramDrawerTriangle::calcuBasicalTriangle()
{
	qreal bottom = m_params->m_triangleSizeRadios.m_bottom;
	qreal left = m_params->m_triangleSizeRadios.m_left;
	qreal right = m_params->m_triangleSizeRadios.m_right;
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
	qreal bottom = m_params->m_triangleSizeRadios.m_bottom;
	qreal left = m_params->m_triangleSizeRadios.m_left;
	qreal right = m_params->m_triangleSizeRadios.m_right;

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



DiagramDrawerLine::DiagramDrawerLine(std::shared_ptr<IDidgramDrawParams> params)
	:m_params(nullptr)
	, m_result(std::make_shared<DrawResultLine>())
    , m_line(QLineF())
{
	if (params == nullptr)
		throw std::runtime_error("error");
	m_params = std::dynamic_pointer_cast<DiagramDrawParamsLine>(params);
	if(m_params == nullptr)
		throw std::runtime_error("error");
	build();
}

void DiagramDrawerLine::build()
{
	if (m_params == nullptr)
		throw std::runtime_error("error");
	if (m_params->m_paramChanged)
	{
		m_line = calcuLine();
		m_params->m_paramChanged = false;
	}

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
}

std::shared_ptr<IDidgramDrawParams> DiagramDrawerLine::getParams()
{
	return m_params;
}


QLineF DiagramDrawerLine::calcuLine()
{
	qreal penwidth = m_params->m_pen.widthF();
	double halfwidth = (m_params->m_spacesize.width() - penwidth) / 2.0;
	double halfheight = (m_params->m_spacesize.height() - penwidth) / 2.0;

	// Convert rotation angle to radians
	qreal angle = m_params->m_rotate;
	double radians = qDegreesToRadians(angle);

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

	QPointF start = m_params->m_center - QPointF(t * dx, t * dy);
	QPointF end = m_params->m_center + QPointF(t * dx, t * dy);

	return QLineF(start, end);
}

bool DrawResultRect::iscontainPoint(QPointF point)
{
	return m_rect.containsPoint(point, Qt::OddEvenFill);
}

QPainterPath DrawResultRect::getPainterPath()
{
	QPainterPathStroker stroker;
	stroker.setWidth(m_painterpen.widthF());
	QPainterPath path;
	path.addPolygon(m_rect);
	return stroker.createStroke(path);
}

bool DrawResultCircle::iscontainPoint(QPointF point)
{
	if (m_circle.size() < 4) return false;

	// 创建椭圆路径
	QPainterPath path;
	path.addEllipse(QRectF(m_circle[0], m_circle[2]));

	return path.contains(point);
}

QPainterPath DrawResultCircle::getPainterPath()
{
	QPainterPathStroker stroker;
	stroker.setWidth(m_painterpen.widthF());
	QPainterPath path;
	path.addEllipse(QRectF(m_circle[0], m_circle[2]));
	return stroker.createStroke(path);
}

bool DrawResultTriangle::iscontainPoint(QPointF point)
{
	return m_triangle.containsPoint(point, Qt::OddEvenFill);
}

QPainterPath DrawResultTriangle::getPainterPath()
{
	QPainterPathStroker stroker;
	stroker.setWidth(m_painterpen.widthF());
	QPainterPath path;
	path.addPolygon(m_triangle);
	return stroker.createStroke(path);
}

bool DrawResultLine::iscontainPoint(QPointF point)
{
	qreal distance = distanceToLine(m_line, point);
	return distance <= linetolerance;
}

QPainterPath DrawResultLine::getPainterPath()
{
	QPainterPathStroker stroker;
	stroker.setWidth(m_painterpen.widthF());

	QPainterPath path;
	path.moveTo(m_line.p1());
	path.lineTo(m_line.p2());

	return stroker.createStroke(path);
}

qreal DrawResultLine::distanceToLine(const QLineF& line, const QPointF& point)
{
	// 向量AP (从line.p1()到point)
	QPointF AP = point - line.p1();
	// 向量AB (线段方向向量)
	QPointF AB = line.p2() - line.p1();

	// 计算AB的长度平方
	qreal ab2 = AB.x() * AB.x() + AB.y() * AB.y();
	// 计算AP与AB的点积
	qreal ap_ab = AP.x() * AB.x() + AP.y() * AB.y();

	// 计算归一化参数t (点在AB上的投影位置)
	qreal t = ap_ab / ab2;

	// 如果t在[0,1]之外，计算到端点的距离
	if (t < 0.0) {
		return QLineF(line.p1(), point).length();
	}
	else if (t > 1.0) {
		return QLineF(line.p2(), point).length();
	}

	// 计算投影点
	QPointF projection = line.p1() + t * AB;
	// 返回点到投影点的距离
	return QLineF(point, projection).length();

}



