#include <qfile.h>
#include <qfiledialog.h>
#include <qurl.h>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "MyMainWindow.h"
#include "drawer.h"
#include "drawparamscreator.h"
#include "propertydatabuilder.h"
#include "myconfig.h"
#include "huabu.h"
#include "tuxing.h"

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

void MyMainWindow::fetchAndSetTooltips()
{
	QNetworkAccessManager* m_networkAccessManager = new QNetworkAccessManager(this);
	QObject::connect(m_networkAccessManager, &QNetworkAccessManager::finished
		, [=](QNetworkReply* reply)
		{
			if (reply->error() == QNetworkReply::NoError)
			{
				QByteArray data = reply->readAll();
				QJsonDocument doc = QJsonDocument::fromJson(data);
				QJsonObject jsonObj = doc.object();

				for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it) {
					if (it.value().isObject()) {
						QString key = it.key();
						QWidget* widget = this->m_tooltipsWidgets.value(key, nullptr);
						if (widget) {
							QJsonObject obj = it.value().toObject();
							QString title = obj["title"].toString();
							QString text = obj["text"].toString();
							widget->setToolTip("<b>" + title + "</b><br/>" + text);
						}
					}
				}
			}
			reply->deleteLater();
		});
	QUrl url("https://m1.apifoxmock.com/m1/6237106-5930859-default/app/buttontips");
	QNetworkRequest request(url);
	m_networkAccessManager->get(request);
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
	initconfig("userconfig.json");

	if (m_settings.contains("window/geometry"))
	{
		restoreGeometry(m_settings.value("window/geometry").toByteArray());
	}
	else
	{
		resize(1200, 1200);
	}
	if (m_settings.contains("window/state"))
	{
		restoreState(m_settings.value("window/state").toByteArray());
	}


	QMenu* filemenu = menuBar()->addMenu("文件");

	// 创建“保存为SVG”动作
	QAction* saveAsSvgAction = new QAction(tr("保存为SVG"), this);
	QAction* loadAsSvgAction = new QAction(tr("加载SVG"), this);
	QAction* saveAsPngAction = new QAction(tr("保存为png"), this);
	QAction* newhuabuAction = new QAction(tr("新建画布"), this);
	QAction* copyTuxingAction = new QAction(tr("复制图形"), this);
	copyTuxingAction->setShortcut(QKeySequence::Copy);
	QAction* pasteTuxingAction = new QAction(tr("粘贴图形"), this);
	pasteTuxingAction->setShortcut(QKeySequence::Paste);
	QAction* quanxuanAction = new QAction(tr("全选"), this);
	quanxuanAction->setShortcut(QKeySequence::SelectAll);
	QAction* deleteAction = new QAction(tr("删除图形"), this);
	deleteAction->setShortcut(QKeySequence::Delete);


	// 将动作添加到文件菜单
	filemenu->addAction(saveAsSvgAction);
	filemenu->addAction(loadAsSvgAction);
	filemenu->addAction(saveAsPngAction);
	filemenu->addAction(newhuabuAction);
	filemenu->addAction(copyTuxingAction);
	filemenu->addAction(pasteTuxingAction);
	filemenu->addAction(quanxuanAction);
	filemenu->addAction(deleteAction);

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
	centralwidgetlayout->setSpacing(0);
	centralwidgetlayout->setContentsMargins(0, 0, 0, 0);
	setCentralWidget(centralwidget);

	//画布父窗口
	m_huabuparentwidget = new QWidget(centralwidget);
	m_huabuparentwidget->setStyleSheet("background-color: rgb(120,120,120);");
	m_huabuparentwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_huabuparentwidget->setMinimumSize(8000, 8000);
	QGridLayout* huabuparentlayout = new QGridLayout(m_huabuparentwidget);



	//滚轮
	m_huabuparentscroll = new QScrollArea(centralwidget);
	m_huabuparentscroll->setWidgetResizable(true);
	m_huabuparentscroll->setWidget(m_huabuparentwidget);
	centralwidgetlayout->addWidget(m_huabuparentscroll);
	m_huabuparentscroll->viewport()->installEventFilter(this);

	//DiagramDrawInterface::getInstance()
	//	.addDrawerCreator(ShapeType::Rect, [](std::shared_ptr<IDidgramDrawParams> params) {
	//	return std::make_shared<DiagramDrawerRect>(params);
	//		})
	//	.addDrawerCreator(ShapeType::Circle, [](std::shared_ptr<IDidgramDrawParams> params) {
	//	return std::make_shared<DiagramDrawerCircle>(params);
	//		})
	//	.addDrawerCreator(ShapeType::Triangle, [](std::shared_ptr<IDidgramDrawParams> params) {
	//	return std::make_shared<DiagramDrawerTriangle>(params);
	//		})
	//	.addDrawerCreator(ShapeType::Line, [](std::shared_ptr<IDidgramDrawParams> params) {
	//	return std::make_shared<DiagramDrawerLine>(params);
	//		})
	//	.addDrawerCreator(ShapeType::Mouse, [](std::shared_ptr<IDidgramDrawParams> params) {
	//	return std::make_shared<DiagramDrawerMouse>(params);
	//		})
	//	.addDrawerCreator(ShapeType::choose, [](std::shared_ptr<IDidgramDrawParams> params) {
	//	return std::make_shared<DiagramDrawerChoose>(params);
	//		})
	//	.addDrawerCreator(ShapeType::Text, [](std::shared_ptr<IDidgramDrawParams> params) {
	//	return std::make_shared<DiagramDrawerText>(params);
	//		});


	//createParamsInterface::getInstance()
	//	.add(ShapeType::Rect, []() {
	//	return std::make_shared<createParamsRect>();
	//		})
	//	.add(ShapeType::Circle, []() {
	//	return std::make_shared<createParamsCircle>();
	//		})
	//	.add(ShapeType::Triangle, []() {
	//	return std::make_shared<createParamsTriangle>();
	//		})
	//	.add(ShapeType::Line, []() {
	//	return std::make_shared<createParamsLine>();
	//		})
	//	.add(ShapeType::Mouse, []() {
	//	return std::make_shared<createParamsMouse>();
	//		})
	//	.add(ShapeType::choose, []() {
	//	return std::make_shared<createParamsChoose>();
	//		})
	//	.add(ShapeType::Text, []() {
	//	return std::make_shared<createParamsText>();
	//		});


	//propertyDataVecOfPropertySetCreatorFactor::getInstance()
	//	.addCreator(myconfig::getInstance().getPenColorName(), []() {
	//	return std::make_shared<PenColorDrawParamsPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getPenWdithName(), []() {
	//	return std::make_shared<PenWidthDrawParamsPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getBrushColorName(), []() {
	//	return std::make_shared<BrushDrawParamsPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getRotateAngleName(), []() {
	//	return std::make_shared<RotateDrawParamsPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getSpaceWidthName(), []() {
	//	return std::make_shared<SpacewidthDrawParamsPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getSpaceHeightName(), []() {
	//	return std::make_shared<SpaceheightDrawParamsPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getScaleName(), []() {
	//	return std::make_shared<ScaleDrawParamsPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getCenterHOffsetName(), []() {
	//	return std::make_shared<CenterHoffsetDrawParamsPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getCenterVOffsetName(), []() {
	//	return std::make_shared<CenterVoffsetDrawParamsPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getNameName(), []() {
	//	return std::make_shared<NamePropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getRectRadioName(), []() {
	//	return std::make_shared<RectRadioDrawParamsPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getCircleRadioName(), []() {
	//	return std::make_shared<CircleRadioDrawParamsPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getTriangleRadioName(), []() {
	//	return std::make_shared<TriangleRadioDrawParamsPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getEdgeTypeName(), []() {
	//	return std::make_shared<TriangleEdgetypeDrawParamsPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getFontFamilyName(), []() {
	//	return std::make_shared<TextFontFamilyDrawParamsPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getFontSizeName(), []() {
	//	return std::make_shared<TextFontSizeDrawParamsPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getCanvasHeightName(), []() {
	//	return std::make_shared<HuabuHeightPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getCanvasWidthName(), []() {
	//	return std::make_shared<HuabuWidthPropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getCanvasScaleName(), []() {
	//	return std::make_shared<HuabuScalePropertyDataBuilder>();
	//		})
	//	.addCreator(myconfig::getInstance().getPenStyleName(), []() {
	//	return std::make_shared<PenStyleDrawParamsPropertyDataBuilder>();
	//		});





	//auto creator = createParamsInterface::getInstance().getParams(ShapeType::Rect);
	//auto p = creator->create();
	auto juxing = new diagram(myqtsvg::ShapeType::Rect);
	m_tooltipsWidgets.insert("rectdrawbutton", juxing);


	//creator = createParamsInterface::getInstance().getParams(ShapeType::Circle);
	//p = creator->create();
	auto yuanxing = new diagram(myqtsvg::ShapeType::Circle);
	m_tooltipsWidgets.insert("quadrilateraldrawbutton", yuanxing);

	//creator = createParamsInterface::getInstance().getParams(ShapeType::Triangle);
	//p = creator->create();
	auto sanjiaoxing = new diagram(myqtsvg::ShapeType::Triangle);
	m_tooltipsWidgets.insert("pentagondrawbutton", sanjiaoxing);

	//creator = createParamsInterface::getInstance().getParams(ShapeType::Line);
	//p = creator->create();
	auto xian = new diagram(myqtsvg::ShapeType::Line);
	m_tooltipsWidgets.insert("linedrawbutton", xian);


	//creator = createParamsInterface::getInstance().getParams(ShapeType::Mouse);
	//p = creator->create();
	auto mouse = new diagram(myqtsvg::ShapeType::Mouse);
	m_tooltipsWidgets.insert("freehandlinedrawbutton", mouse);

	//creator = createParamsInterface::getInstance().getParams(ShapeType::choose);
	//p = creator->create();
	auto choose = new diagram(myqtsvg::ShapeType::choose);
	m_tooltipsWidgets.insert("selectionbutton", choose);

	//creator = createParamsInterface::getInstance().getParams(ShapeType::Text);
	//p = creator->create();
	auto text = new diagram(myqtsvg::ShapeType::Text);
	m_tooltipsWidgets.insert("stardrawbutton", text);

	maintoolbar->addWidget(juxing);
	maintoolbar->addSeparator();
	maintoolbar->addWidget(yuanxing);
	maintoolbar->addSeparator();
	maintoolbar->addWidget(sanjiaoxing);
	maintoolbar->addSeparator();
	maintoolbar->addWidget(xian);
	maintoolbar->addSeparator();
	maintoolbar->addWidget(mouse);
	maintoolbar->addSeparator();
	maintoolbar->addWidget(choose);
	maintoolbar->addSeparator();
	maintoolbar->addWidget(text);
	maintoolbar->addSeparator();

	fetchAndSetTooltips();

	//画布
	huabu* huabuwidget = new huabu(m_huabuparentwidget);
	huabuparentlayout->addWidget(huabuwidget);

	m_propertyWidgetManager = new PropertyWidgetManager(centralwidget);
	auto stackwidget = m_propertyWidgetManager->getstackwidget();
	stackwidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	centralwidgetlayout->addWidget(stackwidget);

	QObject::connect(juxing, &diagram::signalPropertyShow
		, m_propertyWidgetManager, &PropertyWidgetManager::dealclicked);
	QObject::connect(yuanxing, &diagram::signalPropertyShow
		, m_propertyWidgetManager, &PropertyWidgetManager::dealclicked);
	QObject::connect(sanjiaoxing, &diagram::signalPropertyShow
		, m_propertyWidgetManager, &PropertyWidgetManager::dealclicked);
	QObject::connect(xian, &diagram::signalPropertyShow
		, m_propertyWidgetManager, &PropertyWidgetManager::dealclicked);
	QObject::connect(mouse, &diagram::signalPropertyShow
		, m_propertyWidgetManager, &PropertyWidgetManager::dealclicked);
	QObject::connect(choose, &diagram::signalPropertyShow
		, m_propertyWidgetManager, &PropertyWidgetManager::dealclicked);
	QObject::connect(text, &diagram::signalPropertyShow
		, m_propertyWidgetManager, &PropertyWidgetManager::dealclicked);

	QObject::connect(juxing, &diagram::signalMouseDrawing
		, huabuwidget, &huabu::onDiagramClicked);
	QObject::connect(yuanxing, &diagram::signalMouseDrawing
		, huabuwidget, &huabu::onDiagramClicked);
	QObject::connect(sanjiaoxing, &diagram::signalMouseDrawing
		, huabuwidget, &huabu::onDiagramClicked);
	QObject::connect(xian, &diagram::signalMouseDrawing
		, huabuwidget, &huabu::onDiagramClicked);
	QObject::connect(mouse, &diagram::signalMouseDrawing
		, huabuwidget, &huabu::onDiagramClicked);
	QObject::connect(choose, &diagram::signalMouseDrawing
		, huabuwidget, &huabu::onDiagramClicked);
	QObject::connect(text, &diagram::signalMouseDrawing
		, huabuwidget, &huabu::onDiagramClicked);

	QObject::connect(huabuwidget, &huabu::signalPropertyShow, m_propertyWidgetManager, &PropertyWidgetManager::dealclicked);

	QObject::connect(saveAsSvgAction, &QAction::triggered, [=]() {
		QString filepath = QFileDialog::getSaveFileName(this, tr("save svg"), "", tr("SVG files (*.svg);;All files (*)"));
		if (!filepath.isEmpty())
		{
			if (!filepath.endsWith(".svg", Qt::CaseInsensitive))
			{
				filepath.append(".svg");
			}
			huabuwidget->onSaveToSvg(filepath);
		}
		});

	QObject::connect(loadAsSvgAction, &QAction::triggered, [=]() {
		QString filepath = QFileDialog::getOpenFileName(this, tr("openfile"), "", tr("SVG files (*.svg);;All files (*)"));
		if (!filepath.isEmpty())
		{
			huabuwidget->onLoadSvg(filepath);
		}
		});

	QObject::connect(saveAsPngAction, &QAction::triggered, [=]() {
		QString filepath = QFileDialog::getSaveFileName(this, "save png", "", "PNG files (*.png);;All files (*)");
		if (!filepath.isEmpty())
		{
			huabuwidget->onSaveToPng(filepath);
		}
		});
	QObject::connect(newhuabuAction, &QAction::triggered, huabuwidget, &huabu::onnewHuabu);



	QObject::connect(copyTuxingAction, &QAction::triggered, huabuwidget, &huabu::onCopyTuinxg);

	QObject::connect(pasteTuxingAction, &QAction::triggered, huabuwidget, &huabu::oncrtyvTuxing);

	QObject::connect(quanxuanAction, &QAction::triggered, huabuwidget, &huabu::onartyaTuxing);
	QObject::connect(deleteAction, &QAction::triggered, huabuwidget, &huabu::onDeleteTuxing);
}

void MyMainWindow::initconfig(QString filepath)
{
	//configmanager::init(filepath);
	myconfig::getInstance().loadUserJson(filepath);
}

void MyMainWindow::resizeEvent(QResizeEvent* event)
{
	if (m_huabuparentwidget && m_huabuparentscroll) { // 假设这两个指针已经被正确定义和初始化
		int centerx = m_huabuparentwidget->width() / 2 - m_huabuparentscroll->viewport()->size().width() / 2;
		int centery = m_huabuparentwidget->height() / 2 - m_huabuparentscroll->viewport()->size().height() / 2;
		m_huabuparentscroll->horizontalScrollBar()->setValue(centerx);
		m_huabuparentscroll->verticalScrollBar()->setValue(centery);
	}
	QMainWindow::resizeEvent(event);
}

void MyMainWindow::showEvent(QShowEvent* event)
{
	if (m_huabuparentwidget && m_huabuparentscroll) { // 假设这两个指针已经被正确定义和初始化
		int centerx = m_huabuparentwidget->width() / 2 - m_huabuparentscroll->viewport()->size().width() / 2;
		int centery = m_huabuparentwidget->height() / 2 - m_huabuparentscroll->viewport()->size().height() / 2;
		m_huabuparentscroll->horizontalScrollBar()->setValue(centerx);
		m_huabuparentscroll->verticalScrollBar()->setValue(centery);
	}
	QMainWindow::showEvent(event);
}

void MyMainWindow::closeEvent(QCloseEvent* event)
{
	m_settings.setValue("window/geometry", saveGeometry());
	m_settings.setValue("window/state", saveState());
	m_settings.sync();

	//qDebug() << "Saving geometry data:" << saveGeometry().toHex();

	QMainWindow::closeEvent(event);
}


