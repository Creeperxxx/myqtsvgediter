#pragma once
#include <qpoint.h>
#include <qsize.h>
#include <qpainter.h>
#include "config.h"
#include <qdebug.h>
#include <stdexcept>
#include "configmanager.h"
#include "drawtool.h"



class DrawResult
{
public:
	QPen m_painterpen;
	QBrush m_painterbrush;
};

class DrawResultRect : public DrawResult
{
public:
	QRectF m_rect;
};

class DrawResultCircle :public DrawResult
{
public:
	int m_r;
	//center
};

class DrawResultTriangle :public DrawResult
{
public:

};

class DrawResultRightTriangle :public DrawResult
{

};




class IDiagramDrawer
{
public:
	virtual std::shared_ptr<DrawResult> draw(QPainter &painter, std::shared_ptr<IDidgramDrawParams> params) = 0;
};

class DiagramDrawerRect : public IDiagramDrawer
{
public:
	std::shared_ptr<DrawResult> draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params) override;
private:
	QRectF calcurect(DiagramDrawParamsRect* params);
	QSizeF calcusuitablerectsize(DiagramDrawParamsRect* params);
};

class DiagramDrawerCircle : public IDiagramDrawer
{
public:
	std::shared_ptr<DrawResult> draw(QPainter &painter, std::shared_ptr<IDidgramDrawParams> params)override;
private:
	int calcuyuanxingbanjing(DiagramDrawParamsCircle* params);
};

class DiagramDrawerTriangle : public IDiagramDrawer
{
public:
	std::shared_ptr<DrawResult> draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params);
};

class DiagramDrawerRightTriangle :public IDiagramDrawer
{
public:
	std::shared_ptr<DrawResult> draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params);
};










class DiagramDrawInterface
{
public:
	static std::shared_ptr<DrawResult> draw(QPainter &painter, std::shared_ptr<IDidgramDrawParams> params);
private:
	static std::shared_ptr<IDiagramDrawer> create(ShapeType type);
};

//class factorytuxingparams
//{
//public:
//	static std::shared_ptr<IDidgramDrawParams> create(ShapeType type);
//};
