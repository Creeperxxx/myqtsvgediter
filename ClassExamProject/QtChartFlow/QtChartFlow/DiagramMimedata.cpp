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

	return out;

	//out << static_cast<int>(myClass.m_type);


	//// 序列化 m_rectradio
	//out << myClass.m_rectradio.has_value();
	//if (myClass.m_rectradio.has_value()) {
	//	out << myClass.m_rectradio.value();
	//}

	////序列化m_rectrotate
	//out << myClass.m_rectRotate.has_value();
	//if (myClass.m_rectRotate.has_value())
	//{
	//	out << myClass.m_rectRotate.value();
	//}

	//// 序列化 m_circleradio
	//out << myClass.m_circleradio.has_value();
	//if (myClass.m_circleradio.has_value()) {
	//	out << myClass.m_circleradio.value();
	//}

	//// 序列化 m_circlerotate
	//out << myClass.m_circlerotate.has_value();
	//if (myClass.m_circlerotate.has_value())
	//	out << myClass.m_circlerotate.value();

	//// 序列化 m_triangleSideRadios
	//out << myClass.m_triangleSideRadios.has_value();
	//if (myClass.m_triangleSideRadios.has_value()) {
	//	out << myClass.m_triangleSideRadios.value().m_bottom
	//		<< myClass.m_triangleSideRadios.value().m_left
	//		<< myClass.m_triangleSideRadios.value().m_right;
	//}

	//// 序列化edgetype
	//out << myClass.m_triangleEdgeType.has_value();
	//if (myClass.m_triangleEdgeType.has_value())
	//{
	//	out << static_cast<int>(myClass.m_triangleEdgeType.value());
	//}

	//// 序列化 m_triangleRotate
	//out << myClass.m_triangleRotate.has_value();
	//if (myClass.m_triangleRotate.has_value()) {
	//	out << myClass.m_triangleRotate.value();
	//}

	//out << myClass.m_linerotate.has_value();
	//if (myClass.m_linerotate.has_value())
	//{
	//	out << myClass.m_linerotate.value();
	//}

	//return out;
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

	return in;

	//int type;
	//in >> type;
	//myClass.m_type = static_cast<ShapeType>(type);


	//bool hasValue;

	//// 反序列化 m_rectradio
	//in >> hasValue;
	//if (hasValue) {
	//	double value;
	//	in >> value;
	//	myClass.m_rectradio = value;
	//}
	//else {
	//	myClass.m_rectradio.reset();
	//}

	//// 反序列化 m_rectRotate;
	//in >> hasValue;
	//if (hasValue)
	//{
	//	int value;
	//	in >> value;
	//	myClass.m_rectRotate = value;
	//}
	//else
	//{
	//	myClass.m_rectRotate.reset();
	//}


	//// 反序列化 m_circleradio
	//in >> hasValue;
	//if (hasValue) {
	//	double value;
	//	in >> value;
	//	myClass.m_circleradio = value;
	//}
	//else {
	//	myClass.m_circleradio.reset();
	//}

	//// 反序列化 m_circlerotate
	//in >> hasValue;
	//if (hasValue)
	//{
	//	int value;
	//	in >> value;
	//	myClass.m_circlerotate = value;
	//}
	//else
	//{
	//	myClass.m_circlerotate.reset();
	//}

	//// 反序列化 m_triangleSideRadios
	//in >> hasValue;
	//if (hasValue) {
	//	double bottom, left, right;
	//	in >> bottom >> left >> right;
	//	DiagramDrawParamsTriangle::TriangleSizeRadios radios(bottom, left, right);
	//	myClass.m_triangleSideRadios = radios;
	//}
	//else {
	//	myClass.m_triangleSideRadios.reset();
	//}

	//// 反序列化 m_trianglesideradios
	//in >> hasValue;
	//if (hasValue) {
	//	int value;
	//	in >> value;
	//	myClass.m_triangleEdgeType = static_cast<DiagramDrawParamsTriangle::EdgeType>(value);
	//}
	//else {
	//	myClass.m_triangleEdgeType.reset();
	//}

	//// 反序列化 m_triangleRotate
	//in >> hasValue;
	//if (hasValue) {
	//	double value;
	//	in >> value;
	//	myClass.m_triangleRotate = value;
	//}
	//else {
	//	myClass.m_triangleRotate.reset();
	//}

	//in >> hasValue;
	//if (hasValue)
	//{
	//	double value;
	//	in >> value;
	//	myClass.m_linerotate = value;
	//}
	//else
	//{
	//	myClass.m_linerotate.reset();
	//}

	//return in;
	// TODO: 在此处插入 return 语句
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
