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
		break;
	}
}



