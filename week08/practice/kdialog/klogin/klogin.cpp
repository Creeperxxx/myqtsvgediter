#include "pch.h"
#include "klogin.h"
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget* parent) :
    QDialog(parent)
{
    setupUI();
    //setupShadowEffect();

    // 创建一个垂直布局
    QVBoxLayout* layout = new QVBoxLayout(this);

    // 创建一个标签用于显示文本
    QLabel* label = new QLabel("这是登录框", this);

    // 可以设置对齐方式为居中
    label->setAlignment(Qt::AlignCenter);

    // 将标签添加到布局中
    layout->addWidget(label);

    // 设置对话框的布局
    setLayout(layout);

    // 可选：设置对话框的标题
    setWindowTitle("登录");

    // 设置窗口无边框和背景透明以实现更好的阴影效果
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

LoginDialog::~LoginDialog()
{
}
//
//void LoginDialog::setupShadowEffect()
//{
//    // 创建阴影效果
//    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
//    shadowEffect->setBlurRadius(20);
//    shadowEffect->setColor(QColor(0, 0, 0, 160));
//    shadowEffect->setOffset(5, 5);
//
//    // 将阴影效果应用到主窗口
//    this->setGraphicsEffect(shadowEffect);
//
//    // 确保内容区域不透明
//}

void LoginDialog::setupUI()
{
    // 设置标题
    setWindowTitle("用户登录");

}


// 静态方法，用于创建并显示登录对话框
QDialog* LoginDialog::showLoginDialog(QWidget* parent)
{
    LoginDialog* dlg = new LoginDialog(parent);
    return dlg;
}