#include "propertydata.h"
#include <qvariant.h>

propertydata::propertydata(QString name, QVariant data)
	: m_name(name)
	, m_value(data)
{
}

void propertydata::slotValueChanged(QVariant value)
{
	m_value = value;
	emit signalValueChanged(value);
}

QString propertydata::getName()
{
	return m_name;
}

QVariant propertydata::getValue()
{
	return m_value;
}

void propertydata::setValue(QVariant value)
{
	m_value = value;
}

void propertydata::setName(QString name)
{
	m_name = name;
}
