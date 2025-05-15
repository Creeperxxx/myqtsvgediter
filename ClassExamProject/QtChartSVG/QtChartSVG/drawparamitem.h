#pragma once
#include <qpen.h>
#include <qvariant.h>
#include <unordered_map>
#include "configmanager.h"


class IDrawParamItem : public QObject
{
	Q_OBJECT
public:
signals:
	void SignalValueChanged();


public:
	virtual QVariant getValue() = 0;
	virtual void onValueChanged(QVariant value) = 0;//delegate传值过来
	
	
};

class penColorParamItem : public IDrawParamItem
{
public:
	QColor m_pencolor;

	QVariant getValue() override;
	void onValueChanged(QVariant value) override;
};

class penwidthParamItem : public IDrawParamItem
{
public:
	int m_penwidth;

	QVariant getValue() override;
	void onValueChanged(QVariant value) override;
};

class brushColorParamItem : public IDrawParamItem
{
public:
	QColor m_brushcolor;

	QVariant getValue()override;
	void onValueChanged(QVariant value) override;
};

class rotateParamItem : public IDrawParamItem
{
public:
	int m_rotate;

	QVariant getValue()override;
	void onValueChanged(QVariant value) override;
};

class spacewidthParamItem : public IDrawParamItem
{
public:
	int m_spacewidth;

	QVariant getValue()override;
	void onValueChanged(QVariant value) override;
};

class spaceheightParamItem : public IDrawParamItem
{
public:
	int m_spaceheight;

	QVariant getValue()override;
	void onValueChanged(QVariant value) override;
};

class scaleParamItem : public IDrawParamItem
{
public:
	double m_scale;
    
	QVariant getValue()override;
	void onValueChanged(QVariant value) override;
};

class centerhoffsetParamItem : public IDrawParamItem
{
public:
	int m_centerhoffset;

	QVariant getValue()override;
	void onValueChanged(QVariant value) override;
};


class centervoffsetParamItem : public IDrawParamItem
{
public:
	int m_centervoffset;

	QVariant getValue()override;
	void onValueChanged(QVariant value) override;
};

class centerParamItem : public IDrawParamItem
{
public:
	QPoint m_center;

	QVariant getValue()override;
	void onValueChanged(QVariant value) override;
};


class radioParamItem : public IDrawParamItem
{
public:
	double m_radio;

	QVariant getValue()override;
	void onValueChanged(QVariant value) override;
};












class totalParamItem : public IDrawParamItem
{
public:
	totalParamItem();
	totalParamItem(QVariant value);
	QVariant m_value;

	QVariant getValue()override;
	void onValueChanged(QVariant value)override;
};










class paramItemManager
{
public:
	void addParamItem(QString name, std::shared_ptr<IDrawParamItem> item);

	template <typename T>
	T getValueByName(const char* path);
	bool isValueExist(const char* path);
	
	std::unordered_map<QString, std::shared_ptr<IDrawParamItem>> m_paramItemMap;
};

template <typename T>
T paramItemManager::getValueByName(const char* path)
{
	QString name = QString::fromStdString(cfggetval<std::string>(path));
	if (m_paramItemMap.find(name) == m_paramItemMap.end())
		throw std::runtime_error("error");
	QVariant value = m_paramItemMap[name]->getValue();
	if (!value.canConvert<T>())
		throw std::runtime_error("error");
	return value.value<T>();
}



