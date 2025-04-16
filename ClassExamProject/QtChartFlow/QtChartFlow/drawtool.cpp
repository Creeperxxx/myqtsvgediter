#include "drawtool.h"
#include "tuxing.h"
#include "huabu.h"



std::shared_ptr<IDidgramDrawParams> Ibuildtuxingjiedianparamsfordiagram::build(QWidget* item)
{
	DiagramItem* diaitem = dynamic_cast<DiagramItem*>(item);
	if (!diaitem)
		throw std::runtime_error("error");
	std::shared_ptr<IDidgramDrawParams> params = specialbuild(diaitem);
	params->m_center = diaitem->getcenter();
	//params->m_painter = painter;
	params->m_spacesize = diaitem->getspacesize();
	params->m_type = diaitem->gettype();
	params->m_pen = diaitem->getdiagrampen();
	params->m_brush = diaitem->getdiagrambrush();
	return params;
}

std::shared_ptr<IDidgramDrawParams> buildtuxingjiedianparamsfrodiagramrect::specialbuild(DiagramItem* item)
{
	std::shared_ptr<DiagramDrawParamsRect> params = std::make_shared<DiagramDrawParamsRect>();
	params->m_boundingrectradio = item->getDiagramItemRectRadio();
	return params;
}

std::shared_ptr<IDidgramDrawParams> buildtuxingjiedianparamsfordiagramcircle::specialbuild(DiagramItem* item)
{
	std::shared_ptr<DiagramDrawParamsCircle> params = std::make_shared<DiagramDrawParamsCircle>();
	params->m_boundingrectradio = item->getCircleBoundingrectradio();
	return params;
}

std::unique_ptr<Ibuildtuxingjiedianparamsfordiagram> factorybuildtuxingjiedianparamsfordiagram::create(ShapeType type)
{
	switch (type)
	{
	case ShapeType::Rect:
		return std::make_unique<buildtuxingjiedianparamsfrodiagramrect>();
		break;
	case ShapeType::Circle:
		return std::make_unique<buildtuxingjiedianparamsfordiagramcircle>();
	case ShapeType::Triangle:
		return std::make_unique<buildtuxingjiedianparamsfordiagramtriangle>();
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

std::shared_ptr<IDidgramDrawParams> IbuildDiagramPixmapParams::build(QWidget* item)
{
	DiagramItem* diaitem = dynamic_cast<DiagramItem*>(item);
	if (!diaitem)
		throw std::runtime_error("error");
	std::shared_ptr<IDidgramDrawParams> params = specialbuild(diaitem);
	params->m_center = diaitem->getPixmapCenter();
	//params->m_painter = diaitem->getDiagramdiaitemPixmapPainter();
	params->m_spacesize = diaitem->getPixmapSpaceSize();
	params->m_type = diaitem->gettype();
	params->m_pen = diaitem->gethuabupen();
	params->m_brush = diaitem->gethuabubrush();
	return params;
}

std::shared_ptr<IDidgramDrawParams> buildDiagramPixmapParamsRect::specialbuild(DiagramItem* item)
{
	std::shared_ptr<DiagramDrawParamsRect> params = std::make_shared<DiagramDrawParamsRect>();
	params->m_boundingrectradio = item->getDiagramItemRectRadio();
	return params;
}

std::shared_ptr<IDidgramDrawParams> buildDiagramPixmapParamsCircle::specialbuild(DiagramItem* item)
{
	std::shared_ptr<DiagramDrawParamsCircle> params = std::make_shared<DiagramDrawParamsCircle>();
	params->m_boundingrectradio = item->getCircleBoundingrectradio();
	return params;
}

std::unique_ptr<IbuildDiagramPixmapParams> FactoryBuildDiagramPixmapParams::create(ShapeType type)
{
	switch (type)
	{
	case ShapeType::Rect:
		return std::make_unique<buildDiagramPixmapParamsRect>();
		break;
	case ShapeType::Circle:
		return std::make_unique<buildDiagramPixmapParamsCircle>();
		break;
	case ShapeType::Triangle:
		return std::make_unique<buildDiagramPixmapParamsTriangle>();
	default:
		throw std::runtime_error("error");
	}
}

std::shared_ptr<IDidgramDrawParams> buildDiagramPixmapParamsTriangle::specialbuild(DiagramItem* item)
{
	std::shared_ptr<DiagramDrawParamsTriangle> params = std::make_shared<DiagramDrawParamsTriangle>();
	params->m_edgetype = item->getedgetype();
	params->m_rotationAngle = item->getTriangleRotate();
	params->m_triangleSizeRadios = item->gettrianglesideradio();
	return params;
}

std::unique_ptr<IbuildAllDiagramParams> factoryall::create(DiagramItemType widgettype, ShapeType shapetype)
{
	switch (widgettype)
	{
	case DiagramItemType::huabu:
		return FactorybuildHuabuParams::create(shapetype);
		//throw std::runtime_error("error");
		break;
	case DiagramItemType::tuxingku:
		return factorybuildtuxingjiedianparamsfordiagram::create(shapetype);
		break;
	case DiagramItemType::tuxingkupixmap:
		return FactoryBuildDiagramPixmapParams::create(shapetype);
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}


std::shared_ptr<IDidgramDrawParams> buildtuxingjiedianparamsfordiagramtriangle::specialbuild(DiagramItem* item)
{
	std::shared_ptr<DiagramDrawParamsTriangle> params = std::make_shared<DiagramDrawParamsTriangle>();
	params->m_triangleSizeRadios = item->gettrianglesideradio();
	params->m_rotationAngle = item->getTriangleRotate();
	params->m_edgetype = item->getedgetype();
	return params;
}


//ShapeType Tool::shapetypestringtoenum(const std::string& str)
//{
//	if (str.compare(cfggetval<std::string>(qtcf::tuxingTypeNameJuxing)) == 0)
//		return ShapeType::Rect;
//	else if (str.compare(cfggetval<std::string>(qtcf::tuxingTypeNameYuanxing)) == 0)
//		return ShapeType::Circle;
//	else
//		throw std::runtime_error("error");
//}

DiagramDrawParamsTriangle::TriangleSizeRadios::TriangleSizeRadios(double bottom, double left, double right)
	: m_bottom(bottom)
	, m_left(left)
	, m_right(right) { }

DiagramDrawParamsTriangle::EdgeType DiagramDrawParamsTriangle::edgetypestringtoenum(const std::string& str)
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

std::shared_ptr<IDidgramDrawParams> IbuildHuabuParams::build(QWidget* item)
{
	huabu* huabuitem = dynamic_cast<huabu*>(item);
	if (huabuitem == nullptr)
		throw std::runtime_error("error");
	std::shared_ptr<IDidgramDrawParams> params = specialbuild(huabuitem);
	params->m_brush = huabuitem->getbrush();
	params->m_pen = huabuitem->getpen();
	params->m_center = huabuitem->getcenter();
	params->m_spacesize = huabuitem->getspacesize();
	params->m_type = huabuitem->gettype();
	return params;
}

std::shared_ptr<IDidgramDrawParams> buildHuabuParamsRect::specialbuild(huabu* item)
{
	std::shared_ptr<DiagramDrawParamsRect> params = std::make_shared<DiagramDrawParamsRect>();
	if (!item->getmimedata().m_rectradio.has_value())
		throw std::runtime_error("error");
	params->m_boundingrectradio = item->getmimedata().m_rectradio.value();
	return params;
}

std::shared_ptr<IDidgramDrawParams> buildHuabuParamsCircle::specialbuild(huabu* item)
{
	std::shared_ptr<DiagramDrawParamsCircle> params = std::make_shared<DiagramDrawParamsCircle>();
	if (!item->getmimedata().m_circleradio.has_value())
		throw std::runtime_error("error");
	params->m_boundingrectradio = item->getmimedata().m_circleradio.value();
	return params;
}

std::shared_ptr<IDidgramDrawParams> buildHuabuParamsTriangle::specialbuild(huabu* item)
{
	std::shared_ptr<DiagramDrawParamsTriangle> params = std::make_shared<DiagramDrawParamsTriangle>();
	if (!item->getmimedata().m_triangleSideRadios.has_value())
		throw std::runtime_error("error");
	params->m_triangleSizeRadios = item->getmimedata().m_triangleSideRadios.value();

	if (!item->getmimedata().m_triangleEdgeType.has_value())
		throw std::runtime_error("error");
	params->m_edgetype = item->getmimedata().m_triangleEdgeType.value();

	if (!item->getmimedata().m_triangleRotate.has_value())
		throw std::runtime_error("error");
	params->m_rotationAngle = item->getmimedata().m_triangleRotate.value();
	return params;
}

std::unique_ptr<IbuildHuabuParams> FactorybuildHuabuParams::create(ShapeType type)
{
	switch (type)
	{
	case ShapeType::Rect:
		return std::make_unique<buildHuabuParamsRect>();
		break;
	case ShapeType::Circle:
		return std::make_unique<buildHuabuParamsCircle>();
		break;
	case ShapeType::Triangle:
		return std::make_unique<buildHuabuParamsTriangle>();
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}
