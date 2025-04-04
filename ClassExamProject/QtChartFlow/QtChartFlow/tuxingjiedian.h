#pragma once
#include <qpoint.h>
#include <qsize.h>
#include <qpainter.h>

class Ituxingjiedian
{
public:
	Ituxingjiedian(QPoint mousepoint, QSize size);
	virtual void draw(QPainter* painter) const = 0;
protected:
	void calcuTopleftFromMousePoint();

	QPoint m_mousePoint;
	QPoint m_topleft;
	QSize m_size;
};

class juxingjiedian : public Ituxingjiedian
{
public:
	juxingjiedian(QPoint mousepoint, QSize size = QSize(100,50));
	virtual void draw(QPainter* painter) const;
};
