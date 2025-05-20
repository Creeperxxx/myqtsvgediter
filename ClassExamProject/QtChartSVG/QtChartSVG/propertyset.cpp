#include <qpen.h>
#include <qbrush.h>
#include <qfont.h>
#include <qstring.h>
#include <qcolor.h>
#include <qvariant>

#include "diagramdrawparams.h"
#include "propertyset.h"
#include "propertywidget.h"

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
	m_params->m_pen.setColor(value.value<QColor>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onPenWidthChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_params->m_pen.setWidth(value.value<int>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onBrushColorChanged(QVariant value)
{
	if (!value.canConvert<QColor>())
		throw std::runtime_error("error");
	m_params->m_brush.setColor(value.value<QColor>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onRotateChanged(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	m_params->m_rotate = value.value<double>();
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onSpacewidthChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_params->m_spacesize.setWidth(value.value<int>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onSpaceHeightChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_params->m_spacesize.setHeight(value.value<int>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onScaleChanged(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	m_params->m_scale = value.value<double>();
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onCenterHOffset(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_params->m_centerHoffset = value.value<int>();
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onCenterVOffset(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_params->m_centerVoffset = value.value<int>();
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onRectRadioChanged(QVariant value)
{
	if (m_params->m_type != ShapeType::Rect)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsRect>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	if (!value.canConvert<double>())
		throw std::runtime_error("error");

	castparams->m_boundingrectradio = value.value<double>();
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onCricleRadioChanged(QVariant value)
{
	if (m_params->m_type != ShapeType::Circle)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsCircle>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	if (!value.canConvert<double>())
		throw std::runtime_error("error");

	castparams->m_boundingrectradio = value.value<double>();
	emit SignalValueChangedByData();

}

void drawParamsPropertySet::onTriangleRadioChanged(QVariant value)
{
	if (m_params->m_type != ShapeType::Triangle)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	if (!value.canConvert<DiagramDrawParamsTriangle::TriangleSizeRadios>())
		throw std::runtime_error("error");

	castparams->m_triangleSizeRadios = value.value<DiagramDrawParamsTriangle::TriangleSizeRadios>();
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onTriangleEdgetypeRadioChanged(QVariant value)
{
	if (m_params->m_type != ShapeType::Triangle)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	if (!value.canConvert<QString>())
		throw std::runtime_error("error");

	castparams->m_edgetype = DiagramDrawParamsTriangle::edgetypeStringToEnum(value.value<QString>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onTextFamilyChanged(QVariant value)
{
	if (!value.canConvert<QString>())
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsText>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");

	castparams->m_font.setFamily(value.value<QString>());

	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onTextSizeChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsText>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");

	castparams->m_font.setPointSize(value.value<int>());

	emit SignalValueChangedByData();

}

otherPropertySet::~otherPropertySet()
{
}

void otherPropertySet::onHuabuHeightChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_huabuwidth = value.value<int>();
	emit signalHuabuWidthChanged(m_huabuwidth);
}

void otherPropertySet::onHuabuWidthChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_huabuheight = value.value<int>();
	emit signalHuabuHeightChanged(m_huabuheight);
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