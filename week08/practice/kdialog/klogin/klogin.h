#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "klogin_global.h"
#include <QDialog>
#include <qboxlayout.h>
#include <qlabel.h>


class KLOGIN_EXPORT LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog();

    // 导出创建对话框的静态方法
    static QDialog* showLoginDialog(QWidget* parent = nullptr);


private:
    //void setupShadowEffect();
    void setupUI();

    
};

#endif // LOGINDIALOG_H