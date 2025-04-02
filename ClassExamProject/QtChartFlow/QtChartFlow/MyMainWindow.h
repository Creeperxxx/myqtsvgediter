#pragma once

#include <QWidget>
//#include "ui_MyMainWindow.h"
#include <ui_MyMainWindow.h>
#include <qbuttongroup.h>
#include <qdebug.h>


class MyMainWindow : public QWidget
{
	Q_OBJECT

public:
	MyMainWindow(QWidget *parent = nullptr);
	~MyMainWindow();

private:
	void init();
	enum class MenuButtonId
	{
		MenuButtonIdPageStart = 0,
		MenuButtonIdPageArrange = 1,
		MenuButtonIdPagePage = 2,
	};
	void initPageSwitch();
	void pageSwitch(int id);
	Ui::MyMainWindow ui;
	QButtonGroup* menuButtonGroup;
	//Ui::MyMainWindowClass ui;
};
