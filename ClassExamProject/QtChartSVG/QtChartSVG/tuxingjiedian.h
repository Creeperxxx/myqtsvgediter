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
	virtual void build() = 0;
	virtual void draw(QPainter& painter) = 0;
	virtual std::shared_ptr<DrawResult> getResult() = 0;
	virtual std::shared_ptr<IDidgramDrawParams> getParams() = 0;

};

class DiagramDrawerRect : public IDiagramDrawer
{
public:
	DiagramDrawerRect(std::shared_ptr<IDidgramDrawParams> params);
	void build() override;
	void draw(QPainter& painter) override;
	std::shared_ptr<DrawResult> getResult() override;
	std::shared_ptr<IDidgramDrawParams> getParams() override;



private:
	QPolygonF calcuRect();
	QPolygonF calcuBasicalRect();
	QTransform calcuRotateTransform(QPointF center);
	QTransform calcuTranslateTransform(QPointF center);
	QTransform calcuScaleTransform(QRectF rect);

	std::shared_ptr<DiagramDrawParamsRect> m_params;
	std::shared_ptr<DrawResultRect> m_result;
	//bool m_isbuild;
	QPolygonF m_rect;
};

class DiagramDrawerCircle : public IDiagramDrawer
{
public:
	DiagramDrawerCircle(std::shared_ptr<IDidgramDrawParams> params);
	void build() override;
	void draw(QPainter& painter) override;
	std::shared_ptr<DrawResult> getResult() override;
	std::shared_ptr<IDidgramDrawParams> getParams() override;
private:
	QPolygonF calcuBasicalCircle();
	QTransform calcurotatetransform(QPointF center);
	qreal calcuscaleFactor(QRectF bound);
	QTransform calcuscaleTransform();
	QTransform calcuTranslateTransform(QPointF mycenter);

	std::shared_ptr<DiagramDrawParamsCircle> m_params;
	QPolygonF m_circle;
	qreal m_scale;
	const qreal m_initheight;
	std::shared_ptr<DrawResultCircle> m_result;
};

class DiagramDrawerTriangle : public IDiagramDrawer
{
public:
	DiagramDrawerTriangle(std::shared_ptr<IDidgramDrawParams> params);
	void build() override;
	void draw(QPainter& painter) override;
    std::shared_ptr<DrawResult> getResult() override;
	std::shared_ptr<IDidgramDrawParams> getParams() override;
private:
	QPolygonF calcuTriangle();
	QPolygonF calcuBasicalTriangle();
	QTransform calcuRotateTransform();
	QTransform calcuTranslateTransfrom(QPointF trianglecenter);
	QTransform calcuScaleTransform(QRectF trianglerect);

	std::shared_ptr<DiagramDrawParamsTriangle> m_params;
	std::shared_ptr<DrawResultTriangle> m_result;
	QPolygonF m_triangle;
	
};

class DiagramDrawerLine : public IDiagramDrawer
{
public:
	DiagramDrawerLine(std::shared_ptr<IDidgramDrawParams> params);
	void build()override;
	void draw(QPainter& painter)override;
    std::shared_ptr<DrawResult> getResult()override;
	std::shared_ptr<IDidgramDrawParams> getParams()override;
private:
	QLineF calcuLine();
	
	std::shared_ptr<DiagramDrawParamsLine> m_params;
	std::shared_ptr<DrawResultLine> m_result;
	QLineF m_line;
};




class DiagramDrawInterface
{
public:					                       
	static DiagramDrawInterface& getInstance();
	DiagramDrawInterface& addDrawerCreator(ShapeType type, std::function<std::shared_ptr<IDiagramDrawer>(std::shared_ptr<IDidgramDrawParams>)> drawer);
	std::shared_ptr<IDiagramDrawer> getDrawer(std::shared_ptr<IDidgramDrawParams> params);
private:
	std::unordered_map<ShapeType, std::function<std::shared_ptr<IDiagramDrawer>(std::shared_ptr<IDidgramDrawParams>)>> m_drawerMap;
};

