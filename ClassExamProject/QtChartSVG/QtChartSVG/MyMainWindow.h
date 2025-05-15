#pragma once

#include <QWidget>
#include <qmainwindow.h>
#include <qdebug.h>
#include <qsizepolicy.h>
#include <qcolor.h>
#include <string>
#include <qstring.h>
#include <qscrollarea.h>
#include <qtoolbar.h>
#include <qscrollbar.h>
#include <qmenubar.h>
#include <qstackedwidget.h>
#include "shuxingwidget.h"

class MyMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MyMainWindow(QWidget *parent = nullptr);
	~MyMainWindow();

private:
	void init();
	//void initalltuxing();
	void initconfig(const std::string& filepath = "config.json");
	void resizeEvent(QResizeEvent* event) override;
	void showEvent(QShowEvent* event) override;


	QScrollArea* m_huabuparentscroll;
	QWidget* m_huabuparentwidget;

	PropertyWidgetManager* m_propertyWidgetManager;
};
