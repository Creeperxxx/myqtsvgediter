#include "DiagramMimedata.h"


QDataStream& operator<<(QDataStream& out, const DiagramMimedata& myClass)
{
	myClass.serShapetype(out, myClass);
	myClass.serScale(out, myClass);
	myClass.serPen(out, myClass);
	myClass.serBrush(out, myClass);
	myClass.serRectRadio(out, myClass);
	myClass.serRectRotate(out, myClass);
	myClass.serCircleRadio(out, myClass);
	myClass.serCircleRotate(out, myClass);
	myClass.serTriangleSideRadios(out, myClass);
	myClass.serTriangleEdgetype(out, myClass);
	myClass.serTriangleRotate(out, myClass);
	myClass.serLineRotate(out, myClass);

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
	myClass.deserShapetype(in, myClass);
	myClass.deserScale(in, myClass);
	myClass.deserPen(in, myClass);
	myClass.deserBrush(in, myClass);
	myClass.deserRectRadio(in, myClass);
	myClass.deserRectRotate(in, myClass);
	myClass.deserCircleRadio(in, myClass);
	myClass.deserCircleRotate(in, myClass);
	myClass.deserTriangleSideRadios(in, myClass);
	myClass.deserTriangleEdgetype(in, myClass);
	myClass.deserTriangleRotate(in, myClass);
	myClass.deserLineRotate(in, myClass);

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

void DiagramMimedata::serShapetype(QDataStream& out, const DiagramMimedata& myclass) const
{
	out << static_cast<int>(myclass.m_type);
}

void DiagramMimedata::serScale(QDataStream& out, const DiagramMimedata& myclass)const
{
	out << myclass.m_scale;
}

void DiagramMimedata::serPen(QDataStream& out, const DiagramMimedata& myclass)const
{
	out << myclass.m_pen.color() << myclass.m_pen.widthF();
}

void DiagramMimedata::serBrush(QDataStream& out, const DiagramMimedata& myclass)const
{
	out << myclass.m_brush.color();
}

void DiagramMimedata::serRectRadio(QDataStream& out, const DiagramMimedata& myclass)const
{
	out << myclass.m_rectradio.has_value();
	if (myclass.m_rectradio.has_value())
		out << myclass.m_rectradio.value();
}

void DiagramMimedata::serRectRotate(QDataStream& out, const DiagramMimedata& myclass)const
{
	out << myclass.m_rectRotate.has_value();
	if (myclass.m_rectRotate.has_value())
		out << myclass.m_rectRotate.value();
}

void DiagramMimedata::serCircleRadio(QDataStream& out, const DiagramMimedata& myclass)const
{
	out << myclass.m_circleradio.has_value();
	if (myclass.m_circleradio.has_value())
		out << myclass.m_circleradio.value();
}

void DiagramMimedata::serCircleRotate(QDataStream& out, const DiagramMimedata& myclass)const
{
	out << myclass.m_circlerotate.has_value();
	if (myclass.m_circlerotate.has_value())
		out << myclass.m_circlerotate.value();
}

void DiagramMimedata::serTriangleSideRadios(QDataStream& out, const DiagramMimedata& myclass)const
{
	out << myclass.m_triangleSideRadios.has_value();
	if (myclass.m_triangleSideRadios.has_value())
	{
		out << myclass.m_triangleSideRadios.value().m_bottom
			<< myclass.m_triangleSideRadios.value().m_left
			<< myclass.m_triangleSideRadios.value().m_right;
	}
}

void DiagramMimedata::serTriangleEdgetype(QDataStream& out, const DiagramMimedata& myclass)const
{
	out << myclass.m_triangleEdgeType.has_value();
	if (myclass.m_triangleEdgeType.has_value())
		out << static_cast<int>(myclass.m_triangleEdgeType.value());
}

void DiagramMimedata::serTriangleRotate(QDataStream& out, const DiagramMimedata& myclass)const
{
	out << myclass.m_triangleRotate.has_value();
	if (myclass.m_triangleRotate.has_value())
		out << myclass.m_triangleRotate.value();
}

void DiagramMimedata::serLineRotate(QDataStream& out, const DiagramMimedata& myclass)const
{
	out << myclass.m_linerotate.has_value();
	if (myclass.m_linerotate.has_value())
		out << myclass.m_linerotate.value();
}








void DiagramMimedata::deserShapetype(QDataStream& in, DiagramMimedata& myclass)
{
	int value;
	in >> value;
	myclass.m_type = static_cast<ShapeType>(value);
}

void DiagramMimedata::deserScale(QDataStream& in, DiagramMimedata& myclass)
{
	int value;
	in >> value;
	myclass.m_scale = value;
}

void DiagramMimedata::deserPen(QDataStream& in, DiagramMimedata& myclass)
{
	QColor color;
	qreal width;
	in >> color >> width;

	myclass.m_pen = QPen(color, width);
}

void DiagramMimedata::deserBrush(QDataStream& in, DiagramMimedata& myclass)
{
	QColor color;
	in >> color;

	myclass.m_brush = QBrush(color);
}

void DiagramMimedata::deserRectRadio(QDataStream& in, DiagramMimedata& myclass)
{
	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		qreal value;
		in >> value;
		myclass.m_rectradio = value;
	}
}

void DiagramMimedata::deserRectRotate(QDataStream& in, DiagramMimedata& myclass)
{
	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		int value;
		in >> value;
		myclass.m_rectRotate = value;
	}

}

void DiagramMimedata::deserCircleRadio(QDataStream& in, DiagramMimedata& myclass)
{
	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		qreal value;
		in >> value;
		myclass.m_circleradio = value;
	}
}

void DiagramMimedata::deserCircleRotate(QDataStream& in, DiagramMimedata& myclass)
{
	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		int value;
		in >> value;
		myclass.m_circlerotate = value;
	}
}

void DiagramMimedata::deserTriangleSideRadios(QDataStream& in, DiagramMimedata& myclass)
{
	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		qreal bottom;
		qreal left;
		qreal right;
		in >> bottom >> left >> right;
		myclass.m_triangleSideRadios = DiagramDrawParamsTriangle::TriangleSizeRadios(bottom, left, right);
	}
}

void DiagramMimedata::deserTriangleEdgetype(QDataStream& in, DiagramMimedata& myclass)
{
	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		int value;
		in >> value;
		myclass.m_triangleEdgeType = static_cast<DiagramDrawParamsTriangle::EdgeType>(value);
	}
}

void DiagramMimedata::deserTriangleRotate(QDataStream& in, DiagramMimedata& myclass)
{
	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		int value;
		in >> value;
		myclass.m_triangleRotate = value;
	}
}

void DiagramMimedata::deserLineRotate(QDataStream& in, DiagramMimedata& myclass)
{

	bool hasvalue;
	in >> hasvalue;
	if (hasvalue)
	{
		int value;
		in >> value;
		myclass.m_linerotate = value;
	}
}
