#ifndef PROPERTYWIDGET_H
#define PROPERTYWIDGET_H

#include <qwidget.h>
#include <qformlayout.h>
#include <map>
#include <vector>
#include <qstackedwidget.h>
#include "namespace.h"




class IpropertyDelegate;
class propertydata;
class IdelegatePramas;
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

	PropertyWidgetManager(QWidget* parent);
	QStackedWidget* getstackwidget();
	void dealclicked(std::shared_ptr<propertySetManager> setmanager);
	void addPropertyWidget(myqtsvg::propertyWidgetType type, propertyWidget* widget);
private:

	std::map<myqtsvg::propertyWidgetType, propertyWidget*> m_propertyMap;
	QStackedWidget* m_propertyStackWidget;

};


class propertyWidgetManagerIniter
{
public:
	void initmanager(PropertyWidgetManager* widgetmanager);
	static propertyWidgetManagerIniter& getInstance();
	void add(myqtsvg::propertyWidgetType type, std::function<propertyWidget* ()> func);
private:
	propertyWidgetManagerIniter();
	void defaultinit();

	propertyWidget* createDefault();
	propertyWidget* createDiagramRect();
	propertyWidget* createDiagramCircle();
	propertyWidget* createDiagramTriangle();
	propertyWidget* createDiagramLine();
	propertyWidget* createDiagramChoose();
	propertyWidget* createDiagramMouse();
	propertyWidget* createDiagramText();
	propertyWidget* createDiagramPentagon();
	propertyWidget* createDiagramHexagon();
	propertyWidget* createDiagramStar();
	propertyWidget* createCanvas();
	
	void buildName(propertyWidget* widget);
	void buildPen(propertyWidget* widget);
	void buildBrush(propertyWidget* widget);
	void buildCenterMove(propertyWidget* widget);
	void buildScale(propertyWidget* widget);
	void buildRotate(propertyWidget* widget);
	void buildSpacesize(propertyWidget* widget);
	void buildRectRadio(propertyWidget* widget);
	void buildCircleRadio(propertyWidget* widget);
	void buildTriangleRadios(propertyWidget* widget);
	void buildTriangleEdgeType(propertyWidget* widget);
	void buildCanvasSize(propertyWidget* widget);
	void buildFont(propertyWidget* widget);
	void buildPenColor(propertyWidget* widget);
	void buildCanvasScale(propertyWidget* widget);
	
	std::map<myqtsvg::propertyWidgetType, std::function<propertyWidget*()>> m_map;
};
#endif // PROPERTYWIDGET_H
