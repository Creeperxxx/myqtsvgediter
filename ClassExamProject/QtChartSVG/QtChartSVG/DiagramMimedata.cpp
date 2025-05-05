#include "DiagramMimedata.h"


QDataStream& operator<<(QDataStream& out, const DiagramMimedata& myClass)
{
	myClass.serialize(out);
	return out;
	

}

QDataStream& operator>>(QDataStream& in, DiagramMimedata& myClass)
{
	myClass.deserialize(in);
	return in;
}

void DiagramMimedata::serialize(QDataStream& out) const
{
	serShapetype(out);
	serScale(out);
	serPen(out);
	serBrush(out);
	serSpacesize(out);
	serRotate(out);
}

void DiagramMimedata::deserialize(QDataStream& in)
{
	deserShapetype(in);
	deserScale(in);
	deserPen(in);
	deserBrush(in);
	deserSpacesize(in);
	deserRotate(in);
}

void DiagramMimedata::serShapetype(QDataStream& out) const
{
	out << static_cast<int>(m_type);
}

void DiagramMimedata::serScale(QDataStream& out)const
{
	out << m_scale;
}

void DiagramMimedata::serPen(QDataStream& out)const
{
	out << m_pen.color() << m_pen.widthF();
}

void DiagramMimedata::serBrush(QDataStream& out)const
{
	out << m_brush.color();
}


void DiagramMimedata::serSpacesize(QDataStream& out) const
{
	out << m_spacesize;
}

void DiagramMimedata::serRotate(QDataStream& out) const
{
	out << m_rotate;
}








void DiagramMimedata::deserShapetype(QDataStream& in)
{
	int value;
	in >> value;
	m_type = static_cast<ShapeType>(value);
}

void DiagramMimedata::deserScale(QDataStream& in)
{
	qreal value;
	in >> value;
	m_scale = value;
}

void DiagramMimedata::deserPen(QDataStream& in)
{
	QColor color;
	qreal width;
	in >> color >> width;

	m_pen = QPen(color, width);
}

void DiagramMimedata::deserBrush(QDataStream& in)
{
	QColor color;
	in >> color;

	m_brush = QBrush(color);
}


void DiagramMimedata::deserSpacesize(QDataStream& in)
{
	QSizeF value;
	in >> value;
    m_spacesize = value;
}

void DiagramMimedata::deserRotate(QDataStream& in)
{
	in >> m_rotate;
}

void DiagramMimedataRect::serialize(QDataStream& out) const
{
	DiagramMimedata::serialize(out);
	serRectRadio(out);
}

void DiagramMimedataRect::deserialize(QDataStream& in)
{
	DiagramMimedata::deserialize(in);
	deserRectRadio(in);
}

void DiagramMimedataRect::serRectRadio(QDataStream& out) const
{
	out << m_rectRadio;
}

void DiagramMimedataRect::deserRectRadio(QDataStream& in)
{
	in >> m_rectRadio;
}

void DiagramMimedataCircle::serialize(QDataStream& out) const
{
	DiagramMimedata::serialize(out);
	serCircleRadio(out);
}

void DiagramMimedataCircle::deserialize(QDataStream& in)
{
	DiagramMimedata::deserialize(in);
	deserCircleRadio(in);
}

void DiagramMimedataCircle::serCircleRadio(QDataStream& out) const
{
	out << m_circleradio;
}

void DiagramMimedataCircle::deserCircleRadio(QDataStream& in)
{
	in >> m_circleradio;
}

void DiagramMimedataTriangle::serialize(QDataStream& out) const
{
	DiagramMimedata::serialize(out);
	serSideRadios(out);
	serEdgetype(out);
}

void DiagramMimedataTriangle::deserialize(QDataStream& in)
{
	DiagramMimedata::deserialize(in);
	deserSideRadios(in);
	deserEdgetype(in);
}

void DiagramMimedataTriangle::serSideRadios(QDataStream& out) const
{
	out << m_triangleSideRadios.m_bottom
		<<m_triangleSideRadios.m_left
		<<m_triangleSideRadios.m_right;
}

void DiagramMimedataTriangle::serEdgetype(QDataStream& out) const
{
	out << static_cast<int>(m_triangleEdgeType);
}

void DiagramMimedataTriangle::deserSideRadios(QDataStream& in) 
{
	in >> m_triangleSideRadios.m_bottom
		>> m_triangleSideRadios.m_left
        >> m_triangleSideRadios.m_right;
}

void DiagramMimedataTriangle::deserEdgetype(QDataStream& in)
{
	int value;
	in >> value;
	m_triangleEdgeType = static_cast<DiagramDrawParamsTriangle::EdgeType>(value);
}

void DiagramMimedataLine::serialize(QDataStream& out) const
{
	DiagramMimedata::serialize(out);
}

void DiagramMimedataLine::deserialize(QDataStream& in)
{
	DiagramMimedata::deserialize(in);
}
