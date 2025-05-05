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

	virtual void serialize(QDataStream& out) const;
	virtual void deserialize(QDataStream& in);

	ShapeType m_type;
	qreal m_scale;
	QPen m_pen;
	QBrush m_brush;
	QSizeF m_spacesize;
	qreal m_rotate;

	void serShapetype(QDataStream& out) const;
	void serScale(QDataStream& out)const;
	void serPen(QDataStream& out)const;
	void serBrush(QDataStream& out)const;
	void serSpacesize(QDataStream& out)const;
	void serRotate(QDataStream& out)const;

	void deserShapetype(QDataStream& in);
	void deserScale(QDataStream& in);
	void deserPen(QDataStream& in);
	void deserBrush(QDataStream& in);
	void deserSpacesize(QDataStream& in);
	void deserRotate(QDataStream& in);

};

class DiagramMimedataRect : public DiagramMimedata
{
public:
	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;

	void serRectRadio(QDataStream& out)const;
	void deserRectRadio(QDataStream& in);

	qreal m_rectRadio;
};

class DiagramMimedataCircle : public DiagramMimedata
{
public:
	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;

	void serCircleRadio(QDataStream& out) const;
	void deserCircleRadio(QDataStream& in);

	qreal m_circleradio;
};

class DiagramMimedataTriangle : public DiagramMimedata
{
public:
	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;

	void serSideRadios(QDataStream& out)const;
	void serEdgetype(QDataStream& out)const;
	void deserSideRadios(QDataStream& in);
	void deserEdgetype(QDataStream& in);

	DiagramDrawParamsTriangle::TriangleSizeRadios m_triangleSideRadios;
	DiagramDrawParamsTriangle::EdgeType m_triangleEdgeType;
};

class DiagramMimedataLine : public DiagramMimedata
{
public:
	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;


};

