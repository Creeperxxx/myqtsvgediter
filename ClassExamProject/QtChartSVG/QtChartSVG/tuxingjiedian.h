#pragma once
#include <qpoint.h>
#include <qsize.h>
#include <qpainter.h>
#include "config.h"
#include <qdebug.h>
#include <stdexcept>
#include "configmanager.h"
#include "drawtool.h"
#include "qpainterpath.h"
#include <unordered_map>
#include <functional>

constexpr const double linetolerance = 5.0;

class DrawResult
{

public:
	virtual bool iscontainPoint(QPointF point) = 0;
	virtual QPainterPath getPainterPath() = 0;


	QPen m_painterpen;
	QBrush m_painterbrush;
};

class DrawResultRect : public DrawResult
{
public:
	bool iscontainPoint(QPointF point) override;
	QPainterPath getPainterPath() override;

	QPolygonF m_rect;
};

class DrawResultCircle :public DrawResult
{
public:
	bool iscontainPoint(QPointF point) override;
	QPainterPath getPainterPath() override;
	QPolygonF m_circle;
};

class DrawResultTriangle :public DrawResult
{
public:
	bool iscontainPoint(QPointF point)override;
	QPainterPath getPainterPath() override;

	QPolygonF m_triangle;
};

class DrawResultLine : public DrawResult
{
public:
	bool iscontainPoint(QPointF point) override;
	QPainterPath getPainterPath() override;
	QLineF m_line;
private:
	qreal distanceToLine(const QLineF& line, const QPointF& point);
};





class IDiagramDrawer
{
public:
	virtual std::shared_ptr<DrawResult> draw(QPainter &painter, std::shared_ptr<IDidgramDrawParams> params) = 0;
	virtual void build() = 0;
	virtual void draw(QPainter& painter) = 0;
	virtual std::shared_ptr<DrawResult> getResult() = 0;

};

class DiagramDrawerRect : public IDiagramDrawer
{
public:
	DiagramDrawerRect(std::shared_ptr<IDidgramDrawParams> params);
	void build() override;
	void draw(QPainter& painter) override;



	std::shared_ptr<DrawResult> draw(QPainter& painter, std::shared_ptr<IDidgramDrawParams> params) override;
private:
	QPolygonF calcuRect(DiagramDrawParamsRect* params);
	QPolygonF calcuBasicalRect(DiagramDrawParamsRect* params);
	QTransform calcuRotateTransform(DiagramDrawParamsRect* params, QPointF center);
	QTransform calcuTranslateTransform(DiagramDrawParamsRect* params, QPointF center);
	QTransform calcuScaleTransform(DiagramDrawParamsRect* params, QRectF rect);

	std::shared_ptr<DiagramDrawParamsRect> m_params;
	bool m_isbuild;
	QPolygonF m_rect;
};

class DiagramDrawerCircle : public IDiagramDrawer
{
public:
	std::shared_ptr<DrawResult> draw(QPainter &painter, std::shared_ptr<IDidgramDrawParams> params)override;
private:
	QPolygonF calcuBasicalCircle(DiagramDrawParamsCircle* params, QSizeF suitablesize);
	QTransform calcurotatetransform(DiagramDrawParamsCircle* params);
	qreal calcuscaleFactor(DiagramDrawParamsCircle* params, QPolygonF diagram, qreal penwidth);
	QTransform calcuscaleTransform(DiagramDrawParamsCircle* params, qreal scale);
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
	static DiagramDrawInterface& getInstance();
	void addDrawerCreator(ShapeType type, std::function<std::shared_ptr<IDiagramDrawer>(std::shared_ptr<IDidgramDrawParams>)> drawer);
	std::shared_ptr<IDiagramDrawer> getDrawer(ShapeType type, std::shared_ptr<IDidgramDrawParams> params);
private:
	std::unordered_map<ShapeType, std::function<std::shared_ptr<IDiagramDrawer>(std::shared_ptr<IDidgramDrawParams>)>> m_drawerMap;
};

