#pragma once

#include <QWidget>
#include <qmainwindow.h>
//#include "ui_MyMainWindow.h"
#include <ui_MyMainWindow.h>
//#include <qbuttongroup.h>
#include <qdebug.h>
#include "tuxing.h"
#include <qsizepolicy.h>
#include <configmanager.h>
#include <qcolor.h>
#include <string>
#include <qstring.h>
#include "huabu.h"
#include <qscrollarea.h>
#include <qtoolbar.h>
#include <qscrollbar.h>
#include <qsizegrip.h>
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
	//void inittuxingkuwidget();
	//void initshuxingwidget();
	//void inithuabuparentwidget();
	//void inithuabu();
	//void initmenu();

	//enum class MenuButtonId
	//{
		//MenuButtonIdPageStart = 0,
		//MenuButtonIdPageArrange = 1,
		//MenuButtonIdPagePage = 2,
	//};
	//void initPageSwitch();
	//void pageSwitch(int id);
	//Ui::MainWindow* ui;
	//QVBoxLayout* tuxingkulayout;
	//QVBoxLayout* shuxinglayout;
	//QGridLayout* huabuwidgetlayout;
	//huabu* m_huabu;
	//QScrollArea* m_scroll;
	//QButtonGroup* menuButtonGroup;
	//Ui::MyMainWindowClass ui;
};
