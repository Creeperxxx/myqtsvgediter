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
enum class ShapeType
{
	juxing
};


class IDiagramItem : public QWidget
{
	Q_OBJECT
public:
	IDiagramItem(ShapeType type,QString picturePath, QWidget* parent = nullptr);
	virtual ~IDiagramItem() {}
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override; //所有绘画相关的都只能在paintEvent中进行啊我去
	//virtual void drawPixmap(QPainter* painter, QPixmap* pixmap) = 0;

	QPixmap drawPixmap();
	QMimeData* createMimedata();
	//void drawmyself();
	//void loadpictureasmyself(QString path);
	//void initLayout();
	void initSizePolicy();
	//void rectByDraw(QPainter* painter);
	//void rectByLoadpic(QPainter* painter);
	//void drawRect();
	//void mysetlayout();
	


	QPoint dragstartposition; //鼠标按压时的位置
	ShapeType m_shapetype; //形状类型
	//QHBoxLayout* m_layout; //默认用一个布局
	QString m_picturePath; //图片路径
	QSize m_size;
	//QHBoxLayout* m_hlayout;

private:
	std::unique_ptr<drawtuxingkuzujian> gettuxignzujian(ShapeType type);
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

public:
	virtual void drawByDraw(QPainter* painter);
	void drawByLoadpic(QPainter* painter);
	void draw(QPainter* painter);
	void setPicpath(QString picpath);
	void setSourceWidgetSize(QSize size);

protected:
	QPixmap getSuitablePicPixmap(QPixmap pixmap);
	bool m_drawByPainter;
	bool m_drawByLoadpic;
	QString m_picpath;
	QSize m_sourceWidgetSize;
};

class drawtuxingkuzujianjuxing :public drawtuxingkuzujian
{
public:
	void drawByDraw(QPainter* painter) override;

private:
	QRect getSuitableRect();

	QSize m_rectSize;
};

class drawtuxingkuzujianfactory
{
public:
	static std::unique_ptr<drawtuxingkuzujian> createtuxingzujian(ShapeType type);
};
