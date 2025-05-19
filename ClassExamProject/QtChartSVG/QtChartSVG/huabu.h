#pragma once
#include <QWidget>
#include <qevent.h>
#include <qmimedata.h>
#include <qdebug.h>
#include <vector>
#include <list>
#include <cmath>
#include <qsvggenerator.h>
#include <qsettings.h>
#include <qsvgrenderer.h>
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


	void enterEvent(QEvent* event) override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	void onDiagramClicked(std::shared_ptr<IDidgramDrawParams> params);

	void onSaveToSvg(QString filepath);
	void saveToSvg(QString filepath);
	void onLoadSvg(QString filepath);
	void loadSvg(QString filepath);
	void onSaveToPng(QString filepath);
	void SaveToPng(QString filepath);
	void onnewHuabu();

	void onCopyTuinxg();
	void oncrtyvTuxing();

	void closeEvent(QCloseEvent* event) override;

signals:
	void signalPropertyShow(std::shared_ptr<propertySetManager> setmanager);

private:
	void init();
	void initPainter(QPainter& painter);


	void createTuxing(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<IDiagramDrawer> drawer);
	PropertyWidgetManager::propertyobjecttype shapetypeToObjectType(ShapeType type);

	void createTextTuxing(std::shared_ptr<DiagramDrawParamsText> params, std::shared_ptr<IDiagramDrawer> drawer);

	
	std::shared_ptr<IDidgramDrawParams> createDrawParams(std::shared_ptr<IDidgramDrawParams> params);


	std::shared_ptr<std::vector<QString>> createNameVec(ShapeType type);
	std::shared_ptr<std::vector<QString>> rectCreateNameVec();
	std::shared_ptr<std::vector<QString>> circleCreateNameVec();
	std::shared_ptr<std::vector<QString>> triangleCreateNameVec();
	std::shared_ptr<std::vector<QString>> lineCreateNameVec();
	std::shared_ptr<std::vector<QString>> mouseCreateNameVec();
	std::shared_ptr<std::vector<QString>> textCreateNameVec();
	



	QString createTuxingName(ShapeType type);
	int m_tuxingnum;


	std::shared_ptr<propertySetManager> m_setManager;


	std::vector<std::shared_ptr<huabutuxing>> m_tuxingvec;
	std::list<std::shared_ptr<huabutuxing>> m_tuxinglist;

	QColor m_backgroundcolor;
	QString m_mimetype; 

	bool m_ismouseDrawing;
	bool m_isdrawing;
	bool m_hasmove;

	std::shared_ptr<IDidgramDrawParams> m_drawParams;
	std::shared_ptr<IDiagramDrawer> m_drawer;
	std::shared_ptr<IDidgramDrawParams> m_copyParams;
	QPoint m_startpoint;
	QPointF m_endpoint;
	std::shared_ptr<QPainterPath> m_mousepath;
	
	QSvgRenderer* m_svgRenderer;

	QSettings m_setting;

	std::shared_ptr<DiagramDrawParamsRect> m_chooseParams;
	std::shared_ptr<IDiagramDrawer> m_chooseDrawer;
	bool m_ischoosingFirst;
	bool m_ischoosingSecond;


	std::shared_ptr<IDidgramDrawParams> m_choosedParams;
	QPoint m_pasteOffset; // 成员变量，初始值为 QPoint(20, 20)
	QPoint m_lastPasteDelta; // 新增成员变量用于记录偏移增量

	bool m_needcopy;
};
