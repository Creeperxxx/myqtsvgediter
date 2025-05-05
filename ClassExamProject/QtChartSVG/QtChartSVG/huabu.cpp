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
	buildPropertyDataHuaub();
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

	params->m_center = event->posF();
	params->m_paramChanged = true;

	auto drawer = DiagramDrawInterface::getInstance().getDrawer(params);

	std::shared_ptr<huabutuxing> tuxing = std::make_shared<huabutuxing>();
	tuxing->m_params = params;
	tuxing->m_drawer = drawer;
	tuxing->m_name = createTuxingName(params->m_type);
	tuxing->m_center = event->posF();
	tuxing->m_centerhoffset = 0;
	tuxing->m_centervoffset = 0;
	tuxing->m_dataTime = QDateTime::currentMSecsSinceEpoch();
	tuxing->buildPropertyData();
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
		bool iscontain = false;
		QPointF point = event->localPos();
		for (const auto& tuxing : m_tuxingvec)
		{
			if (tuxing->m_ret->iscontainPoint(point))
			{
				emit signalMouseClicked(shapetypeToPropertyType(tuxing->m_type), tuxing->m_propertydataVec);
				iscontain = true;
			}
		}
		if (!iscontain)
		{
			emit signalMouseClicked(PropertyWidgetManager::propertyobjecttype::huabu, m_propertydataHuabuVec);
		}
	}
}



void huabu::initPainter(QPainter& painter)
{
	painter.setRenderHint(QPainter::Antialiasing, true);
}


PropertyWidgetManager::propertyobjecttype huabu::shapeTypeToPropertyobjectType(ShapeType type)
{
	using proptytype = PropertyWidgetManager::propertyobjecttype;
	switch (type)
	{
	case ShapeType::Rect:
		return proptytype::huabuRect;
		break;
	case ShapeType::Circle:
		return proptytype::huabuCircle;
		break;
	case ShapeType::Triangle:
		return proptytype::huabuTriangle;
		break;
	case ShapeType::Line:
		return proptytype::huabuLine;
		break;
	default:
		throw std::runtime_error("error");
	}
}

void huabu::buildPropertyDataHuaub()
{
	std::shared_ptr<propertydata> data = nullptr;
	data = std::make_shared<propertydata>(propertynamename, "huabu");
	m_propertydataHuabuVec.push_back(data);

	data = std::make_shared<propertydata>(propertynamehuabuwidth, QVariant::fromValue(m_size.width()));
	m_propertydataHuabuVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabu::onValueChangedHuabuSizeWidth);

	data = std::make_shared<propertydata>(propertynamehuabuheight, QVariant::fromValue(m_size.height()));
	m_propertydataHuabuVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabu::onValueChangedHuabuSizeHeight);

}

QString huabu::createTuxingName(ShapeType type)
{
	return ShapeTypeTool::shapetypeEnumToQstring(type) + QString::number(m_tuxingnum++);
}

PropertyWidgetManager::propertyobjecttype huabu::shapetypeToPropertyType(ShapeType type)
{
	using ptype = PropertyWidgetManager::propertyobjecttype;
	switch (type)
	{
	case ShapeType::Rect:
		return ptype::huabuRect;
		break;
	case ShapeType::Circle:
		return ptype::huabuCircle;
		break;
	case ShapeType::Triangle:
		return ptype::huabuTriangle;
		break;
	case ShapeType::Line:
		return ptype::huabuLine;
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

void huabutuxing::buildPropertyData()
{
	switch (m_params->m_type)
	{
	case ShapeType::Rect:
		buildPropertyDataRect();
		break;
	case ShapeType::Circle:
		buildPropertyDataCircle();
		break;
	case ShapeType::Triangle:
		buildPropertyDataTriangle();
		break;
	case ShapeType::Line:
		buildPropertyDataLine();
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

void huabutuxing::buildPropertyDataRect()
{
	if (m_params == nullptr)
		throw std::runtime_error("error");
	auto p = dynamic_cast<DiagramDrawParamsRect*>(m_params.get());
	if (p == nullptr)
		throw std::runtime_error("error");

	std::shared_ptr<propertydata> data = nullptr;

	buildPropertyDataName();

	data = std::make_shared<propertydata>(propertynameradio, QVariant::fromValue(p->m_boundingrectradio));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangeRectRadio);

	data = std::make_shared<propertydata>(propertynamerotate, QVariant::fromValue(p->m_rectrotate));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedRectRotate);

	buildPropertyDataScale();
	buildPropertyDataSpacesize();
	buildPropertyDataPenandBrush();
	buildPropertyDataCenteroffset();
}

void huabutuxing::buildPropertyDataCircle()
{
	if (m_params == nullptr)
		throw std::runtime_error("error");
	auto p = dynamic_cast<DiagramDrawParamsCircle*>(m_params.get());
	if (p == nullptr)
		throw std::runtime_error("error");

	std::shared_ptr<propertydata> data = nullptr;
	buildPropertyDataName();

	data = std::make_shared<propertydata>(propertynameradio, QVariant::fromValue(p->m_boundingrectradio));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedCircleRadio);

	data = std::make_shared<propertydata>(propertynamerotate, QVariant::fromValue(p->m_circlerotate));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedCircleRotate);

	buildPropertyDataScale();
	buildPropertyDataSpacesize();
	buildPropertyDataPenandBrush();
	buildPropertyDataCenteroffset();
}

void huabutuxing::buildPropertyDataTriangle()
{
	if (m_params == nullptr)
		throw std::runtime_error("error");
	auto p = dynamic_cast<DiagramDrawParamsTriangle*>(m_params.get());
	if (p == nullptr)
		throw std::runtime_error("error");

	std::shared_ptr<propertydata> data = nullptr;
	buildPropertyDataName();

	data = std::make_shared<propertydata>(propertynamebottomradio, QVariant::fromValue(p->m_triangleSizeRadios.m_bottom));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedTriangeRadioBottom);

	data = std::make_shared<propertydata>(propertynameleftradio, QVariant::fromValue(p->m_triangleSizeRadios.m_left));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedTriangeRadioLeft);

	data = std::make_shared<propertydata>(propertynamerightradio, QVariant::fromValue(p->m_triangleSizeRadios.m_right));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedTriangeRadioRight);

	data = std::make_shared<propertydata>(propertynameedgetype, QVariant::fromValue(DiagramDrawParamsTriangle::edgetypeEnumToString(p->m_edgetype)));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedTriangleEdgetype);

	data = std::make_shared<propertydata>(propertynamerotate, QVariant::fromValue(p->m_rotationAngle));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedTriangleRotate);

	buildPropertyDataScale();
	buildPropertyDataSpacesize();
	buildPropertyDataPenandBrush();
	buildPropertyDataCenteroffset();
}

void huabutuxing::buildPropertyDataLine()
{
	if (m_params == nullptr)
		throw std::runtime_error("error");
	auto p = dynamic_cast<DiagramDrawParamsLine*>(m_params.get());
	if (p == nullptr)
		throw std::runtime_error("error");

	std::shared_ptr<propertydata> data = nullptr;
	buildPropertyDataName();

	data = std::make_shared<propertydata>(propertynamerotate, QVariant::fromValue(p->m_rotationAngle));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedLineRotate);

	buildPropertyDataScale();
	buildPropertyDataPenandBrush();
	buildPropertyDataCenteroffset();

}

void huabutuxing::onValueChangedName(QVariant value)
{
	if (!value.canConvert<QString>())
		throw std::runtime_error("error");
	m_name = value.toString();
}

void huabutuxing::onValueChangeRectRadio(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	dynamic_cast<DiagramDrawParamsRect*>(m_params.get())->m_boundingrectradio = value.toDouble();
	emit signalRepaint();
}

void huabutuxing::onValueChangedRectRotate(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	dynamic_cast<DiagramDrawParamsRect*>(m_params.get())->m_rectrotate = value.toInt();
	emit signalRepaint();
}

void huabutuxing::onValueChangedScale(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	m_params->m_scale = value.toDouble();
	emit signalRepaint();
}

void huabutuxing::onValueChangedSpaceWidth(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	m_params->m_spacesize.setWidth(value.toDouble());
	emit signalRepaint();
}

void huabutuxing::onValueChangedSpaceHeight(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	m_params->m_spacesize.setHeight(value.toDouble());
	emit signalRepaint();
}

void huabutuxing::onValueChangedPenColor(QVariant value)
{
	if (!value.canConvert<QColor>())
		throw std::runtime_error("error");
	m_params->m_pen.setColor(value.value<QColor>());
	emit signalRepaint();
}

void huabutuxing::onValueChangedPenWidth(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_params->m_pen.setWidth(value.toInt());
	emit signalRepaint();
}

void huabutuxing::onvalueChangedBrushColor(QVariant value)
{
	if (!value.canConvert<QColor>())
		throw std::runtime_error("error");
	m_params->m_brush.setColor(value.value<QColor>());
	emit signalRepaint();
}

void huabutuxing::onValueChangedHOffset(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_centerhoffset = value.toInt();
	m_params->m_center.setX(m_center.x() + m_centerhoffset);
	emit signalRepaint();
}

void huabutuxing::onValueChangedVOffset(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_centervoffset = value.toInt();
	m_params->m_center.setY(m_center.y() + m_centervoffset);
	emit signalRepaint();
}

void huabutuxing::onValueChangedCircleRadio(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	dynamic_cast<DiagramDrawParamsCircle*>(m_params.get())->m_boundingrectradio = value.toDouble();
	emit signalRepaint();
}

void huabutuxing::onValueChangedCircleRotate(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	dynamic_cast<DiagramDrawParamsCircle*>(m_params.get())->m_circlerotate = value.toInt();
	emit signalRepaint();
}

void huabutuxing::onValueChangedTriangeRadioBottom(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	dynamic_cast<DiagramDrawParamsTriangle*>(m_params.get())->m_triangleSizeRadios.m_bottom = value.toDouble();
	emit signalRepaint();
}

void huabutuxing::onValueChangedTriangeRadioLeft(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	dynamic_cast<DiagramDrawParamsTriangle*>(m_params.get())->m_triangleSizeRadios.m_left = value.toDouble();
	emit signalRepaint();
}

void huabutuxing::onValueChangedTriangeRadioRight(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	dynamic_cast<DiagramDrawParamsTriangle*>(m_params.get())->m_triangleSizeRadios.m_right = value.toDouble();

	emit signalRepaint();
}

void huabutuxing::onValueChangedTriangleEdgetype(QVariant value)
{
	if (!value.canConvert<QString>())
		throw std::runtime_error("error");
	dynamic_cast<DiagramDrawParamsTriangle*>(m_params.get())->m_edgetype = DiagramDrawParamsTriangle::edgetypeStringToEnum(value.toString());
	emit signalRepaint();
}

void huabutuxing::onValueChangedTriangleRotate(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	dynamic_cast<DiagramDrawParamsTriangle*>(m_params.get())->m_rotationAngle = value.toInt();
	emit signalRepaint();
}

void huabutuxing::onValueChangedLineRotate(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	dynamic_cast<DiagramDrawParamsLine*>(m_params.get())->m_rotationAngle = value.toInt();
	emit signalRepaint();
}

void huabutuxing::buildPropertyDataPenandBrush()
{
	std::shared_ptr<propertydata> data = nullptr;
	data = std::make_shared<propertydata>(propertynamepencolor, QVariant::fromValue(m_pen.color()));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedPenColor);

	data = std::make_shared<propertydata>(propertynamepenwidth, QVariant::fromValue(m_pen.width()));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedPenWidth);

	data = std::make_shared<propertydata>(propertynamebrush, QVariant::fromValue(m_brush.color()));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onvalueChangedBrushColor);
}

void huabutuxing::buildPropertyDataCenteroffset()
{
	std::shared_ptr<propertydata> data = nullptr;
	data = std::make_shared<propertydata>(propertynamehmove, QVariant::fromValue(m_centerhoffset));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedHOffset);

	data = std::make_shared<propertydata>(propertynamevmove, QVariant::fromValue(m_centervoffset));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedVOffset);
}

void huabutuxing::buildPropertyDataSpacesize()
{
	std::shared_ptr<propertydata> data = nullptr;
	data = std::make_shared<propertydata>(propertynametuxingspacewidth, QVariant::fromValue(m_params->m_spacesize.width()));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedSpaceWidth);

	data = std::make_shared<propertydata>(propertynametuxingspaceheight, QVariant::fromValue(m_params->m_spacesize.height()));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedSpaceHeight);
}

void huabutuxing::buildPropertyDataScale()
{
	std::shared_ptr<propertydata> data = nullptr;
	data = std::make_shared<propertydata>(propertynamescale, QVariant::fromValue(m_params->m_scale));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedScale);
}

void huabutuxing::buildPropertyDataName()
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(propertynamename, QVariant::fromValue(m_name));
	m_propertydataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &huabutuxing::onValueChangedName);
}





