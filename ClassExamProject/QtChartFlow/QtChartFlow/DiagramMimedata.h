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
	void serShapetype(QDataStream& out, const DiagramMimedata& myclass) const;
	void serScale(QDataStream& out, const DiagramMimedata& myclass)const;
	void serPen(QDataStream& out, const DiagramMimedata& myclass)const;
	void serBrush(QDataStream& out, const DiagramMimedata& myclass)const;
	void serRectRadio(QDataStream& out, const DiagramMimedata& myclass)const;
	void serRectRotate(QDataStream& out, const DiagramMimedata& myclass)const;
	void serCircleRadio(QDataStream& out, const DiagramMimedata& myclass)const;
	void serCircleRotate(QDataStream& out, const DiagramMimedata& myclass)const;
	void serTriangleSideRadios(QDataStream& out, const DiagramMimedata& myclass)const;
	void serTriangleEdgetype(QDataStream& out, const DiagramMimedata& myclass)const;
	void serTriangleRotate(QDataStream& out, const DiagramMimedata& myclass)const;
	void serLineRotate(QDataStream& out, const DiagramMimedata& myclass)const;

	void deserShapetype(QDataStream& in, DiagramMimedata& myclass);
	void deserScale(QDataStream& in, DiagramMimedata& myclass);
	void deserPen(QDataStream& in,  DiagramMimedata& myclass);
	void deserBrush(QDataStream& in,  DiagramMimedata& myclass);
	void deserRectRadio(QDataStream& in,  DiagramMimedata& myclass);
	void deserRectRotate(QDataStream& in,  DiagramMimedata& myclass);
	void deserCircleRadio(QDataStream& in,  DiagramMimedata& myclass);
	void deserCircleRotate(QDataStream& in,  DiagramMimedata& myclass);
	void deserTriangleSideRadios(QDataStream& in,  DiagramMimedata& myclass);
	void deserTriangleEdgetype(QDataStream& in,  DiagramMimedata& myclass);
	void deserTriangleRotate(QDataStream& in,  DiagramMimedata& myclass);
	void deserLineRotate(QDataStream& in, DiagramMimedata& myclass);

};

