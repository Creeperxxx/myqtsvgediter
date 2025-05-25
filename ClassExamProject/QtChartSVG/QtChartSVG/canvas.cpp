#include <qevent.h>
#include <qmimedata.h>
#include <qmenu.h>
#include <qmessagebox.h>
#include "canvas.h"
#include "myconfig.h"
#include "propertydatabuilder.h"
#include "propertyset.h"
#include "diagramdrawparams.h"
#include "drawer.h"
#include "canvasdiagram.h"
#include "canvasdrawstragety.h"
#include "drawparamscreator.h"



canvas::canvas(QWidget* parent)
	: QWidget(parent)
	, m_mimetype(myconfig::getInstance().getMimetype())
	, m_backgroundcolor(Qt::white)
	, m_pasteOffset(20, 20)
	, m_lastPasteDelta(0, 0)
	, m_chooseRect(0, 0, 0, 0)
	, m_currentMode(canvas::MouseMode::Selecting)
    , m_isDrawing(false)
{
	init();
}

void canvas::init()
{
	setAcceptDrops(true);

	auto& config = myconfig::getInstance();
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	loadSetting();
	m_basesize = size();
	m_scale = 1;


	m_setManager = std::make_shared<propertySetManager>();
	m_setManager->setWidgetType(myqtsvg::propertyWidgetType::canvas);

	auto otherset = std::make_shared<otherPropertySet>();
	otherset->m_name = myconfig::getInstance().getCanvasName();
	otherset->m_canvasHeight = m_basesize.height();
	otherset->m_canvasWidth = m_basesize.width();
	otherset->m_scale = 1;

	std::vector<QString> namevec{
		config.getNameName()
		, config.getCanvasWidthName()
		, config.getCanvasHeightName()
		, config.getCanvasScaleName()
	};

	auto creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(namevec);
	otherset->m_propertyDataVec = creator->create(otherset);

	QObject::connect(otherset.get(), &otherPropertySet::signalCanvasHeightChanged, this, &canvas::onHeightChanged);
	QObject::connect(otherset.get(), &otherPropertySet::signalCanvasWidthChanged, this, &canvas::onWidthChanged);
	QObject::connect(otherset.get(), &otherPropertySet::signalCanvasScaleChanged, this, &canvas::onScaleChagned);
	m_setManager->addPropertySet(config.getOtherSetName(), otherset);
}

canvas::~canvas()
{
}

void canvas::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasFormat(m_mimetype))
	{
		event->acceptProposedAction();
	}

}


void canvas::dropEvent(QDropEvent* event)
{
	QByteArray array = event->mimeData()->data(m_mimetype);
	if (array.isEmpty())
		throw std::runtime_error("error");

	auto params = deserParams(array);
	params->setCenter(event->pos());
	params->setIsDrawInCanvas(true);

	auto drawer = DiagramDrawInterface::getInstance().getDrawer(params);
	if (params->getType() == myqtsvg::ShapeType::Text)
	{
		TextLineEdit::createTextLineEdit(params, this);
	}

	createDiagram(params, drawer);
	update();

	event->acceptProposedAction();
}


void canvas::dragMoveEvent(QDragMoveEvent* event)
{
	if (true == event->mimeData()->hasFormat(m_mimetype))
	{
		event->acceptProposedAction();
	}
}

void canvas::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
    myinitPainter(painter);

	painter.fillRect(this->rect(), m_backgroundcolor);

	if (m_isDrawing)
	{
		m_drawingDrawer->draw(painter);
	}

	painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
	painter.setBrush(Qt::transparent);
	painter.drawRect(m_chooseRect);

	for (auto& diagram : m_diagramVec)
	{
		diagram->getDrawer()->draw(painter);
	}

	if (m_svgRenderer != nullptr)
	{
		m_svgRenderer->render(&painter);
	}

	QWidget::paintEvent(event);
}



void canvas::mousePressEvent(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton)
	{
		QWidget::mousePressEvent(event);
		return;
	}
	m_startpoint = event->pos();
	m_currentpoint = event->pos();

	switch (m_currentMode)
	{
	case canvas::MouseMode::Selecting:
        handleSelectPress();
		break;
	case canvas::MouseMode::Drawing:
        handleDrawPress();
		break;
	default:
		break;
	}
	QWidget::mousePressEvent(event);
	update();
	return;
}

void canvas::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() != Qt::LeftButton)
	{
		QWidget::mouseMoveEvent(event);
		return;
	}
	m_currentpoint = event->pos();

	switch (m_currentMode)
	{
	case canvas::MouseMode::Selecting:
        updateSelectingRect();
		break;
	case canvas::MouseMode::Drawing:
        handleDrawing();
		break;
	case canvas::MouseMode::MovingMultiple:
        handleMoveMultiple();
		break;
	default:
		break;
	}
	QWidget::mouseMoveEvent(event);
	update();
	return;

}

void canvas::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton)
	{
		QWidget::mouseReleaseEvent(event);
		return;
	}
	m_currentpoint = event->pos();

	switch (m_currentMode)
	{
	case canvas::MouseMode::Selecting:
        finalizeSelection();
		break;
	case canvas::MouseMode::Drawing:
        finalizeDrawing();
		break;
	case canvas::MouseMode::MovingMultiple:
        finalizeMove();
		break;
	default:
		break;
	}
	QWidget::mouseReleaseEvent(event);
	update();
	return;

}

void canvas::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (m_choosedParamsvec.size() == 1 && m_choosedParamsvec[0]->getType() == myqtsvg::ShapeType::Text)
		{
			std::dynamic_pointer_cast<DiagramDrawParamsText>(m_choosedParamsvec[0])->getTextEdit()->show();
		}
	}
}

void canvas::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu menu(this);

	QAction* copyAction = menu.addAction("复制");
	QAction* pasteAction = menu.addAction("粘贴");
	QAction* selectAllAction = menu.addAction("全选");
	QAction* deleteAction = menu.addAction("删除");
	QAction* undoAction = menu.addAction("撤销");


	QAction* selectedAction = menu.exec(event->globalPos());

	if (selectedAction == copyAction)
		onCopyDiagram();
	else if (selectedAction == pasteAction)
		onPasteDiagram();
	else if (selectedAction == selectAllAction)
		onSelectAllDiagram();
	else if (selectedAction == deleteAction)
		onDeleteDiagram();
	else if (selectedAction == undoAction)
		onUndoDiagram();
}

void canvas::onDealDiagramUpdate(QRect rect)
{
	update(rect);
}

void canvas::onDiagramClicked(std::shared_ptr<IDidgramDrawParams> params)
{
	m_drawParams = params;
	if (params->getType() == myqtsvg::ShapeType::choose)
	{
		m_currentMode = MouseMode::Selecting;
	}
	else
	{
		m_currentMode = MouseMode::Drawing;
	}
}

void canvas::onSaveToSvg(QString filepath)
{
	QSvgGenerator generator;
	generator.setFileName(filepath);
	QSize size = this->size();
	generator.setSize(size);
	generator.setViewBox(QRect(0, 0, size.width(), size.height()));
	generator.setTitle("保存svg");
	generator.setDescription("使用qt来生成svg图形");

	QPainter painter;
	painter.begin(&generator);

	if (m_svgRenderer != nullptr)
	{
		m_svgRenderer->render(&painter);
	}


	for (const auto& diagram : m_diagramVec) {
		diagram->getDrawer()->draw(painter);
	}


	QMessageBox::information(this, "保存成功", "文件已成功保存为SVG格式。");
}


void canvas::onLoadSvg(QString filepath)
{
	m_svgRenderer = new QSvgRenderer(filepath, this);
	QObject::connect(m_svgRenderer, &QSvgRenderer::repaintNeeded, this, qOverload<>(&canvas::update));
	update();
}


void canvas::onSaveToPng(QString filepath)
{
	QPixmap pixmap(size());
	QPainter painter(&pixmap);
	render(&painter);
	pixmap.save(filepath, "PNG");
	QMessageBox::information(this, "保存成功", "保存成功");
}


void canvas::onNewCanvas()
{
	m_diagramVec.clear();
    m_svgRenderer = nullptr;
	update();
}

void canvas::onCopyDiagram()
{
	m_copyParamsVec.clear();
	if (m_choosedParamsvec.size() == 0)
		return;
	m_copyParamsVec.insert(m_copyParamsVec.end(), m_choosedParamsvec.begin(), m_choosedParamsvec.end());

	m_pasteOffset = QPoint(20, 20);
	m_lastPasteDelta = QPoint(0, 0);
}

void canvas::onPasteDiagram()
{

	if (m_copyParamsVec.size() > 0)
	{
		for (auto& params : m_copyParamsVec)
		{
			auto p = params->clone();
			p->setCenter(p->getCenter() + m_pasteOffset + m_lastPasteDelta);
            m_lastPasteDelta += m_pasteOffset;
			if (p->getType() == myqtsvg::ShapeType::Mouse)
			{
				std::dynamic_pointer_cast<DiagramDrawParamsMouse>(p)->getPaht()->translate(m_pasteOffset + m_lastPasteDelta);
			}
			auto drawer = DiagramDrawInterface::getInstance().getDrawer(p);
			createDiagram(p, drawer);
		}
	}
    update();
}

void canvas::onSelectAllDiagram()
{
	m_choosedParamsvec.clear();
	QRect rect;
	for (auto& tuxing : m_diagramVec)
	{
		m_choosedParamsvec.push_back(tuxing->getParams());
		if (rect.isNull())
		{
			rect = tuxing->getDrawer()->getResult()->getBoundingRect();
		}
		else
		{
			rect = rect.united(tuxing->getDrawer()->getResult()->getBoundingRect());
		}

	}
	m_chooseRect = rect;
	update();

}

void canvas::onDeleteDiagram()
{
	for (auto& params : m_choosedParamsvec)
	{
		for (auto it = m_diagramVec.begin(); it != m_diagramVec.end(); it++)
		{
			auto p = (*it)->getParams();
			if (params.get() == p.get())
			{
				m_diagramVec.erase(it);
				break;
			}
		}
	}
	m_choosedParamsvec.clear();
	m_chooseRect = QRect();
	update();
}

void canvas::onUndoDiagram()
{
	if (!m_diagramVec.empty())
	{
		m_diagramVec.pop_back();
		update();
	}
}

void canvas::closeEvent(QCloseEvent* event)
{

	m_setting.setValue("canvas/size", size());
	m_setting.setValue("canvas/backgroundcolor", m_backgroundcolor);

	QWidget::closeEvent(event);
}

void canvas::adjustCanvasSize()
{
	QSize size = m_basesize * m_scale;
	setFixedSize(size);
}

void canvas::onHeightChanged(int height)
{
	m_basesize.setHeight(height);
	adjustCanvasSize();
}

void canvas::onWidthChanged(int width)
{
	m_basesize.setWidth(width);
	adjustCanvasSize();
}

void canvas::onScaleChagned(double scale)
{
	m_scale = scale;
	adjustCanvasSize();
}



void canvas::myinitPainter(QPainter& painter)
{
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);
}

std::shared_ptr<IDidgramDrawParams> canvas::deserParams(QByteArray& array)
{
	QDataStream stream(array);
	stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);

	int inttype = 0;
	stream >> inttype;
	myqtsvg::ShapeType type = static_cast<myqtsvg::ShapeType>(inttype);

	auto creator = createParamsInterface::getInstance().getParams(type);
	std::shared_ptr<IDidgramDrawParams> params = creator->create();
	stream >> *params;
	return params;
}

void canvas::loadSetting()
{
	QSize canvassize;
	if (m_setting.contains("canvas/size"))
		canvassize = m_setting.value("canvas/size").toSize();
	else
		canvassize = QSize(myconfig::getInstance().getCanvasWidth(), myconfig::getInstance().getCanvasHeight());
	setFixedSize(canvassize);

	if (m_setting.contains("canvas/backgroundcolor"))
	{
		m_backgroundcolor = m_setting.value("canvas/backgroundcolor").value<QColor>();
	}
	else
	{
		m_backgroundcolor = myconfig::getInstance().getCanvasBackgroundColor();
	}

}

void canvas::createDiagram(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<IDiagramDrawer> drawer)
{
	auto& config = myconfig::getInstance();
	std::shared_ptr<canvasDiagram> diagram = std::make_shared<canvasDiagram>();
	diagram->setDrawer(drawer);
	diagram->setParams(params);
	diagram->setSetManager(initPropertySetManager::createPropertySetManager(myqtsvg::canvasShapetypeToPropertyWidgetType(params->getType())
		, params
		, [diagram]()
		{
			diagram->onDealValueChanged();
		}
		,
		{
			config.getCenterVOffsetName()
			, config.getCenterHOffsetName()
		}
	));

	QObject::connect(diagram.get(), &canvasDiagram::signalRepaint, this, &canvas::onDealDiagramUpdate);

	m_diagramVec.push_back(diagram);
}


void canvas::handleSelectPress()
{
	if (m_choosedParamsvec.size() > 0)
	{
		if (m_chooseRect.contains(m_currentpoint))
		{
			m_offsetvec.clear();
			for (auto& params : m_choosedParamsvec)
			{
				params->setCenter(params->getCenter() + QPoint(params->getCenterHOffset(), params->getCenterVOffset()));
				params->setCenterHOffset(0);
				params->setCenterVOffset(0);
				m_offsetvec.push_back(params->getCenter() - m_currentpoint);
			}
			m_rectOffset = m_chooseRect.center() - m_currentpoint;
			m_currentMode = MouseMode::MovingMultiple;

			return;
		}

	}
	m_choosedParamsvec.clear();
	m_chooseRect = QRect(m_currentpoint, QSize(1, 1));

	return;
}

void canvas::handleDrawPress()
{
	m_drawingParams = m_drawParams->clone();
	m_drawingDrawer = DiagramDrawInterface::getInstance().getDrawer(m_drawingParams);
	m_isDrawing = true;
	m_drawingParams->setIsDrawInCanvas(true);

	switch (m_drawingParams->getType())
	{
	case myqtsvg::ShapeType::Mouse:
		m_currentDrawStragety = std::make_unique<mouseDrawStragety>(m_drawingParams);
		break;
	case myqtsvg::ShapeType::Text:
		m_currentDrawStragety = std::make_unique<TextDrawStragety>(m_drawingParams, this);
		break;
	default:
		m_currentDrawStragety = std::make_unique<shapeDrawStragety>(m_drawingParams);
		break;
	}
	m_currentDrawStragety->handlePress(m_startpoint);
}

void canvas::updateSelectingRect()
{
	m_chooseRect = QRect(m_startpoint, m_currentpoint);
	m_chooseRect = m_chooseRect.normalized();
	return;
}

void canvas::handleDrawing()
{
	m_currentDrawStragety->handleMove(m_currentpoint);
}

void canvas::handleMoveMultiple()
{
	for (int i = 0; i < m_choosedParamsvec.size(); i++)
	{
		m_choosedParamsvec[i]->setCenterHOffset(m_currentpoint.x() - m_startpoint.x());
		m_choosedParamsvec[i]->setCenterVOffset(m_currentpoint.y() - m_startpoint.y());
	}
	m_chooseRect.moveCenter(m_rectOffset + m_currentpoint);
	return;
}

void canvas::finalizeSelection()
{
	QRect totalrect;
	QRect boundrect;
	std::shared_ptr<propertySetManager> setmanager;
	for (auto& tuxing : m_diagramVec)
	{
		if (tuxing->getDrawer()->getResult()->getPainterPath().intersects(m_chooseRect))
		{
			boundrect = tuxing->getDrawer()->getResult()->getBoundingRect();
			setmanager = tuxing->getSetManager();
			if (totalrect.isNull())
			{
				totalrect = boundrect;
			}
			else
			{
				totalrect = totalrect.united(boundrect);
			}
			m_choosedParamsvec.push_back(tuxing->getParams());
		}
	}
	if (m_choosedParamsvec.size() == 0)
		setmanager = m_setManager;
	emit signalPropertyShow(setmanager);

	m_chooseRect = totalrect;
}

void canvas::finalizeDrawing()
{
	createDiagram(m_drawingParams, m_drawingDrawer);
	m_drawingDrawer.reset();
	m_drawingParams.reset();
	m_isDrawing = false;
	m_currentDrawStragety.reset();
}

void canvas::finalizeMove()
{
	for (auto& params : m_choosedParamsvec)
	{
		params->setCenter(params->getCenter() + QPoint(params->getCenterHOffset(), params->getCenterVOffset()));

		if (params->getType() == myqtsvg::ShapeType::Mouse)
		{
			std::dynamic_pointer_cast<DiagramDrawParamsMouse>(params)->getPaht()->translate(params->getCenterHOffset(), params->getCenterVOffset());
		}
		params->setCenterHOffset(0);
		params->setCenterVOffset(0);
	}
	m_currentMode = MouseMode::Selecting;
}


