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




class Idiagram : public QWidget
{
	Q_OBJECT
public:
	Idiagram(std::shared_ptr<IDidgramDrawParams> params, QWidget* parent = nullptr);
	void mousePressEvent(QMouseEvent* event)override;
	void mouseMoveEvent(QMouseEvent* event)override;
	void resizeEvent(QResizeEvent* event)override;

	void createQDrag();
	void paintEvent(QPaintEvent* event)override;


	void onParamsValueChanged();

	std::shared_ptr<IDidgramDrawParams> param();
	

	std::shared_ptr<IDiagramDrawer> m_drawer;
	std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> m_propertyDataVec;

signals:
	void signalMouseClicked(PropertyWidgetManager::propertyobjecttype type, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> data);
public:

	QPointF m_dragStartPos;
	bool m_issizefixed;
	std::optional<qreal> m_widgetRadio;
	QString m_mimetype;
	PropertyWidgetManager::propertyobjecttype m_propertyobjecttype;

};

