#pragma once

#include <QWidget>
//#include "ui_MyMainWindow.h"
#include <ui_MyMainWindow.h>
#include <qbuttongroup.h>
#include <qdebug.h>
#include "tuxing.h"
#include <qsizepolicy.h>
#include <configmanager.h>
#include <qcolor.h>
#include <string>
#include <qstring.h>


class MyMainWindow : public QWidget
{
	Q_OBJECT

public:
	MyMainWindow(QWidget *parent = nullptr);
	~MyMainWindow();

private:
	void init();
	void initalltuxing();
	void initconfig(const std::string& filepath = "config.json");
	enum class MenuButtonId
	{
		MenuButtonIdPageStart = 0,
		MenuButtonIdPageArrange = 1,
		MenuButtonIdPagePage = 2,
	};
	void initPageSwitch();
	void pageSwitch(int id);
	Ui::MyMainWindow* ui;
	QButtonGroup* menuButtonGroup;
	//Ui::MyMainWindowClass ui;
};
