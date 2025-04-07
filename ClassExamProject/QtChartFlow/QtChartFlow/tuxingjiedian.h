#pragma once
#include <qpoint.h>
#include <qsize.h>
#include <qpainter.h>
#include "config.h"
#include <qdebug.h>
#include <stdexcept>

enum class ShapeType
{
	juxing,
	yuanxing
};

class tuxingjiedianparams
{
public:
	virtual ~tuxingjiedianparams() {}
	QPoint m_center;
	QSize m_spacesize;
	ShapeType m_type;
	QPainter* m_painter;
};

class tuxingjiedianparamsjuxing : public tuxingjiedianparams
{
public:
	~tuxingjiedianparamsjuxing() {}
	float m_radio;
};

class tuxingjiedianparamsyuanxing :public tuxingjiedianparams
{
public:
	~tuxingjiedianparamsyuanxing() {}
};

class tuxingdrawreturn
{
public:
	QPen m_painterpen;
	QBrush m_painterbrush;
};

class tuxingdrawreturnjuxing : public tuxingdrawreturn
{
public:
	QRectF m_rect;
};

class tuxingdrawreturnyuanxing :public tuxingdrawreturn
{
public:
	int m_r;
};


class Ituxingjiedian
{
public:
	//Ituxingjiedian(QPoint mousepoint, QSize size);
	//Ituxingjiedian(std::unique_ptr<tuxingjiedianparams> params);
	virtual std::shared_ptr<tuxingdrawreturn> draw(std::shared_ptr<tuxingjiedianparams> params) = 0;
	//virtual void draw(QPainter* painter) = 0;
	//void setmousepoint(QPoint mousepoint);
	//void setspacesize(QSize size);
	//QPoint calcuTopleftFromMousePoint(); 
	//QPoint m_topleft;
	//QPen m_pen;
	//QBrush m_brush;
};

class juxingjiedian : public Ituxingjiedian
{
public:
	//juxingjiedian(QPoint mousepoint, QSize size = QSize(100,50));
	//juxingjiedian(std::unique_ptr<tuxingjiedianparams> params);
	//void draw(QPainter* painter) override;
	//void setRadio(float radio);
	std::shared_ptr<tuxingdrawreturn> draw(std::shared_ptr<tuxingjiedianparams> params) override;
private:
	//QSize calcusuitablerectsize(QSize spacesize, int penwidth, float juxingradio);
	//QRect calcurect(QSize spacesize, int penwidth, float radio);
	QRectF calcurect(tuxingjiedianparamsjuxing* params);
	QSizeF calcusuitablerectsize(tuxingjiedianparamsjuxing* params);
};

class yuanxingjiedian : public Ituxingjiedian
{
public:
	//yuanxingjiedian(tuxingjiedianparams* params);
	//void draw(QPainter* painter) override;
	std::shared_ptr<tuxingdrawreturn> draw(std::shared_ptr<tuxingjiedianparams> params)override;
private:
	int calcuyuanxingbanjing(tuxingjiedianparamsyuanxing* params);
	//int calcuyuanxingbanjing(QSize spacesize, int penwidth);
};

class factorytuxingjiedian
{
public:
	static std::shared_ptr<tuxingdrawreturn> draw(std::shared_ptr<tuxingjiedianparams> params);
private:
	static std::shared_ptr<Ituxingjiedian> create(ShapeType type);
};

class factorytuxingparams
{
public:
	static std::shared_ptr<tuxingjiedianparams> create(ShapeType type);
};
