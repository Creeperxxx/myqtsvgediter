#ifndef DRAWRESULT_H
#define DRAWRESULT_H
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

	void setPen(QPen pen);
	QPen getPen();
	void setBrush(QBrush brush);
	QBrush getBrush();

private:
	QPen m_painterpen;
	QBrush m_painterbrush;
};

class DrawResultRect : public DrawResult
{
public:
	bool iscontainPoint(QPointF point) override;
	QPainterPath getPainterPath() override;
	QRect getBoundingRect()override;
	
	void setRect(QPolygonF rect);
	QPolygonF getRect();
private:
	QPolygonF m_rect;
};

class DrawResultCircle :public DrawResult
{
public:
	bool iscontainPoint(QPointF point) override;
	QPainterPath getPainterPath() override;
	QRect getBoundingRect()override;

	void setCircle(QPolygonF circle);
	QPolygonF getCircle();

private:
	QPolygonF m_circle;
};

class DrawResultTriangle :public DrawResult
{
public:
	bool iscontainPoint(QPointF point)override;
	QPainterPath getPainterPath() override;
	QRect getBoundingRect()override;

	void setTriangle(QPolygonF triangle);
	QPolygonF getTriangle();

private:
	QPolygonF m_triangle;
};

class DrawResultLine : public DrawResult
{
public:
	bool iscontainPoint(QPointF point) override;
	QPainterPath getPainterPath() override;
	QRect getBoundingRect()override;

	void setLine(QLineF line);
	QLineF getLine();
private:
	qreal distanceToLine(const QLineF& line, const QPointF& point);

	QLineF m_line;
};

class DrawResultMouse : public DrawResult
{
public:
	bool iscontainPoint(QPointF point)override;
	QPainterPath getPainterPath()override;
	QRect getBoundingRect() override;

	void setPath(QPainterPath path);
	QPainterPath getPath();
private:
	QPainterPath m_path;
};

class DrawResultText : public DrawResult
{
public:
	QPainterPath getPainterPath()override;
	bool iscontainPoint(QPointF point)override;
	QRect getBoundingRect()override;
	
	void setText(QString text);
	QString getText();
	void setFont(QFont font);
	QFont setFont();
	void setRect(QRectF rect);
	QRectF getRect();
private:
	QString m_text;
	QFont m_font;
	QRectF m_rect;
};

class DrawResultPentagon : public DrawResult
{
public:
	QPainterPath getPainterPath() override;
	bool iscontainPoint(QPointF point) override;
	QRect getBoundingRect()override;

	void setPentagon(QPolygonF pentagon);
    QPolygonF getPentagon();
private:
	QPolygonF m_pentagon;
};

class DrawResultHexagon : public DrawResult
{
public:
	QPainterPath getPainterPath() override;
	bool iscontainPoint(QPointF point) override;
	QRect getBoundingRect()override;

	void setHexagon(QPolygonF hexagon);
    QPolygonF getHexagon();
private:

	QPolygonF m_hexagon;
};

class DrawResultStar : public DrawResult
{
public:
	QPainterPath getPainterPath() override;
	bool iscontainPoint(QPointF point) override;
	QRect getBoundingRect()override;

	void setStar(QPolygonF star);
    QPolygonF getStar();
private:

	QPolygonF m_star;

};



#endif // DRAWRESULT_H
