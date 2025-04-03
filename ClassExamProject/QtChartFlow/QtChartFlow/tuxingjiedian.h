#pragma once
#include <qpoint.h>
#include <qsize.h>
#include <qpainter.h>

class Ituxingjiedian
{
public:
	Ituxingjiedian(QPoint topleft, QSize size);
	virtual void draw(QPainter* painter) const = 0;
protected:
	QPoint m_topleft;
	QSize m_size;
};

class juxingjiedian : public Ituxingjiedian
{
public:
	juxingjiedian(QPoint topleft);
	virtual void draw(QPainter* painter) const;
};
