#pragma once
#include <QWidget>
#include <qevent.h>
#include <qmimedata.h>
#include <qdebug.h>
#include <vector>
#include <list>
#include "shuxingwidget.h"
#include "tuxingjiedian.h"
#include "configmanager.h"




class huabutuxing : public QObject
{
	Q_OBJECT
public:
	huabutuxing();


signals:
	void signalRepaint();

public:


	std::shared_ptr<propertySetManager> m_propertySetManager;
	std::shared_ptr<IDiagramDrawer> m_drawer;
};















//data datamanager   一个个data代表一个个属性，然后通过add方式交由datamanager管理。
//datamanager tuxing datamanager交给tuxing管理
//tuxing listerner listermanager tuxing持有listermanager 后者管理一系列listerner
//然后data发出信号给manager manager发出信号给tuxing tuxing发出信号给manager manage
//manager发出信号给listerner listerner看该属性是否由自己管理，如果是，则update






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
	void signalMouseClicked(std::shared_ptr<propertySetManager> setmanager);

private:
	void init();
	void initPainter(QPainter& painter);
	



	QString createTuxingName(ShapeType type);
	int m_tuxingnum;


	std::shared_ptr<propertySetManager> m_setManager;


	std::vector<std::shared_ptr<huabutuxing>> m_tuxingvec;
	std::list<std::shared_ptr<huabutuxing>> m_tuxinglist;

	QColor m_backgroundcolor;
	QSize m_size;
	QString m_mimetype; 
};
