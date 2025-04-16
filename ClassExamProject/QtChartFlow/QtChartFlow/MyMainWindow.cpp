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
	int centerx = m_huabuparentwidget->width() / 2 - m_huabuparentscroll->viewport()->size().width() / 2;
	int centery = m_huabuparentwidget->height() / 2 - m_huabuparentscroll->viewport()->size().height() / 2;
	m_huabuparentscroll->horizontalScrollBar()->setValue(centerx);
	m_huabuparentscroll->verticalScrollBar()->setValue(centery);
	//mainwindowlayout->addWidget(huabuparentscroll);

	GfxLibDiagramItemParams juxingparams(ShapeType::Rect);
	DiagramItem* juxing = new DiagramItem(juxingparams);

	GfxLibDiagramItemParams yuanxingparams(ShapeType::Circle);
	DiagramItem* yuanxing = new DiagramItem(yuanxingparams);

	GfxLibDiagramItemParams sanjiaoxingparams(ShapeType::Triangle);
	DiagramItem* sanjiaoxing = new DiagramItem(sanjiaoxingparams);
	
	maintoolbar->addWidget(juxing);
	maintoolbar->addSeparator();
	maintoolbar->addWidget(yuanxing);
	maintoolbar->addSeparator();
	maintoolbar->addWidget(sanjiaoxing);
	maintoolbar->addSeparator();

	//QSizeGrip* sizegrip = new QSizeGrip(huabuwidget);
	//QVBoxLayout* huabulayout = new QVBoxLayout(huabuwidget);
	//huabulayout->setContentsMargins(0, 0, 0, 0);
	//huabulayout->addWidget(sizegrip, 0, Qt::AlignBottom | Qt::AlignRight);


	//QSizeGrip* huabusizegrip = new QSizeGrip(huabuwidget);
	//QVBoxLayout* huabulayout = new QVBoxLayout(huabuwidget);
	//huabulayout->setContentsMargins(0, 0, 0, 0);

	//QHBoxLayout* huabubottomlayout = new QHBoxLayout();
	//huabubottomlayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	//huabubottomlayout->addWidget(huabusizegrip);
	//huabulayout->addLayout(huabubottomlayout);


	//QToolBar* toolbarshuxing = addToolBar("shuxing");
	//toolbarshuxing->setAllowedAreas(Qt::RightToolBarArea);


	//inittuxingkuwidget();
	//inithuabu();
	//inithuabuparentwidget();
	//initshuxingwidget();
	//initalltuxing();
	//ui->tuxingku->setStyleSheet("")
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


//void MyMainWindow::inittuxingkuwidget()
//{
//	ui->tuxingku->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
//	tuxingkulayout = new QVBoxLayout(ui->tuxingku);
//	//QVBoxLayout* layout = new QVBoxLayout(ui->tuxingku);
//}
//
//void MyMainWindow::initshuxingwidget()
//{
//	ui->shuxingwidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
//	shuxinglayout = new QVBoxLayout(ui->shuxingwidget);
//}
//
//void MyMainWindow::inithuabuparentwidget()
//{
//	//m_scroll = new QScrollArea(this);
//	//m_scroll->setWidgetResizable(true);
//	//m_scroll->setWidget(ui->huabuwidget);
//	huabuwidgetlayout = new QGridLayout(ui->huabuwidget);
//	huabuwidgetlayout->addWidget(m_huabu);
//	
//	m_scroll = new QScrollArea(this);
//	m_scroll->setWidgetResizable(true);
//	m_scroll->setWidget(ui->huabuwidget);
//	ui->horizontalLayout->addWidget(m_scroll);
//
//	ui->huabuwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//	ui->huabuwidget->setMinimumSize(8000,8000);
//}
//
//void MyMainWindow::inithuabu()
//{
//	m_huabu = new huabu();
//}
//
//void MyMainWindow::initmenu()
//{
//	QMenu* filemenu = menuBar()->addMenu("文件");
//}
//
//void MyMainWindow::initalltuxing()
//{
//	GfxLibDiagramItemParams juxingparams(ShapeType::Rect);
//	DiagramItem* juxing = new DiagramItem(juxingparams, ui->tuxingku);
//	tuxingkulayout->addWidget(juxing);
//
//	GfxLibDiagramItemParams yuanxingparams(ShapeType::Circle);
//	DiagramItem* yuanxing = new DiagramItem(yuanxingparams, ui->tuxingku);
//	tuxingkulayout->addWidget(yuanxing);
//
//	GfxLibDiagramItemParams sanjiaoxingparams(ShapeType::Triangle);
//	DiagramItem* sanjiaoxing = new DiagramItem(sanjiaoxingparams, ui->tuxingku);
//	tuxingkulayout->addWidget(sanjiaoxing);
//}

//void MyMainWindow::pageSwitch(int id)
//{
//	MenuButtonId enumId = static_cast<MenuButtonId>(id);
//	switch (enumId)
//	{
//	case MenuButtonId::MenuButtonIdPageStart:
//		ui->ToolStackedWidget->setCurrentWidget(ui->StartPage);
//		break;
//	case MenuButtonId::MenuButtonIdPageArrange:
//		ui->ToolStackedWidget->setCurrentWidget(ui->ArrangePage);
//		break;
//	case MenuButtonId::MenuButtonIdPagePage:
//		ui->ToolStackedWidget->setCurrentWidget(ui->PagePage);
//		break;
//	default:
//		qDebug() << "未知按钮组中按钮id";
//		break;
//	}
//}

//void MyMainWindow::initalltuxing()
//{
//	GfxLibDiagramItemParams Rect = GfxLibDiagramItemParams::builder()
//		.settype(ShapeType::Rect)
//		.setpicpath(QString::fromStdString(cfggetval<std::string>(qtcf::imagePathJuxing)))
//		.setjuxingradio(cfggetval<float>(qtcf::tuxingJuxingRadio))
//		.setdrawbypainter(cfggetval<bool>(qtcf::tuxingJuxingDrawByPainter))
//		.setdrawbyloadpic(cfggetval<bool>(qtcf::tuxingJuxingDrawByLoadpic)).build();
//	DiagramItem* juxingdiagram = new DiagramItem(Rect, ui->tuxingku);
//	ui->tuxingkugridLayout->addWidget(juxingdiagram);
//
//	GfxLibDiagramItemParams Circle = GfxLibDiagramItemParams::builder()
//		.settype(ShapeType::Circle)
//		.setpicpath(QString::fromStdString(cfggetval<std::string>(qtcf::imagePathYuanxing)))
//		.setjuxingradio(cfggetval<float>(qtcf::tuxingJuxingRadio))
//		.setdrawbypainter(cfggetval<bool>(qtcf::tuxingJuxingDrawByPainter))
//		.setdrawbyloadpic(cfggetval<bool>(qtcf::tuxingJuxingDrawByLoadpic)).build();
//	DiagramItem* yuanxingdiagram = new DiagramItem(Circle, ui->tuxingku);
//	ui->tuxingkugridLayout->addWidget(yuanxingdiagram);
//
//	GfxLibDiagramItemParams triangle = GfxLibDiagramItemParams::builder()
//		.setdrawbyloadpic(false)
//		.setdrawbypainter(true)
//		.settype(ShapeType::Triangle).build();
//	DiagramItem* triangleitem = new DiagramItem(triangle, ui->tuxingku);
//	ui->tuxingkugridLayout->addWidget(triangleitem);
//
//}
//{
//	const std::string prifix = "tuxingku.diagramwidget.pen";
//	GfxLibDiagramItemParams juxing(isdiagramitemsizefix
//		, QSize(diagramitemwidth, diagramitemheight)
//		, QSize(diagramitemmaxwidth, diagramitemmaxheight)
//		, QSize(diatramitemminwidth, diatramitemminheight)
//		//, QPen(QColor(QString::fromStdString(configmanager::getinstance()["tuxingku"]["diagramwidget"]["pen"]["color"].get<std::string>())), configmanager::getinstance()["tuxingku"]["diagramwidget"]["pen"]["width"].get<int>())
//		, QPen(QColor(QString::fromStdString(cfggetval<std::string>(prifix + ".color"))), cfggetval<int>(prifix + ".width"))
//		, QBrush(diagramitembrush)
//		, mymimetype
//		, QColor(QString::fromStdString(configmanager::getinstance()["tuxingku"]["diagramwidget"]["backgroundcolor"].get<std::string>()))
//		, imagepathjuxing
//		, diagramitemjuxingradio
//		, QSize(huabutuxingwith, huabutuxingheight)
//		, QPen(huabutuxingpencolor, huabutuxingpenwidth)
//		, QBrush(huabutuxingbrush)
//		//, huabubackgroundcolor
//		, diagramitemradio
//		, ShapeType::juxing
//		, true
//		, true
//		, true);
//	GfxLibDiagramItemParams yuanxing(isdiagramitemsizefix
//		, QSize(diagramitemwidth, diagramitemheight)
//		, QSize(diagramitemmaxwidth, diagramitemmaxheight)
//		, QSize(diatramitemminwidth, diatramitemminheight)
//		, QPen(diagramitempencolor, diagramitempenwidth)
//		, QBrush(diagramitembrush)
//		, mymimetype
//		, diagramitembackgroundcolor
//		, imagepathyuanxing
//		, std::nullopt
//		, QSize(huabutuxingwith, huabutuxingheight)
//		, QPen(huabutuxingpencolor, huabutuxingpenwidth)
//		, QBrush(huabutuxingbrush)
//		//, huabubackgroundcolor
//		, diagramitemradio
//		, ShapeType::yuanxing
//		, true
//		, true
//		, true);
//
//	//IDiagramItem* juxing = new IDiagramItem(ShapeType::juxing, imagepathjuxing, ui.tuxingku);
//	DiagramItem* juxingitem = new DiagramItem(juxing);
//	ui.tuxingkugridLayout->addWidget(juxingitem);
//	DiagramItem* yuanxingitem = new DiagramItem(yuanxing);
//	ui.tuxingkugridLayout->addWidget(yuanxingitem);
//	//IDiagramItem* yuanxing = new IDiagramItem(ShapeType::yuanxing, imagepathyuanxing, ui.tuxingku);
//	//IDiagramItem* yuanxingitem = new IDiagramItem(yuanxing);
//	//ui.tuxingkugridLayout->addWidget(yuanxingitem);
//}
