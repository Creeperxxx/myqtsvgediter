#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_kdialog.h"
#include <qboxlayout.h>
#include <qpushbutton.h>




class kdialog : public QMainWindow
{
    Q_OBJECT

public:
    kdialog(QWidget *parent = nullptr);
    ~kdialog();

    void onLogin();
    void onPurchase();

private:
    Ui::kdialogClass ui;
    QHBoxLayout* m_layout;

    QPushButton* m_login;
    QPushButton* m_purchase;
};
