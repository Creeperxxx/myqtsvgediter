#include "tuxingjiedian.h"

//void juxingjiedian::draw(QPainter* painter)
//{
//	//todo : 也许要save painter，但也许不用（painter由画布那里控制）
//	//painter->drawRect(m_mousePoint.x(), m_mousePoint.y(), m_size.width(), m_size.height());
//	//painter->save();
//	//painter->setPen(m_pen);
//	//painter->setBrush(m_brush);
//	//m_topleft = calcuTopleftFromMousePoint();
//	//painter->drawRect(m_topleft.x(), m_topleft.y(), m_size.width(), m_size.height());
//	painter->drawRect(calcurect(m_spacesize, painter->pen().width(), m_radio));
//	//painter->restore();
//}

//std::shared_ptr<tuxingdrawreturn> DiagramDrawerRect::draw(std::shared_ptr<IDidgramDrawParams> params)

std::shared_ptr<tuxingdrawreturn> DiagramDrawerRect::draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params) 
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

	auto ret = std::make_unique<tuxingdrawreturnjuxing>();
	ret->m_painterpen = pen;
	ret->m_painterbrush = brush;
	ret->m_rect = rect;

	return std::move(ret);
}


//
//Ituxingjiedian::Ituxingjiedian(QPoint mousepoint, QSize size)
//	: m_mousePoint(mousepoint)
//	, m_size(size) 
//Ituxingjiedian::Ituxingjiedian(std::unique_ptr<tuxingjiedianparams> params)
//Ituxingjiedian::Ituxingjiedian(tuxingjiedianparams* params)
//{
//	//m_topleft = calcuTopleftFromMousePoint();
//	if (params)
//	{
//		m_center = params->m_center;
//		m_spacesize = params->m_spacesize;
//		//m_pen = params->m_pen;
//		//m_brush = params->m_brush;
//	}
//	else
//	{
//		qDebug() << "参数为空"; //todo 异常处理
//	}
//}

//juxingjiedian::juxingjiedian(QPoint mousepoint, QSize size)
	//:Ituxingjiedian(mousepoint, size) {}
	//:Ituxingjiedia

//QPoint Ituxingjiedian::calcuTopleftFromMousePoint()
//{
//	return QPoint(m_mousePoint.x() - m_size.width() / 2, m_mousePoint.y() - m_size.height() / 2);
//}

//void Ituxingjiedian::setmousepoint(QPoint mousepoint)
//{
//	m_mousePoint = mousepoint;
//}
//
//void Ituxingjiedian::setspacesize(QSize size)
//{
//	m_size = size;
//}


int DiagramDrawerYuanxing::calcuyuanxingbanjing(DiagramDrawParamsCircle* params)
{
	return (qMin(params->m_spacesize.width(), params->m_spacesize.height()) - params->m_pen.widthF()) / 2;
}

std::shared_ptr<tuxingdrawreturn> DiagramDrawerYuanxing::draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params)
{
	if(!params)
		throw std::runtime_error("error");//todo:except
	auto p = dynamic_cast<DiagramDrawParamsCircle*>(params.get());
	if(!p)
		throw std::runtime_error("error");//todo:except

	auto r = calcuyuanxingbanjing(p);
	painter.drawEllipse(p->m_center, r, r);

	auto ret = std::make_unique<tuxingdrawreturnyuanxing>();
	ret->m_painterbrush = painter.brush();
	ret->m_painterpen = painter.pen();
	ret->m_r = r;

	return std::move(ret);
}

//void yuanxingjiedian::draw(QPainter* painter)
//{
//	int r = calcuyuanxingbanjing(m_spacesize, painter->pen().width());
//	painter->drawEllipse(m_center, r, r);
//}

//int yuanxingjiedian::calcuTopleftFromMousePoint(int penwidth)
//int yuanxingjiedian::calcuyuanxingbanjing(QSize spacesize, int penwidth)
//{
//	return (qMin(spacesize.width(), spacesize.height()) - penwidth) / 2;
//}


QRectF DiagramDrawerRect::calcurect(DiagramDrawParamsRect* params)
{

	QSizeF size = calcusuitablerectsize(params);
	float x = params->m_center.x() - size.width() / 2;
	float y = params->m_center.y() - size.height() / 2;

	return QRectF(x, y, size.width(), size.height());

	//QSize size = calcusuitablerectsize(params);
	//int x = params->m_center.x() - size.width() / 2 - params->m_painter->pen().width() / 2;
	//int y = params->m_center.y() - size.height() / 2 - params->m_painter->pen().width() / 2;
	//return QRect(x, y, size.width(), size.height());
}
//
//QRect juxingjiedian::calcurect(QSize spacesize, int penwidth, float radio)
//{
//	QSize size = calcusuitablerectsize(spacesize, penwidth, radio);
//	int x = m_center.x() - size.width() / 2 - penwidth / 2;
//	int y = m_center.y() - size.height() / 2 - penwidth / 2;
//	return QRect(x, y, size.width(), size.height());
//}

//void juxingjiedian::setRadio(float radio)
//{
//	m_radio = radio;
//}

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
	//int containerwidth = params->m_spacesize.width();
	//int containerheight = params->m_spacesize.height();

	//int widthfromheight = static_cast<int>(containerheight * params->m_radio);
	//int heightfromwidth = static_cast<int>(containerwidth / params->m_radio);

	//int finalwidth;
	//int finalheight;

	//if (heightfromwidth + params->m_painter->pen().width() <= containerheight)
	//{
	//	finalwidth = containerwidth;
	//	finalheight = heightfromwidth;
	//}
	//else
	//{
	//	finalwidth = widthfromheight;
	//	finalheight = containerheight;
	//}
	//return QSize(finalwidth, finalheight);

}

//
//QSize juxingjiedian::calcusuitablerectsize(QSize spacesize, int penwidth, float juxingradio)
//{
//	int containerwidth = spacesize.width();
//	int containerheight = spacesize.height();
//
//	int widthfromheight = static_cast<int>(containerheight * juxingradio);
//	int heightfromwidth = static_cast<int>(containerwidth / juxingradio);
//
//	int finalwidth;
//	int finalheight;
//
//	if (heightfromwidth + penwidth <= containerheight)
//	{
//		finalwidth = containerwidth;
//		finalheight = heightfromwidth;
//	}
//	else
//	{
//		finalwidth = widthfromheight;
//		finalheight = containerheight;
//	}
//	return QSize(finalwidth, finalheight);
//}

//tuxingjiedianparams::tuxingjiedianparams(QPoint mousepoint, QSize size, QPen pen, QBrush brush)
//	: m_mousepoint(mousepoint)
//	, m_size(size)
//	, m_pen(pen)
//	, m_brush(brush) {}
//tuxingjiedianparams::tuxingjiedianparams(QPoint mousepoint, QSize size)
//	:m_mousepoint(mousepoint)
//	, m_size(size) {}

//tuxingjiedianparamsjuxing::tuxingjiedianparamsjuxing(QPoint mousepoint, QSize size, QPen pen, QBrush brush, float radio)
//	: tuxingjiedianparams(mousepoint, size, pen, brush)
//	, m_radio(radio) {}

//juxingjiedian::juxingjiedian(std::unique_ptr<tuxingjiedianparams> params)
//juxingjiedian::juxingjiedian(tuxingjiedianparams* params)
//	: Ituxingjiedian(params)
//	, m_radio(huabujuxingradio)
//{
//	if (params)
//	{
//		tuxingjiedianparamsjuxing* realparams = dynamic_cast<tuxingjiedianparamsjuxing*>(params);
//		m_radio = realparams ? realparams->m_radio : huabujuxingradio;
//	}
//	else
//	{
//
//		qDebug() << "参数为空！";
//	}
//}
//
//yuanxingjiedian::yuanxingjiedian(tuxingjiedianparams* params)
//	:Ituxingjiedian(params) {}
//
//tuxingjiedianparamsyuanxing::tuxingjiedianparamsyuanxing(QPoint mousepoint, QSize size, QPen pen, QBrush brush)
//	: tuxingjiedianparams(mousepoint, size, pen, brush) {}


std::shared_ptr<tuxingdrawreturn> DiagramDrawInterface::draw(QPainter &painter, std::shared_ptr<IDidgramDrawParams> params)
{
	return create(params->m_type)->draw(painter, params);
}


std::shared_ptr<IDiagramDrawer> DiagramDrawInterface::create(ShapeType type)
{
	switch (type)
	{
	default:
	case ShapeType::juxing:
		return std::make_unique<DiagramDrawerRect>();
		break;
	case ShapeType::yuanxing:
		return std::make_unique<DiagramDrawerYuanxing>();
		break;
	}
}

//std::shared_ptr<IDidgramDrawParams> factorytuxingparams::create(ShapeType type)
//{
//	switch (type)
//	{
//	default:
//	case ShapeType::juxing:
//		return std::make_shared<DiagramDrawParamsRect>();
//		break;
//	case ShapeType::yuanxing:
//		return std::make_shared<DiagramDrawParamsCircle>();
//		break;
//	}
//}

ShapeType Tool::shapetypestringtoenum(const std::string& str)
{
	if (str.compare(cfggetval<std::string>(qtcf::tuxingTypeNameJuxing)) == 0)
		return ShapeType::juxing;
	else if (str.compare(cfggetval<std::string>(qtcf::tuxingTypeNameYuanxing)) == 0)
		return ShapeType::yuanxing;
	else
		throw std::runtime_error("error");
}
