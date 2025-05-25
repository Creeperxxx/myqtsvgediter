
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
	auto& config = myconfig::getInstance();
	params->setBrushColor(config.getBrushColor());
	params->setCenter(QPoint(0,0));
	params->setCenterHOffset(0);
	params->setCenterVOffset(0);
	params->setIsDrawInCanvas(false);
	params->setPenColor(config.getPenColor());
	params->setPenwidth(config.getPenWidth());
	params->setPenStyle(myqtsvg::QstringToPenStyle(config.getPenStyle()));
	params->setSpacesize(QSize(config.getSpaceWidth(), config.getSpaceHeight()));
}


std::shared_ptr<IDidgramDrawParams> createParamsRect::createSpecial()
{
	auto& config = myconfig::getInstance();
	auto p = std::make_shared<DiagramDrawParamsRect>();
	p->setRadio(config.getRectRadio());
	p->setRotate(config.getRectRotate());
	p->setScale(config.getRectScale());
	p->setType(myqtsvg::ShapeType::Rect);
	return p;
}

std::shared_ptr<IDidgramDrawParams> createParamsCircle::createSpecial()
{
	auto& config = myconfig::getInstance();
	auto p = std::make_shared<DiagramDrawParamsCircle>();
	p->setRadio(config.getCircleRadio());
	p->setRotate(config.getCircleRotate());
	p->setScale(config.getCircleScale());
	p->setType(myqtsvg::ShapeType::Circle);
	return p;
}

std::shared_ptr<IDidgramDrawParams> createParamsTriangle::createSpecial()
{
	auto& config = myconfig::getInstance();
	auto p = std::make_shared<DiagramDrawParamsTriangle>();
	p->setEdgeType(DiagramDrawParamsTriangle::edgetypeStringToEnum(config.getTriangleEdgetype()));
	p->setRotate(config.getTriangleRotate());
	p->setScale(config.getTriangleScale());
	p->setBottomRadio(config.getTriangleBottomRadio());
	p->setLeftRadio(config.getTriangleLeftRadio());
	p->setRightRadio(config.getTriangleRightRadio());
	p->setType(myqtsvg::ShapeType::Triangle);
	return p;

}

std::shared_ptr<IDidgramDrawParams> createParamsLine::createSpecial()
{
	auto& config = myconfig::getInstance();
	auto p = std::make_shared<DiagramDrawParamsLine>();
	
	p->setRotate(config.getLineRotate());
	p->setScale(config.getLineScale());
	p->setType(myqtsvg::ShapeType::Line);
	return p;
}

std::shared_ptr<IDidgramDrawParams> createParamsMouse::createSpecial()
{
	auto p = std::make_shared<DiagramDrawParamsMouse>();
	p->setType(myqtsvg::ShapeType::Mouse);
	p->setPath(nullptr);
	return p;
}

std::shared_ptr<IDidgramDrawParams> createParamsChoose::createSpecial()
{
	auto p = std::make_shared<DiagramDrawParamsChoose>();
	p->setType(myqtsvg::ShapeType::choose);
	return p;
}

std::shared_ptr<IDidgramDrawParams> createParamsText::createSpecial()
{
	auto& config = myconfig::getInstance();
	auto p = std::make_shared<DiagramDrawParamsText>();
	p->setFontFamily(config.getTextFamily());
	p->setFontSize(config.getTextSize());
	p->setType(myqtsvg::ShapeType::Text);
	p->setTextEdit(nullptr);
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
