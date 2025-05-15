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
	, m_params(nullptr)
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
	if (params == nullptr || params.get() == nullptr)
		throw std::runtime_error("error");
	m_params = params;

	m_propertySetManager = std::make_shared<propertySetManager>();
	switch (params->m_type)
	{
	case ShapeType::Rect:
		m_propertySetManager->m_propertyObjectType = PropertyWidgetManager::propertyobjecttype::diagramRect;
		break;
	case ShapeType::Circle:
		m_propertySetManager->m_propertyObjectType = PropertyWidgetManager::propertyobjecttype::diagramCircle;
		break;
	case ShapeType::Triangle:
		m_propertySetManager->m_propertyObjectType = PropertyWidgetManager::propertyobjecttype::diagramTriangle;
		break;
	case ShapeType::Line:
		m_propertySetManager->m_propertyObjectType = PropertyWidgetManager::propertyobjecttype::diagramLine;
		break;
	default:
		throw std::runtime_error("error");
		break;
	}

	m_drawer = DiagramDrawInterface::getInstance().getDrawer(params);

	std::shared_ptr<drawParamsPropertySet> drawParamsSet = std::make_shared<drawParamsPropertySet>();
	drawParamsSet->m_params = params;
	std::shared_ptr<std::vector<QString>> propertynamevec = std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
			QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::colorname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::widthname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::brushcolorname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::rotatename))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::scalename))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::widthname))
			, QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::heightname))
		});
	switch (params->m_type)
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
	QObject::connect(drawParamsSet.get(), &drawParamsPropertySet::SignalValueChangedByData, this, &Idiagram::onParamsValueChanged);
	m_propertySetManager->addPropertySet(QString("drawParams"), drawParamsSet);



	std::shared_ptr<otherPropertySet> otherset = std::make_shared<otherPropertySet>();
	switch (params->m_type)
	{
	case ShapeType::Rect:
		otherset->m_name = QString::fromStdString(cfggetval<std::string>(qtcf::tuxingku::diagramwidget::name::rect));
		break;
	case ShapeType::Circle:
		otherset->m_name = QString::fromStdString(cfggetval<std::string>(qtcf::tuxingku::diagramwidget::name::circle));
		break;
	case ShapeType::Triangle:
		otherset->m_name = QString::fromStdString(cfggetval<std::string>(qtcf::tuxingku::diagramwidget::name::triangle));
		break;
	case ShapeType::Line:
		otherset->m_name = QString::fromStdString(cfggetval<std::string>(qtcf::tuxingku::diagramwidget::name::line));
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
	propertynamevec = std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::namename))
	});
	creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
	otherset->m_propertyDataVec = creator->create(otherset);
	m_propertySetManager->addPropertySet(QString("other"), otherset);





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
		emit signalMouseClicked(m_propertySetManager);
	}
}

void Idiagram::mouseMoveEvent(QMouseEvent* event)
{
	if (!(event->buttons() & Qt::LeftButton))
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
	QDrag* drag = new QDrag(this);

	QMimeData* qmimedata = new QMimeData();

	QByteArray array;
	QDataStream stream(&array, QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);
	int inttype = static_cast<int>(m_params->m_type);
	stream << inttype;
	stream << *m_params;

	qmimedata->setData(m_mimetype, array);
	drag->setMimeData(qmimedata);

	QPixmap pixmap(m_params->m_spacesize);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);

	m_params->m_center = pixmap.rect().center();
	m_params->m_paramChanged = true;
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

	QSize size = m_params->m_spacesize;
	m_params->m_center = this->rect().center();
	m_params->m_spacesize = this->size();
	m_params->m_paramChanged = true;

	m_drawer->draw(painter);
	m_params->m_spacesize = size;
	m_params->m_paramChanged = true;
}

void Idiagram::onParamsValueChanged()
{
	update();
}




