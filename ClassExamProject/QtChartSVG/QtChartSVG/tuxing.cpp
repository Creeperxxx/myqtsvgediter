#include "tuxing.h"
#include "shuxingwidget.h"
#include "config.h"


Idiagram::Idiagram(std::shared_ptr<IDidgramDrawParams> params, QWidget* parent)
	:QWidget(parent)
	, m_dragStartPos(0,0)
	, m_issizefixed(cfggetval<bool>(qtcf::tuxingku::diagramwidget::issizefix))
	, m_widgetRadio(std::nullopt)
	, m_drawer(nullptr)
	, m_mimetype(QString::fromStdString(cfggetval<std::string>(qtcf::mimetype)))
{
	if (m_issizefixed)
	{
		setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		setFixedWidth(cfggetval<int>(qtcf::tuxingku::diagramwidget::fixwidth));
		setFixedHeight(cfggetval<int>(qtcf::tuxingku::diagramwidget::fixheight));
	}
	else
	{
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		setMaximumHeight(cfggetval<int>(qtcf::tuxingku::diagramwidget::maxheight));
		setMaximumWidth(cfggetval<int>(qtcf::tuxingku::diagramwidget::maxwidth));
		setMinimumWidth(cfggetval<int>(qtcf::tuxingku::diagramwidget::minwidth));
		setMinimumHeight(cfggetval<int>(qtcf::tuxingku::diagramwidget::minheight));
		m_widgetRadio = cfggetval<double>(qtcf::tuxingku::diagramwidget::widgetradio);
	}

	QObject::connect(params.get(), &IDidgramDrawParams::SignalParamsChanged, this, &Idiagram::onParamsValueChanged);

	m_drawer = DiagramDrawInterface::getInstance().getDrawer(params);
	if (m_drawer == nullptr)
		throw std::runtime_error("error");

	auto creator = PropertyDataInterface::getInstance().getCreator(params->m_type);
	m_propertyDataVec = creator->createPropertyData(params);

	using mytype = PropertyWidgetManager::propertyobjecttype;
	switch (params->m_type)
	{
	case ShapeType::Rect:
		m_propertyobjecttype = mytype::diagramRect;
		break;
	case ShapeType::Circle:
		m_propertyobjecttype = mytype::diagramCircle;
		break;
	case ShapeType::Triangle:
		m_propertyobjecttype = mytype::diagramTriangle;
		break;
	case ShapeType::Line:
		m_propertyobjecttype = mytype::diagramLine;
		break;
	default:
		throw std::runtime_error("error");
		break;
	}

}

void Idiagram::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_dragStartPos = event->localPos();
		emit signalMouseClicked(m_propertyobjecttype, m_propertyDataVec);
	}
}

void Idiagram::mouseMoveEvent(QMouseEvent* event)
{
	if (!(event->button() & Qt::LeftButton))
		return;
	if ((event->localPos() - m_dragStartPos).manhattanLength() < QApplication::startDragDistance())
		return;
	createQDrag();
}


void Idiagram::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	if (!m_issizefixed)
	{
		QSize allocated = event->size();
		if (!m_widgetRadio.has_value())
			throw std::runtime_error("error");//todo:except
		const float ratio = m_widgetRadio.value(); // 宽高比

		// 候选方案1：以宽度为主导
		int w1 = qBound(minimumSize().width(), allocated.width(), maximumSize().width());
		int h1 = qRound(w1 / ratio);
		h1 = qBound(minimumSize().height(), h1, maximumSize().height());
		QSize candidateW(w1, h1);

		// 候选方案2：以高度为主导
		int h2 = qBound(minimumSize().height(), allocated.height(), maximumSize().height());
		int w2 = qRound(h2 * ratio);
		w2 = qBound(minimumSize().width(), w2, maximumSize().width());
		QSize candidateH(w2, h2);

		// 选择更接近原始比例的方案（或面积更大的）
		float ratioDiffW = qAbs(candidateW.width() / (float)candidateW.height() - ratio);
		float ratioDiffH = qAbs(candidateH.width() / (float)candidateH.height() - ratio);

		QSize finalSize = (ratioDiffW < ratioDiffH) ? candidateW : candidateH;
		this->resize(finalSize);
	}
}

void Idiagram::createQDrag()
{
	auto params = param();

	QDrag* drag = new QDrag(this);

	QMimeData* qmimedata = new QMimeData();

	QByteArray array;
	QDataStream stream(&array, QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);
	int inttype = static_cast<int>(params->m_type);
	stream << inttype;
	stream << *params;

	qmimedata->setData(m_mimetype, array);
	drag->setMimeData(qmimedata);

	QPixmap pixmap(params->m_spacesize.toSize());
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);

	params->m_center = pixmap.rect().center();
	params->m_spacesize = pixmap.size();
	params->m_paramChanged = true;
	m_drawer->draw(painter);

	drag->setPixmap(pixmap);
	drag->setHotSpot(pixmap.rect().center());
	drag->exec(Qt::CopyAction);
}

void Idiagram::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);

	auto params = param();
	QSizeF size = params->m_spacesize;
	params->m_center = this->rect().center();
	params->m_spacesize = this->size();
	params->m_paramChanged = true;

	m_drawer->draw(painter);
	params->m_spacesize = size;
}

void Idiagram::onParamsValueChanged()
{
	param()->m_paramChanged = true;
	update();
}

std::shared_ptr<IDidgramDrawParams> Idiagram::param()
{
	return m_drawer == nullptr ? nullptr : m_drawer->getParams();
}



