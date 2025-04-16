#include "huabu.h"

huabu::huabu(QWidget* parent)
	: QWidget(parent)
	, m_pen(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::tuxingspace::pen::color))), cfggetval<int>(qtcf::huabu::tuxingspace::pen::width))
	, m_brush(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::tuxingspace::brush))))
	, m_tuxingspacesize(QSizeF(cfggetval<double>(qtcf::huabu::tuxingspace::spacewidth), cfggetval<double>(qtcf::huabu::tuxingspace::spaceheight)))
	, m_juxingradio(cfggetval<double>(qtcf::tuxing::rectangle::radio))
	, m_mimetype(QString::fromStdString(cfggetval<std::string>(qtcf::mimetype)))
{
	//ui.setupUi(this);
	init();
}

void huabu::init()
{
	setAcceptDrops(true);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setFixedSize(800, 600);
	//setMinimumSize(400, 400);
}

//QSize huabu::sizeHint() const
//{
//	return QSize(800, 600);
//}

//void huabu::mousePressEvent(QMouseEvent* event)
//{
//	if (event->button() == Qt::LeftButton)
//	{
//		m_dragStartPos = event->position().toPoint();
//		m_dragStartSize = size();
//		m_isResizing = true;
//	}
//}
//
//void huabu::mouseMoveEvent(QMouseEvent* event)
//{
//	if (m_isResizing && (event->buttons() & Qt::LeftButton))
//	{
//		QPoint delta = event->pos() - m_dragStartPos;
//		QSize newSize = m_dragStartSize + QSize(delta.x(), delta.y());
//
//		// 确保不小于最小大小
//		newSize = newSize.expandedTo(minimumSize());
//
//		resize(newSize);
//		updateGeometry(); // 通知布局系统大小已更改
//	}
//}
//
//void huabu::mouseReleaseEvent(QMouseEvent* event)
//{
//	if (event->button() == Qt::LeftButton)
//	{
//		m_isResizing = false;
//	}
//}

DiagramMimedata huabu::getmimedata()
{
	if (!m_mimedata.has_value())
		throw std::runtime_error("error");
	return m_mimedata.value();
}

QPen huabu::getpen()
{
	return m_pen;
}

QBrush huabu::getbrush()
{
	return m_brush;
}

QSizeF huabu::getspacesize()
{
	return m_tuxingspacesize;
}

QPointF huabu::getcenter()
{
	if (!m_dropevetcenter.has_value())
		throw std::runtime_error("error");
	return m_dropevetcenter.value();
}

ShapeType huabu::gettype()
{
	if (!m_mimedata.has_value())
		throw std::runtime_error("error");
	return m_mimedata.value().m_type;
}

huabu::~huabu()
{
}

void huabu::dragEnterEvent(QDragEnterEvent* event)
{
	if (true == event->mimeData()->hasFormat(QString::fromStdString(cfggetval<std::string>(qtcf::mimetype))))
	{
		event->acceptProposedAction();
	}

}

//std::unique_ptr<IbuildAllDiagramParams> FactoryBuildDiagramParams::create(DiagramItemType type)
//{
//	switch (type)
//	{
//	case DiagramItemType::tuxingku:
//		return std::make_unique<Ibuildtuxingjiedianparamsfordiagram>();
//		break;
//	case DiagramItemType::tuxingkupixmap:
//		return std::make_unique<IbuildDiagramPixmapParams>();
//		break;
//	case DiagramItemType::huabu:
//		break;
//	default:
//		throw std::runtime_error("error");
//		break;
//	}
//}



std::shared_ptr<IDidgramDrawParams> huabu::createtuxingparams(ShapeType type)
{
	switch (type)
	{
	default:
	case ShapeType::Rect:
	{
		std::shared_ptr<DiagramDrawParamsRect> p = std::make_shared<DiagramDrawParamsRect>();
		p->m_boundingrectradio = m_juxingradio;
		return p;
	}
	break;
	case ShapeType::Circle:
	{
		std::shared_ptr<DiagramDrawParamsCircle> p = std::make_shared<DiagramDrawParamsCircle>();
		return p;
	}

	break;
	}
}

void huabu::dropEvent(QDropEvent* event)
{
	//获取信息，这个事件要做什么
	//重绘
	//QByteArray array = event->mimeData()->data(QString::fromStdString(cfggetval<std::string>(qtcf::mimetype)));
	//ShapeType type = getshapetypefrombytearray(array);
	QByteArray array = event->mimeData()->data(m_mimetype);
	DiagramMimedata data;
	QDataStream stream(array);
	stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);

	stream >> data;
	m_mimedata = data;

	m_dropevetcenter = event->position();

	std::shared_ptr<huabutuxing> tuxing = std::make_shared<huabutuxing>();
	std::shared_ptr<IDidgramDrawParams> params = factoryall::create(DiagramItemType::huabu, data.m_type)->build(this);
	m_mimedata.reset();
	m_dropevetcenter.reset();

	//tuxing->m_ret = factorytuxingjiedian::draw(params);
	tuxing->m_params = params;

	m_tuxingvec.push_back(tuxing);

	//std::unique_ptr<tuxingjiedianparams> params = maketuxingparams(type, event->position().toPoint());
	//std::unique_ptr<Ituxingjiedian> tuxingjiedian = tuxingjiedianfactory::createtuxingjiedian(type, std::move(params));
	//tuxingjiedian->setmousepoint(event->position().toPoint());
	//tuxingjiedian->setspacesize(QSize(huabutuxingwith, huabutuxingheight));
	//m_tuxingvec.push_back(std::move(tuxingjiedian));
	update(); //触发重绘，会调用paintEvent，可以选择重绘部分区域
	event->acceptProposedAction();
}

//std::unique_ptr<IDidgramDrawParams> huabu::maketuxingparams(ShapeType type, QPoint mousepoint)
//{
//	switch (type)
//	{
//	default:
//	case ShapeType::juxing:
//		//return std::make_unique<tuxingjiedianparamsjuxing>(event->position().toPoint(), m_tuxingspacesize, m_pen, m_brush, m_juxingradio);
//	{
//		auto p = std::make_unique<DiagramDrawParamsRect>();
//		p->m_center = mousepoint;
//		p->m_spacesize = m_tuxingspacesize;
//		p->m_radio = m_juxingradio; //todo : juxingparams初始化
//		return std::move(p);
//	}
//	break;
//	case ShapeType::yuanxing:
//		//return std::make_unique<tuxingjiedianparamsyuanxing>(event->position().toPoint(), m_tuxingspacesize, m_pen, m_brush);
//	{
//		auto p = std::make_unique<DiagramDrawParamsCircle>();
//		p->m_center = mousepoint;
//		p->m_spacesize = m_tuxingspacesize;
//		return std::move(p);//todo:yuanxingparams初始化
//	}
//	break;
//	}
//}

//ShapeType huabu::getshapetypefrombytearray(QByteArray array)
//{
//	bool flag;
//	int typeint = array.toInt(&flag);
//	ShapeType type;
//	if (!flag)
//	{
//		type = ShapeType::Rect;
//	}
//	else
//	{
//		type = static_cast<ShapeType>(typeint);
//	}
//	return type;
//}

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
	initPainter(painter);
	painter.fillRect(this->rect(), Qt::white);
	for (auto& diagram : m_tuxingvec)
	{
		diagram->m_ret = DiagramDrawInterface::draw(painter, diagram->m_params);
	}
	//for (auto it = m_tuxingvec.begin(); it != m_tuxingvec.end(); ++it)
	//{
		////(*it)->m_params->m_painter = painter;
		//(*it)->m_ret = DiagramDrawInterface::draw(painter, (*it)->m_params);
	//}
	//painter->fillRect(this->rect(), Qt::white);

	//for (auto& tuxing : m_tuxingvec) //不允许复制好像，但加const就会对draw限制
	//{
		//tuxing->draw(painter);
	//}
}

//void huabu::drawBaseBackground(QPainter* painter)
//void huabu::drawBaseBackground()
//{
//	//this->setAutoFillBackground(true);
//	this->setStyleSheet("background-color: white; * { background-color: transparent; border: none; }");
//
//}

//void huabu::init()
//{
//	setAcceptDrops(true);
//	//drawBaseBackground();
//	//initpenandbrush(huabupencolor, huabupenwidth, huabubrush);
//	initpenandbrush(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabuTuxingSpacePenColor))), cfggetval<int>(qtcf::huabuTuxingSpacePenWidth), QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabuTuxingSpaceBrush))));
//	//m_tuxingspacesize = QSize(huabutuxingwith, huabutuxingheight);
//	m_tuxingspacesize = QSize(cfggetval<int>(qtcf::huabuTuxingSpaceSpaceWidth), cfggetval<int>(qtcf::huabuTuxingSpaceSpaceWidth) / cfggetval<float>(qtcf::huabuTuxingSpaceSpaceradio));
//
//	m_juxingradio = cfggetval<float>(qtcf::huabuTuxingJuxingRadio);
//
//	//setAttribute(Qt::WA_StaticContents);
//	//setAttribute(Qt::WA_OpaquePaintEvent);
//
//	// 启用抗锯齿
//	//setAttribute(Qt::WA_AlwaysShowToolTips);
//}


void huabu::initPainter(QPainter& painter)
{
	painter.setPen(m_pen);
	painter.setBrush(m_brush);
	painter.setRenderHint(QPainter::Antialiasing, true);

}

QPainter* huabu::initPainter()
{
	QPainter* painter = new QPainter(this);
	// 设置画笔
	painter->setPen(m_pen);
	//设置画刷
	painter->setBrush(m_brush);
	//设置抗锯齿
	painter->setRenderHint(QPainter::Antialiasing, true);
	return painter;
	//设置合成模式
	//m_painter->setCompositionMode()
}

//Ituxingjiedian* tuxingjiedianfactory::createtuxignjiedian(ShapeType type, QDropEvent* event)
//std::unique_ptr<Ituxingjiedian> tuxingjiedianfactory::createtuxingjiedian(ShapeType type, std::unique_ptr<tuxingjiedianparams> params)
//{
//	switch (type)
//	{
//	default:
//	case ShapeType::juxing:
//		return std::make_unique<juxingjiedian>(params.get());
//		break;
//	case ShapeType::yuanxing:
//		return std::make_unique<yuanxingjiedian>(params.get());
//		break;
//		//return new yuanxingjiedian(params.get());
//	}
//}

//void huabu::initpenandbrush(QBrush pencolor, int penwidth, QBrush brush)
//{
//	m_pen = QPen(pencolor, penwidth);
//	m_brush = brush;
//}





