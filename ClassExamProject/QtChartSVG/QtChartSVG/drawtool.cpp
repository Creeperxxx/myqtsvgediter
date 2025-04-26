#include "drawtool.h"



DiagramDrawParamsTriangle::TriangleSizeRadios::TriangleSizeRadios()
	:m_bottom(0.0)
	, m_left(0.0)
	, m_right(0.0)
{
}

DiagramDrawParamsTriangle::TriangleSizeRadios::TriangleSizeRadios(double bottom, double left, double right)
	: m_bottom(bottom)
	, m_left(left)
	, m_right(right) {
}

DiagramDrawParamsTriangle::EdgeType DiagramDrawParamsTriangle::edgetypeStringToEnum(const QString& str)
{
	if (str == "bottom")
		return EdgeType::Bottom;
	else if (str == "left")
		return EdgeType::Left;
	else if (str == "right")
		return EdgeType::Right;
	else
		throw std::runtime_error("error");
}

QString DiagramDrawParamsTriangle::edgetypeEnumToString(EdgeType edgetype)
{
	switch (edgetype)
	{
	case DiagramDrawParamsTriangle::EdgeType::Left:
		return QString("left");
		break;
	case DiagramDrawParamsTriangle::EdgeType::Right:
		return QString("right");
		break;
	case DiagramDrawParamsTriangle::EdgeType::Bottom:
		return QString("bottom");
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

//std::shared_ptr<IDidgramDrawParams> IbuildHuabuParams::build(QWidget* item)
//{
//	huabu* huabuitem = dynamic_cast<huabu*>(item);
//	if (huabuitem == nullptr)
//		throw std::runtime_error("error");
//	std::shared_ptr<IDidgramDrawParams> params = specialbuild(huabuitem);
//	params->m_brush = huabuitem->getbrush();
//	params->m_pen = huabuitem->getpen();
//	params->m_center = huabuitem->getselfdrawcenter();
//	params->m_spacesize = huabuitem->getselfdrawspacesize();
//	params->m_type = huabuitem->gettype();
//	return params;
//}
//
//std::shared_ptr<IDidgramDrawParams> buildHuabuParamsRect::specialbuild(huabu* item)
//{
//	std::shared_ptr<DiagramDrawParamsRect> params = std::make_shared<DiagramDrawParamsRect>();
//	if (!item->getmimedata().m_rectradio.has_value())
//		throw std::runtime_error("error");
//	params->m_boundingrectradio = item->getmimedata().m_rectradio.value();
//	return params;
//}
//
//std::shared_ptr<IDidgramDrawParams> buildHuabuParamsCircle::specialbuild(huabu* item)
//{
//	std::shared_ptr<DiagramDrawParamsCircle> params = std::make_shared<DiagramDrawParamsCircle>();
//	if (!item->getmimedata().m_circleradio.has_value())
//		throw std::runtime_error("error");
//	params->m_boundingrectradio = item->getmimedata().m_circleradio.value();
//	return params;
//}
//
//std::shared_ptr<IDidgramDrawParams> buildHuabuParamsTriangle::specialbuild(huabu* item)
//{
//	std::shared_ptr<DiagramDrawParamsTriangle> params = std::make_shared<DiagramDrawParamsTriangle>();
//	if (!item->getmimedata().m_triangleSideRadios.has_value())
//		throw std::runtime_error("error");
//	params->m_triangleSizeRadios = item->getmimedata().m_triangleSideRadios.value();
//
//	if (!item->getmimedata().m_triangleEdgeType.has_value())
//		throw std::runtime_error("error");
//	params->m_edgetype = item->getmimedata().m_triangleEdgeType.value();
//
//	if (!item->getmimedata().m_triangleRotate.has_value())
//		throw std::runtime_error("error");
//	params->m_rotationAngle = item->getmimedata().m_triangleRotate.value();
//	return params;
//}
//
//std::unique_ptr<IbuildHuabuParams> FactorybuildHuabuParams::create(ShapeType type)
//{
//	switch (type)
//	{
//	case ShapeType::Rect:
//		return std::make_unique<buildHuabuParamsRect>();
//		break;
//	case ShapeType::Circle:
//		return std::make_unique<buildHuabuParamsCircle>();
//		break;
//	case ShapeType::Triangle:
//		return std::make_unique<buildHuabuParamsTriangle>();
//		break;
//	case ShapeType::Line:
//		return std::make_unique<buildHuabuParamsLine>();
//		break;
//	default:
//		throw std::runtime_error("error");
//		break;
//	}
//}
//
//std::shared_ptr<IDidgramDrawParams> buildtuxingjiedianparamsfordiagramline::specialbuild(DiagramItem* item)
//{
//	std::shared_ptr<DiagramDrawParamsLine> params = std::make_shared<DiagramDrawParamsLine>();
//	params->m_rotationAngle = item->getLineRotate();
//	return params;
//}
//
//std::shared_ptr<IDidgramDrawParams> buildDiagramPixmapParamsLine::specialbuild(DiagramItem* item)
//{
//	std::shared_ptr<DiagramDrawParamsLine> params = std::make_shared<DiagramDrawParamsLine>();
//	params->m_rotationAngle = item->getLineRotate();
//	return params;
//}
//
//std::shared_ptr<IDidgramDrawParams> buildHuabuParamsLine::specialbuild(huabu* item)
//{
//	std::shared_ptr<DiagramDrawParamsLine> params = std::make_shared<DiagramDrawParamsLine>();
//	params->m_rotationAngle = item->getlinerotate();
//	return params;
//}

QString ShapeTypeTool::shapetypeEnumToQstring(ShapeType type)
{
	switch (type)
	{
	case ShapeType::Rect:
		return QString("Rect");
		break;
	case ShapeType::Circle:
		return QString("Circle");
		break;
	case ShapeType::Triangle:
		return QString("Triangle");
		break;
	case ShapeType::Line:
		return QString("Line");
		break;
	default:
		throw std::runtime_error("error");
	}
}

ShapeType ShapeTypeTool::shapetypeQstringToEnum(const QString& type)
{
	if (type == "Rect")
	{
		return ShapeType::Rect;
	}
	else if (type == "Circle")
	{
		return ShapeType::Circle;
	}
	else if (type == "Triangle")
	{
		return ShapeType::Triangle;
	}
	else if (type == "Line")
	{
		return ShapeType::Line;
	}
	else
	{
		throw std::runtime_error("error");
	}
}
