#include "tuxingjiedian.h"

void juxingjiedian::draw(QPainter* painter) const
{
	painter->setPen(QPen(Qt::black,2));
	painter->setBrush(Qt::transparent);
	painter->drawRect(m_topleft.x(), m_topleft.y(), m_size.width(), m_size.height());
}
Ituxingjiedian::Ituxingjiedian(QPoint topleft, QSize size)
	: m_topleft(topleft)
	, m_size(size) {}

juxingjiedian::juxingjiedian(QPoint topleft)
	:Ituxingjiedian(topleft, QSize(100, 50)) {}
	//:Ituxingjiedia

