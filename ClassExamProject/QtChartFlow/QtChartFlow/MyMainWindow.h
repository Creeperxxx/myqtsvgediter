#pragma once

#include <QWidget>
#include "ui_MyMainWindow.h"

class MyMainWindow : public QWidget
{
	Q_OBJECT

public:
	MyMainWindow(QWidget *parent = nullptr);
	~MyMainWindow();

private:
	Ui::MyMainWindowClass ui;
};
