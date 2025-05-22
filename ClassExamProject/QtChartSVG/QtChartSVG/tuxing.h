#pragma once
#include <qwidget.h>
#include <qpoint.h>
#include <qmimedata.h>
#include <qpainter.h>
#include <qevent.h>
#include <qbytearray.h>
#include <qdrag.h>
#include <qsizepolicy.h>
#include <memory>
#include <qcolor.h>
#include <stdexcept>
#include <optional>
#include <qapplication.h>
#include "propertywidget.h"
#include "diagramdrawparams.h"
#include "drawer.h"
#include "namespace.h"







class diagram : public QWidget
{
	Q_OBJECT
public:
	diagram(myqtsvg::ShapeType type, QWidget* parent = nullptr);
	void mousePressEvent(QMouseEvent* event)override;
	void mouseMoveEvent(QMouseEvent* event)override;
	void resizeEvent(QResizeEvent* event)override;

	void createQDrag();
	void paintEvent(QPaintEvent* event)override;


	void onParamsValueChanged();








	std::shared_ptr<IDiagramDrawer> m_drawer;
	std::shared_ptr<IDidgramDrawParams> m_params;
	std::shared_ptr<propertySetManager> m_propertySetManager;

signals:
	void signalMouseDrawing(std::shared_ptr<IDidgramDrawParams> params);
	void signalPropertyShow(std::shared_ptr<propertySetManager> setmanager);
public:


	void myinitSizePolicy();
	void initDrawParams(myqtsvg::ShapeType type);
	void initDrawer();

	QPointF m_dragStartPos;
	bool m_issizefixed;
	double m_widgetRadio;
	QString m_mimetype;

};

