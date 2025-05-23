#pragma once
#include "drawparamscreator.h"
#include "myconfig.h"

std::shared_ptr<IDidgramDrawParams> ICreateParams::create()
{
	auto p = createSpecial();
	createRest(p);
	return p;
}

void ICreateParams::createRest(std::shared_ptr<IDidgramDrawParams> params)
{
	params->m_brush = QBrush(myconfig::getInstance().getBrushColor());
	params->m_center = QPoint(0, 0);
	params->m_centerHoffset = 0;
	params->m_centerVoffset = 0;
	params->m_ischoosed = false;
	params->m_isdrawInHuabu = false;
	params->m_pen = QPen(myconfig::getInstance().getPenColor()
		, myconfig::getInstance().getPenWidth());
	params->m_spacesize = QSize(myconfig::getInstance().getSpaceWidth()
		, myconfig::getInstance().getSpaceHeight());
}

std::shared_ptr<IDidgramDrawParams> createParamsRect::createSpecial()
{
	auto p = std::make_shared<DiagramDrawParamsRect>();
	p->m_boundingrectradio = myconfig::getInstance().getRectRadio();
	p->m_rotate = myconfig::getInstance().getRectRotate();
	p->m_scale = myconfig::getInstance().getRectScale();
	p->m_type = myqtsvg::ShapeType::Rect;
	return p;
}

std::shared_ptr<IDidgramDrawParams> createParamsCircle::createSpecial()
{
	auto p = std::make_shared<DiagramDrawParamsCircle>();
	p->m_boundingrectradio = myconfig::getInstance().getCircleRadio();
	p->m_rotate = myconfig::getInstance().getCircleRotate();
	p->m_scale = myconfig::getInstance().getCircleScale();
	p->m_type = myqtsvg::ShapeType::Circle;
	return p;
}

std::shared_ptr<IDidgramDrawParams> createParamsTriangle::createSpecial()
{
	auto p = std::make_shared<DiagramDrawParamsTriangle>();
	p->m_edgetype = DiagramDrawParamsTriangle::edgetypeStringToEnum(myconfig::getInstance().getTriangleEdgetype());
	p->m_rotate = myconfig::getInstance().getTriangleRotate();
	p->m_scale = myconfig::getInstance().getTriangleScale();
	p->m_type = myqtsvg::ShapeType::Triangle;
	p->m_triangleSizeRadios = DiagramDrawParamsTriangle::TriangleSizeRadios(
		myconfig::getInstance().getTriangleBottomRadio(),
		myconfig::getInstance().getTriangleLeftRadio(),
		myconfig::getInstance().getTriangleRightRadio()
	);
	return p;

}

std::shared_ptr<IDidgramDrawParams> createParamsLine::createSpecial()
{
	auto p = std::make_shared<DiagramDrawParamsLine>();
	p->m_rotate = myconfig::getInstance().getLineRotate();
	p->m_scale = myconfig::getInstance().getLineScale();
	p->m_type = myqtsvg::ShapeType::Line;
	return p;
}

std::shared_ptr<IDidgramDrawParams> createParamsMouse::createSpecial()
{
	auto p = std::make_shared<DiagramDrawParamsMouse>();
	p->m_type = myqtsvg::ShapeType::Mouse;
	return p;
}

std::shared_ptr<IDidgramDrawParams> createParamsChoose::createSpecial()
{
	auto p = std::make_shared<DiagramDrawParamsChoose>();
	p->m_type = myqtsvg::ShapeType::choose;
	return p;
}

std::shared_ptr<IDidgramDrawParams> createParamsText::createSpecial()
{
	auto p = std::make_shared<DiagramDrawParamsText>();
	p->m_font.setFamily(myconfig::getInstance().getTextFamily());
	p->m_font.setPointSize(myconfig::getInstance().getTextSize());
	p->m_type = myqtsvg::ShapeType::Text;
	return p;
}

void createParamsInterface::defaultInit()
{
	add(myqtsvg::ShapeType::Rect, []()
		{
			return std::make_shared<createParamsRect>();
		})
		.add(myqtsvg::ShapeType::Circle, []()
			{
				return std::make_shared<createParamsCircle>();
			})
		.add(myqtsvg::ShapeType::Triangle, []()
			{
				return std::make_shared<createParamsTriangle>();
			})
		.add(myqtsvg::ShapeType::Line, []()
			{
				return std::make_shared<createParamsLine>();
			})
		.add(myqtsvg::ShapeType::Mouse, []()
			{
				return std::make_shared<createParamsMouse>();
			})
		.add(myqtsvg::ShapeType::choose, []()
			{
				return std::make_shared<createParamsChoose>();
			})
		.add(myqtsvg::ShapeType::Text, []()
			{
				return std::make_shared<createParamsText>();
			});
}

createParamsInterface& createParamsInterface::getInstance()
{
	static createParamsInterface instance;
	return instance;
}

createParamsInterface& createParamsInterface::add(myqtsvg::ShapeType type, std::function<std::shared_ptr<ICreateParams>()> creator)
{
	m_map[type] = creator;
	return *this;
}

std::shared_ptr<ICreateParams> createParamsInterface::getParams(myqtsvg::ShapeType type)
{
	if (m_map.find(type) != m_map.end())
	{
		return m_map[type]();
	}
	else
	{
		throw std::runtime_error("error");
	}
}

createParamsInterface::createParamsInterface()
{
	defaultInit();
}
