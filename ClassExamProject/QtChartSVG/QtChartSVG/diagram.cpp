#include "diagram.h"
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

	m_propertySetManager = initPropertySetManager::createPropertySetManager(
		myqtsvg::diagramShapetypeToPropertyWidgetType(m_params->getType())
		, m_params
		, [this]() {
			this->onParamsValueChanged();
		});

}

void diagram::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_dragStartPos = event->pos();
		emit signalPropertyShow(m_propertySetManager);
		emit signalMouseDrawing(m_params);
	}
	QWidget::mousePressEvent(event);
}

void diagram::mouseMoveEvent(QMouseEvent* event)
{
	if (!(event->buttons() & Qt::LeftButton))
		return;
	if ((event->pos() - m_dragStartPos).manhattanLength() < QApplication::startDragDistance())
		return;
	if (m_params->getType() == myqtsvg::ShapeType::Mouse
		|| m_params->getType() == myqtsvg::ShapeType::choose)
		return;
	createQDrag();
	QWidget::mouseMoveEvent(event);
}


void diagram::resizeEvent(QResizeEvent* event)
{
	if (!m_issizefixed)
	{
		QSize allocated = event->size();
		float ratio = m_widgetRadio;

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

	QWidget::resizeEvent(event);
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
	QWidget::paintEvent(event);
}

void diagram::onParamsValueChanged()
{
	update();
}

void diagram::setDrawer(std::shared_ptr<IDiagramDrawer> drawer)
{
	m_drawer = drawer;
}

std::shared_ptr<IDiagramDrawer> diagram::getDrawer()
{
	return m_drawer;
}

void diagram::setParams(std::shared_ptr<IDidgramDrawParams> params)
{
	m_params = params;
}

std::shared_ptr<IDidgramDrawParams> diagram::getParams()
{
	return m_params;
}

void diagram::setSetManager(std::shared_ptr<propertySetManager> setmanager)
{
	m_propertySetManager = setmanager;
}

std::shared_ptr<propertySetManager> diagram::getSetManager()
{
	return m_propertySetManager;
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
	if (params == nullptr || params->getType() != type)
		throw std::runtime_error("error");
	m_params = params;
}

void diagram::initDrawer()
{
	m_drawer = DiagramDrawInterface::getInstance().getDrawer(m_params);
	if (m_drawer == nullptr)
		throw std::runtime_error("error");
}






