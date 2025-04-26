#include "MyMainWindow.h"

//MyMainWindow::MyMainWindow(QWidget* parent)
	//: QMainWindow(parent)
//{
	//init();
//}
MyMainWindow::MyMainWindow(QWidget* parent)
	:QMainWindow(parent)
{
	init();
}

MyMainWindow::~MyMainWindow()
{
	//delete ui;
}

//void MyMainWindow::initPageSwitch()
//{
//	ui->StartButton->setCheckable(true);
//	ui->StartButton->setChecked(true);
//	ui->PageButton->setCheckable(true);
//	ui->ArrangeButton->setCheckable(true);
//	//QStackedWidget* a = ui.ToolStackedWidget;
//
//	menuButtonGroup->addButton(ui->StartButton, static_cast<int>(MenuButtonId::MenuButtonIdPageStart));
//	menuButtonGroup->addButton(ui->ArrangeButton, static_cast<int>(MenuButtonId::MenuButtonIdPageArrange));
//	menuButtonGroup->addButton(ui->PageButton, static_cast<int>(MenuButtonId::MenuButtonIdPagePage));
//	//connect(menuButtonGroup, &QButtonGroup::idClicked, ui.ToolStackedWidget, &QStackedWidget::setCurrentIndex);
//	connect(menuButtonGroup, &QButtonGroup::idClicked, this, &MyMainWindow::pageSwitch);
//}

void MyMainWindow::init()
{
	initconfig("config.json");
	resize(1200, 1200);
	//ui = new Ui::MainWindow();
	//ui->setupUi(this);
	//menuButtonGroup = new QButtonGroup(this);
	//initPageSwitch();
	//菜单
	QMenu* filemenu = menuBar()->addMenu("文件");

	//主toolbar
	QToolBar* maintoolbar = addToolBar("main toolbar");
	addToolBar(Qt::LeftToolBarArea, maintoolbar);
	maintoolbar->setAllowedAreas(Qt::LeftToolBarArea);
	maintoolbar->setFloatable(false); // 禁止浮动
	maintoolbar->setMovable(false);   // 禁止移动	

	//主窗口布局
	//QHBoxLayout* mainwindowlayout = new QHBoxLayout(this);

	//中心窗口和布局
	QWidget* centralwidget = new QWidget(this);
	//mainwindowlayout->addWidget(centralwidget);
	QHBoxLayout* centralwidgetlayout = new QHBoxLayout(centralwidget);
	setCentralWidget(centralwidget);

	//画布父窗口
	m_huabuparentwidget = new QWidget(centralwidget);
	m_huabuparentwidget->setStyleSheet("background-color: rgb(120,120,120);");
	m_huabuparentwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_huabuparentwidget->setMinimumSize(8000, 8000);
	QGridLayout* huabuparentlayout = new QGridLayout(m_huabuparentwidget);

	//画布
	huabu* huabuwidget = new huabu(m_huabuparentwidget);
	huabuparentlayout->addWidget(huabuwidget);

	//滚轮
	m_huabuparentscroll = new QScrollArea(centralwidget);
	m_huabuparentscroll->setWidgetResizable(true);
	m_huabuparentscroll->setWidget(m_huabuparentwidget);
	centralwidgetlayout->addWidget(m_huabuparentscroll);

	GfxLibDiagramItemParams juxingparams(ShapeType::Rect);
	DiagramItem* juxing = new DiagramItem(juxingparams);

	GfxLibDiagramItemParams yuanxingparams(ShapeType::Circle);
	DiagramItem* yuanxing = new DiagramItem(yuanxingparams);

	GfxLibDiagramItemParams sanjiaoxingparams(ShapeType::Triangle);
	DiagramItem* sanjiaoxing = new DiagramItem(sanjiaoxingparams);

	GfxLibDiagramItemParams lineparams(ShapeType::Line);
	DiagramItem* line = new DiagramItem(lineparams);

	maintoolbar->addWidget(juxing);
	maintoolbar->addSeparator();
	maintoolbar->addWidget(yuanxing);
	maintoolbar->addSeparator();
	maintoolbar->addWidget(sanjiaoxing);
	maintoolbar->addSeparator();
	maintoolbar->addWidget(line);
	maintoolbar->addSeparator();

	m_propertyWidgetManager = new PropertyWidgetManager(centralwidget);
	centralwidgetlayout->addWidget(m_propertyWidgetManager->getstackwidget());

	QObject::connect(juxing, &DiagramItem::signalMouseClicked
		, m_propertyWidgetManager, &PropertyWidgetManager::dealclicked);
	QObject::connect(yuanxing, &DiagramItem::signalMouseClicked
		, m_propertyWidgetManager, &PropertyWidgetManager::dealclicked);
	QObject::connect(sanjiaoxing, &DiagramItem::signalMouseClicked
		, m_propertyWidgetManager, &PropertyWidgetManager::dealclicked);
	QObject::connect(line, &DiagramItem::signalMouseClicked
		, m_propertyWidgetManager, &PropertyWidgetManager::dealclicked);

	QObject::connect(huabuwidget, &huabu::signalMouseClicked, m_propertyWidgetManager, &PropertyWidgetManager::dealclicked);

}

void MyMainWindow::initconfig(const std::string& filepath)
{
	configmanager::init(filepath);
}

void MyMainWindow::resizeEvent(QResizeEvent* event)
{
	QMainWindow::resizeEvent(event);
	if (m_huabuparentwidget && m_huabuparentscroll) { // 假设这两个指针已经被正确定义和初始化
		int centerx = m_huabuparentwidget->width() / 2 - m_huabuparentscroll->viewport()->size().width() / 2;
		int centery = m_huabuparentwidget->height() / 2 - m_huabuparentscroll->viewport()->size().height() / 2;
		m_huabuparentscroll->horizontalScrollBar()->setValue(centerx);
		m_huabuparentscroll->verticalScrollBar()->setValue(centery);
	}
}

void MyMainWindow::showEvent(QShowEvent* event)
{
	QMainWindow::showEvent(event);
	if (m_huabuparentwidget && m_huabuparentscroll) { // 假设这两个指针已经被正确定义和初始化
		int centerx = m_huabuparentwidget->width() / 2 - m_huabuparentscroll->viewport()->size().width() / 2;
		int centery = m_huabuparentwidget->height() / 2 - m_huabuparentscroll->viewport()->size().height() / 2;
		m_huabuparentscroll->horizontalScrollBar()->setValue(centerx);
		m_huabuparentscroll->verticalScrollBar()->setValue(centery);
	}
}

