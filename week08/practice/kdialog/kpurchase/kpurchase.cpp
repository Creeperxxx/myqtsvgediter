// kpurchase.cpp : 定义静态库的函数。
//

#include "pch.h"
#include "kpurchase.h"


purchasedialog::purchasedialog(QWidget* parent)
	:QDialog(parent)
{
	QHBoxLayout* layout = new QHBoxLayout();
	setLayout(layout);
	QPushButton* ok = new QPushButton("购买会员");
	layout->addWidget(ok);
}
