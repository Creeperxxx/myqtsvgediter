#pragma once
#include <qwidget.h>
#include <qformlayout.h>
#include <map>
#include <vector>
#include <qstackedwidget.h>

class IdelegateParams;
class propertydata;
class IpropertyDelegate;


class propertyWidget : public QWidget
{
	Q_OBJECT
public:
	propertyWidget(QWidget* parent = nullptr);
	void setstackwidgetindex(int index);
	int getstackwidgetindex();

	void addPropertyItem(QString name, std::shared_ptr<IdelegatePramas> params);
	void addShowingData(std::shared_ptr<propertydata> data);

private:
	std::shared_ptr<IpropertyDelegate> createDelegate(std::shared_ptr<IdelegatePramas> params);



	std::map<QString, std::shared_ptr<IpropertyDelegate>> m_propertyDelegateMap;
	QFormLayout* m_shuxinglayout;
	int m_stackwidgetindex;
};

class propertySetManager;
class PropertyWidgetManager : public QWidget
{
	Q_OBJECT
public:

	enum class propertyobjecttype
	{
		defaulttype,
		diagramRect,
		diagramCircle,
		diagramTriangle,
		diagramLine,
		diagramMouse,
		diagramText,

		huabu,
		huabuRect,
		huabuCircle,
		huabuTriangle,
		huabuLine,
		huabuMouse,
		huabuText
	};
	PropertyWidgetManager(QWidget* parent);
	QStackedWidget* getstackwidget();
	void dealclicked(std::shared_ptr<propertySetManager> setmanager);
private:

	void createonceWidget();
	propertyWidget* createOriginalPropertyWidget();
	void buildDiagramRectPropertyWidget(propertyWidget* widget);
	void buildDiagramCirclePropertyWidget(propertyWidget* widget);
	void buildDiagramTrianglePropertyWidget(propertyWidget* widget);
	void buildDiagramLinePropertyWidget(propertyWidget* widget);
	void buildDiagramMousePropertyWidget(propertyWidget* widget);
	void buildDiagramTextPropertyWidget(propertyWidget* widget);
	void buildDiagramHuabuPropertyWidget(propertyWidget* widget);
	void buildDefaultPropertyWidget(propertyWidget* widget);


	void addPropertyWidget(propertyobjecttype type, propertyWidget* widget);


	void buildPropertyWidgetName(propertyWidget* widget);
	void buildPropertyWidgetRectRadio(propertyWidget* widget);
	void buildPropertyWidgetRotate(propertyWidget* widget);
	void buildProertyWidgetCircleRadio(propertyWidget* widget);
	void buildPropertyWidgetScale(propertyWidget* widget);
	void buildPropertyWidgetTriangleSideRadio(propertyWidget* widget);
	void buildPropertyWidgetTriangleEdgetype(propertyWidget* widget);
	void buildPropertyWidgetPenAndBrush(propertyWidget* widget);
	void buildPropertyWidgetSpacesize(propertyWidget* widget);
	void buildPropertyWidgetHuabuSize(propertyWidget* widget);
	void buildPropertyWidgetCentermove(propertyWidget* widget);
	void buildPropertyWidgetPen(propertyWidget* widget);





	std::map<propertyobjecttype, propertyWidget*> m_propertyMap;
	QStackedWidget* m_propertyStackWidget;

};