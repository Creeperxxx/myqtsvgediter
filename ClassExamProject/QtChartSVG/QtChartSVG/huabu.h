#pragma once
#include <QWidget>
#include <qevent.h>
#include <qmimedata.h>
#include <qdebug.h>
#include <qvector.h>
#include "tuxingjiedian.h"
#include <qpalette.h>
#include "config.h"
#include "tuxing.h"
#include "configmanager.h"
#include <vector>
#include "DiagramMimedata.h"
#include "shuxingwidget.h"
#include "qlinkedlist.h"




class huabutuxing : public QWidget
{
	Q_OBJECT
public:
signals:
	void signalRepaint();

public:
	void buildPropertyData();
	void buildPropertyDataRect();
	void buildPropertyDataCircle();
	void buildPropertyDataTriangle();
	void buildPropertyDataLine();


	void onValueChangedName(QVariant value);
	void onValueChangeRectRadio(QVariant value);
	void onValueChangedRectRotate(QVariant value);
	void onValueChangedScale(QVariant value);
	void onValueChangedSpaceWidth(QVariant value);
	void onValueChangedSpaceHeight(QVariant value);
	void onValueChangedPenColor(QVariant value);
	void onValueChangedPenWidth(QVariant value);
	void onvalueChangedBrushColor(QVariant value);
	void onValueChangedHOffset(QVariant value);
	void onValueChangedVOffset(QVariant value);
	void onValueChangedCircleRadio(QVariant value);
	void onValueChangedCircleRotate(QVariant value);
	void onValueChangedTriangeRadioBottom(QVariant value);
	void onValueChangedTriangeRadioLeft(QVariant value);
	void onValueChangedTriangeRadioRight(QVariant value);
	void onValueChangedTriangleEdgetype(QVariant value);
	void onValueChangedTriangleRotate(QVariant value);
	void onValueChangedLineRotate(QVariant value);

	std::vector<std::shared_ptr<propertydata>> m_propertydataVec;
	QPen m_pen;
	QBrush m_brush;
	std::shared_ptr<IDidgramDrawParams> m_params;
	std::shared_ptr<IDiagramDrawer> m_drawer;
	QString m_name;
	QPointF m_center;
	int m_centerhoffset;
	int m_centervoffset;

	qint64 m_dataTime; //z值
private:
	void buildPropertyDataPenandBrush();
	void buildPropertyDataCenteroffset();
	void buildPropertyDataSpacesize();
	void buildPropertyDataScale();
	void buildPropertyDataName();
};







class huabu : public QWidget
{
	Q_OBJECT

public:
	huabu(QWidget *parent = nullptr);
	~huabu();
	void dragEnterEvent(QDragEnterEvent* event)override; //拖动操作进入控件触发，通过mime判断是否接受拖动操作
	void dragMoveEvent(QDragMoveEvent* event) override; //拖动操作在控件中移动时触发，通常用于限制
	void dropEvent(QDropEvent* event) override; //完成拖动时触发，完成数据交换
	void paintEvent(QPaintEvent* event) override;

	void onValueChangedHuabuSizeWidth(QVariant value);
	void onValueChangedHuabuSizeHeight(QVariant value);

	void mousePressEvent(QMouseEvent* event) override;

signals:
	void signalMouseClicked(PropertyWidgetManager::propertyobjecttype type, std::vector<std::shared_ptr<propertydata>> data);

private:
	void init();
	void initPainter(QPainter& painter);
	

	PropertyWidgetManager::propertyobjecttype shapeTypeToPropertyobjectType(ShapeType type);

	void buildPropertyDataHuaub();
	std::vector<std::shared_ptr<propertydata>> m_propertydataHuabuVec;

	QString createTuxingName(ShapeType type);
	int m_tuxingnum;

	PropertyWidgetManager::propertyobjecttype shapetypeToPropertyType(ShapeType type);



	std::vector<std::shared_ptr<huabutuxing>> m_tuxingvec;
	QLinkedList<std::shared_ptr<huabutuxing>> m_tuxinglist;

	QColor m_backgroundcolor;
	QSize m_size;
	QString m_mimetype; 
};
