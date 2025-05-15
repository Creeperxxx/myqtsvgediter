#include "huabu.h"
#include <qdatetime.h>

huabu::huabu(QWidget* parent)
	: QWidget(parent)
	, m_mimetype(QString::fromStdString(cfggetval<std::string>(qtcf::mimetype)))
	, m_backgroundcolor(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::backgroundcolor))))
	, m_size(QSize(cfggetval<int>(qtcf::huabu::width), cfggetval<int>(qtcf::huabu::height)))
	, m_tuxingnum(0)
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

	std::shared_ptr<huabutuxing> tuxing = std::make_shared<huabutuxing>();
	tuxing->m_drawer = drawer;
	tuxing->m_propertySetManager = std::make_shared<propertySetManager>();



	std::shared_ptr<drawParamsPropertySet> drawParamsSet = std::make_shared<drawParamsPropertySet>();
	drawParamsSet->m_params = params;
	std::shared_ptr<std::vector<QString>> propertynamevec = std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::colorname))
		, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::widthname))
		, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::brushcolorname))
		, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::widthname))
		, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::heightname))
		, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::rotatename))
		, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::scalename))
		, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centerhoffsetname))
		, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centervoffsetname))
	});
	switch (type)
	{
	case ShapeType::Rect:
	{
		propertynamevec->push_back(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::rectangle::radioname)));
	}
		break;
	case ShapeType::Circle:
	{
		propertynamevec->push_back(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::circle::radioname)));
	}
		break;
	case ShapeType::Triangle:
	{
		propertynamevec->push_back(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::triangle::edgeradio::bottomname)));
		propertynamevec->push_back(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::triangle::edgeradio::leftname)));
		propertynamevec->push_back(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::triangle::edgeradio::rightname)));
		propertynamevec->push_back(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::triangle::edgetypename)));
	}
		break;
	case ShapeType::Line:
		break;
	default:
		throw std::runtime_error("error");
	}
	auto creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
	drawParamsSet->m_propertyDataVec = creator->create(drawParamsSet);
	QObject::connect(drawParamsSet.get(), &drawParamsPropertySet::SignalValueChangedByData, tuxing.get(), &huabutuxing::signalRepaint);
	tuxing->m_propertySetManager->addPropertySet(QString("drawParams"), drawParamsSet);
	







	std::shared_ptr<otherPropertySet> otherset = std::make_shared<otherPropertySet>();
	otherset->m_name = createTuxingName(type);
	otherset->m_zvalue = QDateTime::currentMSecsSinceEpoch();
	propertynamevec = std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::namename))
	});
	creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
	otherset->m_propertyDataVec = creator->create(otherset);
	tuxing->m_propertySetManager->addPropertySet(QString("otherset"), otherset);
	






	QObject::connect(tuxing.get(), &huabutuxing::signalRepaint, this, qOverload<>(&huabu::update));

	m_tuxingvec.push_back(tuxing);

	update(); //触发重绘，会调用paintEvent，可以选择重绘部分区域
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

void huabu::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
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



void huabu::initPainter(QPainter& painter)
{
	painter.setRenderHint(QPainter::Antialiasing, true);
}




QString huabu::createTuxingName(ShapeType type)
{
	return ShapeTypeTool::shapetypeEnumToQstring(type) + QString::number(m_tuxingnum++);
}


huabutuxing::huabutuxing()
{

}


