#pragma once
#include <memory>
#include <qpoint.h>
#include <qbrush.h>
#include <qpen.h>
#include <stdexcept>
#include <qstring.h>
//#include "huabu.h"
//#include "tuxing.h"
//#include "huabu.h"


//class DiagramItem;

enum class ShapeType
{
	Rect,
	Circle,
	Triangle,
	Line
};

namespace ShapeTypeTool
{
	QString shapetypeEnumToQstring(ShapeType type);
	ShapeType shapetypeQstringToEnum(const QString& type);
}

//旋转角度由double变int


class IDidgramDrawParams
{
public:
	virtual ~IDidgramDrawParams() {}
	QPointF m_center;
	QSizeF m_spacesize;
	ShapeType m_type;
	qreal m_scale;
	QPen m_pen;
	QBrush m_brush;
};

class DiagramDrawParamsRect : public IDidgramDrawParams
{
public:
	~DiagramDrawParamsRect() {}
	double m_boundingrectradio;
	int m_rectrotate;
};

class DiagramDrawParamsCircle :public IDidgramDrawParams
{
public:
	qreal m_boundingrectradio;
	int m_circlerotate;

	~DiagramDrawParamsCircle() {}
};

class DiagramDrawParamsTriangle : public IDidgramDrawParams
{
public:

	enum class EdgeType
	{
		Left,
		Right,
		Bottom
	};
	static EdgeType edgetypeStringToEnum(const QString& edgetype);
	static QString edgetypeEnumToString(EdgeType edgetype);
	~DiagramDrawParamsTriangle() {}
	class TriangleSizeRadios
	{
	public:
		TriangleSizeRadios();
		TriangleSizeRadios(double bottom, double left, double right);
		qreal m_bottom;
		qreal m_left;
		qreal m_right;
	};
	TriangleSizeRadios m_triangleSizeRadios;
	EdgeType m_edgetype;
	int m_rotationAngle;
};

class DiagramDrawParamsLine : public IDidgramDrawParams
{
public:
	int m_rotationAngle;
};







