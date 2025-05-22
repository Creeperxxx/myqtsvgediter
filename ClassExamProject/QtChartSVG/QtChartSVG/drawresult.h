#pragma once
#include "qpainterpath.h"
#include "qpoint.h"
#include "qpen.h"
#include "qfont.h"

class DrawResult
{

public:
	virtual bool iscontainPoint(QPointF point) = 0;
	virtual QPainterPath getPainterPath() = 0;

	virtual QRect getBoundingRect() = 0;

	QPen m_painterpen;
	QBrush m_painterbrush;
};

class DrawResultRect : public DrawResult
{
public:
	bool iscontainPoint(QPointF point) override;
	QPainterPath getPainterPath() override;
	QRect getBoundingRect()override;

	QPolygonF m_rect;
};

class DrawResultCircle :public DrawResult
{
public:
	bool iscontainPoint(QPointF point) override;
	QPainterPath getPainterPath() override;
	QRect getBoundingRect()override;
	QPolygonF m_circle;
};

class DrawResultTriangle :public DrawResult
{
public:
	bool iscontainPoint(QPointF point)override;
	QPainterPath getPainterPath() override;
	QRect getBoundingRect()override;

	QPolygonF m_triangle;
};

class DrawResultLine : public DrawResult
{
public:
	bool iscontainPoint(QPointF point) override;
	QPainterPath getPainterPath() override;
	QRect getBoundingRect()override;
	QLineF m_line;
private:
	qreal distanceToLine(const QLineF& line, const QPointF& point);
};

class DrawResultMouse : public DrawResult
{
public:
	bool iscontainPoint(QPointF point)override;
	QPainterPath getPainterPath()override;
	QRect getBoundingRect() override;

	QPainterPath m_path;
};

class DrawResultText : public DrawResult
{
public:

	QPainterPath getPainterPath()override;
	bool iscontainPoint(QPointF point)override;
	QRect getBoundingRect()override;
	QString m_text;
	QFont m_font;
	QRectF m_rect;
};
