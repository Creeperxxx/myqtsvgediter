#ifndef PROPERTYSET_H
#define PROPERTYSET_H

#include <qobject.h>
#include <memory>
#include <unordered_map>
#include "propertywidget.h"
#include "namespace.h"

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
signals:
	void SignalValueChangedByData();
public:
	~drawParamsPropertySet()override;


	void onPenColorChanged(QVariant value);
	void onPenWidthChanged(QVariant value);
	void onPenStyleChanged(QVariant value);
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

	std::shared_ptr<IDidgramDrawParams> m_params;
};

class otherPropertySet :public IpropertySet
{
	Q_OBJECT
signals:
	void signalCanvasWidthChanged(int width);
	void signalCanvasHeightChanged(int height);
	void signalCanvasScaleChanged(double scale);
public:
	~otherPropertySet()override;

	void onCanvasHeightChanged(QVariant value);
	void onCanvasWidthChanged(QVariant value);
	void onCanvasScaleChanged(QVariant value);


	QString m_name;
	int m_canvasWidth;
	int m_canvasHeight;
	double m_scale;
};






class propertySetManager
{
public:
	void addPropertySet(QString name, std::shared_ptr<IpropertySet> set);
	std::shared_ptr<IpropertySet> getPropertySet(QString name);

	void dealShowData(propertyWidget* widget);
	void setWidgetType(myqtsvg::propertyWidgetType type);
	myqtsvg::propertyWidgetType getWidgetType();


private:
	std::unordered_map<QString, std::shared_ptr<IpropertySet>> m_propertySetMap;
	myqtsvg::propertyWidgetType m_propertyWidgetType;
};

class initPropertySetManager
{
public:
	static std::shared_ptr<propertySetManager> createPropertySetManager(
		myqtsvg::propertyWidgetType type
		, std::shared_ptr<IDidgramDrawParams> params
		, std::function<void()> repaintcallback
		, const std::vector<QString>& additionalProperties = {});
		
};

#endif // PROPERTYSET_H
