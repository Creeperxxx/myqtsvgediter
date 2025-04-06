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
#include "config.h"
#include <qsizepolicy.h>
#include <memory>
#include <qcolor.h>
#include <optional>
#include <stdexcept>

enum class ShapeType
{
	juxing,
	yuanxing
};

class drawtuxingkuzujian;
class drawtuxingkuzujianjuxing;
class drawtuxingkuzujianfactory;


class IDiagramItem : public QWidget
{
	Q_OBJECT
public:
	//IDiagramItem(ShapeType type,QString picturePath = imagepathjuxing, QWidget* parent = nullptr);
	//IDiagramItem(ShapeType type, QString picturepath = imagepathjuxing, QColor* backgroundcolor, QWidget* parent = nullptr);
	virtual ~IDiagramItem() {}
private:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override; //所有绘画相关的都只能在paintEvent中进行啊我去
	//virtual void drawPixmap(QPainter* painter, QPixmap* pixmap) = 0;
	void resizeEvent(QResizeEvent* event) override;

	void init(bool issizefixed, std::optional<QSize> fixsize, std::optional<QSize> maxsize, std::optional<QSize> minsize
		, QColor color, int penwidth, QBrush brush
		, QString mimetype, QColor backgroundcolor, std::optional<float> widgetradio);
	QPixmap createPixmap(QSize targetwidgetsize, QPen targetpen, QBrush targetbrush, QColor targetbackgroundcolor);
	QMimeData* createMimedata(QString mimetype);
	//void initSizePolicy();
	void setsizepolicyexpanding();
	//void initWidgetSize(QSize size);
	void initWidgetSize(std::optional<QSize> fixsize, std::optional<QSize> maxsize, std::optional<QSize> minsize);
	void initmaxandminsize(QSize maxsize, QSize minsize);
	void inittuxingzujian();
	void initpenandbrush(QColor color, int penwidth, QBrush brush);
	//void initmimetype(QString mimetype);
	void updatetuxingsize();

	//void updatetuxingsize(bool iswidgetsizefixed);
	//void drawmyself();
	//void loadpictureasmyself(QString path);
	//void initLayout();
	//void rectByDraw(QPainter* painter);
	//void rectByLoadpic(QPainter* painter);
	//void drawRect();
	//void mysetlayout();

	QString m_mimetype;
	QPoint dragstartposition; //鼠标按压时的位置
	ShapeType m_shapetype; //形状类型
	//QHBoxLayout* m_layout; //默认用一个布局
	QString m_picturePath; //图片路径
	std::unique_ptr<drawtuxingkuzujian> m_tuxing;
	//QHBoxLayout* m_hlayout;
	QPen m_pen;
	QBrush m_brush;
	QColor m_backgroundcolor;
	bool m_iswidgetsizefixed;
	std::optional<float> m_widgetradio;
};

//class juxingDiagramItem : public IDiagramItem
//{
//	Q_OBJECT
//public:
//	~juxingDiagramItem() override;
//	juxingDiagramItem(QWidget* parent = nullptr);
//protected:
//	QPixmap drawPixmap() override;
//};

class drawtuxingkuzujian
{
	//drawtuxingkuzujian(QSize sourceWidgetSize, QString picpath = imagepathjuxing, bool drawbypainter = false, bool drawbyloadpic = false);
public:
	virtual void drawByDraw(QPainter* painter);
	void drawByLoadpic(QPainter* painter);
	void draw(QPainter* painter);
	void setSourcewidgetsize(QSize size);
	void setpicpath(QString path);

	bool m_drawByPainter = false;
	bool m_drawByLoadpic = false;
	QString m_picpath;
	QSize m_sourceWidgetSize;

protected:
	QPixmap getSuitablePicPixmap(QPixmap pixmap);
};

class drawtuxingkuzujianjuxing :public drawtuxingkuzujian
{
public:
	drawtuxingkuzujianjuxing();
	void drawByDraw(QPainter* painter) override;
private:
	QRect calcusuitablerect(int penWidth);
	QRect m_rect;
//private:
	//QRect getSuitableRect();

};

class drawtuxingkuzujianfactory
{
public:
	static std::unique_ptr<drawtuxingkuzujian> createtuxingzujian(ShapeType type);
};

//class drawtuxingkuzujianyuanxing :public drawtuxingkuzujian
//{
//public:
//	drawtuxingkuzujianyuanxing();
//	void drawByDraw(QPainter* painter) override;
//	int m_r;
//private:
//	int getSuitableR();
//};

class drawtuxingkuzujianyuanxing : public drawtuxingkuzujian
{
public:
	drawtuxingkuzujianyuanxing();
	void drawByDraw(QPainter* painter) override;
private:
	int calcusuitable(int penwidth);
	int m_r;
};
