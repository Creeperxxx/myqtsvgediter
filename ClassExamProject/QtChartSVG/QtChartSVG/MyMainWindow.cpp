#include <qfiledialog.h>
#include <qmenubar.h>
#include <qnetworkaccessmanager.h>
#include <qnetworkreply.h>
#include <qnetworkrequest.h>
#include <qtoolbar.h>
#include <qscrollbar.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include "canvas.h"
#include "diagram.h"
#include "mymainwindow.h"

MyMainWindow::MyMainWindow(QWidget* parent)
	:QMainWindow(parent)
{
	init();
}

MyMainWindow::~MyMainWindow()
{
}

void MyMainWindow::fetchAndSetTooltips()
{
	QNetworkAccessManager* m_networkAccessManager = new QNetworkAccessManager(this);
	QObject::connect(m_networkAccessManager, &QNetworkAccessManager::finished
		, this, &MyMainWindow::dealNetworkReply);
	QUrl url("https://m1.apifoxmock.com/m1/6237106-5930859-default/app/buttontips");
	QNetworkRequest request(url);
	m_networkAccessManager->get(request);
}

void MyMainWindow::init()
{
	initconfig(":/new/userconfig.json");

	// 恢复窗口状态
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

	setupAction();
	setupMainLayout();
	setupToolbar();
	fetchAndSetTooltips();

	connect(m_canvasWidget, &canvas::signalPropertyShow,
		m_propertyWidgetManager, &PropertyWidgetManager::onDealClicked);

}

void MyMainWindow::initconfig(QString filepath)
{
	myconfig::getInstance().loadUserJson(filepath);
}

void MyMainWindow::setupAction()
{
	QMenu* filemenu = menuBar()->addMenu("文件");

	auto addAction = [&](const QString& text, std::function<void()> slot, const QKeySequence& shortcut = {}) {
		QAction* action = new QAction(text, this);
		if (!shortcut.isEmpty()) action->setShortcut(shortcut);
		filemenu->addAction(action);
		connect(action, &QAction::triggered, slot);
		return action;
		};

	addAction("保存为SVG", [this]() {this->savesvg(); });
	addAction("加载SVG", [this]() { this->loadfile(); });
	addAction("保存为png", [this]() { this->savepng(); });
	addAction("新建画布", [this]() {this->newcanvas(); });
	addAction("复制图形", [this]() {this->copyDiagram(); }, QKeySequence::Copy);
	addAction("粘贴图形", [this]() {this->pasteDiagram(); }, QKeySequence::Paste);
	addAction("全选", [this]() {this->selectAllDiagram(); }, QKeySequence::SelectAll);
	addAction("删除图形", [this]() {this->deleteDiagram(); }, QKeySequence::Delete);
	addAction("撤销", [this]() {this->undoDiagram(); }, QKeySequence::Undo);
}

void MyMainWindow::savesvg()
{
	QString filepath = QFileDialog::getSaveFileName(this, "save svg", "", "SVG files (*.svg);;All files (*)");
	if (!filepath.isEmpty())
	{
		if (!filepath.endsWith(".svg", Qt::CaseInsensitive))
		{
			filepath.append(".svg");
		}
		m_canvasWidget->onSaveToSvg(filepath);
	}
}

void MyMainWindow::loadfile()
{
	QString filepath = QFileDialog::getOpenFileName(this, "openfile", "", "SVG files (*.svg);;All files (*)");
	if (!filepath.isEmpty())
	{
		m_canvasWidget->onLoadSvg(filepath);
	}
}

void MyMainWindow::savepng()
{
	QString filepath = QFileDialog::getSaveFileName(this, "save png", "", "PNG files (*.png);;All files (*)");
	if (!filepath.isEmpty())
	{
		m_canvasWidget->onSaveToPng(filepath);
	}
}

void MyMainWindow::newcanvas()
{
	m_canvasWidget->onNewCanvas();
}

void MyMainWindow::copyDiagram()
{
	m_canvasWidget->onCopyDiagram();
}

void MyMainWindow::pasteDiagram()
{
	m_canvasWidget->onPasteDiagram();
}

void MyMainWindow::selectAllDiagram()
{
	m_canvasWidget->onSelectAllDiagram();
}

void MyMainWindow::deleteDiagram()
{
	m_canvasWidget->onDeleteDiagram();
}

void MyMainWindow::undoDiagram()
{
	m_canvasWidget->onUndoDiagram();
}

void MyMainWindow::dealNetworkReply(QNetworkReply* reply)
{
	if (reply->error() != QNetworkReply::NoError)
	{
		return;
	}
	QByteArray data = reply->readAll();
	QJsonDocument doc = QJsonDocument::fromJson(data);
	QJsonObject jsonObj = doc.object();

	for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it)
	{
		if (it.value().isObject())
		{
			QString key = it.key();
			QWidget* widget = this->m_tooltipsWidgets.value(key, nullptr);
			if (widget != nullptr)
			{
				QJsonObject obj = it.value().toObject();
				QString title = obj["title"].toString();
				QString text = obj["text"].toString();
				widget->setToolTip("<b>" + title + "</b><br/>" + text);
			}
		}
	}
	reply->deleteLater();
}

void MyMainWindow::setupToolbar()
{
	QToolBar* maintoolbar = addToolBar("maintoolbar");
	maintoolbar->setObjectName("maintoolbar");
	addToolBar(Qt::LeftToolBarArea, maintoolbar);
	maintoolbar->setAllowedAreas(Qt::LeftToolBarArea);
	maintoolbar->setFloatable(false);
	maintoolbar->setMovable(false);

	const QVector<std::pair<QString, myqtsvg::ShapeType>> shapes = {
		{"rectdrawbutton", myqtsvg::ShapeType::Rect},
		{"quadrilateraldrawbutton", myqtsvg::ShapeType::Circle},
		{"zoominbutton", myqtsvg::ShapeType::Triangle},
		{"linedrawbutton", myqtsvg::ShapeType::Line},
		{"freehandlinedrawbutton", myqtsvg::ShapeType::Mouse},
		{"selectionbutton", myqtsvg::ShapeType::choose},
		{"textdrawbutton", myqtsvg::ShapeType::Text},
		{"pentagondrawbutton", myqtsvg::ShapeType::Pentagon},
		{"zoomoutbutton", myqtsvg::ShapeType::Hexagon},
		{"stardrawbutton", myqtsvg::ShapeType::Star}
	};

	for (const auto& [key, type] : shapes) {
		auto* diagramWidget = new diagram(type);
		m_tooltipsWidgets.insert(key, diagramWidget);
		maintoolbar->addWidget(diagramWidget);
		maintoolbar->addSeparator();

		connect(diagramWidget, &diagram::signalPropertyShow,
			m_propertyWidgetManager, &PropertyWidgetManager::onDealClicked);
		connect(diagramWidget, &diagram::signalMouseDrawing,
			m_canvasWidget, &canvas::onDiagramClicked);
	}
}

void MyMainWindow::setupMainLayout()
{
	QWidget* centralwidget = new QWidget(this);
	QHBoxLayout* centralwidgetlayout = new QHBoxLayout(centralwidget);
	centralwidgetlayout->setSpacing(0);
	centralwidgetlayout->setContentsMargins(0, 0, 0, 0);
	setCentralWidget(centralwidget);

	// 画布父窗口
	m_canvasParentWidget = new QWidget(centralwidget);
	m_canvasParentWidget->setStyleSheet("background-color: rgb(120,120,120);");
	m_canvasParentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_canvasParentWidget->setMinimumSize(8000, 8000);
	new QGridLayout(m_canvasParentWidget);

	// 滚动区域
	m_canvasParentScroll = new QScrollArea(centralwidget);
	m_canvasParentScroll->setWidgetResizable(true);
	m_canvasParentScroll->setWidget(m_canvasParentWidget);
	centralwidgetlayout->addWidget(m_canvasParentScroll);
	m_canvasParentScroll->viewport()->installEventFilter(this);

	// 画布
	m_canvasWidget = new canvas(m_canvasParentWidget);
	m_canvasParentWidget->layout()->addWidget(m_canvasWidget);

	// 属性面板
	m_propertyWidgetManager = new PropertyWidgetManager(centralwidget);
	auto stackwidget = m_propertyWidgetManager->getStackwidget();
	centralwidgetlayout->addWidget(stackwidget);
}

void MyMainWindow::resizeEvent(QResizeEvent* event)
{
	//这个和下面那个都是为了让画布居中显示
	if (m_canvasParentWidget && m_canvasParentScroll) 
	{ 
		int centerx = m_canvasParentWidget->width() / 2 - m_canvasParentScroll->viewport()->size().width() / 2;
		int centery = m_canvasParentWidget->height() / 2 - m_canvasParentScroll->viewport()->size().height() / 2;
		m_canvasParentScroll->horizontalScrollBar()->setValue(centerx);
		m_canvasParentScroll->verticalScrollBar()->setValue(centery);
	}
	QMainWindow::resizeEvent(event);
}

void MyMainWindow::showEvent(QShowEvent* event)
{
	if (m_canvasParentWidget && m_canvasParentScroll) 
	{ 
		int centerx = m_canvasParentWidget->width() / 2 - m_canvasParentScroll->viewport()->size().width() / 2;
		int centery = m_canvasParentWidget->height() / 2 - m_canvasParentScroll->viewport()->size().height() / 2;
		m_canvasParentScroll->horizontalScrollBar()->setValue(centerx);
		m_canvasParentScroll->verticalScrollBar()->setValue(centery);
	}
	QMainWindow::showEvent(event);
}

void MyMainWindow::closeEvent(QCloseEvent* event)
{
	m_settings.setValue("window/geometry", saveGeometry());
	m_settings.setValue("window/state", saveState());
	m_settings.sync();


	QMainWindow::closeEvent(event);
}


