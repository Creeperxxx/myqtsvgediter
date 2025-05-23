#include "kdialog.h"
#include "qlibrary.h"
#include "qdialog.h"
#include "shadoweffecthelper.h"
#include "../kpurchase/kpurchase.h"

kdialog::kdialog(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_layout = new QHBoxLayout();
    setLayout(m_layout);

    m_login = new QPushButton("login");
    m_layout->addWidget(m_login);

    m_purchase = new QPushButton("purchase");
    m_layout->addWidget(m_purchase);

    QObject::connect(m_login, &QPushButton::clicked, this, &kdialog::onLogin);
    QObject::connect(m_purchase, &QPushButton::clicked, this, &kdialog::onPurchase);
}

kdialog::~kdialog()
{}

void kdialog::onLogin()
{
    typedef QDialog* (*ShowLoginDialogFunc)(QWidget*);
    QLibrary library("klogin");
    if (library.load())
    {
        ShowLoginDialogFunc showDialog = (ShowLoginDialogFunc)library.resolve("showLoginDialog");
        if (showDialog)
        {
            //两个对话框
            auto dialog1 = showDialog(this);
            auto dialog2 = showDialog(this);
            ShadowEffectHelper::applyShadow(dialog1);
            dialog1->exec();
            dialog2->exec();
        }
        library.unload();
    }

}

void kdialog::onPurchase()
{
    auto dialog1 = new purchasedialog(this);
    auto dialog2 = new purchasedialog(this);
    ShadowEffectHelper::applyShadow(dialog1);
    dialog1->exec();
    dialog2->exec();
}
