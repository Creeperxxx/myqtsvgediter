#pragma once
#include <qstring.h>
#include "drawtool.h"
#include "optional"
#include <qdatastream.h>

class DiagramMimedata
{
public:
	friend QDataStream& operator<<(QDataStream& out, const DiagramMimedata& myClass);
	friend QDataStream& operator>>(QDataStream& in, DiagramMimedata& myClass);

	ShapeType m_type;
	qreal m_scale;
	QPen m_pen;
	QBrush m_brush;

	std::optional<qreal> m_rectradio;
	std::optional<int> m_rectRotate;

	std::optional<qreal> m_circleradio;
	std::optional<int> m_circlerotate;

	std::optional<DiagramDrawParamsTriangle::TriangleSizeRadios> m_triangleSideRadios;
	std::optional<DiagramDrawParamsTriangle::EdgeType> m_triangleEdgeType;
	std::optional<int> m_triangleRotate;

	std::optional<int> m_linerotate;
	void serShapetype(QDataStream& out) const;
	void serScale(QDataStream& out)const;
	void serPen(QDataStream& out)const;
	void serBrush(QDataStream& out)const;
	void serRectRadio(QDataStream& out)const;
	void serRectRotate(QDataStream& out)const;
	void serCircleRadio(QDataStream& out)const;
	void serCircleRotate(QDataStream& out)const;
	void serTriangleSideRadios(QDataStream& out)const;
	void serTriangleEdgetype(QDataStream& out)const;
	void serTriangleRotate(QDataStream& out)const;
	void serLineRotate(QDataStream& out)const;

	void deserShapetype(QDataStream& in);
	void deserScale(QDataStream& in);
	void deserPen(QDataStream& in);
	void deserBrush(QDataStream& in);
	void deserRectRadio(QDataStream& in);
	void deserRectRotate(QDataStream& in);
	void deserCircleRadio(QDataStream& in);
	void deserCircleRotate(QDataStream& in);
	void deserTriangleSideRadios(QDataStream& in);
	void deserTriangleEdgetype(QDataStream& in);
	void deserTriangleRotate(QDataStream& in);
	void deserLineRotate(QDataStream& in);

};

