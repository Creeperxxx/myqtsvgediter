#include "drawparamitem.h"
#include "drawtool.h"

QVariant penColorParamItem::getValue()
{
	return QVariant::fromValue(m_pencolor);
}

void penColorParamItem::onValueChanged(QVariant value)
{
	if (!value.canConvert<QColor>())
		throw std::runtime_error("error");
	m_pencolor = value.value<QColor>();
	emit SignalValueChanged();
}

QVariant penwidthParamItem::getValue()
{
	return QVariant::fromValue(m_penwidth);
}

void penwidthParamItem::onValueChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_penwidth = value.value<int>();
	emit SignalValueChanged();	
}

QVariant brushColorParamItem::getValue()
{
	return QVariant::fromValue(m_brushcolor);
}

void brushColorParamItem::onValueChanged(QVariant value)
{
	if (!value.canConvert<QColor>())
		throw std::runtime_error("error");
	m_brushcolor = value.value<QColor>();
	emit SignalValueChanged();
}

QVariant rotateParamItem::getValue()
{
	return QVariant::fromValue(m_rotate);
}

void rotateParamItem::onValueChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_rotate = value.value<int>();
	emit SignalValueChanged();
}

QVariant spacewidthParamItem::getValue()
{
	return QVariant::fromValue(m_spacewidth);
}

void spacewidthParamItem::onValueChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_spacewidth = value.value<int>();
	emit SignalValueChanged();
}

QVariant spaceheightParamItem::getValue()
{
	return QVariant::fromValue(m_spaceheight);
}

void spaceheightParamItem::onValueChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_spaceheight = value.value<int>();
	emit SignalValueChanged();
}

QVariant scaleParamItem::getValue()
{
	return QVariant::fromValue(m_scale);
}

void scaleParamItem::onValueChanged(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	m_scale = value.value<double>();
	emit SignalValueChanged();
}

QVariant centerhoffsetParamItem::getValue()
{
	return QVariant::fromValue(m_centerhoffset);
}

void centerhoffsetParamItem::onValueChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_centerhoffset = value.value<int>();
	emit SignalValueChanged();
}

QVariant centervoffsetParamItem::getValue()
{
	return QVariant::fromValue(m_centervoffset);
}

void centervoffsetParamItem::onValueChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_centervoffset = value.value<int>();
	emit SignalValueChanged();
}

QVariant centerParamItem::getValue()
{
	return QVariant::fromValue(m_center);
}

void centerParamItem::onValueChanged(QVariant value)
{
	if (!value.canConvert<QPoint>())
		throw std::runtime_error("error");
	m_center = value.value<QPoint>();
	emit SignalValueChanged();
}


QVariant radioParamItem::getValue()
{
	return QVariant::fromValue(m_radio);
}

void radioParamItem::onValueChanged(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	m_radio = value.value<double>();
	emit SignalValueChanged();
}

totalParamItem::totalParamItem()
{
}

totalParamItem::totalParamItem(QVariant value)
{
	m_value = value;
}

QVariant totalParamItem::getValue()
{
	return m_value;
}

void totalParamItem::onValueChanged(QVariant value)
{
	m_value = value;
	emit SignalValueChanged();
}


void paramItemManager::addParamItem(QString name, std::shared_ptr<IDrawParamItem> item)
{
	if (name.isEmpty() || item == nullptr || item.get() == nullptr)
		throw std::runtime_error("error");

	m_paramItemMap[name] = item;
}

bool paramItemManager::isValueExist(const char* path)
{
	QString name = QString::fromStdString(cfggetval<std::string>(path));
	if (m_paramItemMap.find(name) == m_paramItemMap.end())
		return false;
	else
		return true;
}

