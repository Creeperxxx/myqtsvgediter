#include "tuxing.h"
#include "propertywidget.h"
#include "propertyset.h"
#include "propertydatabuilder.h"
#include "myconfig.h"

diagram::diagram(std::shared_ptr<IDidgramDrawParams> params, QWidget* parent)
	:QWidget(parent)
	, m_dragStartPos(0, 0)
	, m_issizefixed(myconfig::getInstance().getDiagramButtonIsSizeFixed())
	, m_widgetRadio(1)
	, m_drawer(nullptr)
	, m_mimetype(myconfig::getInstance().getMimetype())
	, m_params(nullptr)
{
	if (m_issizefixed)
	{
		setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		setFixedSize(myconfig::getInstance().getDiagramButtonFixedSize());
	}
	else
	{
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		setMaximumSize(myconfig::getInstance().getDiagramButtonMaxSize());
		setMinimumSize(myconfig::getInstance().getDiagramButtonMinSize());
		m_widgetRadio = myconfig::getInstance().getDiagramButtonWidgetRadio();
	}
	if (params == nullptr || params.get() == nullptr)
		throw std::runtime_error("error");
	m_params = params;

	m_propertySetManager = std::make_shared<propertySetManager>();
	m_propertySetManager->m_propertyObjectType = shapetypeToPropertytype(params->m_type);

	m_drawer = DiagramDrawInterface::getInstance().getDrawer(params);

	std::shared_ptr<drawParamsPropertySet> drawParamsSet = std::make_shared<drawParamsPropertySet>();
	drawParamsSet->m_params = params;
	auto propertynamevec = createNameVec(params->m_type);
	auto creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
	drawParamsSet->m_propertyDataVec = creator->create(drawParamsSet);
	QObject::connect(drawParamsSet.get(), &drawParamsPropertySet::SignalValueChangedByData, this, &diagram::onParamsValueChanged);
	m_propertySetManager->addPropertySet(QString("drawParams"), drawParamsSet);



	std::shared_ptr<otherPropertySet> otherset = std::make_shared<otherPropertySet>();
	otherset->m_name = getName(params->m_type);
	propertynamevec = std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		myconfig::getInstance().getNameName()
	});
	creator = propertyDataVecOfPropertySetCreatorFactor::getInstance().create(propertynamevec);
	otherset->m_propertyDataVec = creator->create(otherset);
	m_propertySetManager->addPropertySet(QString("other"), otherset);


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
	if (m_params->m_type == ShapeType::Mouse 
		|| m_params->m_type == ShapeType::choose)
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

	QSize size = m_params->m_spacesize;
	m_params->m_center = this->rect().center();
	m_params->m_spacesize = this->size();

	m_drawer->draw(painter);
	m_params->m_spacesize = size;
}

void diagram::onParamsValueChanged()
{
	update();
}

std::shared_ptr<std::vector<QString>> diagram::createNameVec(ShapeType type)
{
	switch (type)
	{
	case ShapeType::Rect:
		return createNameVecRect();
		break;
	case ShapeType::Circle:
		return createNameVecCircle();
		break;
	case ShapeType::Triangle:
		return createNameVecTriangle();
		break;
	case ShapeType::Line:
		return createNameVecLine();
		break;
	case ShapeType::Mouse:
		return createNamgeVecMouse();
		break;
	case ShapeType::choose:
		return createNameVecChoose();
		break;
	case ShapeType::Text:
		return createNameVecText();
	default:
		throw std::runtime_error("error");
		break;
	}
}

std::shared_ptr<std::vector<QString>> diagram::createNameVecRect()
{
	return std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		myconfig::getInstance().getPenColorName()
			, myconfig::getInstance().getPenWdithName()
			, myconfig::getInstance().getBrushColorName()
			, myconfig::getInstance().getPenStyleName()
			, myconfig::getInstance().getRotateAngleName()
			, myconfig::getInstance().getScaleName()
			, myconfig::getInstance().getSpaceWidthName()
			, myconfig::getInstance().getSpaceHeightName()
			, myconfig::getInstance().getRectRadioName()
	});
}

std::shared_ptr<std::vector<QString>> diagram::createNameVecCircle()
{
	return std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		myconfig::getInstance().getPenColorName()
			, myconfig::getInstance().getPenWdithName()
			, myconfig::getInstance().getPenStyleName()
			, myconfig::getInstance().getBrushColorName()
			, myconfig::getInstance().getRotateAngleName()
			, myconfig::getInstance().getScaleName()
			, myconfig::getInstance().getSpaceWidthName()
			, myconfig::getInstance().getSpaceHeightName()
			, myconfig::getInstance().getCircleRadioName()
	});
}

std::shared_ptr<std::vector<QString>> diagram::createNameVecTriangle()
{
	return std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		myconfig::getInstance().getPenColorName()
			, myconfig::getInstance().getPenWdithName()
			, myconfig::getInstance().getPenStyleName()
			, myconfig::getInstance().getBrushColorName()
			, myconfig::getInstance().getRotateAngleName()
			, myconfig::getInstance().getScaleName()
			, myconfig::getInstance().getSpaceWidthName()
			, myconfig::getInstance().getSpaceHeightName()
			, myconfig::getInstance().getTriangleRadioName()
			, myconfig::getInstance().getEdgeTypeName()
	});


}

std::shared_ptr<std::vector<QString>> diagram::createNameVecLine()
{
	return std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		myconfig::getInstance().getPenColorName()
			, myconfig::getInstance().getPenWdithName()
			, myconfig::getInstance().getPenStyleName()
			, myconfig::getInstance().getBrushColorName()
			, myconfig::getInstance().getRotateAngleName()
			, myconfig::getInstance().getScaleName()
			, myconfig::getInstance().getSpaceWidthName()
			, myconfig::getInstance().getSpaceHeightName()
	});
}

std::shared_ptr<std::vector<QString>> diagram::createNamgeVecMouse()
{
	return std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		myconfig::getInstance().getPenColorName()
			, myconfig::getInstance().getPenWdithName()
			, myconfig::getInstance().getPenStyleName()
	});
}

std::shared_ptr<std::vector<QString>> diagram::createNameVecChoose()
{
	return std::make_shared<std::vector<QString>>();
}

std::shared_ptr<std::vector<QString>> diagram::createNameVecText()
{
	return std::make_shared<std::vector<QString>>(std::initializer_list<QString>{
		myconfig::getInstance().getPenColorName()
			, myconfig::getInstance().getFontFamilyName()
			, myconfig::getInstance().getFontSizeName()
	});
}

QString diagram::getName(ShapeType type)
{
	switch (type)
	{
	case ShapeType::Rect:
		return myconfig::getInstance().getRectName();
		break;
	case ShapeType::Circle:
		return myconfig::getInstance().getCircleName();
		break;
	case ShapeType::Triangle:
		return myconfig::getInstance().getTriangleName();
		break;
	case ShapeType::Line:
		return myconfig::getInstance().getLineName();
		break;
	case ShapeType::Mouse:
		return myconfig::getInstance().getMouseName();
		break;
	case ShapeType::choose:
		return myconfig::getInstance().getChooseName();
		break;
	case ShapeType::Text:
		return myconfig::getInstance().getTextName();
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

PropertyWidgetManager::propertyobjecttype diagram::shapetypeToPropertytype(ShapeType type)
{
	switch (type)
	{
	case ShapeType::Rect:
		return PropertyWidgetManager::propertyobjecttype::diagramRect;
		break;
	case ShapeType::Circle:
		return PropertyWidgetManager::propertyobjecttype::diagramCircle;
		break;
	case ShapeType::Triangle:
		return PropertyWidgetManager::propertyobjecttype::diagramTriangle;
		break;
	case ShapeType::Line:
		return PropertyWidgetManager::propertyobjecttype::diagramLine;
		break;
	case ShapeType::Mouse:
		return PropertyWidgetManager::propertyobjecttype::diagramMouse;
		break;
	case ShapeType::choose:
		return PropertyWidgetManager::propertyobjecttype::defaulttype;
		break;
	case ShapeType::Text:
		return PropertyWidgetManager::propertyobjecttype::diagramText;
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}



