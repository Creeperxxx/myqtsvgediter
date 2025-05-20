#include "propertydatabuilder.h"
#include "propertydata.h"
#include "propertyset.h"
#include "myconfig.h"
#include "diagramdrawparams.h"

void IDrawParamsPropertyDataBuilder::build(std::shared_ptr<IpropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	if (set == nullptr || set.get() == nullptr
		|| datavec == nullptr || datavec.get() == nullptr)
		throw std::runtime_error("error");

	auto castset = std::dynamic_pointer_cast<drawParamsPropertySet>(set);
	if (castset == nullptr)
		throw std::runtime_error("error");

	probuild(castset, datavec);
}

void PenColorDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getPenColorName()
		, QVariant(set->m_params->m_pen.color()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onPenColorChanged);
}

void PenWidthDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getBrushColorName()
		, QVariant::fromValue(set->m_params->m_pen.width()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onPenWidthChanged);
}

void BrushDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getBrushColorName()
		, QVariant::fromValue(set->m_params->m_brush.color()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onBrushColorChanged);
}

void RotateDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getRotateAngleName()
		, QVariant::fromValue(set->m_params->m_rotate));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onRotateChanged);
}

void SpacewidthDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getSpaceWidthName()
		, set->m_params->m_spacesize.width());
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onSpacewidthChanged);
}

void SpaceheightDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getSpaceHeightName()
		, QVariant::fromValue(set->m_params->m_spacesize.height()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onSpaceHeightChanged);
}

void ScaleDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getScaleName()
		, QVariant::fromValue(set->m_params->m_scale));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onScaleChanged);
}

void CenterHoffsetDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getCenterHOffsetName()
		, QVariant(set->m_params->m_centerHoffset));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onCenterHOffset);
}

void CenterVoffsetDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getCenterHOffsetName()
		, QVariant::fromValue(set->m_params->m_centerVoffset));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onCenterVOffset);
}

void RectRadioDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsRect>(set->m_params);
	if (castparams == nullptr || castparams.get() == nullptr)
		throw std::runtime_error("error");
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getRectRadioName()
		, QVariant::fromValue(castparams->m_boundingrectradio));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onRectRadioChanged);
}



void CircleRadioDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsCircle>(set->m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getCircleRadioName()
		, QVariant::fromValue(castparams->m_boundingrectradio));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onCricleRadioChanged);
}

void TriangleRadioDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(set->m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getTriangleRadioName()
		, QVariant::fromValue(castparams->m_triangleSizeRadios));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onTriangleRadioChanged);
}


void TriangleEdgetypeDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(set->m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	QString edgetype = DiagramDrawParamsTriangle::edgetypeEnumToString(castparams->m_edgetype);
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getEdgeTypeName()
		, QVariant::fromValue(edgetype));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onTriangleEdgetypeRadioChanged);
}

void TextFontFamilyDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsText>(set->m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	auto data = std::make_shared<propertydata>(myconfig::getInstance().getFontFamilyName()
		, QVariant::fromValue(castparams->m_font.family()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onTextFamilyChanged);
}

void TextFontSizeDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsText>(set->m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	auto data = std::make_shared<propertydata>(myconfig::getInstance().getFontSizeName()
		, QVariant::fromValue(castparams->m_font.pointSize()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onTextSizeChanged);
}

void NamePropertyDataBuilder::probuild(std::shared_ptr<otherPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getNameName()
		, QVariant::fromValue(set->m_name));
	datavec->push_back(data);

}

void HuabuHeightPropertyDataBuilder::probuild(std::shared_ptr<otherPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto data = std::make_shared<propertydata>(myconfig::getInstance().getCanvasHeightName()
		, QVariant::fromValue(set->m_huabuheight));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &otherPropertySet::onHuabuHeightChanged);
}

void HuabuWidthPropertyDataBuilder::probuild(std::shared_ptr<otherPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto data = std::make_shared<propertydata>(myconfig::getInstance().getCanvasWidthName()
		, QVariant::fromValue(set->m_huabuwidth));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &otherPropertySet::onHuabuWidthChanged);
}

void IOtherPropertyDataBuilder::build(std::shared_ptr<IpropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	if (set == nullptr || set.get() == nullptr
		|| datavec == nullptr || datavec.get() == nullptr)
		throw std::runtime_error("error");
	auto castset = std::dynamic_pointer_cast<otherPropertySet>(set);
	if (castset == nullptr)
		throw std::runtime_error("error");
	probuild(castset, datavec);
}

std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> propertyDataVecOfPropertySetCreator::create(std::shared_ptr<IpropertySet> set)
{
	auto datavec = std::make_shared<std::vector<std::shared_ptr<propertydata>>>();

	for (auto& builder : m_builders)
	{
		builder->build(set, datavec);
	}
	return datavec;
}

void propertyDataVecOfPropertySetCreator::addBuilder(std::shared_ptr<IpropertyDataBuilder> builder)
{
	if (builder == nullptr || builder.get() == nullptr)
		throw std::runtime_error("error");
	m_builders.push_back(builder);
}

propertyDataVecOfPropertySetCreatorFactor& propertyDataVecOfPropertySetCreatorFactor::getInstance()
{
	static propertyDataVecOfPropertySetCreatorFactor instance;
	return instance;
}

std::shared_ptr<propertyDataVecOfPropertySetCreator> propertyDataVecOfPropertySetCreatorFactor::create(std::shared_ptr<std::vector<QString>> propertynamevec)
{
	if (propertynamevec == nullptr || propertynamevec.get() == nullptr)
		throw std::runtime_error("error");

	std::shared_ptr<propertyDataVecOfPropertySetCreator> creator = std::make_shared<propertyDataVecOfPropertySetCreator>();
	for (const auto& name : *propertynamevec)
	{
		if (m_builderCreatefunc.find(name) != m_builderCreatefunc.end())
		{
			creator->addBuilder(m_builderCreatefunc[name]());
		}
		else
		{
			throw std::runtime_error("error");
		}
	}
	return creator;
}

propertyDataVecOfPropertySetCreatorFactor& propertyDataVecOfPropertySetCreatorFactor::addCreator(QString name, std::function<std::shared_ptr<IpropertyDataBuilder>()> func)
{
	if (m_builderCreatefunc.find(name) != m_builderCreatefunc.end())
	{
		throw std::runtime_error("error");
	}
	else
	{
		m_builderCreatefunc[name] = func;
	}
	return *this;
}

propertyDataVecOfPropertySetCreatorFactor::propertyDataVecOfPropertySetCreatorFactor()
{
}