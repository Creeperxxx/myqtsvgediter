#include "huabu.h"
#include <qdatetime.h>

huabu::huabu(QWidget* parent)
	: QWidget(parent)
	, m_mimetype(QString::fromStdString(cfggetval<std::string>(qtcf::mimetype)))
	, m_backgroundcolor(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::backgroundcolor))))
	, m_size(QSize(cfggetval<int>(qtcf::huabu::width), cfggetval<int>(qtcf::huabu::height)))
	, m_tuxingnum(0)
	, m_ismouseDrawing(false)
{
	//ui.setupUi(this);
	init();
}

void huabu::init()
{
	setAcceptDrops(true);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setFixedSize(m_size);
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
	default:
		throw std::runtime_error("error");
		break;
	}

	stream >> *params;


	//std::shared_ptr<IDidgramDrawParams> params = buildParamsSpecial(data);

	params->m_center = event->pos();
	params->m_paramChanged = true;

	auto drawer = DiagramDrawInterface::getInstance().getDrawer(params);
	createTuxing(params, drawer);
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
	//drawBaseBackground(m_painter);
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
}

void huabu::onValueChangedHuabuSizeWidth(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_size.setWidth(value.toInt());
	this->setFixedWidth(value.toInt());
	//update();
}

void huabu::onValueChangedHuabuSizeHeight(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_size.setHeight(value.toInt());
	this->setFixedHeight(value.toInt());
	//update();
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
}

void huabu::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (m_ismouseDrawing)
		{
			m_isdrawing = true;
			m_hasmove = false;

			auto params = createDrawParams();
			m_copyParams = params;
			m_drawer = DiagramDrawInterface::getInstance().getDrawer(params);
			m_startpoint = event->pos();
			if (m_copyParams->m_type == ShapeType::Mouse)
			{
				m_copyParams->m_isdrawInHuabu = true;
				auto drawer = std::dynamic_pointer_cast<DiagramDrawerMouse>(m_drawer);
				m_mousepath = std::make_shared<QPainterPath>();
				m_mousepath->moveTo(m_startpoint);
				drawer->m_path = m_mousepath;
			}
			if (m_copyParams->m_type == ShapeType::Text)
			{
				auto params = std::dynamic_pointer_cast<DiagramDrawParamsText>(m_copyParams);
				params->m_isdrawInHuabu = true;
				auto lineedit = new TransparentEditableLabel(this);
				lineedit->setText("编辑");
				lineedit->setGeometry(m_startpoint.x(), m_startpoint.y(), 100, 30);
				auto drawer = std::dynamic_pointer_cast<DiagramDrawerText>(m_drawer);
				drawer->m_lineedit = lineedit;
				createTuxing(m_copyParams, m_drawer);
			}
		}
		else
		{

		}
		//bool iscontain = false;
		QPointF point = event->localPos();
		//for (const auto& tuxing : m_tuxingvec)
		//{
			//if (tuxing->m_ret->iscontainPoint(point))
			//{
				//emit signalMouseClicked(shapetypeToPropertyType(tuxing->m_type), tuxing->m_propertydataVec);
				//iscontain = true;
			//}
		//}
		//if (!iscontain)
		//{
			//emit signalMouseClicked(PropertyWidgetManager::propertyobjecttype::huabu, m_propertydataHuabuVec);
		//}
	}
}

void huabu::mouseMoveEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		if (m_ismouseDrawing && m_isdrawing)
		{
			m_endpoint = event->pos();
			if (m_copyParams->m_type != ShapeType::Mouse)
			{
				auto center = (m_startpoint + m_endpoint) / 2;
				QSize size = QSize(std::max(1, std::abs(m_startpoint.x() - m_endpoint.x()))
					, std::max(1, std::abs(m_startpoint.y() - m_endpoint.y())));
				m_copyParams->m_center = center;
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
}

void huabu::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (m_ismouseDrawing && m_isdrawing && m_hasmove)
		{
			createTuxing(m_copyParams, m_drawer);
			//auto tuxing = std::make_shared<huabutuxing>();
			//tuxing->m_drawer = m_drawer;
			//tuxing->m_propertySetManager = std::make_shared<propertySetManager>();

			//auto drawparams = std::make_shared<drawParamsPropertySet>();
			//drawparams->m_params = m_copyParams;
			//auto propertynamevec = createNameVec(m_copyParams->m_type);
			//auto creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
			//drawparams->m_propertyDataVec = creator->create(drawparams);
			//QObject::connect(drawparams.get(), &drawParamsPropertySet::SignalValueChangedByData, tuxing.get(), &huabutuxing::signalRepaint);
			//tuxing->m_propertySetManager->addPropertySet("drawParams", drawparams);

			//auto otherparams = std::make_shared<otherPropertySet>();
			//otherparams->m_name = createTuxingName(ShapeType::Mouse);
			//otherparams->m_zvalue = QDateTime::currentMSecsSinceEpoch();
			//auto otherparamsnamevec = std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
			//	QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::namename))
			//});
			//auto othercreator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(otherparamsnamevec);
			//otherparams->m_propertyDataVec = othercreator->create(otherparams);
			//tuxing->m_propertySetManager->addPropertySet("otherset", otherparams);

			//QObject::connect(tuxing.get(), &huabutuxing::signalRepaint, this, qOverload<>(&huabu::update));

			//m_tuxingvec.push_back(tuxing);
			m_isdrawing = false;
			update();
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



void huabu::initPainter(QPainter& painter)
{
	painter.setRenderHint(QPainter::Antialiasing, true);
}

void huabu::createTuxing(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<IDiagramDrawer> drawer)
{
	std::shared_ptr<huabutuxing> tuxing = std::make_shared<huabutuxing>();
	tuxing->m_drawer = drawer;
	tuxing->m_propertySetManager = std::make_shared<propertySetManager>();



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

std::shared_ptr<IDidgramDrawParams> huabu::createDrawParams()
{
	switch (m_drawParams->m_type)
	{
	case ShapeType::Rect:
		return std::make_shared<DiagramDrawParamsRect>(*std::dynamic_pointer_cast<DiagramDrawParamsRect>(m_drawParams));
		break;
	case ShapeType::Circle:
		return std::make_shared<DiagramDrawParamsCircle>(*std::dynamic_pointer_cast<DiagramDrawParamsCircle>(m_drawParams));
		break;
	case ShapeType::Triangle:
		return std::make_shared<DiagramDrawParamsTriangle>(*std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(m_drawParams));
		break;
	case ShapeType::Line:
		return std::make_shared<DiagramDrawParamsLine>(*std::dynamic_pointer_cast<DiagramDrawParamsLine>(m_drawParams));
		break;
	case ShapeType::Mouse:
		return std::make_shared<DiagramDrawParamsMouse>(*std::dynamic_pointer_cast<DiagramDrawParamsMouse>(m_drawParams));
		break;
	case ShapeType::choose:
		return std::make_shared<DiagramDrawParamsChoose>(*std::dynamic_pointer_cast<DiagramDrawParamsChoose>(m_drawParams));
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




QString huabu::createTuxingName(ShapeType type)
{
	return ShapeTypeTool::shapetypeEnumToQstring(type) + QString::number(m_tuxingnum++);
}


huabutuxing::huabutuxing()
{

}


