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
		, QVariant(set->m_params->getPenColor()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onPenColorChanged);
}

void PenWidthDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getPenWdithName()
		, QVariant::fromValue(set->m_params->getPenwidth()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onPenWidthChanged);
}

void BrushDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getBrushColorName()
		, QVariant::fromValue(set->m_params->getBrushColor()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onBrushColorChanged);
}

void RotateDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getRotateAngleName()
		, QVariant::fromValue(set->m_params->getRotate()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onRotateChanged);
}

void SpacewidthDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getSpaceWidthName()
		, set->m_params->getSpacesize().width());
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onSpacewidthChanged);
}

void SpaceheightDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getSpaceHeightName()
		, QVariant::fromValue(set->m_params->getSpacesize().height()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onSpaceHeightChanged);
}

void ScaleDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getScaleName()
		, QVariant::fromValue(set->m_params->getScale()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onScaleChanged);
}

void CenterHoffsetDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getCenterHOffsetName()
		, QVariant(set->m_params->getCenterHOffset()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onCenterHOffset);
}

void CenterVoffsetDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getCenterVOffsetName()
		, QVariant::fromValue(set->m_params->getCenterVOffset()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onCenterVOffset);
}

void RectRadioDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsRect>(set->m_params);
	if (castparams == nullptr || castparams.get() == nullptr)
		throw std::runtime_error("error");
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getRectRadioName()
		, QVariant::fromValue(castparams->getRadio()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onRectRadioChanged);
}



void CircleRadioDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsCircle>(set->m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getCircleRadioName()
		, QVariant::fromValue(castparams->getRadio()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onCricleRadioChanged);
}

void TriangleRadioDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(set->m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(myconfig::getInstance().getTriangleRadioName()
		, QVariant::fromValue(castparams->getRadios()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onTriangleRadioChanged);
}


void TriangleEdgetypeDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(set->m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	QString edgetype = DiagramDrawParamsTriangle::edgetypeEnumToString(castparams->getEdgeType());
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
		, QVariant::fromValue(castparams->getFontFamily()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onTextFamilyChanged);
}

void TextFontSizeDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsText>(set->m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	auto data = std::make_shared<propertydata>(myconfig::getInstance().getFontSizeName()
		, QVariant::fromValue(castparams->getFontSize()));
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
		, QVariant::fromValue(set->m_canvasHeight));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &otherPropertySet::onCanvasHeightChanged);
}

void HuabuWidthPropertyDataBuilder::probuild(std::shared_ptr<otherPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto data = std::make_shared<propertydata>(myconfig::getInstance().getCanvasWidthName()
		, QVariant::fromValue(set->m_canvasWidth));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &otherPropertySet::onCanvasWidthChanged);
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

void propertyDataVecOfPropertySetCreator::addBuilder(std::unique_ptr<IpropertyDataBuilder> builder)
{
	if (builder == nullptr)
		throw std::runtime_error("error");
	m_builders.push_back(std::move(builder));
}

propertyDataVecOfPropertySetCreatorFactor& propertyDataVecOfPropertySetCreatorFactor::getInstance()
{
	static propertyDataVecOfPropertySetCreatorFactor instance;
	return instance;
}

std::unique_ptr<propertyDataVecOfPropertySetCreator> propertyDataVecOfPropertySetCreatorFactor::create(const std::vector<QString>& propertynamevec)
{
	std::unique_ptr<propertyDataVecOfPropertySetCreator> creator = std::make_unique<propertyDataVecOfPropertySetCreator>();
	for (const auto& name : propertynamevec)
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

propertyDataVecOfPropertySetCreatorFactor& propertyDataVecOfPropertySetCreatorFactor::addCreator(QString name, std::function<std::unique_ptr<IpropertyDataBuilder>()> func)
{
	m_builderCreatefunc[name] = func;
	return *this;
}

propertyDataVecOfPropertySetCreatorFactor::propertyDataVecOfPropertySetCreatorFactor()
{
	defaultinit();
}

void propertyDataVecOfPropertySetCreatorFactor::defaultinit()
{
	auto& config = myconfig::getInstance();
	addCreator(config.getPenColorName(), []() {
		return std::make_unique<PenColorDrawParamsPropertyDataBuilder>();
		})
		.addCreator(config.getPenStyleName(), []() {
		return std::make_unique<PenStyleDrawParamsPropertyDataBuilder>();
			})
		.addCreator(config.getPenWdithName(), []() {
		return std::make_unique<PenWidthDrawParamsPropertyDataBuilder>();
			})
		.addCreator(config.getBrushColorName(), []() {
		return std::make_unique<BrushDrawParamsPropertyDataBuilder>();
			})
		.addCreator(config.getRotateAngleName(), []() {
		return std::make_unique<RotateDrawParamsPropertyDataBuilder>();
			})
		.addCreator(config.getSpaceWidthName(), []() {
		return std::make_unique<SpacewidthDrawParamsPropertyDataBuilder>();
			})
		.addCreator(config.getSpaceHeightName(), []() {
		return std::make_unique<SpaceheightDrawParamsPropertyDataBuilder>();
			})
		.addCreator(config.getScaleName(), []() {
		return std::make_unique<ScaleDrawParamsPropertyDataBuilder>();
			})
		.addCreator(config.getCenterHOffsetName(), []() {
		return std::make_unique<CenterHoffsetDrawParamsPropertyDataBuilder>();
			})
		.addCreator(config.getCenterVOffsetName(), []() {
		return std::make_unique<CenterVoffsetDrawParamsPropertyDataBuilder>();
			})
		.addCreator(config.getNameName(), []() {
		return std::make_unique<NamePropertyDataBuilder>();
			})
		.addCreator(config.getRectRadioName(), []() {
		return std::make_unique<RectRadioDrawParamsPropertyDataBuilder>();
			})
		.addCreator(config.getCircleRadioName(), []() {
		return std::make_unique<CircleRadioDrawParamsPropertyDataBuilder>();
			})
		.addCreator(config.getTriangleRadioName(), []() {
		return std::make_unique<TriangleRadioDrawParamsPropertyDataBuilder>();
			})
		.addCreator(config.getEdgeTypeName(), []() {
		return std::make_unique<TriangleEdgetypeDrawParamsPropertyDataBuilder>();
			})
		.addCreator(config.getFontFamilyName(), []() {
		return std::make_unique<TextFontFamilyDrawParamsPropertyDataBuilder>();
			})
		.addCreator(config.getFontSizeName(), []() {
		return std::make_unique<TextFontSizeDrawParamsPropertyDataBuilder>();
			})
		.addCreator(config.getCanvasHeightName(), []() {
		return std::make_unique<HuabuHeightPropertyDataBuilder>();
			})
		.addCreator(config.getCanvasWidthName(), []() {
		return std::make_unique<HuabuWidthPropertyDataBuilder>();
			})
		.addCreator(config.getCanvasScaleName(), []() {
		return std::make_unique<HuabuScalePropertyDataBuilder>();
			});
}

void HuabuScalePropertyDataBuilder::probuild(std::shared_ptr<otherPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto data = std::make_shared<propertydata>(myconfig::getInstance().getCanvasScaleName()
		, QVariant::fromValue(set->m_scale));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &otherPropertySet::onCanvasScaleChanged);
}

void PenStyleDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto data = std::make_shared<propertydata>(myconfig::getInstance().getPenStyleName()
		, QVariant::fromValue(myqtsvg::PenStyleToQstring(set->m_params->getPenstyle())));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onPenStyleChanged);
}
