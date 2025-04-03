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
#include "ui_juxingitem.h"

enum class ShapeType
{
	juxing
};


class IDiagramItem : public QWidget
{
public:
	IDiagramItem(QWidget* parent = nullptr);
	virtual ~IDiagramItem() {}
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	//virtual void drawPixmap(QPainter* painter, QPixmap* pixmap) = 0;
	virtual QPixmap drawPixmap() = 0;
	QMimeData* createMimedata();
	QPoint dragstartposition;
	ShapeType m_shapetype;
};

class juxingDiagramItem : public IDiagramItem
{
	Q_OBJECT
public:
	~juxingDiagramItem() override;
	juxingDiagramItem(QWidget* parent = nullptr);
protected:
	QPixmap drawPixmap() override;
	Ui::juxingitemwidget ui;
};
