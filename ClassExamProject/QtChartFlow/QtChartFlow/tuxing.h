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
//#include "ui_juxingitem.h"
#include <qlabel.h>
#include <qboxlayout.h>
#include <qsizepolicy.h>
#include <memory>
#include <qcolor.h>
#include <optional>
#include <stdexcept>
#include "configmanager.h"
#include "config.h"
#include "drawtool.h"
#include "tuxingjiedian.h"
#include "DiagramMimedata.h"
#include "shuxingwidget.h"
//#include <moc_MyMainWindow.cpp>
//#include "shuxingwidget.h"

//class factory


constexpr auto propertynamediagramrect = "diagram rect";


class DiagramItem;
//class IDiagramDrawParams;


class GfxLibDiagramitemDrawer
{
	//drawtuxingkuzujian(QSize sourceWidgetSize, QString picpath = imagepathjuxing, bool drawbypainter = false, bool drawbyloadpic = false);
public:
	//GfxLibDiagramitemDrawer(bool drawbypainter = false, bool drawbyloadpic = false, bool isdrawbypainter = true);
	static void draw(QPainter& painter, DiagramItem* item);
private:
	static void drawByLoadpic(QPainter& painter, DiagramItem* item);
	static void drawByDraw(QPainter& painter, DiagramItem* item);
	static QPixmap getSuitablePicPixmap(QPixmap pixmap, DiagramItem* item);
	//std::shared_ptr<IDidgramDrawParams> buildparams(DiagramItem* item);

	//bool m_drawByPainter;
	//bool m_drawByLoadpic;
	//bool m_isdrawByPainter;
};




class GfxLibDiagramItemParams
{
//public:
	//class builder
	//{
	//public:
	//	builder();
	//	builder& settype(ShapeType type);
	//	builder& setpicpath(QString path);
	//	builder& setjuxingradio(float radio);
	//	builder& setdrawbypainter(bool flag);
	//	builder& setdrawbyloadpic(bool flag);
	//	builder& setTriangleSideRadios(double bottom, double left, double right);
	//	builder& setTriangleEdgeTypeAndRotate(DiagramDrawParamsTriangle::EdgeType type, double rorate);
	//	GfxLibDiagramItemParams build();
	//private:
	//	void defaultinit();
	//	void setsomethingaftertype(ShapeType type);	
	//	//也许可以为参数分类
	//	bool m_issizefixed;
	//	std::optional<QSizeF> m_fixsize = std::nullopt;
	//	std::optional<QSizeF> m_maxsize = std::nullopt;
	//	std::optional<QSizeF> m_minsize = std::nullopt;
	//	QPen m_pen;
	//	QBrush m_brush;
	//	QString m_mimetype;
	//	QColor m_backgroundcolor;
	//	//std::optional<float> m_widgetradio = std::nullopt;
	//	ShapeType m_type; //todo
	//	std::optional<QString> m_picpath = std::nullopt;
	//	std::optional<float> m_juxingradio = std::nullopt; //todo
	//	QSizeF m_huabutuxingspacesize; 
	//	QPen m_huabutuxingpen;
	//	QBrush m_huabutuxingbrush; //todo:好像没必要，毕竟是透明
	//	bool m_drawByPainter;
	//	bool m_drawByloadpic;
	//	DiagramDrawParamsTriangle::TriangleSizeRadios m_triangleSideRadios;//todo
	//	DiagramDrawParamsTriangle::EdgeType m_triangleEdgeType; //todo
	//	double m_triangleEdgeRotate; //todo
	//	bool m_isdrawByPainter; 
	//};
	//GfxLibDiagramItemParams(bool issizefixed
	//	, std::optional<QSizeF> fixsize
	//	, std::optional<QSizeF> maxsize
	//	, std::optional<QSizeF> minsize
	//	, QPen pen
	//	, QBrush brush
	//	, QString mimetype
	//	, QColor backgroundcolor
	//	, std::optional<QString> picpath
	//	, std::optional<float> juxingradio
	//	, QSizeF huabutuxingspacesize
	//	, QPen huabutuxingpen
	//	, QBrush huabutuxingbrush
	//	//, std::optional<float> widgetradio
	//	, ShapeType type
	//	, bool drawbypainter
	//	, bool drawbyloacpic
	//	, bool isdrawbypainter);
	
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
	QSizeF m_huabutuxingspacesize;
	//QPen m_huabutuxingpen;
	//QBrush m_huabutuxingbrush;
	bool m_drawByPainter; 
	bool m_drawByloadpic; 
	bool m_isdrawByPainter;
	qreal m_scale;
	
	
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
	//IDiagramItem(ShapeType type,QString picturePath = imagepathjuxing, QWidget* parent = nullptr);
	//IDiagramItem(ShapeType type, QString picturepath = imagepathjuxing, QColor* backgroundcolor, QWidget* parent = nullptr);
	virtual ~DiagramItem() {}
	DiagramItem(GfxLibDiagramItemParams params, QWidget* parent = nullptr);
	QString getpicpath();
	QSizeF getselfdrawspacesize();
	QPointF getselfdrawcenter();
	ShapeType gettype();
	double getDiagramItemRectRadio();

	//float getHuabuRectRadio();
	//QPainter* getDiagramItemPainter();
	//QPainter* getDiagramItemPixmapPainter();
	QPoint getPixmapCenter();
	QSizeF getPixmapSpaceSize();
	qreal getPixmapScale();
	QPen gethuabupen();
	QBrush gethuabubrush();
	QPen getdiagrampen();
	QBrush getdiagrambrush();
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

	//void setPropertyWidgetManger(PropertyWidgetManager* manager);

	void createPropertyWidget();
	void buildRectPropertyData();
	void buildCirclePropertyData();
	void buildTrianglePropertyData();
	void buildLinePropertyData();

	PropertyWidgetManager::propertyobjecttype m_propertywidgettype;
	std::unordered_map<QString, std::shared_ptr<propertyItem>> m_propertyDataMap;
	std::vector<std::shared_ptr<propertyItem>> m_propertyDataVec;

	void buildPropertyDataPenAndBrush();




//信号
signals:
	void signalMouseClicked(PropertyWidgetManager::propertyobjecttype type, std::vector<std::shared_ptr<propertyItem>> data);

private:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override; //所有绘画相关的都只能在paintEvent中进行啊我去
	void resizeEvent(QResizeEvent* event) override;
	//virtual void drawPixmap(QPainter* painter, QPixmap* pixmap) = 0;

	//void init(bool issizefixed, std::optional<QSize> fixsize, std::optional<QSize> maxsize, std::optional<QSize> minsize
	//	, QColor color, int penwidth, QBrush brush
	//	, QString mimetype, QColor backgroundcolor, std::optional<float> widgetradio);
	void init();
	QPixmap drawDiagramPixmap(); //创建拖动预览
	QMimeData* createDragMimeData(); //创建拖动mimedata
	QByteArray createDiagramMimedataDeliveryparams();
	void setsizepolicyexpanding();
	void initWidgetSize();
	void initmaxandminsize();
	//void initDiagramDrawer();
	//void initDiagramPixmap();
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


	//void inittuxingzujian();

	//std::shared_ptr<GfxLibDiagramitemDrawer> createtuxing();
	//std::shared_ptr<tuxingjiedianparams> createpixmapparams();
	//void updatetuxingsize();

	//void initpenandbrush(QColor color, int penwidth, QBrush brush);
	//void initpenandbrush();
	//QPixmap createPixmap(QSize targetwidgetsize, QPen targetpen, QBrush targetbrush, QColor targetbackgroundcolor);
	//void initSizePolicy();
	//void initWidgetSize(QSize size);
	//void initWidgetSize(std::optional<QSize> fixsize, std::optional<QSize> maxsize, std::optional<QSize> minsize);
	//void initmaxandminsize(QSize maxsize, QSize minsize);
	//void initmimetype(QString mimetype);
	//void updatetuxingsize(bool iswidgetsizefixed);
	//void drawmyself();
	//void loadpictureasmyself(QString path);
	//void initLayout();
	//void rectByDraw(QPainter* painter);
	//void rectByLoadpic(QPainter* painter);
	//void drawRect();
	//void mysetlayout();

	QPoint dragstartposition; //鼠标按压时的位置
	GfxLibDiagramItemParams m_params;
	QPen m_huabupen;
	QBrush m_huabubrush;
	QSizeF m_huabuspacesize;
	//PropertyWidgetManager* m_propertyWidgetManger;
	//qreal m_pixmapScale;
	
	//std::shared_ptr<GfxLibDiagramitemDrawer> m_diagramDrawer;
	//QPixmap* m_diagramDragPixmap;
	//QMimeData* m_diagramDragMimedata; //貌似一个mimedata只能用一次
	//QString m_mimetype;
	//std::optional<float> m_widgetradio;
	//ShapeType m_shapetype; //形状类型
	//QHBoxLayout* m_layout; //默认用一个布局
	//QString m_picturePath; //图片路径
	//QHBoxLayout* m_hlayout;
	//QPen m_pen;
	//QBrush m_brush;
	//QColor m_backgroundcolor;
	//bool m_iswidgetsizefixed;
};

//class GfxLibDiagramItemDrawerRect :public IGfxLibDiagramitemDrawer
//{
//public:
//	GfxLibDiagramItemDrawerRect();
//	void drawByDraw(QPainter* painter) override;
//	//private:
//		//QRect calcusuitablerect(int penWidth);
//		//QRect m_rect;
//	//private:
//		//QRect getSuitableRect();
//
//};

//class drawtuxingkuzujianfactory
//{
//public:
//	static std::unique_ptr<drawtuxingkuzujian> createtuxingzujian(ShapeType type);
//};

//class drawtuxingkuzujianyuanxing :public drawtuxingkuzujian
//{
//public:
//	drawtuxingkuzujianyuanxing();
//	void drawByDraw(QPainter* painter) override;
//	int m_r;
//private:
//	int getSuitableR();
//};
//
//class GfxLibDiagramItemDrawerCircle : public IGfxLibDiagramitemDrawer
//{
//public:
//	GfxLibDiagramItemDrawerCircle();
//	void drawByDraw(QPainter* painter) override;
//	//private:
//		//int calcusuitable(int penwidth);
//		//int m_r;
//};


//class factorydrawtuxingkuzujian
//{
//public:
//	static std::unique_ptr<drawtuxingkuzujian> create(ShapeType type);
//};



//class juxingDiagramItem : public IDiagramItem
//{
//	Q_OBJECT
//public:
//	~juxingDiagramItem() override;
//	juxingDiagramItem(QWidget* parent = nullptr);
//protected:
//	QPixmap drawPixmap() override;
//};


