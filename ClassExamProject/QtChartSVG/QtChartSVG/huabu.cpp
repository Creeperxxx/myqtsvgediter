#include "huabu.h"
#include <qdatetime.h>

huabu::huabu(QWidget* parent)
	: QWidget(parent)
	, m_mimetype(QString::fromStdString(cfggetval<std::string>(qtcf::mimetype)))
	, m_backgroundcolor(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::backgroundcolor))))
	, m_tuxingnum(0)
	, m_ismouseDrawing(false)
	, m_isdrawing(false)
	, m_hasmove(false)
	, m_pasteOffset(20, 20)
	, m_lastPasteDelta(0, 0)
{
	//ui.setupUi(this);
	init();
}

void huabu::init()
{
	setAcceptDrops(true);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	QSize canvassize;
	if (m_setting.contains("canvas/size"))
	{
		canvassize = m_setting.value("canvas/size").toSize();
	}
	else
	{
		canvassize = QSize(cfggetval<int>(qtcf::huabu::width), cfggetval<int>(qtcf::huabu::height));
		m_setting.setValue("canvas/size", canvassize);
	}
	setFixedSize(canvassize);

	if (m_setting.contains("canvas/backgroundcolor"))
	{
		m_backgroundcolor = m_setting.value("canvas/backgroundcolor").value<QColor>();
	}
	else
	{
		m_backgroundcolor = QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::backgroundcolor)));
		m_setting.setValue("canvas/backgroundcolor", m_backgroundcolor);
	}


	m_setManager = std::make_shared<propertySetManager>();
	m_setManager->m_propertyObjectType = PropertyWidgetManager::propertyobjecttype::huabu;

	auto otherset = std::make_shared<otherPropertySet>();
	otherset->m_name = QString::fromStdString(cfggetval<std::string>(qtcf::huabu::name));
	otherset->m_huabuheight = canvassize.height();
	otherset->m_huabuwidth = canvassize.width();

	std::shared_ptr<std::vector<QString>> namevec = std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::namename))
			, QString::fromStdString(cfggetval<std::string>(qtcf::huabu::heightname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::huabu::widthname))
	});
	auto creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(namevec);
	otherset->m_propertyDataVec = creator->create(otherset);
	QObject::connect(otherset.get(), &otherPropertySet::signalHuabuHeightChanged, [=](int height)
		{
			this->setFixedHeight(height);
		});
	QObject::connect(otherset.get(), &otherPropertySet::signalHuabuWidthChanged, [=](int width) {
		this->setFixedWidth(width);
		});
	m_setManager->addPropertySet("other", otherset);


	m_ischoosingFirst = false;
	m_ischoosingSecond = false;
	m_chooseParams = std::make_shared<DiagramDrawParamsRect>();
	m_chooseParams->m_centerHoffset = 0;
	m_chooseParams->m_centerVoffset = 0;
	m_chooseParams->m_ischoosed = false;
	m_chooseParams->m_pen = QPen(Qt::blue, 2, Qt::DashLine);
	m_chooseParams->m_brush = Qt::transparent;
	m_chooseParams->m_rotate = 0;
	m_chooseParams->m_scale = 1;
	m_chooseParams->m_type = ShapeType::Rect;

	m_chooseDrawer = DiagramDrawInterface::getInstance().getDrawer(m_chooseParams);
}


huabu::~huabu()
{
}

void huabu::dragEnterEvent(QDragEnterEvent* event)
{
	if (true == event->mimeData()->hasFormat(m_mimetype))
	{
		event->acceptProposedAction();
	}

}


void huabu::dropEvent(QDropEvent* event)
{
	QByteArray array = event->mimeData()->data(m_mimetype);
	if (array.isEmpty())
		throw std::runtime_error("error");
	QDataStream stream(array);
	stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);

	int inttype = 0;
	stream >> inttype;
	ShapeType type = static_cast<ShapeType>(inttype);
	std::shared_ptr<IDidgramDrawParams> params = nullptr;
	switch (type)
	{
	case ShapeType::Rect:
		params = std::make_shared<DiagramDrawParamsRect>();
		break;
	case ShapeType::Circle:
		params = std::make_shared<DiagramDrawParamsCircle>();
		break;
	case ShapeType::Triangle:
		params = std::make_shared<DiagramDrawParamsTriangle>();
		break;
	case ShapeType::Line:
		params = std::make_shared<DiagramDrawParamsLine>();
		break;
	case ShapeType::Text:
		params = std::make_shared<DiagramDrawParamsText>();
		break;
	default:
		throw std::runtime_error("error");
		break;
	}

	stream >> *params;


	//std::shared_ptr<IDidgramDrawParams> params = buildParamsSpecial(data);

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
	if (true == event->mimeData()->hasFormat(QString::fromStdString(cfggetval<std::string>(qtcf::mimetype))))
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

	for (auto& diagram : m_tuxingvec)
	{
		diagram->m_drawer->draw(painter);
	}

	if (m_svgRenderer != nullptr)
	{
		m_svgRenderer->render(&painter);
	}

	if (m_ischoosingFirst && m_ischoosingSecond)
	{
		m_chooseDrawer->draw(painter);
	}
	QWidget::paintEvent(event);
}


void huabu::enterEvent(QEvent* event)
{
	if (m_drawParams == nullptr || m_drawParams->m_type == ShapeType::choose)
	{
		m_ismouseDrawing = false;
	}
	else
	{
		m_ismouseDrawing = true;
	}
	QWidget::enterEvent(event);
}

void huabu::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_startpoint = event->pos();
		if (m_ismouseDrawing)
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
		}
	}
	QWidget::mousePressEvent(event);
}

void huabu::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		m_endpoint = event->pos();
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

		}
	}
	QWidget::mouseMoveEvent(event);
}

void huabu::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (m_ismouseDrawing && m_isdrawing)
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
		}
	}
}

void huabu::onDiagramClicked(std::shared_ptr<IDidgramDrawParams> params)
{
	if (params == nullptr || params.get() == nullptr)
		throw std::runtime_error("error");
	m_drawParams = params;
	if (params->m_type == ShapeType::choose)
	{
		m_ismouseDrawing = false;
	}
	else
	{
		m_ismouseDrawing = true;
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
	if (m_choosedParams != nullptr)
	{
		m_needcopy = true;
	}
}

void huabu::oncrtyvTuxing()
{
	if (m_needcopy && m_choosedParams != nullptr)
	{
		auto params = createDrawParams(m_choosedParams);
		params->m_center += m_pasteOffset + m_lastPasteDelta;
		m_lastPasteDelta += m_pasteOffset;
		auto drawer = DiagramDrawInterface::getInstance().getDrawer(params);
		createTuxing(params, drawer);
		update();
	}
}

void huabu::closeEvent(QCloseEvent* event)
{

	m_setting.setValue("canvas/size", size());
	m_setting.setValue("canvas/backgroundcolor", m_backgroundcolor);

	QWidget::closeEvent(event);
}



void huabu::initPainter(QPainter& painter)
{
	painter.setRenderHint(QPainter::Antialiasing, true);
}

void huabu::createTuxing(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<IDiagramDrawer> drawer)
{
	std::shared_ptr<huabutuxing> tuxing = std::make_shared<huabutuxing>();
	tuxing->m_drawer = drawer;
	tuxing->m_propertySetManager = std::make_shared<propertySetManager>();
	tuxing->m_propertySetManager->m_propertyObjectType = shapetypeToObjectType(params->m_type);



	std::shared_ptr<drawParamsPropertySet> drawParamsSet = std::make_shared<drawParamsPropertySet>();
	drawParamsSet->m_params = params;

	auto propertynamevec = createNameVec(params->m_type);
	auto creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
	drawParamsSet->m_propertyDataVec = creator->create(drawParamsSet);
	QObject::connect(drawParamsSet.get(), &drawParamsPropertySet::SignalValueChangedByData, tuxing.get(), &huabutuxing::signalRepaint);
	tuxing->m_propertySetManager->addPropertySet(QString("drawParams"), drawParamsSet);








	std::shared_ptr<otherPropertySet> otherset = std::make_shared<otherPropertySet>();
	otherset->m_name = createTuxingName(params->m_type);
	otherset->m_zvalue = QDateTime::currentMSecsSinceEpoch();
	propertynamevec = std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::namename))
	});
	creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
	otherset->m_propertyDataVec = creator->create(otherset);
	tuxing->m_propertySetManager->addPropertySet(QString("otherset"), otherset);







	QObject::connect(tuxing.get(), &huabutuxing::signalRepaint, this, qOverload<>(&huabu::update));

	m_tuxingvec.push_back(tuxing);
}

PropertyWidgetManager::propertyobjecttype huabu::shapetypeToObjectType(ShapeType type)
{
	switch (type)
	{
	case ShapeType::Rect:
		return PropertyWidgetManager::propertyobjecttype::huabuRect;
		break;
	case ShapeType::Circle:
		return PropertyWidgetManager::propertyobjecttype::huabuCircle;
		break;
	case ShapeType::Triangle:
		return PropertyWidgetManager::propertyobjecttype::huabuTriangle;
		break;
	case ShapeType::Line:
		return PropertyWidgetManager::propertyobjecttype::huabuLine;
		break;
	case ShapeType::Mouse:
		return PropertyWidgetManager::propertyobjecttype::huabuMouse;
		break;
	case ShapeType::choose:
		return PropertyWidgetManager::propertyobjecttype::huabu;
		break;
	case ShapeType::Text:
		return PropertyWidgetManager::propertyobjecttype::huabuText;
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

void huabu::createTextTuxing(std::shared_ptr<DiagramDrawParamsText> params, std::shared_ptr<IDiagramDrawer> drawer)
{
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
	switch (params->m_type)
	{
	case ShapeType::Rect:
		return std::make_shared<DiagramDrawParamsRect>(*std::dynamic_pointer_cast<DiagramDrawParamsRect>(params));
		break;
	case ShapeType::Circle:
		return std::make_shared<DiagramDrawParamsCircle>(*std::dynamic_pointer_cast<DiagramDrawParamsCircle>(params));
		break;
	case ShapeType::Triangle:
		return std::make_shared<DiagramDrawParamsTriangle>(*std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(params));
		break;
	case ShapeType::Line:
		return std::make_shared<DiagramDrawParamsLine>(*std::dynamic_pointer_cast<DiagramDrawParamsLine>(params));
		break;
	case ShapeType::Mouse:
		return std::make_shared<DiagramDrawParamsMouse>(*std::dynamic_pointer_cast<DiagramDrawParamsMouse>(params));
		break;
	case ShapeType::choose:
		return std::make_shared<DiagramDrawParamsChoose>(*std::dynamic_pointer_cast<DiagramDrawParamsChoose>(params));
		break;
	case ShapeType::Text:
		return std::make_shared<DiagramDrawParamsText>(*std::dynamic_pointer_cast<DiagramDrawParamsText>(params));
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

std::shared_ptr<std::vector<QString>> huabu::createNameVec(ShapeType type)
{
	switch (type)
	{
	case ShapeType::Rect:
		return rectCreateNameVec();
		break;
	case ShapeType::Circle:
		return circleCreateNameVec();
		break;
	case ShapeType::Triangle:
		return triangleCreateNameVec();
		break;
	case ShapeType::Line:
		return lineCreateNameVec();
		break;
	case ShapeType::Mouse:
		return mouseCreateNameVec();
		break;
	case ShapeType::Text:
		return textCreateNameVec();
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

std::shared_ptr<std::vector<QString>> huabu::rectCreateNameVec()
{
	return std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::colorname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::widthname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::brushcolorname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::rotatename))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::scalename))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::widthname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::heightname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::rectangle::radioname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centerhoffsetname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centervoffsetname))
	});
}

std::shared_ptr<std::vector<QString>> huabu::circleCreateNameVec()
{
	return std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::colorname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::widthname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::brushcolorname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::rotatename))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::scalename))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::widthname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::heightname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::circle::radioname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centerhoffsetname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centervoffsetname))
	});
}

std::shared_ptr<std::vector<QString>> huabu::triangleCreateNameVec()
{
	return std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::colorname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::widthname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::brushcolorname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::rotatename))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::scalename))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::widthname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::heightname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::triangle::edgeradio::radioname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::triangle::edgetypename))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centerhoffsetname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centervoffsetname))
	});
}

std::shared_ptr<std::vector<QString>> huabu::lineCreateNameVec()
{
	return std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::colorname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::widthname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::brushcolorname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::rotatename))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::scalename))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::widthname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::heightname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centerhoffsetname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centervoffsetname))
	});
}

std::shared_ptr<std::vector<QString>> huabu::mouseCreateNameVec()
{
	return std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::colorname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::widthname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centerhoffsetname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centervoffsetname))
	});
}

std::shared_ptr<std::vector<QString>> huabu::textCreateNameVec()
{
	return std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::colorname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::text::familyname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::text::sizename))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centerhoffsetname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centervoffsetname))
	});
}




QString huabu::createTuxingName(ShapeType type)
{
	return ShapeTypeTool::shapetypeEnumToQstring(type) + QString::number(m_tuxingnum++);
}


huabutuxing::huabutuxing()
{

}


