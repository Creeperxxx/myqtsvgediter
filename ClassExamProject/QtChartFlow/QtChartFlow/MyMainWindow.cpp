#include "MyMainWindow.h"

MyMainWindow::MyMainWindow(QWidget* parent)
	: QWidget(parent)
{
	init();
}

MyMainWindow::~MyMainWindow()
{
	delete ui;
}

void MyMainWindow::initPageSwitch()
{
	ui->StartButton->setCheckable(true);
	ui->StartButton->setChecked(true);
	ui->PageButton->setCheckable(true);
	ui->ArrangeButton->setCheckable(true);
	//QStackedWidget* a = ui.ToolStackedWidget;

	menuButtonGroup->addButton(ui->StartButton, static_cast<int>(MenuButtonId::MenuButtonIdPageStart));
	menuButtonGroup->addButton(ui->ArrangeButton, static_cast<int>(MenuButtonId::MenuButtonIdPageArrange));
	menuButtonGroup->addButton(ui->PageButton, static_cast<int>(MenuButtonId::MenuButtonIdPagePage));
	//connect(menuButtonGroup, &QButtonGroup::idClicked, ui.ToolStackedWidget, &QStackedWidget::setCurrentIndex);
	connect(menuButtonGroup, &QButtonGroup::idClicked, this, &MyMainWindow::pageSwitch);
}

void MyMainWindow::init()
{
	initconfig("config.json");
	ui = new Ui::MyMainWindow();
	ui->setupUi(this);
	menuButtonGroup = new QButtonGroup(this);
	initPageSwitch();
	initalltuxing();
}

void MyMainWindow::initconfig(const std::string& filepath)
{
	configmanager::init(filepath);
}

void MyMainWindow::pageSwitch(int id)
{
	MenuButtonId enumId = static_cast<MenuButtonId>(id);
	switch (enumId)
	{
	case MenuButtonId::MenuButtonIdPageStart:
		ui->ToolStackedWidget->setCurrentWidget(ui->StartPage);
		break;
	case MenuButtonId::MenuButtonIdPageArrange:
		ui->ToolStackedWidget->setCurrentWidget(ui->ArrangePage);
		break;
	case MenuButtonId::MenuButtonIdPagePage:
		ui->ToolStackedWidget->setCurrentWidget(ui->PagePage);
		break;
	default:
		qDebug() << "未知按钮组中按钮id";
		break;
	}
}

void MyMainWindow::initalltuxing()
{
	GfxLibDiagramItemParams Rect = GfxLibDiagramItemParams::builder()
		.settype(ShapeType::Rect)
		.setpicpath(QString::fromStdString(cfggetval<std::string>(qtcf::imagePathJuxing)))
		.setjuxingradio(cfggetval<float>(qtcf::tuxingJuxingRadio))
		.setdrawbypainter(cfggetval<bool>(qtcf::tuxingJuxingDrawByPainter))
		.setdrawbyloadpic(cfggetval<bool>(qtcf::tuxingJuxingDrawByLoadpic)).build();
	DiagramItem* juxingdiagram = new DiagramItem(Rect, ui->tuxingku);
	ui->tuxingkugridLayout->addWidget(juxingdiagram);
	GfxLibDiagramItemParams Circle = GfxLibDiagramItemParams::builder()
		.settype(ShapeType::Circle)
		.setpicpath(QString::fromStdString(cfggetval<std::string>(qtcf::imagePathYuanxing)))
		.setjuxingradio(cfggetval<float>(qtcf::tuxingJuxingRadio))
		.setdrawbypainter(cfggetval<bool>(qtcf::tuxingJuxingDrawByPainter))
		.setdrawbyloadpic(cfggetval<bool>(qtcf::tuxingJuxingDrawByLoadpic)).build();
	DiagramItem* yuanxingdiagram = new DiagramItem(Circle, ui->tuxingku);
	ui->tuxingkugridLayout->addWidget(yuanxingdiagram);

}
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
