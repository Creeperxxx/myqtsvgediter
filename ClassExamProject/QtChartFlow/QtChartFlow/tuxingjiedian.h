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
	//QRectF m_rect;
	QPolygonF m_rect;
};

class DrawResultCircle :public DrawResult
{
public:
	QPolygonF m_circle;
	//center
};

class DrawResultTriangle :public DrawResult
{
public:
	QPolygonF m_triangle;
};

class DrawResultLine : public DrawResult
{
public:
	QLineF m_line;
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
	//QRectF calcurect(DiagramDrawParamsRect* params);
	//QSizeF calcusuitablerectsize(DiagramDrawParamsRect* params);
	QPolygonF calcuRect(DiagramDrawParamsRect* params, qreal penwidth);
	QPolygonF calcuBasicalRect(DiagramDrawParamsRect* params);
	QTransform calcuRotateTransform(DiagramDrawParamsRect* params);
	QTransform calcuTranslateTransform(QPolygonF diagram, DiagramDrawParamsRect* params);
	QTransform calcuScaleTransform(QPolygonF diagram, DiagramDrawParamsRect* params, qreal penwidth);

	
};

class DiagramDrawerCircle : public IDiagramDrawer
{
public:
	std::shared_ptr<DrawResult> draw(QPainter &painter, std::shared_ptr<IDidgramDrawParams> params)override;
private:
	//int calcuyuanxingbanjing(DiagramDrawParamsCircle* params);
	//QRectF calcuboundingrect(DiagramDrawParamsCircle* params);
	QPolygonF calcuBasicalCircle(DiagramDrawParamsCircle* params, QSizeF suitablesize, qreal penwidth);
	QTransform calcurotatetransform(DiagramDrawParamsCircle* params);
	qreal calcuscaleFactor(DiagramDrawParamsCircle* params, QPolygonF diagram, qreal penwidth);
	QTransform calcuscaleTransform(DiagramDrawParamsCircle* params, qreal scale, qreal penwidth);
	QSizeF calcuboundingrectsize(DiagramDrawParamsCircle* params, qreal penwidth);
};

class DiagramDrawerTriangle : public IDiagramDrawer
{
public:
	std::shared_ptr<DrawResult> draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params);
private:
	QPolygonF calcuTriangle(DiagramDrawParamsTriangle* params);
	QPolygonF calcuUpsidedowntriangle(double bottom, double left, double right);
	QTransform calcuRotateTransform(double bottom, double left, double right, DiagramDrawParamsTriangle::EdgeType edgetype, double angle);
	QTransform calcuTranslateTransfrom(QPointF trianglecenter, QPointF widgetcenter);
	QTransform calcuScaleTransform(QRectF trianglerect, DiagramDrawParamsTriangle* params);
	QRectF calcuwidgetrect(QPointF cente, QSizeF size);
};

class DiagramDrawerLine : public IDiagramDrawer
{
public:
	std::shared_ptr<DrawResult> draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params);
private:
	QLineF calcuLine(DiagramDrawParamsLine* params);
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
