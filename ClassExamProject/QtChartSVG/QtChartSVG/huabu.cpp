#include <qdatetime.h>
#include <qmenu.h>
#include "qmessagebox.h"
#include "huabu.h"
#include "propertydatabuilder.h"
#include "myconfig.h"
#include "drawparamscreator.h"
#include "propertynamevec.h"
#include "propertyset.h"

huabu::huabu(QWidget* parent)
	: QWidget(parent)
	, m_mimetype(myconfig::getInstance().getMimetype())
	, m_backgroundcolor(Qt::white)
	, m_pasteOffset(20, 20)
	, m_lastPasteDelta(0, 0)
	, m_chooseRect(0, 0, 0, 0)
	, m_currentMode(huabu::MouseMode::None)
{
	init();
}

void huabu::init()
{
	setAcceptDrops(true);

	auto& config = myconfig::getInstance();
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	loadSetting();
	m_basesize = size();
	m_scale = 1;


	m_setManager = std::make_shared<propertySetManager>();
	m_setManager->m_propertyWidgetType = myqtsvg::propertywidgettype::huabu;

	auto otherset = std::make_shared<otherPropertySet>();
	otherset->m_name = myconfig::getInstance().getCanvasName();
	otherset->m_huabuheight = m_basesize.height();
	otherset->m_huabuwidth = m_basesize.width();
	otherset->m_scale = 1;

	std::vector<QString> namevec{
		config.getNameName()
		, config.getCanvasWidthName()
		, config.getCanvasHeightName()
		, config.getCanvasScaleName()
	};

	auto creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(namevec);
	otherset->m_propertyDataVec = creator->create(otherset);

	QObject::connect(otherset.get(), &otherPropertySet::signalHuabuHeightChanged, this, &huabu::onHeightChanged);
	QObject::connect(otherset.get(), &otherPropertySet::signalHuabuWidthChanged, this, &huabu::onWidthChanged);
	QObject::connect(otherset.get(), &otherPropertySet::signalCanvasScaleChanged, this, &huabu::onScaleChagned);
	m_setManager->addPropertySet(config.getOtherSetName(), otherset);


}


huabu::~huabu()
{
}

void huabu::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasFormat(m_mimetype))
	{
		event->acceptProposedAction();
	}

}


void huabu::dropEvent(QDropEvent* event)
{
	QByteArray array = event->mimeData()->data(m_mimetype);
	if (array.isEmpty())
		throw std::runtime_error("error");

	auto params = deserParams(array);
	params->setCenter(event->pos());
	params->setIsDrawInCanvas(true);

	auto drawer = DiagramDrawInterface::getInstance().getDrawer(params);
	if(params->getType() == myqtsvg::ShapeType::Text)
	{
		TextLineEdit::createTextLineEdit(params, this);
	}

	createTuxing(params, drawer);
	update();

	event->acceptProposedAction();
}


void huabu::dragMoveEvent(QDragMoveEvent* event)
{
	if (true == event->mimeData()->hasFormat(m_mimetype))
	{
		event->acceptProposedAction();
	}
}

void huabu::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	initPainter(painter);

	painter.fillRect(this->rect(), m_backgroundcolor);

	//if (m_isdrawing && m_hasmove)
	//{
		//m_drawer->draw(painter);
	//}
	if (m_isDrawing)
	{
		m_drawingDrawer->draw(painter);
	}

	painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
	painter.setBrush(Qt::transparent);
	painter.drawRect(m_chooseRect);

	for (auto& diagram : m_tuxingvec)
	{
		diagram->m_drawer->draw(painter);
	}

	if (m_svgRenderer != nullptr)
	{
		m_svgRenderer->render(&painter);
	}

	QWidget::paintEvent(event);
}



void huabu::mousePressEvent(QMouseEvent* event)
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
	case huabu::MouseMode::Selecting:
		handleSelectPress(event);
		break;
	case huabu::MouseMode::Drawing:
		handleDrawPress(event);
		break;
	default:
		break;
	}
	QWidget::mousePressEvent(event);
	update();
	return;
	
}

void huabu::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() != Qt::LeftButton)
	{
		QWidget::mouseMoveEvent(event);
		return;
	}
	m_currentpoint = event->pos();

	switch (m_currentMode)
	{
	case huabu::MouseMode::Selecting:
		updateSelectingRect(event);
		break;
	case huabu::MouseMode::Drawing:
		handleDrawing(event);
		break;
	case huabu::MouseMode::MovingMultiple:
		handleMoveMultiple(event);
		break;
	default:
		break;
	}
	QWidget::mouseMoveEvent(event);
	update();
	return;

}

void huabu::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton)
	{
		QWidget::mouseReleaseEvent(event);
		return;
	}

	switch (m_currentMode)
	{
	case huabu::MouseMode::Selecting:
		finalizeSelection(event);
		break;
	case huabu::MouseMode::Drawing:
		finalizeDrawing(event);
		break;
	case huabu::MouseMode::MovingMultiple:
		finalizeMove(event);
		break;
	default:
		break;
	}
	QWidget::mouseReleaseEvent(event);
	update();
	return;

}

void huabu::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (m_choosedParamsvec.size() == 1 && m_choosedParamsvec[0]->getType() == myqtsvg::ShapeType::Text)
		{
			std::dynamic_pointer_cast<DiagramDrawParamsText>(m_choosedParamsvec[0])->getTextEdit()->show();
		}
	}
}

void huabu::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu menu(this);

	QAction* copyAction = menu.addAction("复制");
	QAction* pasteAction = menu.addAction("粘贴");
	QAction* selectAllAction = menu.addAction("全选");
	QAction* deleteAction = menu.addAction("删除");


	QAction* selectedAction = menu.exec(event->globalPos());

	if (selectedAction == copyAction) 
		onCopyTuinxg();
	else if (selectedAction == pasteAction) 
		onpasteTuxing();
	else if (selectedAction == selectAllAction)
		onSelectAllTuxing();
	else if (selectedAction == deleteAction)
		onDeleteTuxing();
}

void huabu::onDiagramClicked(std::shared_ptr<IDidgramDrawParams> params)
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

void huabu::onSaveToSvg(QString filepath)
{
	QSvgGenerator generator;
	generator.setFileName(filepath); // 设置SVG文件路径
	QSize size = this->size();
	generator.setSize(size); // 设置画布大小
	generator.setViewBox(QRect(0, 0, size.width(), size.height())); // 设置视图框
	generator.setTitle(QObject::tr("SVG Generator Example Drawing"));
	generator.setDescription(QObject::tr("This SVG drawing was generated using Qt"));

	QPainter painter;
	painter.begin(&generator); // 开始在SVG设备上绘制

	if (m_svgRenderer != nullptr)
	{
		m_svgRenderer->render(&painter);
	}

	// 遍历所有形状，并根据它们的属性进行绘制
	for (const auto& shape : m_tuxingvec) {
		shape->m_drawer->draw(painter);
	}

	painter.end(); // 结束绘制

	QMessageBox::information(this, tr("保存成功"), tr("文件已成功保存为SVG格式。"));
}


void huabu::onLoadSvg(QString filepath)
{
	m_svgRenderer = new QSvgRenderer(filepath, this);
	QObject::connect(m_svgRenderer, &QSvgRenderer::repaintNeeded, this, qOverload<>(&huabu::update));
	update();
}


void huabu::onSaveToPng(QString filepath)
{
	QPixmap pixmap(size());
	QPainter painter(&pixmap);
	render(&painter);
	pixmap.save(filepath, "PNG");
	QMessageBox::information(this, "保存成功", "保存成功");
}


void huabu::onnewHuabu()
{
	m_tuxingvec.clear();
	update();
}

void huabu::onCopyTuinxg()
{
	m_copyParamsVec.clear();
	if (m_choosedParamsvec.size() == 0)
		return;
	m_copyParamsVec.insert(m_copyParamsVec.end(), m_choosedParamsvec.begin(), m_choosedParamsvec.end());

	m_pasteOffset = QPoint(20, 20);
	m_lastPasteDelta = QPoint(0, 0);
}

void huabu::onpasteTuxing()
{

	if (m_copyParamsVec.size() > 0)
	{
		for (auto& params : m_copyParamsVec)
		{
			auto p = params->clone();
			//p->m_center += m_pasteOffset + m_lastPasteDelta;
			p->setCenter(p->getCenter() + m_pasteOffset + m_lastPasteDelta);
			//if (p->m_type == myqtsvg::ShapeType::Mouse)
			if(p->getType() == myqtsvg::ShapeType::Mouse)
			{
				std::dynamic_pointer_cast<DiagramDrawParamsMouse>(p)->getPaht()->translate(m_pasteOffset + m_lastPasteDelta);
			}
			auto drawer = DiagramDrawInterface::getInstance().getDrawer(p);
			//if (p->m_type == myqtsvg::ShapeType::Text)
			//{
				//auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsText>(p);
				//if (castparams == nullptr)
					//throw std::runtime_error("error");
				//TextLineEdit::createTextLineEdit(castparams, this);
			//}
			createTuxing(p, drawer);
		}
	}
}

void huabu::onSelectAllTuxing()
{
	m_choosedParamsvec.clear();
	QRect rect;
	for (auto& tuxing : m_tuxingvec)
	{
		m_choosedParamsvec.push_back(tuxing->m_params);
		if (rect.isNull())
		{
			rect = tuxing->m_drawer->getResult()->getBoundingRect();
		}
		else
		{
			rect = rect.united(tuxing->m_drawer->getResult()->getBoundingRect());
		}

	}
	m_chooseRect = rect;
	update();

}

void huabu::onDeleteTuxing()
{
	for (auto& params : m_choosedParamsvec)
	{
		for (auto it = m_tuxingvec.begin(); it != m_tuxingvec.end(); it++)
		{
			auto p = (*it)->m_params;
			if (params.get() == p.get())
			{
				m_tuxingvec.erase(it);
				break;
			}
		}
	}
	m_choosedParamsvec.clear();
	m_chooseRect = QRect();
	update();
}

void huabu::closeEvent(QCloseEvent* event)
{

	m_setting.setValue("canvas/size", size());
	m_setting.setValue("canvas/backgroundcolor", m_backgroundcolor);

	QWidget::closeEvent(event);
}

void huabu::adjustcanvassize()
{
	QSize size = m_basesize * m_scale;
	setFixedSize(size);
}

void huabu::onHeightChanged(int height)
{
	m_basesize.setHeight(height);
	adjustcanvassize();
}

void huabu::onWidthChanged(int width)
{
	m_basesize.setWidth(width);
	adjustcanvassize();
}

void huabu::onScaleChagned(double scale)
{
	m_scale = scale;
	adjustcanvassize();
}



void huabu::initPainter(QPainter& painter)
{
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);
}

std::shared_ptr<IDidgramDrawParams> huabu::deserParams(QByteArray& array)
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

void huabu::loadSetting()
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

void huabu::createTuxing(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<IDiagramDrawer> drawer)
{
	auto& config = myconfig::getInstance();
	std::shared_ptr<huabutuxing> tuxing = std::make_shared<huabutuxing>();
	tuxing->m_drawer = drawer;
	tuxing->m_params = params;
	tuxing->m_propertySetManager = initPropertySetManager::createPropertySetManager(myqtsvg::huabuShapetypeToPropertyWidgetType(params->getType())
		, params
		, [tuxing]()
		{
			emit tuxing->signalRepaint();
		}
		, {
			config.getCenterVOffsetName()
			, config.getCenterHOffsetName()
		});

		QObject::connect(tuxing.get(), &huabutuxing::signalRepaint, this, qOverload<>(&huabu::update));

		m_tuxingvec.push_back(tuxing);
}

huabutuxing::huabutuxing()
{

}

void huabu::handleSelectPress(QMouseEvent* event)
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
			m_offset = m_chooseRect.center() - m_currentpoint;
			m_currentMode = MouseMode::MovingMultiple;

			return;
		}

	}
	m_choosedParamsvec.clear();
	m_chooseRect = QRect(m_currentpoint, QSize(1, 1));

	return;
}

void huabu::handleDrawPress(QMouseEvent* event)
{
	m_drawingParams = m_drawParams->clone();
	m_drawingDrawer = DiagramDrawInterface::getInstance().getDrawer(m_drawingParams);
	m_isDrawing = true;
	//m_drawingParams->m_isdrawInHuabu = true;
	m_drawingParams->setIsDrawInCanvas(true);

	//switch (m_drawParams->m_type)
	switch(m_drawingParams->getType())
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

void huabu::updateSelectingRect(QMouseEvent* event)
{
	m_chooseRect = QRect(m_startpoint, m_currentpoint);
	m_chooseRect = m_chooseRect.normalized();
	return;
}

void huabu::handleDrawing(QMouseEvent* event)
{
	m_currentDrawStragety->handleMove(m_currentpoint);
}

void huabu::handleMoveMultiple(QMouseEvent* event)
{
	for (int i = 0; i < m_choosedParamsvec.size(); i++)
	{
		m_choosedParamsvec[i]->setCenterHOffset(m_currentpoint.x() - m_startpoint.x());
		m_choosedParamsvec[i]->setCenterVOffset(m_currentpoint.y() - m_startpoint.y());
	}
	m_chooseRect.moveCenter(m_offset + m_currentpoint);
	return;
}

void huabu::finalizeSelection(QMouseEvent* event)
{
	QRect totalrect;
	QRect boundrect;
	for (auto& tuxing : m_tuxingvec)
	{
		if (tuxing->m_drawer->getResult()->getPainterPath().intersects(m_chooseRect))
		{
			boundrect = tuxing->m_drawer->getResult()->getBoundingRect();
			if (totalrect.isNull())
			{
				totalrect = boundrect;
			}
			else
			{
				totalrect = totalrect.united(boundrect);
			}
			m_choosedParamsvec.push_back(tuxing->m_params);
		}
	}
	m_chooseRect = totalrect;
}

void huabu::finalizeDrawing(QMouseEvent* event)
{
	createTuxing(m_drawingParams, m_drawingDrawer);
	m_drawingDrawer.reset();
	m_drawingParams.reset();
	m_isDrawing = false;
	m_currentDrawStragety.reset();
}

void huabu::finalizeMove(QMouseEvent* event)
{
	for (auto& params : m_choosedParamsvec)
	{
		params->setCenter(params->getCenter() + QPoint(params->getCenterHOffset(), params->getCenterVOffset()));
		
		if(params->getType() == myqtsvg::ShapeType::Mouse)
		{
			std::dynamic_pointer_cast<DiagramDrawParamsMouse>(params)->getPaht()->translate(params->getCenterHOffset(), params->getCenterVOffset());
		}
		params->setCenterHOffset(0);
		params->setCenterVOffset(0);
	}
	m_currentMode = MouseMode::Selecting;
}


