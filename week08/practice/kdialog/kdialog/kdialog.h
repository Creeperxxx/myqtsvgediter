#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_kdialog.h"

class kdialog : public QMainWindow
{
    Q_OBJECT

public:
    kdialog(QWidget *parent = nullptr);
    ~kdialog();

private:
    Ui::kdialogClass ui;
};
