#ifndef CANVAS_H
#define CANVAS_H

#include <vector>
#include <qwidget.h>
#include <qsvggenerator.h>
#include <qsvgrenderer.h>
#include <qsettings.h>


class canvasDiagram;
class IDidgramDrawParams;
class propertySetManager;
class drawStragety;
class IDiagramDrawer;

class canvas : public QWidget 
{
	Q_OBJECT

public:
	canvas(QWidget* parent = nullptr);
	~canvas();


	void onDiagramClicked(std::shared_ptr<IDidgramDrawParams> params);  

	void onSaveToSvg(QString filepath);
	void onLoadSvg(QString filepath);
	void onSaveToPng(QString filepath);
	void onNewCanvas();

	void onCopyDiagram();
	void onPasteDiagram();
	void onSelectAllDiagram();
	void onDeleteDiagram();
	void onUndoDiagram();

	void adjustCanvasSize();
protected:

	void dragEnterEvent(QDragEnterEvent* event)override; 
	void dragMoveEvent(QDragMoveEvent* event) override; 
	void dropEvent(QDropEvent* event) override; 
	void paintEvent(QPaintEvent* event) override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;

	void closeEvent(QCloseEvent* event) override;
	void contextMenuEvent(QContextMenuEvent* event)override;

	void onDealDiagramUpdate(QRect rect);
	void onHeightChanged(int height);
	void onWidthChanged(int width);
	void onScaleChagned(double scale);
signals:
	void signalPropertyShow(std::shared_ptr<propertySetManager> setmanager); //显示属性

private:
	void init();
    void myinitPainter(QPainter& painter);
	std::shared_ptr<IDidgramDrawParams> deserParams(QByteArray& array);  //反序列图形绘制参数
	void loadSetting();

	//用来创建一个图形
	void createDiagram(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<IDiagramDrawer> drawer);

    void handleSelectPress();
    void handleDrawPress();
    void updateSelectingRect();
    void handleDrawing();
    void handleMoveMultiple();
    void finalizeSelection();
    void finalizeDrawing();
    void finalizeMove();


	QColor m_backgroundcolor;
	QString m_mimetype;
	QSize m_basesize;
	double m_scale;

	std::vector<std::shared_ptr<canvasDiagram>> m_diagramVec;

	std::shared_ptr<propertySetManager> m_setManager;

	QSvgRenderer* m_svgRenderer;
	QSettings m_setting;

	std::shared_ptr<IDidgramDrawParams> m_drawParams; //当前选择的绘制参数

	enum class MouseMode {
		None,
		Selecting,
		Drawing,
		MovingMultiple
	};
	MouseMode m_currentMode;

	QPoint m_startpoint;
	QPoint m_currentpoint;

	QPoint m_pasteOffset; 
	QPoint m_lastPasteDelta; 

	QRect m_chooseRect;
	QPoint m_rectOffset;

	//被选中的图形的vec
	std::vector<std::shared_ptr<IDidgramDrawParams>>  m_choosedParamsvec;
	std::vector<QPoint> m_offsetvec;  //移动时产生的offset


	std::unique_ptr<drawStragety> m_currentDrawStragety;
	bool m_isDrawing;
	std::shared_ptr<IDiagramDrawer> m_drawingDrawer;  //鼠标拖动时产生的临时图形
	std::shared_ptr<IDidgramDrawParams> m_drawingParams;

	std::vector<std::shared_ptr<IDidgramDrawParams>> m_copyParamsVec; //复制的图形
};

#endif // CANVAS_H
