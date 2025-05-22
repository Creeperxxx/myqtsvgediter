#pragma once
#include <memory>
#include <qpainter.h>
#include "drawresult.h"
#include "diagramdrawparams.h"

class IDiagramDrawer
{
public:

	virtual void build() = 0;
	virtual void draw(QPainter& painter) = 0;
	virtual std::shared_ptr<DrawResult> getResult() = 0;

protected:
	QPoint calcuRealCenter(QPoint center, int hmove, int vmove);
};

class DiagramDrawerRect : public IDiagramDrawer
{
public:
	DiagramDrawerRect(std::shared_ptr<IDidgramDrawParams> param);
	void build() override;
	void draw(QPainter& painter) override;
	std::shared_ptr<DrawResult> getResult() override;



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
private:
	QPolygonF calcuBasicalCircle();
	QTransform calcurotatetransform(QPointF center);
	qreal calcuscaleFactor(QRectF bound);
	QTransform calcuscaleTransform(QPointF center);
	QTransform calcuTranslateTransform(QPointF mycenter, QPointF targetpoint);

	std::shared_ptr<DiagramDrawParamsCircle> m_params;
	QPolygonF m_circle;
	qreal m_scale;
	const int m_initheight;
	std::shared_ptr<DrawResultCircle> m_result;
};

class DiagramDrawerTriangle : public IDiagramDrawer
{
public:
	DiagramDrawerTriangle(std::shared_ptr<IDidgramDrawParams> params);
	void build() override;
	void draw(QPainter& painter) override;
	std::shared_ptr<DrawResult> getResult() override;
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
private:
	QLineF calcuLine();

	std::shared_ptr<DiagramDrawParamsLine> m_params;
	std::shared_ptr<DrawResultLine> m_result;
	QLineF m_line;
};

class DiagramDrawerMouse : public IDiagramDrawer
{
public:
	DiagramDrawerMouse(std::shared_ptr<IDidgramDrawParams> params);
	void build()override;
	void draw(QPainter& painter)override;
	std::shared_ptr<DrawResult> getResult()override;


	std::shared_ptr<DiagramDrawParamsMouse> m_params;
	std::shared_ptr<DrawResultMouse> m_result;
};


class DiagramDrawerText : public IDiagramDrawer
{
public:
	DiagramDrawerText(std::shared_ptr<IDidgramDrawParams> params);
	void build() override;
	void draw(QPainter& painter);
	std::shared_ptr<DrawResult> getResult() override;

	std::shared_ptr<DiagramDrawParamsText> m_params;
	std::shared_ptr<DrawResultText> m_result;
};

class DiagramDrawerChoose : public IDiagramDrawer
{
public:
	DiagramDrawerChoose(std::shared_ptr<IDidgramDrawParams> params);
	void build() override;
	void draw(QPainter& painter) override;
	std::shared_ptr<DrawResult> getResult() override;


	std::shared_ptr<DiagramDrawParamsChoose> m_params;
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
