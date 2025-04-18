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

	std::optional<double> m_rectradio;
	std::optional<int> m_rectRotate;

	std::optional<double> m_circleradio;
	std::optional<int> m_circlerotate;

	std::optional<DiagramDrawParamsTriangle::TriangleSizeRadios> m_triangleSideRadios;
	std::optional<DiagramDrawParamsTriangle::EdgeType> m_triangleEdgeType;
	std::optional<double> m_triangleRotate;

	std::optional<double> m_linerotate;
};

