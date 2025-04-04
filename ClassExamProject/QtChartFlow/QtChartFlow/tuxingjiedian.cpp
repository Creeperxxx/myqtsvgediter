#include "tuxingjiedian.h"

void juxingjiedian::draw(QPainter* painter) const
{
	//todo : 也许要save painter，但也许不用（painter由画布那里控制）
	painter->setPen(QPen(Qt::black,2));
	painter->setBrush(Qt::transparent);
	//painter->drawRect(m_mousePoint.x(), m_mousePoint.y(), m_size.width(), m_size.height());
	painter->drawRect(m_topleft.x(), m_topleft.y(), m_size.width(), m_size.height());
}

Ituxingjiedian::Ituxingjiedian(QPoint mousepoint, QSize size)
	: m_mousePoint(mousepoint)
	, m_size(size) 
{
	calcuTopleftFromMousePoint();
}

juxingjiedian::juxingjiedian(QPoint mousepoint, QSize size)
	:Ituxingjiedian(mousepoint, size) {}
	//:Ituxingjiedia

void Ituxingjiedian::calcuTopleftFromMousePoint()
{
	m_topleft = QPoint(m_mousePoint.x() - m_size.width() / 2, m_mousePoint.y() - m_size.height() / 2);
}
