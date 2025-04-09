#pragma once
#include <qpoint.h>
#include <qsize.h>
#include <qpainter.h>
#include "config.h"
#include <qdebug.h>
#include <stdexcept>
#include "configmanager.h"

enum class ShapeType
{
	juxing,
	yuanxing
};

class Tool
{
public:
	static ShapeType shapetypestringtoenum(const std::string& str);
};

class IDidgramDrawParams
{
public:
	virtual ~IDidgramDrawParams() {}
	QPoint m_center;
	QSize m_spacesize;
	ShapeType m_type;
	QPen m_pen;
	QBrush m_brush;
	//QPainter* m_painter;
};

class DiagramDrawParamsRect : public IDidgramDrawParams
{
public:
	~DiagramDrawParamsRect() {}
	float m_radio;
};

class DiagramDrawParamsCircle :public IDidgramDrawParams
{
public:
	~DiagramDrawParamsCircle() {}
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


class IDiagramDrawer
{
public:
	//Ituxingjiedian(QPoint mousepoint, QSize size);
	//Ituxingjiedian(std::unique_ptr<tuxingjiedianparams> params);
	virtual std::shared_ptr<tuxingdrawreturn> draw(QPainter &painter, std::shared_ptr<IDidgramDrawParams> params) = 0;
	//virtual void draw(QPainter* painter) = 0;
	//void setmousepoint(QPoint mousepoint);
	//void setspacesize(QSize size);
	//QPoint calcuTopleftFromMousePoint(); 
	//QPoint m_topleft;
	//QPen m_pen;
	//QBrush m_brush;
};

class DiagramDrawerRect : public IDiagramDrawer
{
public:
	//juxingjiedian(QPoint mousepoint, QSize size = QSize(100,50));
	//juxingjiedian(std::unique_ptr<tuxingjiedianparams> params);
	//void draw(QPainter* painter) override;
	//void setRadio(float radio);
	std::shared_ptr<tuxingdrawreturn> draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params) override;
private:
	//QSize calcusuitablerectsize(QSize spacesize, int penwidth, float juxingradio);
	//QRect calcurect(QSize spacesize, int penwidth, float radio);
	QRectF calcurect(DiagramDrawParamsRect* params);
	QSizeF calcusuitablerectsize(DiagramDrawParamsRect* params);
};

class DiagramDrawerYuanxing : public IDiagramDrawer
{
public:
	//yuanxingjiedian(tuxingjiedianparams* params);
	//void draw(QPainter* painter) override;
	std::shared_ptr<tuxingdrawreturn> draw(QPainter &painter, std::shared_ptr<IDidgramDrawParams> params)override;
private:
	int calcuyuanxingbanjing(DiagramDrawParamsCircle* params);
	//int calcuyuanxingbanjing(QSize spacesize, int penwidth);
};

class DiagramDrawInterface
{
public:
	static std::shared_ptr<tuxingdrawreturn> draw(QPainter &painter, std::shared_ptr<IDidgramDrawParams> params);
private:
	static std::shared_ptr<IDiagramDrawer> create(ShapeType type);
};

//class factorytuxingparams
//{
//public:
//	static std::shared_ptr<IDidgramDrawParams> create(ShapeType type);
//};
