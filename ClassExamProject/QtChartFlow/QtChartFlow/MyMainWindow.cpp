#include "MyMainWindow.h"

MyMainWindow::MyMainWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init();
		
}

MyMainWindow::~MyMainWindow()
{	
}

void MyMainWindow::initPageSwitch()
{
	ui.StartButton->setCheckable(true);
	ui.StartButton->setChecked(true);
	ui.PageButton->setCheckable(true);
	ui.ArrangeButton->setCheckable(true);
	//QStackedWidget* a = ui.ToolStackedWidget;

	menuButtonGroup->addButton(ui.StartButton, static_cast<int>(MenuButtonId::MenuButtonIdPageStart));
	menuButtonGroup->addButton(ui.ArrangeButton, static_cast<int>(MenuButtonId::MenuButtonIdPageArrange));
	menuButtonGroup->addButton(ui.PageButton, static_cast<int>(MenuButtonId::MenuButtonIdPagePage));
	//connect(menuButtonGroup, &QButtonGroup::idClicked, ui.ToolStackedWidget, &QStackedWidget::setCurrentIndex);
	connect(menuButtonGroup, &QButtonGroup::idClicked, this, &MyMainWindow::pageSwitch);
}

void MyMainWindow::init()
{
	menuButtonGroup = new QButtonGroup(this);
	initPageSwitch();
	initalltuxing();
}

void MyMainWindow::pageSwitch(int id)
{
	MenuButtonId enumId = static_cast<MenuButtonId>(id);
	switch (enumId)
	{
	case MenuButtonId::MenuButtonIdPageStart:
		ui.ToolStackedWidget->setCurrentWidget(ui.StartPage);
		break;
	case MenuButtonId::MenuButtonIdPageArrange:
		ui.ToolStackedWidget->setCurrentWidget(ui.ArrangePage);
		break;
	case MenuButtonId::MenuButtonIdPagePage:
		ui.ToolStackedWidget->setCurrentWidget(ui.PagePage);
		break;
	default:
		qDebug() << "未知按钮组中按钮id";
		break;
	}
}

void MyMainWindow::initalltuxing()
{
	IDiagramItem* juxing = new IDiagramItem(ShapeType::juxing, imagepathjuxing, ui.tuxingku);
	ui.tuxingkugridLayout->addWidget(juxing);
	IDiagramItem* yuanxing = new IDiagramItem(ShapeType::yuanxing, imagepathyuanxing, ui.tuxingku);
	ui.tuxingkugridLayout->addWidget(yuanxing);
}
