#include "DiagramMimedata.h"


QDataStream& operator<<(QDataStream& out, const DiagramMimedata& myClass)
{
	out << static_cast<int>(myClass.m_type);


	// 序列化 m_rectradio
	out << myClass.m_rectradio.has_value();
	if (myClass.m_rectradio.has_value()) {
		out << myClass.m_rectradio.value();
	}

	//序列化m_rectrotate
	out << myClass.m_rectRotate.has_value();
	if (myClass.m_rectRotate.has_value())
	{
		out << myClass.m_rectRotate.value();
	}

	// 序列化 m_circleradio
	out << myClass.m_circleradio.has_value();
	if (myClass.m_circleradio.has_value()) {
		out << myClass.m_circleradio.value();
	}

	// 序列化 m_circlerotate
	out << myClass.m_circlerotate.has_value();
	if (myClass.m_circlerotate.has_value())
		out << myClass.m_circlerotate.value();

	// 序列化 m_triangleSideRadios
	out << myClass.m_triangleSideRadios.has_value();
	if (myClass.m_triangleSideRadios.has_value()) {
		out << myClass.m_triangleSideRadios.value().m_bottom
			<< myClass.m_triangleSideRadios.value().m_left
			<< myClass.m_triangleSideRadios.value().m_right;
	}

	// 序列化edgetype
	out << myClass.m_triangleEdgeType.has_value();
	if (myClass.m_triangleEdgeType.has_value())
	{
		out << static_cast<int>(myClass.m_triangleEdgeType.value());
	}

	// 序列化 m_triangleRotate
	out << myClass.m_triangleRotate.has_value();
	if (myClass.m_triangleRotate.has_value()) {
		out << myClass.m_triangleRotate.value();
	}

	out << myClass.m_linerotate.has_value();
	if (myClass.m_linerotate.has_value())
	{
		out << myClass.m_linerotate.value();
	}

	return out;
}

QDataStream& operator>>(QDataStream& in, DiagramMimedata& myClass)
{
	int type;
	in >> type;
	myClass.m_type = static_cast<ShapeType>(type);


	bool hasValue;

	// 反序列化 m_rectradio
	in >> hasValue;
	if (hasValue) {
		double value;
		in >> value;
		myClass.m_rectradio = value;
	}
	else {
		myClass.m_rectradio.reset();
	}

	// 反序列化 m_rectRotate;
	in >> hasValue;
	if (hasValue)
	{
		int value;
		in >> value;
		myClass.m_rectRotate = value;
	}
	else
	{
		myClass.m_rectRotate.reset();
	}


	// 反序列化 m_circleradio
	in >> hasValue;
	if (hasValue) {
		double value;
		in >> value;
		myClass.m_circleradio = value;
	}
	else {
		myClass.m_circleradio.reset();
	}

	// 反序列化 m_circlerotate
	in >> hasValue;
	if (hasValue)
	{
		int value;
		in >> value;
		myClass.m_circlerotate = value;
	}
	else
	{
		myClass.m_circlerotate.reset();
	}

	// 反序列化 m_triangleSideRadios
	in >> hasValue;
	if (hasValue) {
		double bottom, left, right;
		in >> bottom >> left >> right;
		DiagramDrawParamsTriangle::TriangleSizeRadios radios(bottom, left, right);
		myClass.m_triangleSideRadios = radios;
	}
	else {
		myClass.m_triangleSideRadios.reset();
	}

	// 反序列化 m_trianglesideradios
	in >> hasValue;
	if (hasValue) {
		int value;
		in >> value;
		myClass.m_triangleEdgeType = static_cast<DiagramDrawParamsTriangle::EdgeType>(value);
	}
	else {
		myClass.m_triangleEdgeType.reset();
	}

	// 反序列化 m_triangleRotate
	in >> hasValue;
	if (hasValue) {
		double value;
		in >> value;
		myClass.m_triangleRotate = value;
	}
	else {
		myClass.m_triangleRotate.reset();
	}

	in >> hasValue;
	if (hasValue)
	{
		double value;
		in >> value;
		myClass.m_linerotate = value;
	}
	else
	{
		myClass.m_linerotate.reset();
	}

	return in;
	// TODO: 在此处插入 return 语句
}
