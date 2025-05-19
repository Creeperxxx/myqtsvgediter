#pragma once
#include <qobject.h>
#include <qvariant.h>
#include <qstring.h>

class propertydata : public QObject 
{
	Q_OBJECT

signals:
	void signalValueChanged(QVariant value);

public:
	propertydata(QString name, QVariant data);
	QString getName();
	QVariant getValue();


	void slotValueChanged(QVariant value);
public:
	QString m_name;
	QVariant m_data;

};
