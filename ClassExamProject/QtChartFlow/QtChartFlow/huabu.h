#pragma once

#include <QWidget>
//#include "ui_huabu.h"
#include <qevent.h>
#include <qmimedata.h>
#include <qdebug.h>
#include <qvector.h>
#include "tuxingjiedian.h"
#include <qpalette.h>
#include "config.h"
#include "tuxing.h"
#include "configmanager.h"
#include <vector>
#include "DiagramMimedata.h"

//class tuxingjiedianfactory
//{
//public:
//	//static Ituxingjiedian* createtuxignjiedian(ShapeType type, tuxingjiedianparams* params);
//	static std::unique_ptr<Ituxingjiedian> createtuxingjiedian(ShapeType type, std::unique_ptr<tuxingjiedianparams> params);
//};



class huabutuxing
{

public:
	std::shared_ptr<IDidgramDrawParams> m_params;
	std::shared_ptr<DrawResult> m_ret;
};







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
	//QSize sizeHint() const override;

	//void mousePressEvent(QMouseEvent* event) override;
	//void mouseMoveEvent(QMouseEvent* event) override;
	//void mouseReleaseEvent(QMouseEvent* event) override;

	DiagramMimedata getmimedata();
	QPen getpen();
	QBrush getbrush();
	QSizeF getspacesize();
	QPointF getcenter();
	ShapeType gettype();
	
	//std::unique_ptr<IDidgramDrawParams> maketuxingparams(ShapeType type, QPoint mousepoint);

private:
	void init();
	QPainter* initPainter(); //todo ： 从某个类中读取画笔配置
	void initPainter(QPainter& painter);
	
	//void initpenandbrush(QBrush color, int penwidth, QBrush brush);
	//void InitPainterPen();
	//void InitPainterBrunsh();
	//void drawBaseBackground(QPainter* painter);
	//void drawBaseBackground();
	//ShapeType getshapetypefrombytearray(QByteArray array);
	std::shared_ptr<IDidgramDrawParams> createtuxingparams(ShapeType type);
	//std::shared_ptr<tuxingjiedianparams> maketuxingparams(ShapeType type, QDropEvent* event);

	//Ui::huabuClass ui;
	//QVector<Ituxingjiedian*> m_tuxingvec;
	//std::vector<std::unique_ptr<Ituxingjiedian>> m_tuxingvec; //不能用qvector，提示说qvector的元素必须支持复制操作，？？？
	//QPainter* m_painter; //有点逆天，painter只能在paintevent函数中使用？？？
	std::vector<std::shared_ptr<huabutuxing>> m_tuxingvec;
	QPen m_pen;
	QBrush m_brush;
	QSizeF m_tuxingspacesize;//每个图形分配的区域
	float m_juxingradio; //也许可以希望mimetype中携带数据？
	QString m_mimetype; 
	std::optional<DiagramMimedata> m_mimedata;
	std::optional<QPointF> m_dropevetcenter;

	bool m_isResizing = false;
	QPoint m_dragStartPos;
	QSize m_dragStartSize;
};
