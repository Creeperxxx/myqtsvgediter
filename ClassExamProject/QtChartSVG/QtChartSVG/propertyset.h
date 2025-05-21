#pragma once
#include <qobject.h>
#include <memory>
#include <unordered_map>
#include "propertywidget.h"

class IDidgramDrawParams;
class IpropertySet : public QObject
{
public:
	virtual ~IpropertySet();
	void addShowData(propertyWidget* widget);
public:
	std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> m_propertyDataVec;

};

class drawParamsPropertySet : public IpropertySet
{
	Q_OBJECT
public:
signals:
	void SignalValueChangedByData();
public:
	~drawParamsPropertySet()override;


	//如果绘画参数发生了改变，应该发出哪些信号？
	void onPenColorChanged(QVariant value);
	void onPenWidthChanged(QVariant value);
	void onBrushColorChanged(QVariant value);
	void onRotateChanged(QVariant value);
	void onSpacewidthChanged(QVariant value);
	void onSpaceHeightChanged(QVariant value);
	void onScaleChanged(QVariant value);
	void onCenterHOffset(QVariant value);
	void onCenterVOffset(QVariant value);

	void onRectRadioChanged(QVariant value);
	void onCricleRadioChanged(QVariant value);
	void onTriangleRadioChanged(QVariant value);
	void onTriangleEdgetypeRadioChanged(QVariant value);

	void onTextFamilyChanged(QVariant value);
	void onTextSizeChanged(QVariant value);

	//也许还需要 属性改变通知propertydata的信号和槽机制



	std::shared_ptr<IDidgramDrawParams> m_params;
};

class otherPropertySet :public IpropertySet
{
	Q_OBJECT
signals:
	void signalHuabuWidthChanged(int width);
	void signalHuabuHeightChanged(int height);
public:
	~otherPropertySet()override;

	void onHuabuHeightChanged(QVariant value);
	void onHuabuWidthChanged(QVariant value);


	QString m_name;
	qint64 m_zvalue;
	int m_huabuwidth;
	int m_huabuheight;
};






class propertySetManager
{
public:
	void addPropertySet(QString name, std::shared_ptr<IpropertySet> set);
	std::shared_ptr<IpropertySet> getPropertySet(QString name);

	void dealShowData(propertyWidget* widget);


	std::unordered_map<QString, std::shared_ptr<IpropertySet>> m_propertySetMap;
	PropertyWidgetManager::propertyobjecttype m_propertyObjectType;
};
