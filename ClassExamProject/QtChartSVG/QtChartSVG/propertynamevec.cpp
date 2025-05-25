#include "propertynamevec.h"
#include "myconfig.h"
#include "stdexcept"

propertyNameVecInterface& propertyNameVecInterface::getinstance()
{
	static propertyNameVecInterface instance;
	return instance;
}

void propertyNameVecInterface::addPropertyNameVec(myqtsvg::ShapeType type, std::vector<QString> vec)
{
	m_propertyNameMap[type] = vec;
}

std::vector<QString> propertyNameVecInterface::getPropertyNameVec(myqtsvg::ShapeType type, std::vector<QString> vec)
{
	if (m_propertyNameMap.find(type) == m_propertyNameMap.end())
		throw std::runtime_error("error");
	std::vector<QString> ret(m_propertyNameMap[type]);
	ret.insert(ret.end(), vec.begin(), vec.end());
	return ret;
}

propertyNameVecInterface::propertyNameVecInterface()
{
	defaultinit();
}

void propertyNameVecInterface::defaultinit()
{
	setRectPropertyNameVec();
	setTrianglePropertyNameVec();
	setCirclePropertyNameVec();
	setLinePropertyNameVec();
	setChoosePropertyNameVec();
	setMousePropertyNameVec();
	setTextPropertyNameVec();
	setPentagonPropertyNameVec();
	setHexagonPropertyNameVec();
	setStarPropertyNameVec();
}

void propertyNameVecInterface::setRectPropertyNameVec()
{
	auto& config = myconfig::getInstance();
	m_propertyNameMap[myqtsvg::ShapeType::Rect] = {
		config.getPenColorName()
		, config.getPenStyleName()
		, config.getPenWdithName()
		, config.getBrushColorName()
		, config.getRotateAngleName()
		, config.getScaleName()
		, config.getSpaceWidthName()
		, config.getSpaceHeightName()
		, config.getRectRadioName()
	};
}

void propertyNameVecInterface::setTrianglePropertyNameVec()
{
	auto& config = myconfig::getInstance();
	m_propertyNameMap[myqtsvg::ShapeType::Triangle] = {
		config.getPenColorName()
		, config.getPenStyleName()
		, config.getPenWdithName()
		, config.getBrushColorName()
		, config.getRotateAngleName()
		, config.getScaleName()
		, config.getSpaceWidthName()
		, config.getSpaceHeightName()
		, config.getTriangleRadioName()
		, config.getEdgeTypeName()
	};
}

void propertyNameVecInterface::setCirclePropertyNameVec()
{
	auto& config = myconfig::getInstance();
	m_propertyNameMap[myqtsvg::ShapeType::Circle] = {
		config.getPenColorName()
		, config.getPenStyleName()
		, config.getPenWdithName()
		, config.getBrushColorName()
		, config.getRotateAngleName()
		, config.getScaleName()
		, config.getSpaceWidthName()
		, config.getSpaceHeightName()
		, config.getCircleRadioName()
	};
}

void propertyNameVecInterface::setLinePropertyNameVec()
{
	auto& config = myconfig::getInstance();
	m_propertyNameMap[myqtsvg::ShapeType::Line] = {
		config.getPenColorName()
		, config.getPenStyleName()
		, config.getPenWdithName()
		, config.getBrushColorName()
		, config.getRotateAngleName()
		, config.getScaleName()
		, config.getSpaceWidthName()
		, config.getSpaceHeightName()
	};
}

void propertyNameVecInterface::setChoosePropertyNameVec()
{
	m_propertyNameMap[myqtsvg::ShapeType::choose] = {};
}

void propertyNameVecInterface::setMousePropertyNameVec()
{
	auto& config = myconfig::getInstance();
	m_propertyNameMap[myqtsvg::ShapeType::Mouse] = {
		config.getPenColorName()
		, config.getPenWdithName()
		, config.getPenStyleName()
	};
}

void propertyNameVecInterface::setTextPropertyNameVec()
{
	auto& config = myconfig::getInstance();
	m_propertyNameMap[myqtsvg::ShapeType::Text] = {
		config.getPenColorName()
		, config.getFontFamilyName()
		, config.getFontSizeName()
	};
}

void propertyNameVecInterface::setPentagonPropertyNameVec()
{
	auto& config = myconfig::getInstance();
	m_propertyNameMap[myqtsvg::ShapeType::Pentagon] = {
		config.getPenColorName()
		, config.getPenStyleName()
		, config.getPenWdithName()
		, config.getBrushColorName()
		, config.getSpaceWidthName()
		, config.getSpaceHeightName()
	};
}

void propertyNameVecInterface::setHexagonPropertyNameVec()
{
	auto& config = myconfig::getInstance();
	m_propertyNameMap[myqtsvg::ShapeType::Hexagon] = {
		config.getPenColorName()
		, config.getPenStyleName()
		, config.getPenWdithName()
		, config.getBrushColorName()
		, config.getSpaceWidthName()
		, config.getSpaceHeightName()
	};
}

void propertyNameVecInterface::setStarPropertyNameVec()
{
	auto& config = myconfig::getInstance();
	m_propertyNameMap[myqtsvg::ShapeType::Star] = {
		config.getPenColorName()
		, config.getPenStyleName()
		, config.getPenWdithName()
		, config.getBrushColorName()
		, config.getSpaceWidthName()
		, config.getSpaceHeightName()
	};
}










