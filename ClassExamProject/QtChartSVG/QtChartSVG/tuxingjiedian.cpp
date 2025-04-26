#include "tuxingjiedian.h"
#include <cmath>
#include <qmath.h>


std::shared_ptr<DrawResult> DiagramDrawerRect::draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params)
{
	if (!params)
		throw std::runtime_error("error");//todo:except
	DiagramDrawParamsRect* p = dynamic_cast<DiagramDrawParamsRect*>(params.get());
	if (!p)
		throw std::runtime_error("error");//todo:except

	//QRectF rect = calcurect(p);
	//painter.drawRect(rect);
	QPolygonF rect = calcuRect(p, painter.pen().widthF());
	painter.drawPolygon(rect);

	//QPen pen = painter.pen();
	//QBrush brush = painter.brush();

	auto ret = std::make_unique<DrawResultRect>();
	ret->m_painterpen = painter.pen();
	ret->m_painterbrush = painter.brush();
	ret->m_rect = rect;

	return ret;
}

QPolygonF DiagramDrawerCircle::calcuBasicalCircle(DiagramDrawParamsCircle* params, QSizeF suitablesize)
{
	qreal x = params->m_center.x() - suitablesize.width() / 2.0;
	qreal y = params->m_center.y() - suitablesize.height() / 2.0;

	QPointF lefttop(x, y);
	QPointF leftbottom(x, y + suitablesize.height());
	QPointF righttop(x + suitablesize.width(), y);
	QPointF rightbottom(x + suitablesize.width() , y + suitablesize.height() );

	QPolygonF circle;
	circle << lefttop << leftbottom << rightbottom << righttop;

	return circle;
}

QTransform DiagramDrawerCircle::calcurotatetransform(DiagramDrawParamsCircle* params)
{
	QTransform rotatetransform;
	rotatetransform.translate(params->m_center.x(), params->m_center.y());
	rotatetransform.rotate(params->m_circlerotate);
	rotatetransform.translate(-params->m_center.x(), -params->m_center.y());

	return rotatetransform;
}

qreal DiagramDrawerCircle::calcuscaleFactor(DiagramDrawParamsCircle* params, QPolygonF diagram, qreal penwidth)
{
	QRectF bounds = diagram.boundingRect();

	qreal scalex = params->m_spacesize.width() / (bounds.width() + penwidth);
	qreal scaley = params->m_spacesize.height() / (bounds.height() + penwidth);

	qreal scale = qMin(scalex, scaley) * params->m_scale;

	return scale;
}

QTransform DiagramDrawerCircle::calcuscaleTransform(DiagramDrawParamsCircle* params, qreal scale)
{

	QTransform scaletransform;
	scaletransform.translate(params->m_center.x(), params->m_center.y());
	scaletransform.scale(scale, scale);
	scaletransform.translate(-params->m_center.x(), -params->m_center.y());

	return scaletransform;
}


QSizeF DiagramDrawerCircle::calcuboundingrectsize(DiagramDrawParamsCircle* params, qreal penwidth)
{
	double availablewidth = params->m_spacesize.width() - penwidth;
	double availableheight = params->m_spacesize.height() - penwidth;

	double rectwidth, rectheight;
	if (availablewidth / availableheight - params->m_boundingrectradio > 1e-6)
	{
		rectheight = availableheight;
		rectwidth = rectheight * params->m_boundingrectradio;
	}
	else
	{
		rectwidth = availablewidth;
		rectheight = rectwidth / params->m_boundingrectradio;
	}
	return QSizeF(rectwidth, rectheight);
}


//int DiagramDrawerCircle::calcuyuanxingbanjing(DiagramDrawParamsCircle* params)
//{
//	return (qMin(params->m_spacesize.width(), params->m_spacesize.height()) - params->m_pen.widthF()) / 2;
//}

std::shared_ptr<DrawResult> DiagramDrawerCircle::draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params)
{
	if (!params)
		throw std::runtime_error("error");//todo:except
	auto p = dynamic_cast<DiagramDrawParamsCircle*>(params.get());
	if (!p)
		throw std::runtime_error("error");//todo:except

	//auto r = calcuyuanxingbanjing(p);
	//QRectF boundingrect = calcuboundingrect(p);
	//painter.drawEllipse(boundingrect);

	QSizeF suitablesize = calcuboundingrectsize(p, painter.pen().widthF());
	QPolygonF circle = calcuBasicalCircle(p, suitablesize);

	QTransform rotatetransform = calcurotatetransform(p);
	circle = circle * rotatetransform;

	qreal scale = calcuscaleFactor(p, circle, painter.pen().widthF());
	QTransform scaletransform = calcuscaleTransform(p, scale);
	circle = circle * scaletransform;


	//painter.drawPolygon(circle);


	painter.translate(p->m_center);
	painter.rotate(p->m_circlerotate);
	painter.scale(scale, scale);

	QPen oldpen(painter.pen());
	QPen pen(painter.pen());
	pen.setWidthF(oldpen.widthF() / scale);
	painter.setPen(pen);
	painter.drawEllipse(QPointF(0, 0), suitablesize.width() / 2.0, suitablesize.height() / 2.0);

	painter.resetTransform();
	painter.setPen(oldpen);





	auto ret = std::make_unique<DrawResultCircle>();
	ret->m_painterbrush = painter.brush();
	ret->m_painterpen = painter.pen();
	ret->m_circle = circle;

	return ret;
}


//QRectF DiagramDrawerRect::calcurect(DiagramDrawParamsRect* params)
//{
//
//	QSizeF size = calcusuitablerectsize(params);
//	float x = params->m_center.x() - size.width() / 2;
//	float y = params->m_center.y() - size.height() / 2;
//
//	return QRectF(x, y, size.width(), size.height());
//
//}

//QRectF DiagramDrawerCircle::calcuboundingrect(DiagramDrawParamsCircle* params)
//{
//	QSizeF size = calcuboundingrectsize(params);
//	float x = params->m_center.x() - size.width() / 2;
//	float y = params->m_center.y() - size.height() / 2;
//
//	return QRectF(x, y, size.width(), size.height());
//}

//QSizeF DiagramDrawerRect::calcusuitablerectsize(DiagramDrawParamsRect* params)
//{
//	float availablewidth = params->m_spacesize.width() - 2 * params->m_pen.widthF();
//	float availableheight = params->m_spacesize.height() - 2 * params->m_pen.widthF();
//
//	float rectwidth, rectheight;
//	if (availablewidth / availableheight > params->m_boundingrectradio)
//	{
//		rectheight = availableheight;
//		rectwidth = rectheight * params->m_boundingrectradio;
//	}
//	else
//	{
//		rectwidth = availablewidth;
//		rectheight = rectwidth / params->m_boundingrectradio;
//	}
//	return QSizeF(rectwidth, rectheight);
//}

QPolygonF DiagramDrawerRect::calcuBasicalRect(DiagramDrawParamsRect* params)
{
	double height = 50; //这里很有意思，如果设置的很小，比如小于1，最终缩放会出问题，会缩放的很小
	//越小，缩放得到的越小。但设置很大不会出问题

	if (params->m_boundingrectradio < 1e-10)
		throw std::runtime_error("error");
	double width = height * params->m_boundingrectradio;


	QPointF lefttop(0.0, 0.0);
	QPointF leftbottom(0.0, height);
	QPointF righttop(width, 0.0);
	QPointF rightbottom(width, height);

	QPolygonF basicalrect;
	basicalrect << lefttop << leftbottom << rightbottom << righttop;
	return basicalrect;
}

QTransform DiagramDrawerRect::calcuRotateTransform(DiagramDrawParamsRect* params)
{
	QTransform rotatetransform;
	rotatetransform.translate(params->m_center.x(), params->m_center.y());
	rotatetransform.rotate(params->m_rectrotate);
	rotatetransform.translate(-params->m_center.x(), -params->m_center.y());
	return rotatetransform;
}

QTransform DiagramDrawerRect::calcuTranslateTransform(QPolygonF diagram, DiagramDrawParamsRect* params)
{
	QTransform translateTransform;
	QPointF diagramcenter = diagram.boundingRect().center();
	QPointF finalpoint = params->m_center - diagramcenter;
	translateTransform.translate(finalpoint.x(), finalpoint.y());
	return translateTransform;
}

QTransform DiagramDrawerRect::calcuScaleTransform(QPolygonF diagram, DiagramDrawParamsRect* params, qreal penwidth)
{
	QTransform scaletransform;
	qreal scalex = params->m_spacesize.width() / (diagram.boundingRect().width() + penwidth);
	qreal scaley = params->m_spacesize.height() / (diagram.boundingRect().height() + penwidth);
	qreal scale = qMin(scalex, scaley) * params->m_scale;
	if (scale < 1e-10)
		throw std::runtime_error("error");
	qreal centerx = diagram.boundingRect().center().x();
	qreal centery = diagram.boundingRect().center().y();
	scaletransform.translate(centerx, centery);
	scaletransform.scale(scale, scale);
	scaletransform.translate(-centerx, -centery);
	return scaletransform;
}

QPolygonF DiagramDrawerRect::calcuRect(DiagramDrawParamsRect* params, qreal penwidth)
{
	QPolygonF rect = calcuBasicalRect(params);
	rect = rect * calcuRotateTransform(params);
	rect = rect * calcuTranslateTransform(rect, params);
	rect = rect * calcuScaleTransform(rect, params, penwidth);

	return rect;
}






std::shared_ptr<DrawResult> DiagramDrawInterface::draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params)
{
	return create(params->m_type)->draw(painter, params);
}


std::shared_ptr<IDiagramDrawer> DiagramDrawInterface::create(ShapeType type)
{
	switch (type)
	{
	case ShapeType::Rect:
		return std::make_unique<DiagramDrawerRect>();
		break;
	case ShapeType::Circle:
		return std::make_unique<DiagramDrawerCircle>();
	case ShapeType::Triangle:
		return std::make_unique<DiagramDrawerTriangle>();
		break;
	case ShapeType::Line:
		return std::make_unique<DiagramDrawerLine>();
	default:
		throw std::runtime_error("error");
	}
}

std::shared_ptr<DrawResult> DiagramDrawerTriangle::draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params)
{
	if (!params)
		throw std::runtime_error("error");//todo:except
	auto p = dynamic_cast<DiagramDrawParamsTriangle*>(params.get());
	if (!p)
		throw std::runtime_error("error");//todo:except
	QPolygonF triangle = calcuTriangle(p);
	painter.drawPolygon(triangle);

	std::shared_ptr<DrawResultTriangle> ret = std::make_shared<DrawResultTriangle>();
	ret->m_painterbrush = painter.brush();
	ret->m_painterpen = painter.pen();
	ret->m_triangle = triangle;
	return ret;
}

QRectF DiagramDrawerTriangle::calcuwidgetrect(QPointF cente, QSizeF size)
{
	QRectF widgetrect = QRectF(cente.x() - size.width() / 2, cente.y() - size.height() / 2, size.width(), size.height());
	return widgetrect;
}

QPolygonF DiagramDrawerTriangle::calcuTriangle(DiagramDrawParamsTriangle* params)
{
	QPolygonF triangle = calcuUpsidedowntriangle(params->m_triangleSizeRadios.m_bottom, params->m_triangleSizeRadios.m_left, params->m_triangleSizeRadios.m_right);
	QTransform rotateTransform = calcuRotateTransform(params->m_triangleSizeRadios.m_bottom, params->m_triangleSizeRadios.m_left, params->m_triangleSizeRadios.m_right, params->m_edgetype, params->m_rotationAngle);
	QTransform translateTransform = calcuTranslateTransfrom(triangle.boundingRect().center(), params->m_center);

	triangle = triangle * rotateTransform * translateTransform;
	QTransform scaleTransform = calcuScaleTransform(triangle.boundingRect(), params);
	triangle = triangle * scaleTransform;
	return triangle;
}

QTransform DiagramDrawerTriangle::calcuTranslateTransfrom(QPointF trianglecenter, QPointF widgetcenter)
{
	QTransform translateTransform;
	translateTransform.translate(widgetcenter.x() - trianglecenter.x(), widgetcenter.y() - trianglecenter.y());
	return translateTransform;
}

QTransform DiagramDrawerTriangle::calcuScaleTransform(QRectF trianglerect, DiagramDrawParamsTriangle* params)
{
	QTransform scaleTransform;
	QRectF widget = calcuwidgetrect(params->m_center, params->m_spacesize);
	qreal scalex = widget.width() / trianglerect.width();
	qreal scaley = widget.height() / trianglerect.height();
	qreal scale = qMin(scalex, scaley) * params->m_scale;

	scaleTransform.translate(trianglerect.center().x(), trianglerect.center().y());
	scaleTransform.scale(scale, scale);
	scaleTransform.translate(-trianglerect.center().x(), -trianglerect.center().y());

	return scaleTransform;
}


QPolygonF DiagramDrawerTriangle::calcuUpsidedowntriangle(double bottom, double left, double right)
{
	QPointF topleft = QPointF(0, 0);
	QPointF topright = QPointF(bottom, 0);
	qreal constheta = (left * left + bottom * bottom - right * right) / (2 * left * bottom);
	constheta = qBound(-1.0, constheta, 1.0);
	qreal theta = acos(constheta);
	qreal x = left * cos(theta);
	qreal y = left * sin(theta);
	QPointF bottompoint = QPointF(x, y);

	QPolygonF triangle;
	triangle << topleft << topright << bottompoint;
	return triangle;
}

QTransform DiagramDrawerTriangle::calcuRotateTransform(double bottom, double left, double right, DiagramDrawParamsTriangle::EdgeType edgetype, double angle)
{
	// 验证三角形边长是否有效
	if (bottom <= 0 || left <= 0 || right <= 0 ||
		bottom + left <= right || bottom + right <= left || left + right <= bottom) {
		return QTransform(); // 返回单位矩阵表示无效输入
	}

	// 计算顶点C的坐标
	double cosA = (left * left + bottom * bottom - right * right) / (2 * left * bottom);
	double sinA = std::sqrt(1 - cosA * cosA);
	QPointF C(left * cosA, left * sinA);

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
		break; // 无效输入，保持无旋转
	}

	QTransform rotateTransform;
	//rotateTransform.scale(1, -1);
	rotateTransform.scale(-1, 1);
	QTransform transform;
	transform.setMatrix(newcos, -newsin, 0,
		newsin, newcos, 0,
		0, 0, 1);
	return rotateTransform * transform;
}

std::shared_ptr<DrawResult> DiagramDrawerLine::draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params)
{
	if (!params)
		throw std::invalid_argument("Diagram draw parameters cannot be null.");

	auto p = dynamic_cast<DiagramDrawParamsLine*>(params.get());
	if (!p)
		throw std::invalid_argument("Invalid diagram draw parameters type.");


	QLineF line = calcuLine(p);
	// Draw the line
	painter.drawLine(line);

	// Prepare the result
	std::shared_ptr<DrawResultLine> ret = std::make_shared<DrawResultLine>();
	ret->m_line = line;

	ret->m_painterbrush = painter.brush();
	ret->m_painterpen = painter.pen();

	return ret;
	//if (!params)
	//	throw std::runtime_error("error");//todo:except
	//auto p = dynamic_cast<DiagramDrawParamsLine*>(params.get());
	//if (!p)
	//	throw std::runtime_error("error");//todo:except
	//
	//double halfwidth = p->m_spacesize.width() / 2;
	//double halfheight = p->m_spacesize.height() / 2;

	//double radians = qDegreesToRadians(p->m_rotationAngle);

	//double dx = std::cos(radians);
	//double dy = std::sin(radians);

	//double t = std::max(std::abs(halfwidth / dx), std::abs(halfheight / dy));
	//QPointF start = p->m_center - t * QPointF(dx, dy);
	//QPointF end = p->m_center + t * QPointF(dx, dy);

	//painter.drawLine(start, end);

	//std::shared_ptr<DrawResultLine> ret = std::make_shared<DrawResultLine>();
	//return ret;
}

QLineF DiagramDrawerLine::calcuLine(DiagramDrawParamsLine* params)
{
	double halfwidth = params->m_spacesize.width() / 2.0;
	double halfheight = params->m_spacesize.height() / 2.0;

	// Convert rotation angle to radians
	double radians = qDegreesToRadians(static_cast<double>(params->m_rotationAngle));

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
	double t = std::min(tWidth, tHeight) * params->m_scale;

	QPointF start = params->m_center - QPointF(t * dx, t * dy);
	QPointF end = params->m_center + QPointF(t * dx, t * dy);

	return QLineF(start, end);
}

bool DrawResultRect::iscontainPoint(QPointF point)
{
	return m_rect.containsPoint(point, Qt::OddEvenFill);
}

bool DrawResultCircle::iscontainPoint(QPointF point)
{
	if (m_circle.size() < 4) return false;

	// 创建椭圆路径
	QPainterPath path;
	path.addEllipse(QRectF(m_circle[0], m_circle[2]));

	return path.contains(point);
}

bool DrawResultTriangle::iscontainPoint(QPointF point)
{
	return m_triangle.containsPoint(point, Qt::OddEvenFill);
}

bool DrawResultLine::iscontainPoint(QPointF point)
{
	qreal distance = distanceToLine(m_line, point);
	return distance <= linetolerance;
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



