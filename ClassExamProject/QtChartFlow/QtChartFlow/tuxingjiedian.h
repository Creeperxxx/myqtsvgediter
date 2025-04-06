#pragma once
#include <qpoint.h>
#include <qsize.h>
#include <qpainter.h>
#include "config.h"
#include <qdebug.h>

class tuxingjiedianparams
{
public:
	//tuxingjiedianparams(QPoint mousepoint, QSize size, QPen pen, QBrush);
	//tuxingjiedianparams(QPoint mousepoint, QSize size);
	virtual ~tuxingjiedianparams() {}
	QPoint m_mousepoint;
	QSize m_size;
	//const QPen m_pen;
	//const QBrush m_brush;
};

class tuxingjiedianparamsjuxing : public tuxingjiedianparams
{
public:
	//tuxingjiedianparamsjuxing(QPoint mousepoint
		//, QSize size
		//, QPen pen
		//, QBrush brush
		//, float radio);
	~tuxingjiedianparamsjuxing() {}
	float m_radio;
};

class tuxingjiedianparamsyuanxing :public tuxingjiedianparams
{
public:
	//tuxingjiedianparamsyuanxing(QPoint mousepoint
	//	, QSize size
	//	, QPen pen
	//	, QBrush brush);
	~tuxingjiedianparamsyuanxing() {}

};



class Ituxingjiedian
{
public:
	//Ituxingjiedian(QPoint mousepoint, QSize size);
	//Ituxingjiedian(std::unique_ptr<tuxingjiedianparams> params);
	Ituxingjiedian(tuxingjiedianparams* params);
	virtual void draw(QPainter* painter) = 0;
	//void setmousepoint(QPoint mousepoint);
	//void setspacesize(QSize size);
protected:
	//QPoint calcuTopleftFromMousePoint(); 

	QPoint m_mousePoint;
	//QPoint m_topleft;
	QSize m_size;
	//QPen m_pen;
	//QBrush m_brush;
};

class juxingjiedian : public Ituxingjiedian
{
public:
	//juxingjiedian(QPoint mousepoint, QSize size = QSize(100,50));
	//juxingjiedian(std::unique_ptr<tuxingjiedianparams> params);
	juxingjiedian(tuxingjiedianparams* params);
	void draw(QPainter* painter) override;
	//void setRadio(float radio);
	QSize calcusuitablerectsize(QSize spacesize, int penwidth, float juxingradio);
private:
	QRect calcurect(QSize spacesize, int penwidth, float radio);
	float m_radio;
};

class yuanxingjiedian : public Ituxingjiedian
{
public:
	yuanxingjiedian(tuxingjiedianparams* params);
	void draw(QPainter* painter) override;
private:
	int calcuyuanxingbanjing(QSize spacesize, int penwidth);
};
