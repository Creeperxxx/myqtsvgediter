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
#include "propertyset.h"
#include "drawer.h"
#include "canvasdrawstragety.h"






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
	std::shared_ptr<IDidgramDrawParams> m_params;
};











class huabu : public QWidget
{
	Q_OBJECT

public:
	huabu(QWidget* parent = nullptr);
	~huabu();
	void dragEnterEvent(QDragEnterEvent* event)override; //拖动操作进入控件触发，通过mime判断是否接受拖动操作
	void dragMoveEvent(QDragMoveEvent* event) override; //拖动操作在控件中移动时触发，通常用于限制
	void dropEvent(QDropEvent* event) override; //完成拖动时触发，完成数据交换
	void paintEvent(QPaintEvent* event) override;


	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void contextMenuEvent(QContextMenuEvent* event)override;

	void onDiagramClicked(std::shared_ptr<IDidgramDrawParams> params);

	void onSaveToSvg(QString filepath);
	void onLoadSvg(QString filepath);
	void onSaveToPng(QString filepath);
	void onnewHuabu();

	void onCopyTuinxg();
	void onpasteTuxing();
	void onSelectAllTuxing();
	void onDeleteTuxing();

	void closeEvent(QCloseEvent* event) override;

	void adjustcanvassize();
	void onHeightChanged(int height);
	void onWidthChanged(int width);
	void onScaleChagned(double scale);

signals:
	void signalPropertyShow(std::shared_ptr<propertySetManager> setmanager);

private:
	void init();
	void initPainter(QPainter& painter);
	std::shared_ptr<IDidgramDrawParams> deserParams(QByteArray& array);
	void loadSetting();


	void createTuxing(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<IDiagramDrawer> drawer);

	void handleSelectPress(QMouseEvent* event);
	void handleDrawPress(QMouseEvent* event);
	void updateSelectingRect(QMouseEvent* event);
	void handleDrawing(QMouseEvent* event);
	void handleMoveMultiple(QMouseEvent* event);
	void finalizeSelection(QMouseEvent* event);
	void finalizeDrawing(QMouseEvent* event);
	void finalizeMove(QMouseEvent* event);


	std::shared_ptr<propertySetManager> m_setManager;

	std::vector<std::shared_ptr<huabutuxing>> m_tuxingvec;

	QColor m_backgroundcolor;
	QString m_mimetype;


	std::shared_ptr<IDidgramDrawParams> m_drawParams;
	QPoint m_startpoint;
	QPoint m_currentpoint;

	QSvgRenderer* m_svgRenderer;

	QSettings m_setting;

	QPoint m_pasteOffset; // 成员变量，初始值为 QPoint(20, 20)
	QPoint m_lastPasteDelta; // 新增成员变量用于记录偏移增量


	QRect m_chooseRect;
	QPoint m_offset;
	std::vector<std::shared_ptr<IDidgramDrawParams>>  m_choosedParamsvec;
	std::vector<QPoint> m_offsetvec;

	QSize m_basesize;
	double m_scale;


	enum class MouseMode {
		None,
		Selecting,
		Drawing,
		MovingMultiple
	};


	MouseMode m_currentMode;


	std::unique_ptr<drawStragety> m_currentDrawStragety;
	bool m_isDrawing;
	std::shared_ptr<IDiagramDrawer> m_drawingDrawer;
	std::shared_ptr<IDidgramDrawParams> m_drawingParams;

	std::vector<std::shared_ptr<IDidgramDrawParams>> m_copyParamsVec;
};
