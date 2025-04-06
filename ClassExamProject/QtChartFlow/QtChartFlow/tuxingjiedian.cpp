#include "tuxingjiedian.h"

void juxingjiedian::draw(QPainter* painter)
{
	//todo : 也许要save painter，但也许不用（painter由画布那里控制）
	//painter->drawRect(m_mousePoint.x(), m_mousePoint.y(), m_size.width(), m_size.height());
	//painter->save();
	//painter->setPen(m_pen);
	//painter->setBrush(m_brush);
	//m_topleft = calcuTopleftFromMousePoint();
	//painter->drawRect(m_topleft.x(), m_topleft.y(), m_size.width(), m_size.height());
	painter->drawRect(calcurect(m_size, painter->pen().width(), m_radio));
	//painter->restore();
}
//
//Ituxingjiedian::Ituxingjiedian(QPoint mousepoint, QSize size)
//	: m_mousePoint(mousepoint)
//	, m_size(size) 
//Ituxingjiedian::Ituxingjiedian(std::unique_ptr<tuxingjiedianparams> params)
Ituxingjiedian::Ituxingjiedian(tuxingjiedianparams* params)
{
	//m_topleft = calcuTopleftFromMousePoint();
	if (params)
	{
		m_mousePoint = params->m_mousepoint;
		m_size = params->m_size;
		//m_pen = params->m_pen;
		//m_brush = params->m_brush;
	}
	else
	{
		qDebug() << "参数为空"; //todo 异常处理
	}
}

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

void yuanxingjiedian::draw(QPainter* painter)
{
	int r = calcuyuanxingbanjing(m_size, painter->pen().width());
	painter->drawEllipse(m_mousePoint, r, r);
}

//int yuanxingjiedian::calcuTopleftFromMousePoint(int penwidth)
int yuanxingjiedian::calcuyuanxingbanjing(QSize spacesize, int penwidth)
{
	return (qMin(spacesize.width(), spacesize.height()) - penwidth) / 2;
}

QRect juxingjiedian::calcurect(QSize spacesize, int penwidth, float radio)
{
	QSize size = calcusuitablerectsize(spacesize, penwidth, radio);
	int x = m_mousePoint.x() - size.width() / 2 - penwidth / 2;
	int y = m_mousePoint.y() - size.height() / 2 - penwidth / 2;
	return QRect(x, y, size.width(), size.height());
}

//void juxingjiedian::setRadio(float radio)
//{
//	m_radio = radio;
//}

QSize juxingjiedian::calcusuitablerectsize(QSize spacesize, int penwidth, float juxingradio)
{
	int containerwidth = spacesize.width();
	int containerheight = spacesize.height();

	int widthfromheight = static_cast<int>(containerheight * juxingradio);
	int heightfromwidth = static_cast<int>(containerwidth / juxingradio);

	int finalwidth;
	int finalheight;

	if (heightfromwidth + penwidth <= containerheight)
	{
		finalwidth = containerwidth;
		finalheight = heightfromwidth;
	}
	else
	{
		finalwidth = widthfromheight;
		finalheight = containerheight;
	}
	return QSize(finalwidth, finalheight);
}

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
juxingjiedian::juxingjiedian(tuxingjiedianparams* params)
	: Ituxingjiedian(params)
	, m_radio(huabujuxingradio)
{
	if (params)
	{
		tuxingjiedianparamsjuxing* realparams = dynamic_cast<tuxingjiedianparamsjuxing*>(params);
		m_radio = realparams ? realparams->m_radio : huabujuxingradio;
	}
	else
	{

		qDebug() << "参数为空！";
	}
}

yuanxingjiedian::yuanxingjiedian(tuxingjiedianparams* params)
	:Ituxingjiedian(params) {}
//
//tuxingjiedianparamsyuanxing::tuxingjiedianparamsyuanxing(QPoint mousepoint, QSize size, QPen pen, QBrush brush)
//	: tuxingjiedianparams(mousepoint, size, pen, brush) {}
