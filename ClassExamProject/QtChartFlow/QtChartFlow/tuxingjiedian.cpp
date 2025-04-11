#include "tuxingjiedian.h"


std::shared_ptr<DrawResult> DiagramDrawerRect::draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params) 
{
	if (!params)
		throw std::runtime_error("error");//todo:except
	DiagramDrawParamsRect* p = dynamic_cast<DiagramDrawParamsRect*>(params.get());
	if(!p)
		throw std::runtime_error("error");//todo:except

	QRectF rect = calcurect(p);
	painter.drawRect(rect);

	QPen pen = painter.pen();
	QBrush brush = painter.brush();

	auto ret = std::make_unique<DrawResultRect>();
	ret->m_painterpen = pen;
	ret->m_painterbrush = brush;
	ret->m_rect = rect;

	return std::move(ret);
}


int DiagramDrawerCircle::calcuyuanxingbanjing(DiagramDrawParamsCircle* params)
{
	return (qMin(params->m_spacesize.width(), params->m_spacesize.height()) - params->m_pen.widthF()) / 2;
}

std::shared_ptr<DrawResult> DiagramDrawerCircle::draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params)
{
	if(!params)
		throw std::runtime_error("error");//todo:except
	auto p = dynamic_cast<DiagramDrawParamsCircle*>(params.get());
	if(!p)
		throw std::runtime_error("error");//todo:except

	auto r = calcuyuanxingbanjing(p);
	painter.drawEllipse(p->m_center, r, r);

	auto ret = std::make_unique<DrawResultCircle>();
	ret->m_painterbrush = painter.brush();
	ret->m_painterpen = painter.pen();
	ret->m_r = r;

	return std::move(ret);
}


QRectF DiagramDrawerRect::calcurect(DiagramDrawParamsRect* params)
{

	QSizeF size = calcusuitablerectsize(params);
	float x = params->m_center.x() - size.width() / 2;
	float y = params->m_center.y() - size.height() / 2;

	return QRectF(x, y, size.width(), size.height());

}

QSizeF DiagramDrawerRect::calcusuitablerectsize(DiagramDrawParamsRect* params)
{
	float availablewidth = params->m_spacesize.width() - 2 * params->m_pen.widthF();
	float availableheight = params->m_spacesize.height() - 2 * params->m_pen.widthF();

	float rectwidth, rectheight;
	if (availablewidth / availableheight > params->m_radio)
	{
		rectheight = availableheight;
		rectwidth = rectheight * params->m_radio;
	}
	else
	{
		rectwidth = availablewidth;
		rectheight = rectwidth / params->m_radio;
	}
	return QSizeF(rectwidth, rectheight);
}


std::shared_ptr<DrawResult> DiagramDrawInterface::draw(QPainter &painter, std::shared_ptr<IDidgramDrawParams> params)
{
	return create(params->m_type)->draw(painter, params);
}


std::shared_ptr<IDiagramDrawer> DiagramDrawInterface::create(ShapeType type)
{
	switch (type)
	{
	default:
	case ShapeType::Rect:
		return std::make_unique<DiagramDrawerRect>();
		break;
	case ShapeType::Circle:
		return std::make_unique<DiagramDrawerCircle>();
	case ShapeType::Triangle:
		return std::make_unique<DiagramDrawerTriangle>();
		break;
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
	QTransform rotateTransform = calcuRotateTransform(params->m_triangleSizeRadios.m_bottom,params->m_triangleSizeRadios.m_left, params->m_triangleSizeRadios.m_right, params->m_edgetype, params->m_rotationAngle);
	QTransform translateTransform = calcuTranslateTransfrom(triangle.boundingRect().center(), params->m_center);

	triangle = triangle * rotateTransform * translateTransform;
	QTransform scaleTransform = calcuScaleTransform(triangle.boundingRect(), calcuwidgetrect(params->m_center, params->m_spacesize));
	triangle = triangle * scaleTransform;
	return triangle;
}

QTransform DiagramDrawerTriangle::calcuTranslateTransfrom(QPointF trianglecenter, QPointF widgetcenter)
{
	QTransform translateTransform;
	translateTransform.translate(widgetcenter.x() - trianglecenter.x(), widgetcenter.y() - trianglecenter.y());
	return translateTransform;
}

QTransform DiagramDrawerTriangle::calcuScaleTransform(QRectF trianglerect, QRectF widget)
{
	QTransform scaleTransform;
	qreal scalex = widget.width() / trianglerect.width();
	qreal scaley = widget.height() / trianglerect.height();
	qreal scale = qMin(scalex, scaley) * 0.9;

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
	double sinA = qSqrt(1 - cosA * cosA);
	QPointF C(left * cosA, left * sinA);

	// 计算各边的方向
	double targetsin = qSin(qDegreesToRadians(angle));
	double targetcos = qCos(qDegreesToRadians(angle));

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
	rotateTransform.scale(1, -1);
	QTransform transform;
	transform.setMatrix(newcos, -newsin, 0,
		newsin, newcos, 0,
		0, 0, 1);
	return rotateTransform * transform;
}




