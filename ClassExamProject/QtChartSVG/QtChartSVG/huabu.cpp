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
	, m_isdrawing(false)
	, m_hasmove(false)
	, m_pasteOffset(20, 20)
	, m_lastPasteDelta(0, 0)
	, m_isselecting(true)
	, m_chooseRect(0, 0, 0, 0)
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
	params->m_center = event->pos();

	auto drawer = DiagramDrawInterface::getInstance().getDrawer(params);
	if (params->m_type == ShapeType::Text)
	{
		createTextTuxing(std::dynamic_pointer_cast<DiagramDrawParamsText>(params), drawer);
	}
	else
	{
		createTuxing(params, drawer);
	}

	update();

	//std::shared_ptr<huabutuxing> tuxing = std::make_shared<huabutuxing>();
	//tuxing->m_drawer = drawer;
	//tuxing->m_propertySetManager = std::make_shared<propertySetManager>();



	//std::shared_ptr<drawParamsPropertySet> drawParamsSet = std::make_shared<drawParamsPropertySet>();
	//drawParamsSet->m_params = params;

	//auto propertynamevec = createNameVec(params->m_type);
	//auto creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
	//drawParamsSet->m_propertyDataVec = creator->create(drawParamsSet);
	//QObject::connect(drawParamsSet.get(), &drawParamsPropertySet::SignalValueChangedByData, tuxing.get(), &huabutuxing::signalRepaint);
	//tuxing->m_propertySetManager->addPropertySet(QString("drawParams"), drawParamsSet);








	//std::shared_ptr<otherPropertySet> otherset = std::make_shared<otherPropertySet>();
	//otherset->m_name = createTuxingName(type);
	//otherset->m_zvalue = QDateTime::currentMSecsSinceEpoch();
	//propertynamevec = std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
	//	QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::namename))
	//});
	//creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
	//otherset->m_propertyDataVec = creator->create(otherset);
	//tuxing->m_propertySetManager->addPropertySet(QString("otherset"), otherset);







	//QObject::connect(tuxing.get(), &huabutuxing::signalRepaint, this, qOverload<>(&huabu::update));

	//m_tuxingvec.push_back(tuxing);

	//update(); //触发重绘，会调用paintEvent，可以选择重绘部分区域
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
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);

	painter.fillRect(this->rect(), m_backgroundcolor);

	if (m_isdrawing && m_hasmove)
	{
		m_drawer->draw(painter);
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

	//if (m_ischoosingFirst && m_ischoosingSecond)
	//{
	//	m_chooseDrawer->draw(painter);
	//}
	QWidget::paintEvent(event);
}



void huabu::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_startpoint = event->pos();
		m_currentpoint = event->pos();
		//if (m_mode == Mode::Select)
		if (m_isselecting)
		{
			m_ischooseingone = false;
			//关于选中，只有点击图像才表示选中，点击其他地方则取消选中
			if (m_choosedParamsvec.size() > 0)
			{
				if (m_chooseRect.contains(m_startpoint))
				{
					m_offsetvec.clear();
					for (auto& params : m_choosedParamsvec)
					{
						m_offsetvec.push_back(params->m_center - m_startpoint);
					}
					m_offset = m_chooseRect.center() - m_startpoint;
					QWidget::mousePressEvent(event);
					return;
				}
			}
			if (m_choosingParams != nullptr)
			{
				m_offset = m_choosingParams->m_center - event->pos();
				if (m_chooseRect.contains(m_startpoint))
					m_ischooseingone = true;
			}
			else
			{
				for (auto& diagram : m_tuxingvec)
				{
					if (diagram->m_drawer->getResult()->iscontainPoint(event->pos()))
					{
						m_ischooseingone = true;
						m_chooseRect = diagram->m_drawer->getResult()->getBoundingRect();
						m_choosingParams = std::dynamic_pointer_cast<drawParamsPropertySet>(diagram->m_propertySetManager->getPropertySet("drawParams"))->m_params;
						m_offset = m_choosingParams->m_center - event->pos();
						emit signalPropertyShow(diagram->m_propertySetManager);
						break;
					}
				}
			}



			if (!m_ischooseingone)
			{

				//没有选中图形 
				m_chooseRect = QRect(m_startpoint, m_startpoint);
				m_choosingParams = nullptr;
				emit signalPropertyShow(m_setManager);

			}
			m_choosedParamsvec.clear();
			update();
		}
		else
		{
			m_isdrawing = true;
			m_hasmove = false;

			auto params = createDrawParams(m_drawParams);
			m_copyParams = params;
			m_drawer = DiagramDrawInterface::getInstance().getDrawer(params);



			if (m_copyParams->m_type == ShapeType::Mouse)
			{
				m_copyParams->m_isdrawInHuabu = true;
				auto drawer = std::dynamic_pointer_cast<DiagramDrawerMouse>(m_drawer);
				m_mousepath = std::make_shared<QPainterPath>();
				m_mousepath->moveTo(m_startpoint);
				drawer->m_params->m_path = m_mousepath;
			}
		}














		/*	if (m_ismouseDrawing)
			{
				m_isdrawing = true;
				m_hasmove = false;

				auto params = createDrawParams(m_drawParams);
				m_copyParams = params;
				m_drawer = DiagramDrawInterface::getInstance().getDrawer(params);



				if (m_copyParams->m_type == ShapeType::Mouse)
				{
					m_copyParams->m_isdrawInHuabu = true;
					auto drawer = std::dynamic_pointer_cast<DiagramDrawerMouse>(m_drawer);
					m_mousepath = std::make_shared<QPainterPath>();
					m_mousepath->moveTo(m_startpoint);
					drawer->m_path = m_mousepath;
				}
			}
			else
			{
				QPointF point = event->localPos();
				bool iscontain = false;
				for (const auto& tuxing : m_tuxingvec)
				{
					if (tuxing->m_drawer->getResult()->iscontainPoint(point))
					{
						emit signalPropertyShow(tuxing->m_propertySetManager);
						iscontain = true;
						auto params = std::dynamic_pointer_cast<drawParamsPropertySet>(tuxing->m_propertySetManager->getPropertySet("drawParams"))->m_params;

						if (m_choosedParams != nullptr)
						{
							if (params.get() == m_choosedParams.get())
							{
								if (params->m_ischoosed)
								{
									params->m_ischoosed = false;
									m_choosedParams = nullptr;
								}
								else
								{
									params->m_ischoosed = true;
								}
							}
							else
							{
								m_choosedParams->m_ischoosed = false;
								params->m_ischoosed = true;
							}
						}
						m_choosedParams = params;
						if (params->m_type == ShapeType::Text)
						{
							auto p = std::dynamic_pointer_cast<DiagramDrawParamsText>(params);
							p->m_textedit->show();
						}
						break;
					}
				}
				if (!iscontain)
				{
					emit signalPropertyShow(m_setManager);
					m_ischoosingFirst = true;
					m_choosedParams = nullptr;
				}
				else
				{
					m_ischoosingFirst = false;

					update();
				}
			}*/
	}
	QWidget::mousePressEvent(event);
}

void huabu::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		m_currentpoint = event->pos();
		m_endpoint = event->pos();
		//if (m_mode == Mode::Select)
		if (m_isselecting)
		{
			if (m_choosedParamsvec.size() > 0)
			{
				for (int i = 0; i < m_choosedParamsvec.size(); i++)
				{
					//m_choosedParamsvec[i]->m_center = m_offsetvec[i] + m_currentpoint;

					m_choosedParamsvec[i]->m_centerHoffset = m_currentpoint.x() - m_startpoint.x();
					m_choosedParamsvec[i]->m_centerVoffset = m_currentpoint.y() - m_startpoint.y();
				}
				m_chooseRect.moveCenter(m_offset + m_currentpoint);
				update();
				return;
			}
			if (m_ischooseingone)
			{
				QPoint newpoint = m_offset + event->pos();
				m_choosingParams->m_center = newpoint;
				m_chooseRect.moveCenter(newpoint);
				update();
			}
			else
			{
				m_chooseRect = QRect(m_startpoint, m_currentpoint);
				m_chooseRect.moveBottomRight(m_currentpoint);
				update();
			}
		}
		else
		{
			if (m_isdrawing)
			{
				if (m_copyParams->m_type == ShapeType::Text)
				{
					m_isdrawing = false;
					return;
				}
				if (m_copyParams->m_type != ShapeType::Mouse)
				{
					auto center = (m_startpoint + m_endpoint) / 2;
					QSize size = QSize(std::max(1, static_cast<int>(std::abs(m_startpoint.x() - m_endpoint.x())))
						, std::max(1, static_cast<int>(std::abs(m_startpoint.y() - m_endpoint.y()))));
					m_copyParams->m_center = center.toPoint();
					m_copyParams->m_spacesize = size;
				}
				else
				{
					m_mousepath->lineTo(m_endpoint);
				}
				m_hasmove = true;
				update();
			}
		}













		/*m_endpoint = event->pos();
		if (m_ismouseDrawing && m_isdrawing)
		{
			if (m_copyParams->m_type == ShapeType::Text)
			{
				m_isdrawing = false;
				return;
			}
			if (m_copyParams->m_type != ShapeType::Mouse)
			{
				auto center = (m_startpoint + m_endpoint) / 2;
				QSize size = QSize(std::max(1, static_cast<int>(std::abs(m_startpoint.x() - m_endpoint.x())))
					, std::max(1, static_cast<int>(std::abs(m_startpoint.y() - m_endpoint.y()))));
				m_copyParams->m_center = center.toPoint();
				m_copyParams->m_spacesize = size;
			}
			else
			{
				m_mousepath->lineTo(m_endpoint);
			}
			m_hasmove = true;
			update();
		}
		else
		{
			if (m_ischoosingFirst)
			{
				m_ischoosingSecond = true;
				auto center = (m_startpoint + m_endpoint) / 2;
				QSize size = QSize(std::max(1, static_cast<int>(std::abs(m_startpoint.x() - m_endpoint.x())))
					, std::max(1, static_cast<int>(std::abs(m_startpoint.y() - m_endpoint.y()))));
				m_chooseParams->m_center = center.toPoint();
				m_chooseParams->m_spacesize = size;
				m_chooseParams->m_boundingrectradio = size.width() * 1.00 / size.height() * 1.00;
				update();

			}

		}*/
	}
	QWidget::mouseMoveEvent(event);
}

void huabu::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		//if (m_mode == Mode::Select)
		if (m_isselecting)
		{
			if (!m_ischooseingone)
			{
				if (m_choosedParamsvec.size() > 0)
				{
					for (auto& tuxing : m_choosedParamsvec)
					{
						tuxing->m_center += QPoint(tuxing->m_centerHoffset, tuxing->m_centerVoffset);
						if (tuxing->m_type == ShapeType::Mouse)
						{
							std::dynamic_pointer_cast<DiagramDrawParamsMouse>(tuxing)->m_path->translate(tuxing->m_centerHoffset, tuxing->m_centerVoffset);
						}
						tuxing->m_centerHoffset = 0;
						tuxing->m_centerVoffset = 0;
					}
					QWidget::mouseReleaseEvent(event);
					return;
				}
				QRect totalrect;
				for (auto& tuxing : m_tuxingvec)
				{
					if (tuxing->m_drawer->getResult()->getPainterPath().intersects(m_chooseRect))
					{
						//QRect temprect = tuxing->m_drawer->getResult()->getPainterPath().boundingRect().toRect();
						QRect temprect = tuxing->m_drawer->getResult()->getBoundingRect();
						if (temprect.isNull())
						{
							totalrect = temprect;
						}
						else
						{
							totalrect = totalrect.united(temprect);
						}

						m_choosedParamsvec.push_back(std::dynamic_pointer_cast<drawParamsPropertySet>(tuxing->m_propertySetManager->getPropertySet("drawParams"))->m_params);
					}
				}
				m_chooseRect = totalrect;
				if (m_choosedParamsvec.size() == 1)
				{
					m_choosingParams = m_choosedParamsvec[0];
					m_choosedParamsvec.clear();
					m_ischooseingone = true;
				}
				update();
			}
		}
		else
		{
			if (m_copyParams->m_type == ShapeType::Text)
			{
				auto params = std::dynamic_pointer_cast<DiagramDrawParamsText>(m_copyParams);
				params->m_center = m_startpoint;
				createTextTuxing(params, m_drawer);
			}
			else
			{
				if (m_hasmove)
				{
					createTuxing(m_copyParams, m_drawer);
				}
			}
			m_isdrawing = false;
			update();
		}















		/*if (m_ismouseDrawing && m_isdrawing)
		{
			if (m_copyParams->m_type == ShapeType::Text)
			{
				auto params = std::dynamic_pointer_cast<DiagramDrawParamsText>(m_copyParams);
				params->m_center = m_startpoint;
				createTextTuxing(params, m_drawer);
			}
			else
			{
				if (m_hasmove)
				{
					createTuxing(m_copyParams, m_drawer);
				}
			}
			m_isdrawing = false;
			update();
		}
		else
		{
			if (m_ischoosingFirst && m_ischoosingSecond)
			{
				auto center = m_chooseParams->m_center;
				auto size = m_chooseParams->m_spacesize;
				auto topleft = center - QPoint(size.width() / 2, size.height() / 2);
				auto rect = QRect(topleft, size);
				for (auto& tuxing : m_tuxingvec)
				{
					if (tuxing->m_drawer->getResult()->getPainterPath().intersects(rect))
					{
						auto params = std::dynamic_pointer_cast<drawParamsPropertySet>(tuxing->m_propertySetManager->getPropertySet("drawParams"))->m_params;
						params->m_ischoosed = true;
					}
				}
				m_ischoosingFirst = false;
				m_ischoosingSecond = false;
				update();
			}
		}*/
	}
}

void huabu::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (m_choosingParams != nullptr && m_choosingParams->m_type == ShapeType::Text)
		{
			std::dynamic_pointer_cast<DiagramDrawParamsText>(m_choosingParams)->m_textedit->show();
		}
	}
}

void huabu::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu menu(this);

	// 添加菜单项
	QAction* copyAction = menu.addAction("复制");
	QAction* pasteAction = menu.addAction("粘贴");
	QAction* quanxuanaction = menu.addAction("全选");
	QAction* deleteAction = menu.addAction("删除");


	// 显示菜单
	QAction* selectedAction = menu.exec(event->globalPos());

	// 根据选择执行相应操作
	if (selectedAction == copyAction) {
		// 执行复制操作
		onCopyTuinxg();
	}
	else if (selectedAction == pasteAction) {
		// 执行粘贴操作
		oncrtyvTuxing();
	}
	else if (selectedAction == quanxuanaction)
	{
		onartyaTuxing();
	}
	else if (selectedAction == deleteAction)
	{
		onDeleteTuxing();
	}
}

void huabu::onDiagramClicked(std::shared_ptr<IDidgramDrawParams> params)
{
	m_drawParams = params;
	if (params->m_type == myqtsvg::ShapeType::choose)
	{
		m_isselecting = true;
	}
	else
	{
		m_isselecting = false;
	}
}

void huabu::onSaveToSvg(QString filepath)
{
	saveToSvg(filepath);
}

void huabu::saveToSvg(QString filepath)
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
	loadSvg(filepath);
}

void huabu::loadSvg(QString filepath)
{
	m_svgRenderer = new QSvgRenderer(filepath, this);
	QObject::connect(m_svgRenderer, &QSvgRenderer::repaintNeeded, this, qOverload<>(&huabu::update));
	update();
}

void huabu::onSaveToPng(QString filepath)
{
	SaveToPng(filepath);
}

void huabu::SaveToPng(QString filepath)
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
	m_neededcopyParamsVec.clear();
	if (m_choosingParams != nullptr)
	{
		m_neededcopyParamsVec.push_back(m_choosingParams);
	}
	if (m_choosedParamsvec.size() > 0)
	{
		for (auto& params : m_choosedParamsvec)
		{
			m_neededcopyParamsVec.push_back(params);
		}
	}
	m_pasteOffset = QPoint(20, 20);
	m_lastPasteDelta = QPoint(0, 0);
}

void huabu::oncrtyvTuxing()
{
	if (m_neededcopyParamsVec.size() > 0)
	{
		for (auto& params : m_neededcopyParamsVec)
		{
			auto p = createDrawParams(params);
			p->m_center += m_pasteOffset + m_lastPasteDelta;
			if (p->m_type == myqtsvg::ShapeType::Mouse)
			{
				std::dynamic_pointer_cast<DiagramDrawParamsMouse>(p)->m_path->translate(m_pasteOffset + m_lastPasteDelta);
			}
			auto drawer = DiagramDrawInterface::getInstance().getDrawer(p);
			if (p->m_type == myqtsvg::ShapeType::Text)
			{
				createTextTuxing(std::dynamic_pointer_cast<DiagramDrawParamsText>(p), drawer);
			}
			else
			{
				createTuxing(p, drawer);
			}
		}
		m_lastPasteDelta += m_pasteOffset;
		update();
	}
}

void huabu::onartyaTuxing()
{
	m_choosingParams = nullptr;
	m_choosedParamsvec.clear();
	QRect rect;
	for (auto& tuxing : m_tuxingvec)
	{
		m_choosedParamsvec.push_back(std::dynamic_pointer_cast<drawParamsPropertySet>(tuxing->m_propertySetManager->getPropertySet("drawParams"))->m_params);
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
	if (m_choosingParams != nullptr)
	{
		m_choosedParamsvec.push_back(m_choosingParams);
	}
	for (auto& params : m_choosedParamsvec)
	{
		for (auto it = m_tuxingvec.begin(); it != m_tuxingvec.end(); it++)
		{
			auto p = std::dynamic_pointer_cast<drawParamsPropertySet>((*it)->m_propertySetManager->getPropertySet("drawParams"))->m_params;
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
	tuxing->m_propertySetManager = initPropertySetManager::createPropertySetManager(myqtsvg::huabuShapetypeToPropertyWidgetType(params->m_type)
		, params
		, [tuxing]()
		{
			emit tuxing->signalRepaint();
		}
		, {
			config.getCenterVOffsetName()
			, config.getCenterHOffsetName()
		});

	std::shared_ptr<drawParamsPropertySet> drawParamsSet = std::make_shared<drawParamsPropertySet>();
	drawParamsSet->m_params = params;



	std::vector<QString> othernamevec{
		config.getCenterVOffsetName()
		, config.getCenterHOffsetName() };
	auto propertynamevec = propertyNameVecInterface::getinstance().getPropertyNameVec(params->m_type, othernamevec);
	auto creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
	drawParamsSet->m_propertyDataVec = creator->create(drawParamsSet);
	QObject::connect(drawParamsSet.get(), &drawParamsPropertySet::SignalValueChangedByData, tuxing.get(), &huabutuxing::signalRepaint);
	tuxing->m_propertySetManager->addPropertySet(config.getDrawParamsSetName(), drawParamsSet);





	std::shared_ptr<otherPropertySet> otherset = std::make_shared<otherPropertySet>();
	otherset->m_name = myqtsvg::ShapetypeEnumToQstring(params->m_type);
	otherset->m_zvalue = QDateTime::currentMSecsSinceEpoch();
	propertynamevec.clear();
	propertynamevec.push_back(config.getNameName());
	creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
	otherset->m_propertyDataVec = creator->create(otherset);
	tuxing->m_propertySetManager->addPropertySet(config.getOtherSetName(), otherset);







	QObject::connect(tuxing.get(), &huabutuxing::signalRepaint, this, qOverload<>(&huabu::update));

	m_tuxingvec.push_back(tuxing);
}


void huabu::createTextTuxing(std::shared_ptr<DiagramDrawParamsText> params, std::shared_ptr<IDiagramDrawer> drawer)
{
	params->m_isdrawInHuabu = true;
	auto textedit = new TextLineEdit(this);
	textedit->setParent(this);
	textedit->setTextColor(params->m_pen.color());
	textedit->setBackGroundColor(m_backgroundcolor);
	textedit->setText("编辑");
	textedit->setFont(params->m_font);
	textedit->adjustsize();

	QSize size = textedit->size();

	QPoint topleft = QPoint(params->m_center.x() - size.width() / 2, params->m_center.y() - size.height() / 2);
	textedit->move(topleft);
	textedit->show();

	QObject::connect(textedit, &TextLineEdit::signalHasFocusOut, [=]() {
		auto rect = textedit->geometry();
		params->m_center = rect.center();
		params->m_spacesize = rect.size();

		if (!textedit->isHidden())
		{
			textedit->hide();
		}
		});
	params->m_textedit = textedit;

	createTuxing(params, drawer);
}

std::shared_ptr<IDidgramDrawParams> huabu::createDrawParams(std::shared_ptr<IDidgramDrawParams> params)
{
	return params->clone();
}







huabutuxing::huabutuxing()
{

}


