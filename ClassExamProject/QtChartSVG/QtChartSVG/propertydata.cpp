#include "propertydata.h"
#include <qvariant.h>

propertydata::propertydata(QString name, QVariant data)
	: m_name(name)
	, m_data(data)
{
}

void propertydata::slotValueChanged(QVariant value)
{
	m_data = value;
	emit signalValueChanged(value);
}

QString propertydata::getName()
{
	return m_name;
}

QVariant propertydata::getValue()
{
	return m_data;
}
