#ifndef PROPERTYDATA_H
#define PROPERTYDATA_H


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
	void setValue(QVariant value);
	void setName(QString name);

	void slotValueChanged(QVariant value);
private:
	QString m_name;
	QVariant m_value;

};

#endif // PROPERTYDATA_H
