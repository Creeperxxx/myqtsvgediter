#pragma once

#include <QWidget>
#include "ui_huabu.h"
#include <qevent.h>
#include <qmimedata.h>
#include <qdebug.h>
#include "tuxing.h"
#include <qvector.h>
#include "tuxingjiedian.h"
#include <qpalette.h>

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

private:
	void init();
	QPainter* initPainter();
	//void InitPainterPen();
	//void InitPainterBrunsh();
	//void drawBaseBackground(QPainter* painter);
	void drawBaseBackground();
	Ui::huabuClass ui;
	QVector<Ituxingjiedian*> m_tuxingvec;
	//QPainter* m_painter; //有点逆天，painter只能在paintevent函数中使用？？？
};
