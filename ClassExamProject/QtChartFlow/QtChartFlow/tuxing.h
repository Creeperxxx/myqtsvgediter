#pragma once
#include <qwidget.h>
#include <qpoint.h>
#include <qmimedata.h>
#include <qpainter.h>
#include <qevent.h>
#include <qdrag.h>
#include <qapplication.h>

enum class ShapeType
{
	juxing
};


class IDiagramItem : public QWidget
{
protected:
	virtual ~IDiagramItem() = 0;
};

class juxingDiagramItem : public IDiagramItem
{
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
private:
	QPoint dragstartposition;
};
