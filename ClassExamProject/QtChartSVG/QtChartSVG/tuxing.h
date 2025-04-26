#pragma once
#include <qwidget.h>
#include <qpoint.h>
#include <qmimedata.h>
#include <qpainter.h>
#include <qevent.h>
#include <qbytearray.h>
#include <qdrag.h>
#include <qapplication.h>
#include <qimage.h>
#include <qlabel.h>
#include <qboxlayout.h>
#include <qsizepolicy.h>
#include <memory>
#include <qcolor.h>
#include <stdexcept>
#include "drawtool.h"
#include "tuxingjiedian.h"
#include "DiagramMimedata.h"
#include "shuxingwidget.h"
#include <optional>
#include "configmanager.h"
#include "config.h"



constexpr auto propertynamediagramrect = "diagram rect";


class DiagramItem;


class GfxLibDiagramitemDrawer
{
public:
	//GfxLibDiagramitemDrawer(bool drawbypainter = false, bool drawbyloadpic = false, bool isdrawbypainter = true);
	static void draw(QPainter& painter, DiagramItem* item);
private:
	static void drawByLoadpic(QPainter& painter, DiagramItem* item);
	static void drawByDraw(QPainter& painter, DiagramItem* item);
	static QPixmap getSuitablePicPixmap(QPixmap pixmap, DiagramItem* item);
};




class GfxLibDiagramItemParams
{
	
public:
	GfxLibDiagramItemParams(ShapeType type);
	bool m_issizefixed;
	std::optional<double> m_widgetradio;
	std::optional<QSizeF> m_fixsize;
	std::optional<QSizeF> m_maxsize;
	std::optional<QSizeF> m_minsize;
	QPen m_pen;
	QBrush m_brush;
	QString m_mimetype;
	QColor m_backgroundcolor;
	ShapeType m_type;
	std::optional<QString> m_picpath; 
	bool m_drawByPainter; 
	bool m_drawByloadpic; 
	bool m_isdrawByPainter;
	qreal m_scale;
	QSizeF m_spacesize;
	
	
	std::optional<double> m_rectRadio;
	void setRectRadio(double radio);
	std::optional<int> m_rectRotate;
	void setRectRotate(int rotate);

	std::optional<DiagramDrawParamsTriangle::TriangleSizeRadios> m_triangleSideRadios; 
	void setTriangleSideRadio(double bottom, double left, double right);
	std::optional<DiagramDrawParamsTriangle::EdgeType> m_triangleEdgeType; 
	void setTriangleEdgeType(const QString& edgetype);
	std::optional<double> m_triangleEdgeRotate;
	void setTriangleRotate(double rotate);

	std::optional<double> m_circleboundingrectradio; 
	void setCircleRadio(double radio);
	std::optional<int> m_circlerotate;
	void setCircleRotate(int rotate);

	std::optional<double> m_linerotate;
	void setLineRotate(double rotate);
private:
	void defaultinit();
	void otherInitAfterType();


	//QColor m_huabubackgroundcolor;
};


class PropertyWidgetManager;
class propertyItem;
enum class PropertyWidgetManager::propertyobjecttype;
enum class delegateType;

class DiagramItem : public QWidget
{
	Q_OBJECT
public:
	virtual ~DiagramItem() {}
	DiagramItem(GfxLibDiagramItemParams params, QWidget* parent = nullptr);
	QString getpicpath();
	QSizeF getselfdrawspacesize();
	QPointF getselfdrawcenter();
	ShapeType gettype();
	double getDiagramItemRectRadio();

	QPointF getPixmapCenter();
	QSizeF getPixmapSpaceSize();
	double getCircleBoundingrectradio();
	DiagramDrawParamsTriangle::TriangleSizeRadios gettrianglesideradio();
	DiagramDrawParamsTriangle::EdgeType getedgetype();
	double getTriangleRotate();
	double getLineRotate();

	std::shared_ptr<IDidgramDrawParams> builddrawparams();
	std::shared_ptr<IDidgramDrawParams> buildPixmapDrawParams();

	bool getdrawbypainter();
	bool getdrawbyloadpic();
	bool getisdrawbypainter();


	void onPenColorChanged(QVariant value);
	void onPenWidthChanged(QVariant newwidth);
	void onPenBrushChanged(QVariant newbrush);

	void onRectRadioChanged(QVariant value);
	void onRectRotateChanged(QVariant value);

	void onCircleRadioChanged(QVariant newradio);
	void onCircleRotateChanged(QVariant newrottate);

	void onTriangleSideRadioChangedBottom(QVariant bottom);
	void onTriangleSideRadioChangedLeft(QVariant left);
	void onTriangleSideRadioChangedRight(QVariant right);
	void onTriangleEdgeTypeChanged(QVariant value);
	void onTriangleEdgeRotateChanged(QVariant value);

	void onLineRotateChanged(QVariant rotate);

	void onScaleChanged(QVariant value);

	void onSpacesizeWidthChanged(QVariant value);
	void onSpacesizeHeightChanged(QVariant value);

	//void setPropertyWidgetManger(PropertyWidgetManager* manager);

	void createPropertyWidget();
	void buildRectPropertyData();
	void buildCirclePropertyData();
	void buildTrianglePropertyData();
	void buildLinePropertyData();

	PropertyWidgetManager::propertyobjecttype m_propertywidgettype;
	std::vector<std::shared_ptr<propertydata>> m_propertyDataVec;

	void buildPropertyDataPenandBrush();




//信号
signals:
	void signalMouseClicked(PropertyWidgetManager::propertyobjecttype type, std::vector<std::shared_ptr<propertydata>> data);

private:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override; //所有绘画相关的都只能在paintEvent中进行啊我去
	void resizeEvent(QResizeEvent* event) override;

	void init();
	QPixmap drawDiagramPixmap(); //创建拖动预览
	QMimeData* createDragMimeData(); //创建拖动mimedata
	QByteArray createDiagramMimedataDeliveryparams();
	void setsizepolicyexpanding();
	void initWidgetSize();
	void initmaxandminsize();
	void initDiagramPainter(QPainter& painter);
	void initDiagramPixmapPainter(QPainter& painter);

	void buildRectMimedata(DiagramMimedata& data);
	void buildCircleMimedata(DiagramMimedata& data);
	void buildTriangleMimedata(DiagramMimedata& data);
	void buildLineMimedata(DiagramMimedata& data);

	std::shared_ptr<IDidgramDrawParams> builddrawparamsrest(std::shared_ptr<IDidgramDrawParams> params);
	std::shared_ptr<IDidgramDrawParams> builddrawparamsrect();
	std::shared_ptr<IDidgramDrawParams> builddrawparamscircle();
	std::shared_ptr<IDidgramDrawParams> builddrawparamstriangle();
	std::shared_ptr<IDidgramDrawParams> builddrawparamsline();

	std::shared_ptr<IDidgramDrawParams> buildPixmapDrawParamsRest(std::shared_ptr<IDidgramDrawParams> params);

	std::shared_ptr<IDidgramDrawParams> buildspecialbytype();


	QPoint dragstartposition; //鼠标按压时的位置
	GfxLibDiagramItemParams m_params;
};



