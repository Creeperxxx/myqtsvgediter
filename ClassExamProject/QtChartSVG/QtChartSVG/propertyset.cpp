#include <qpen.h>
#include <qbrush.h>
#include <qfont.h>
#include <qstring.h>
#include <qcolor.h>
#include <qvariant>

#include "diagramdrawparams.h"
#include "propertyset.h"
#include "propertywidget.h"
#include "myconfig.h"
#include "propertynamevec.h"
#include "propertydatabuilder.h"

void IpropertySet::addShowData(propertyWidget* widget)
{
	for (auto& data : *m_propertyDataVec)
	{
		widget->addShowingData(data);
	}
}

drawParamsPropertySet::~drawParamsPropertySet()
{
}

void drawParamsPropertySet::onPenColorChanged(QVariant value)
{
	if (!value.canConvert<QColor>())
		throw std::runtime_error("error");
	m_params->setPenColor(value.value<QColor>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onPenWidthChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_params->setPenwidth(value.value<int>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onPenStyleChanged(QVariant value)
{
	if (!value.canConvert<QString>())
		throw std::runtime_error("error");
	m_params->setPenStyle(myqtsvg::QstringToPenStyle(value.value<QString>()));

	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onBrushColorChanged(QVariant value)
{
	if (!value.canConvert<QColor>())
		throw std::runtime_error("error");
	m_params->setBrushColor(value.value<QColor>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onRotateChanged(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	m_params->setRotate(value.value<double>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onSpacewidthChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	QSize spacesize = m_params->getSpacesize();
	spacesize.setWidth(value.value<int>());
	m_params->setSpacesize(spacesize);
	emit SignalValueChangedByData();

}

void drawParamsPropertySet::onSpaceHeightChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	QSize size = m_params->getSpacesize();
	size.setHeight(value.value<int>());
	m_params->setSpacesize(size);
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onScaleChanged(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	m_params->setScale(value.value<double>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onCenterHOffset(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_params->setCenterHOffset(value.value<int>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onCenterVOffset(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_params->setCenterVOffset(value.value<int>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onRectRadioChanged(QVariant value)
{
	if (m_params->getType() != myqtsvg::ShapeType::Rect)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsRect>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	if (!value.canConvert<double>())
		throw std::runtime_error("error");

	castparams->setRadio(value.value<double>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onCricleRadioChanged(QVariant value)
{
	if (m_params->getType() != myqtsvg::ShapeType::Circle)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsCircle>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	if (!value.canConvert<double>())
		throw std::runtime_error("error");

	castparams->setRadio(value.value<double>());
	emit SignalValueChangedByData();

}

void drawParamsPropertySet::onTriangleRadioChanged(QVariant value)
{
	if (m_params->getType() != myqtsvg::ShapeType::Triangle)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	if (!value.canConvert<DiagramDrawParamsTriangle::sideRadios>())
		throw std::runtime_error("error");

	castparams->setRadios(value.value<DiagramDrawParamsTriangle::sideRadios>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onTriangleEdgetypeRadioChanged(QVariant value)
{
	if (m_params->getType() != myqtsvg::ShapeType::Triangle)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	if (!value.canConvert<QString>())
		throw std::runtime_error("error");

	castparams->setEdgeType(DiagramDrawParamsTriangle::edgetypeStringToEnum(value.value<QString>()));
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onTextFamilyChanged(QVariant value)
{
	if (!value.canConvert<QString>())
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsText>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");

	castparams->setFontFamily(value.value<QString>());

	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onTextSizeChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsText>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");

	castparams->setFontSize(value.value<int>());

	emit SignalValueChangedByData();

}

otherPropertySet::~otherPropertySet()
{
}

IpropertySet::~IpropertySet()
{

}

void otherPropertySet::onCanvasHeightChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_canvasWidth = value.value<int>();
	emit signalCanvasWidthChanged(m_canvasWidth);
}

void otherPropertySet::onCanvasWidthChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_canvasHeight = value.value<int>();
	emit signalCanvasHeightChanged(m_canvasHeight);
}

void otherPropertySet::onCanvasScaleChanged(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	m_scale = value.value<double>();
	emit signalCanvasScaleChanged(m_scale);
}

void propertySetManager::addPropertySet(QString name, std::shared_ptr<IpropertySet> set)
{
	if (m_propertySetMap.find(name) != m_propertySetMap.end())
		throw std::runtime_error("error");
	m_propertySetMap[name] = set;
}

std::shared_ptr<IpropertySet> propertySetManager::getPropertySet(QString name)
{
	if (m_propertySetMap.find(name) == m_propertySetMap.end())
		throw std::runtime_error("error");
	return m_propertySetMap[name];
}


void propertySetManager::dealShowData(propertyWidget* widget)
{
	for (auto& data : m_propertySetMap)
	{
		data.second->addShowData(widget);
	}
}

void propertySetManager::setWidgetType(myqtsvg::propertyWidgetType type)
{
	m_propertyWidgetType = type;
}

myqtsvg::propertyWidgetType propertySetManager::getWidgetType()
{
	return m_propertyWidgetType;
}

std::shared_ptr<propertySetManager> initPropertySetManager::createPropertySetManager(myqtsvg::propertyWidgetType type
	, std::shared_ptr<IDidgramDrawParams> params
	, std::function<void()> repaintcallback
	, const std::vector<QString>& additionalProperties)
{
	auto& config = myconfig::getInstance();

	auto setManager = std::make_shared<propertySetManager>();
	setManager->setWidgetType(type);

	auto drawParamsSet = std::make_shared<drawParamsPropertySet>();
	drawParamsSet->m_params = params;

	auto propertynamevec = propertyNameVecInterface::getinstance().getPropertyNameVec(params->getType(), additionalProperties);
	auto creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
	drawParamsSet->m_propertyDataVec = creator->create(drawParamsSet);
	QObject::connect(drawParamsSet.get(), &drawParamsPropertySet::SignalValueChangedByData, repaintcallback);
	setManager->addPropertySet(config.getDrawParamsSetName(), drawParamsSet);

	auto otherset = std::make_shared<otherPropertySet>();
	otherset->m_name = myqtsvg::ShapetypeEnumToQstring(params->getType());
	propertynamevec.clear();
	propertynamevec.push_back(config.getNameName());
	creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
	otherset->m_propertyDataVec = creator->create(otherset);
	setManager->addPropertySet(config.getOtherSetName(), otherset);

	return setManager;
}
