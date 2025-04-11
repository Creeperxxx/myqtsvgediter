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

//class factory


class DiagramItem;
//class IDiagramDrawParams;


class GfxLibDiagramitemDrawer
{
	//drawtuxingkuzujian(QSize sourceWidgetSize, QString picpath = imagepathjuxing, bool drawbypainter = false, bool drawbyloadpic = false);
public:
	GfxLibDiagramitemDrawer(bool drawbypainter = false, bool drawbyloadpic = false, bool isdrawbypainter = true);
	void draw(QPainter& painter, DiagramItem* item);
private:
	void drawByLoadpic(QPainter& painter, DiagramItem* item);
	void drawByDraw(QPainter& painter, DiagramItem* item);
	QPixmap getSuitablePicPixmap(QPixmap pixmap, DiagramItem* item);
	std::shared_ptr<IDidgramDrawParams> buildparams(DiagramItem* item);

	bool m_drawByPainter;
	bool m_drawByLoadpic;
	bool m_isdrawByPainter;
};




class GfxLibDiagramItemParams
{
public:
	class builder
	{
	public:
		builder();
		builder& settype(ShapeType type);
		builder& setpicpath(QString path);
		builder& setjuxingradio(float radio);
		builder& setdrawbypainter(bool flag);
		builder& setdrawbyloadpic(bool flag);
		builder& setTriangleSideRadios(double bottom, double left, double right);
		builder& setTriangleEdgeTypeAndRotate(DiagramDrawParamsTriangle::EdgeType type, double rorate);
		GfxLibDiagramItemParams build();
	private:
		void defaultinit();
		bool m_issizefixed;
		std::optional<QSize> m_fixsize = std::nullopt;
		std::optional<QSize> m_maxsize = std::nullopt;
		std::optional<QSize> m_minsize = std::nullopt;
		QPen m_pen;
		QBrush m_brush;
		QString m_mimetype;
		QColor m_backgroundcolor;
		std::optional<float> m_widgetradio = std::nullopt;
		ShapeType m_type;
		std::optional<QString> m_picpath = std::nullopt;
		std::optional<float> m_juxingradio = std::nullopt;
		QSize m_huabutuxingspacesize;
		QPen m_huabutuxingpen;
		QBrush m_huabutuxingbrush;
		bool m_drawByPainter;
		bool m_drawByloadpic;
		DiagramDrawParamsTriangle::TriangleSizeRadios m_triangleSideRadios;
		DiagramDrawParamsTriangle::EdgeType m_triangleEdgeType;
		double m_triangleEdgeRotate;	bool m_isdrawByPainter;
	};
	GfxLibDiagramItemParams(bool issizefixed
		, std::optional<QSize> fixsize
		, std::optional<QSize> maxsize
		, std::optional<QSize> minsize
		, QPen pen
		, QBrush brush
		, QString mimetype
		, QColor backgroundcolor
		, std::optional<QString> picpath
		, std::optional<float> juxingradio
		, QSize huabutuxingspacesize
		, QPen huabutuxingpen
		, QBrush huabutuxingbrush
		, std::optional<float> widgetradio
		, ShapeType type
		, bool drawbypainter
		, bool drawbyloacpic
		, bool isdrawbypainter);

	bool m_issizefixed;
	std::optional<QSize> m_fixsize;
	std::optional<QSize> m_maxsize;
	std::optional<QSize> m_minsize;
	QPen m_pen;
	QBrush m_brush;
	QString m_mimetype;
	QColor m_backgroundcolor;
	std::optional<float> m_widgetradio;
	ShapeType m_type;
	std::optional<QString> m_picpath;
	std::optional<float> m_juxingradio;
	QSize m_huabutuxingspacesize;
	QPen m_huabutuxingpen;
	QBrush m_huabutuxingbrush;
	bool m_drawByPainter;
	bool m_drawByloadpic;
	bool m_isdrawByPainter;
	DiagramDrawParamsTriangle::TriangleSizeRadios m_triangleSideRadios;
	DiagramDrawParamsTriangle::EdgeType m_triangleEdgeType;
	double m_triangleEdgeRotate;

	//QColor m_huabubackgroundcolor;
};



class DiagramItem : public QWidget
{
	Q_OBJECT
public:
	//IDiagramItem(ShapeType type,QString picturePath = imagepathjuxing, QWidget* parent = nullptr);
	//IDiagramItem(ShapeType type, QString picturepath = imagepathjuxing, QColor* backgroundcolor, QWidget* parent = nullptr);
	virtual ~DiagramItem() {}
	DiagramItem(GfxLibDiagramItemParams params, QWidget* parent);
	QString getpicpath();
	QSize getspacesize();
	QPoint getcenter();
	ShapeType gettype();
	float getDiagramItemRectRadio();
	float getHuabuRectRadio();
	//QPainter* getDiagramItemPainter();
	//QPainter* getDiagramItemPixmapPainter();
	QPoint getPixmapCenter();
	QSize getPixmapSpaceSize();
	QPen gethuabupen();
	QBrush gethuabubrush();
	QPen getdiagrampen();
	QBrush getdiagrambrush();

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
	void setsizepolicyexpanding();
	void initWidgetSize();
	void initmaxandminsize();
	void initDiagramDrawer();
	//void initDiagramPixmap();
	void initDiagramPainter(QPainter& painter);
	void initDiagramPixmapPainter(QPainter& painter);
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
	std::shared_ptr<GfxLibDiagramitemDrawer> m_diagramDrawer;
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


