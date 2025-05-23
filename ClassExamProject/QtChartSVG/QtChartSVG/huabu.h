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



class drawStragety
{
public:
	virtual void handlePress(QPoint point) = 0;
	virtual void handleMove(QPoint point) = 0;
	virtual void handleRelease() = 0;

	QPoint m_startpoint;
	QPoint m_endpoint;
};

class shapeDrawStragety : public drawStragety
{
public:
	shapeDrawStragety(std::shared_ptr<IDidgramDrawParams> params);
	void handlePress(QPoint point) override;
    void handleMove(QPoint point) override;
    void handleRelease() override;

	std::shared_ptr<IDidgramDrawParams> m_params;
};

class mouseDrawStragety : public drawStragety
{
public:
	mouseDrawStragety(std::shared_ptr<IDidgramDrawParams> params);
	void handlePress(QPoint point) override;
    void handleMove(QPoint point) override;
    void handleRelease() override;

	std::shared_ptr<DiagramDrawParamsMouse> m_params;
	std::shared_ptr<QPainterPath> m_path;
};

class TextDrawStragety : public drawStragety
{
public:
    TextDrawStragety(std::shared_ptr<IDidgramDrawParams> params, QWidget* parent);
    void handlePress(QPoint point) override;
    void handleMove(QPoint point) override;
    void handleRelease() override;

	std::shared_ptr<DiagramDrawParamsText> m_params;
	QWidget* m_parent;

};















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
	void saveToSvg(QString filepath);
	void onLoadSvg(QString filepath);
	void loadSvg(QString filepath);
	void onSaveToPng(QString filepath);
	void SaveToPng(QString filepath);
	void onnewHuabu();

	void onCopyTuinxg();
	void oncrtyvTuxing();
	void onartyaTuxing();
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
	void createTextTuxing(std::shared_ptr<DiagramDrawParamsText> params, std::shared_ptr<IDiagramDrawer> drawer);


	std::shared_ptr<IDidgramDrawParams> createDrawParams(std::shared_ptr<IDidgramDrawParams> params);





	std::shared_ptr<propertySetManager> m_setManager;


	std::vector<std::shared_ptr<huabutuxing>> m_tuxingvec;

	QColor m_backgroundcolor;
	QString m_mimetype;

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


	std::shared_ptr<IDidgramDrawParams> m_choosedParams;
	QPoint m_pasteOffset; // 成员变量，初始值为 QPoint(20, 20)
	QPoint m_lastPasteDelta; // 新增成员变量用于记录偏移增量




	QPoint m_currentpoint;
	bool m_isselecting;


	QPoint m_dragoffset;
	std::vector<std::shared_ptr<huabutuxing>> m_selectedShapes;
	std::shared_ptr<DiagramDrawParamsRect> m_selectingParams;
	std::shared_ptr<DiagramDrawerRect> m_selectingDrawer;


	bool m_ischooseingone;
	QRect m_chooseRect;
	QPoint m_offset;
	std::shared_ptr<IDidgramDrawParams> m_choosingParams;
	std::vector<std::shared_ptr<IDidgramDrawParams>>  m_choosedParamsvec;
	std::vector<QPoint> m_offsetvec;

	std::vector<std::shared_ptr<IDidgramDrawParams>> m_neededcopyParamsVec;

	QSize m_basesize;
	double m_scale;



	enum class MouseMode {
		None,
		Selecting,
		Drawing,
		MovingMultiple
	};
	void handleSelectPress(QMouseEvent* event);
	void handleDrawPress(QMouseEvent* event);
	void updateSelectingRect(QMouseEvent* event);
	void handleDrawing(QMouseEvent* event);
	void handleMoveMultiple(QMouseEvent* event);
	void finalizeSelection(QMouseEvent* event);
	void finalizeDrawing(QMouseEvent* event);
	void finalizeMove(QMouseEvent* event);

	MouseMode m_currentMode;

	bool m_ismouseDrawing;

	std::unique_ptr<drawStragety> m_currentDrawStragety;
	bool m_isDrawing;
	std::shared_ptr<IDiagramDrawer> m_drawingDrawer;
	std::shared_ptr<IDidgramDrawParams> m_drawingParams;
};
