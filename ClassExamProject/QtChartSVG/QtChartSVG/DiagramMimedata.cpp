#include "DiagramMimedata.h"


QDataStream& operator<<(QDataStream& out, const DiagramMimedata& myClass)
{
	myClass.serShapetype(out);
	myClass.serScale(out);
	myClass.serPen(out);
	myClass.serBrush(out);
	myClass.serRectRadio(out);
	myClass.serRectRotate(out);
	myClass.serCircleRadio(out);
	myClass.serCircleRotate(out);
	myClass.serTriangleSideRadios(out);
	myClass.serTriangleEdgetype(out);
	myClass.serTriangleRotate(out);
	myClass.serLineRotate(out);
	myClass.serSpacesize(out);

	return out;

}

QDataStream& operator>>(QDataStream& in, DiagramMimedata& myClass)
{
	myClass.deserShapetype(in);
	myClass.deserScale(in);
	myClass.deserPen(in);
	myClass.deserBrush(in);
	myClass.deserRectRadio(in);
	myClass.deserRectRotate(in);
	myClass.deserCircleRadio(in);
	myClass.deserCircleRotate(in);
	myClass.deserTriangleSideRadios(in);
	myClass.deserTriangleEdgetype(in);
	myClass.deserTriangleRotate(in);
	myClass.deserLineRotate(in);
	myClass.deserSpacesize(in);

	return in;

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

void DiagramMimedata::serRectRadio(QDataStream& out)const
{
	out << m_rectradio.has_value();
	if (m_rectradio.has_value())
		out << m_rectradio.value();
}

void DiagramMimedata::serRectRotate(QDataStream& out)const
{
	out << m_rectRotate.has_value();
	if (m_rectRotate.has_value())
		out << m_rectRotate.value();
}

void DiagramMimedata::serCircleRadio(QDataStream& out)const
{
	out << m_circleradio.has_value();
	if (m_circleradio.has_value())
		out << m_circleradio.value();
}

void DiagramMimedata::serCircleRotate(QDataStream& out)const
{
	out << m_circlerotate.has_value();
	if (m_circlerotate.has_value())
		out << m_circlerotate.value();
}

void DiagramMimedata::serTriangleSideRadios(QDataStream& out)const
{
	out << m_triangleSideRadios.has_value();
	if (m_triangleSideRadios.has_value())
	{
		out << m_triangleSideRadios.value().m_bottom
			<< m_triangleSideRadios.value().m_left
			<< m_triangleSideRadios.value().m_right;
	}
}

void DiagramMimedata::serTriangleEdgetype(QDataStream& out)const
{
	out << m_triangleEdgeType.has_value();
	if (m_triangleEdgeType.has_value())
		out << static_cast<int>(m_triangleEdgeType.value());
}

void DiagramMimedata::serTriangleRotate(QDataStream& out)const
{
	out << m_triangleRotate.has_value();
	if (m_triangleRotate.has_value())
		out << m_triangleRotate.value();
}

void DiagramMimedata::serLineRotate(QDataStream& out)const
{
	out << m_linerotate.has_value();
	if (m_linerotate.has_value())
		out << m_linerotate.value();
}

void DiagramMimedata::serSpacesize(QDataStream& out) const
{
	out << m_spacesize;
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

void DiagramMimedata::deserRectRadio(QDataStream& in)
{
	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		qreal value;
		in >> value;
		m_rectradio = value;
	}
}

void DiagramMimedata::deserRectRotate(QDataStream& in)
{
	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		int value;
		in >> value;
		m_rectRotate = value;
	}

}

void DiagramMimedata::deserCircleRadio(QDataStream& in)
{
	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		qreal value;
		in >> value;
		m_circleradio = value;
	}
}

void DiagramMimedata::deserCircleRotate(QDataStream& in)
{
	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		int value;
		in >> value;
		m_circlerotate = value;
	}
}

void DiagramMimedata::deserTriangleSideRadios(QDataStream& in)
{
	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		qreal bottom;
		qreal left;
		qreal right;
		in >> bottom >> left >> right;
		m_triangleSideRadios = DiagramDrawParamsTriangle::TriangleSizeRadios(bottom, left, right);
	}
}

void DiagramMimedata::deserTriangleEdgetype(QDataStream& in)
{
	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		int value;
		in >> value;
		m_triangleEdgeType = static_cast<DiagramDrawParamsTriangle::EdgeType>(value);
	}
}

void DiagramMimedata::deserTriangleRotate(QDataStream& in)
{
	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		int value;
		in >> value;
		m_triangleRotate = value;
	}
}

void DiagramMimedata::deserLineRotate(QDataStream& in)
{

	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		int value;
		in >> value;
		m_linerotate = value;
	}
}

void DiagramMimedata::deserSpacesize(QDataStream& in)
{
	QSizeF value;
	in >> value;
    m_spacesize = value;
}


