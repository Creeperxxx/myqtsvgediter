#include "tuxing.h"
#include "propertywidget.h"
#include "propertyset.h"
#include "propertydatabuilder.h"
#include "myconfig.h"
#include "namespace.h"
#include "propertynamevec.h"
#include "drawparamscreator.h"

diagram::diagram(myqtsvg::ShapeType type, QWidget* parent)
	:QWidget(parent)
	, m_dragStartPos(0, 0)
	, m_issizefixed(myconfig::getInstance().getDiagramButtonIsSizeFixed())
	, m_widgetRadio(1)
	, m_drawer(nullptr)
	, m_mimetype(myconfig::getInstance().getMimetype())
	, m_params(nullptr)
	, m_propertySetManager(nullptr)
{
	auto& config = myconfig::getInstance();

	myinitSizePolicy();
	initDrawParams(type);
	initDrawer();

	m_propertySetManager = initPropertySetManager::createPropertySetManager(myqtsvg::diagramShapetypeToPropertyWidgetType(m_params->getType())
		, m_params
		, [this]() {
			this->onParamsValueChanged();
		});

	//m_propertySetManager = std::make_shared<propertySetManager>();
	//m_propertySetManager->m_propertyWidgetType = myqtsvg::diagramShapetypeToPropertyWidgetType(m_params->m_type);


	//std::shared_ptr<drawParamsPropertySet> drawParamsSet = std::make_shared<drawParamsPropertySet>();
	//drawParamsSet->m_params = m_params;
	//auto propertynamevec = propertyNameVecInterface::getinstance().getPropertyNameVec(type);
	//auto creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
	//drawParamsSet->m_propertyDataVec = creator->create(drawParamsSet);
	//QObject::connect(drawParamsSet.get(), &drawParamsPropertySet::SignalValueChangedByData, this, &diagram::onParamsValueChanged);
	//m_propertySetManager->addPropertySet(config.getDrawParamsSetName(), drawParamsSet);

	//std::shared_ptr<otherPropertySet> otherset = std::make_shared<otherPropertySet>();
	//otherset->m_name = myqtsvg::ShapetypeEnumToQstring(type);
	//propertynamevec.clear();
	//propertynamevec.push_back(config.getNameName());
	//creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
	//otherset->m_propertyDataVec = creator->create(otherset);
	//m_propertySetManager->addPropertySet(config.getOtherSetName(), otherset);

}

void diagram::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_dragStartPos = event->localPos();
		emit signalPropertyShow(m_propertySetManager);
		emit signalMouseDrawing(m_params);
	}
	QWidget::mousePressEvent(event);
}

void diagram::mouseMoveEvent(QMouseEvent* event)
{
	if (!(event->buttons() & Qt::LeftButton))
		return;
	if ((event->localPos() - m_dragStartPos).manhattanLength() < QApplication::startDragDistance())
		return;
	if (m_params->getType() == myqtsvg::ShapeType::Mouse
		|| m_params->getType() == myqtsvg::ShapeType::choose)
		return;
	createQDrag();
	QWidget::mouseMoveEvent(event);
}


void diagram::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	if (!m_issizefixed)
	{
		QSize allocated = event->size();
		const float ratio = m_widgetRadio; // 宽高比

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

void diagram::createQDrag()
{
	QDrag* drag = new QDrag(this);

	QMimeData* qmimedata = new QMimeData();

	QByteArray array;
	QDataStream stream(&array, QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);
	int inttype = static_cast<int>(m_params->getType());
	stream << inttype;
	stream << *m_params;

	qmimedata->setData(m_mimetype, array);
	drag->setMimeData(qmimedata);

	QPixmap pixmap(m_params->getSpacesize());
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);

	m_params->setCenter(pixmap.rect().center());
	m_drawer->draw(painter);

	drag->setPixmap(pixmap);
	drag->setHotSpot(pixmap.rect().center());
	drag->exec(Qt::CopyAction);
}

void diagram::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);

	QSize spacesize = m_params->getSpacesize();
	m_params->setCenter(rect().center());
	m_params->setSpacesize(size());

	m_drawer->draw(painter);
	m_params->setSpacesize(spacesize);
}

void diagram::onParamsValueChanged()
{
	update();
}

void diagram::myinitSizePolicy()
{
	auto& config = myconfig::getInstance();
	if (m_issizefixed)
	{
		setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		setFixedSize(config.getDiagramButtonFixedSize());
	}
	else
	{
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		setMaximumSize(config.getDiagramButtonMaxSize());
		setMinimumSize(config.getDiagramButtonMinSize());
		m_widgetRadio = config.getDiagramButtonWidgetRadio();
	}
}

void diagram::initDrawParams(myqtsvg::ShapeType type)
{
	auto creator = createParamsInterface::getInstance().getParams(type);
	auto params = creator->create();
	if (params == nullptr || params.get() == nullptr || params->getType() != type)
		throw std::runtime_error("error");
	m_params = params;
}

void diagram::initDrawer()
{
	m_drawer = DiagramDrawInterface::getInstance().getDrawer(m_params);
	if (m_drawer == nullptr || m_drawer.get() == nullptr)
		throw std::runtime_error("error");
}






