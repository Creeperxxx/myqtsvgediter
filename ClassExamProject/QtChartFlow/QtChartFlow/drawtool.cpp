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
	params->m_radio = item->getDiagramItemRectRadio();
	return params;
}

std::shared_ptr<IDidgramDrawParams> buildtuxingjiedianparamsfordiagramcircle::specialbuild(DiagramItem* item)
{
	std::shared_ptr<DiagramDrawParamsCircle> params = std::make_shared<DiagramDrawParamsCircle>();

	return params;
}

std::unique_ptr<Ibuildtuxingjiedianparamsfordiagram> factorybuildtuxingjiedianparamsfordiagram::create(ShapeType type)
{
	switch (type)
	{
	default:
	case ShapeType::Rect:
		return std::make_unique<buildtuxingjiedianparamsfrodiagramrect>();
		break;
	case ShapeType::Circle:
		return std::make_unique<buildtuxingjiedianparamsfordiagramcircle>();
	case ShapeType::Triangle:
		return std::make_unique<buildtuxingjiedianparamsfordiagramtriangle>();
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
	params->m_radio = item->getHuabuRectRadio();
	return params;
}

std::shared_ptr<IDidgramDrawParams> buildDiagramPixmapParamsCircle::specialbuild(DiagramItem* item)
{
	std::shared_ptr < DiagramDrawParamsCircle > params = std::make_shared < DiagramDrawParamsCircle>();
	return params;
}

std::unique_ptr<IbuildDiagramPixmapParams> FactoryBuildDiagramPixmapParams::create(ShapeType type)
{
	switch (type)
	{
	default:
	case ShapeType::Rect:
		return std::make_unique<buildDiagramPixmapParamsRect>();
		break;
	case ShapeType::Circle:
		return std::make_unique<buildDiagramPixmapParamsCircle>();
		break;
	}
}

std::unique_ptr<IbuildAllDiagramParams> factoryall::create(DiagramItemType widgettype, ShapeType shapetype)
{
	switch (widgettype)
	{
	default:
	case DiagramItemType::huabu:
	case DiagramItemType::tuxingku:
		return factorybuildtuxingjiedianparamsfordiagram::create(shapetype);
		break;
	case DiagramItemType::tuxingkupixmap:
		return FactoryBuildDiagramPixmapParams::create(shapetype);
		break;
	}
}


std::shared_ptr<IDidgramDrawParams> buildtuxingjiedianparamsfordiagramtriangle::specialbuild(DiagramItem* item)
{
	std::shared_ptr<DiagramDrawParamsTriangle> params = std::make_shared<DiagramDrawParamsTriangle>();
	params->m_bottom = 5;
	params->m_left = 3;
	params->m_right = 4;
	params->m_edgetype = DiagramDrawParamsTriangle::EdgeType::Left;
	params->m_rotationAngle = 90;
	return params;
}


ShapeType Tool::shapetypestringtoenum(const std::string& str)
{
	if (str.compare(cfggetval<std::string>(qtcf::tuxingTypeNameJuxing)) == 0)
		return ShapeType::Rect;
	else if (str.compare(cfggetval<std::string>(qtcf::tuxingTypeNameYuanxing)) == 0)
		return ShapeType::Circle;
	else
		throw std::runtime_error("error");
}
