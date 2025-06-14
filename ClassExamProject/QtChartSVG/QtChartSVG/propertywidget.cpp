#include <qfontdatabase.h>
#include "propertyset.h"
#include "propertydata.h"
#include "propertydelegage.h"
#include "myconfig.h"
#include "propertydelegateparams.h"
#include "propertywidget.h"


propertyWidget::propertyWidget(QWidget* parent)
	: QWidget(parent)
	, m_stackIndex(0)
{
	m_layout = new QFormLayout(this);
	m_layout->setAlignment(Qt::AlignLeft);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(0);
	setLayout(m_layout);

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

void propertyWidget::setStackIndex(int index)
{
	m_stackIndex = index;
}

int propertyWidget::getStackIndex()
{
	return m_stackIndex;
}

void propertyWidget::addPropertyDelegate(QString name, std::shared_ptr<IdelegatePramas> params)
{
	std::shared_ptr<IpropertyDelegate> delegate = createDelegate(params);

	if (delegate == nullptr)
		throw std::runtime_error("error");
	if (m_propertyDelegateMap.find(name) != m_propertyDelegateMap.end())
		throw std::runtime_error("error");

	QWidget* delegatewidget = delegate->getEditWidget();
	if (delegatewidget == nullptr)
		throw std::runtime_error("error");
	m_layout->addRow(name, delegatewidget);
	m_propertyDelegateMap[name] = delegate;
}

void propertyWidget::addShowingData(std::shared_ptr<propertydata> data)
{
	if (m_propertyDelegateMap.find(data->getName()) == m_propertyDelegateMap.end())
		throw std::runtime_error("error");

	m_propertyDelegateMap[data->getName()]->setData(data);
}

std::shared_ptr<IpropertyDelegate> propertyWidget::createDelegate(std::shared_ptr<IdelegatePramas> params)
{
	switch (params->m_type)
	{
	case myqtsvg::delegateType::Double:
		return std::make_shared<doubleDelegate>(params);
		break;
	case myqtsvg::delegateType::Int:
		return std::make_shared<intDelegate>(params);
		break;
	case myqtsvg::delegateType::Enum:
		return std::make_shared<enumDelegate>(params);
		break;
	case myqtsvg::delegateType::Color:
		return std::make_shared<colorDelete>(params);
		break;
	case myqtsvg::delegateType::String:
		return std::make_shared<stringDelegate>(params);
		break;
	case myqtsvg::delegateType::TriangleSides:
		return std::make_shared<triangleSideRadioDelegate>(params);
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

PropertyWidgetManager::PropertyWidgetManager(QWidget* parent)
{
	m_stackwidget = new QStackedWidget(parent);
	m_stackwidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_stackwidget->setContentsMargins(0, 0, 0, 0);
	m_stackwidget->setFrameShape(QFrame::NoFrame);
	m_stackwidget->setFrameShadow(QFrame::Plain);


	propertyWidgetManagerIniter::getInstance().initmanager(this);
	propertyWidget* defaultwidget = m_propertyMap[myqtsvg::propertyWidgetType::defaultWidget];
	m_stackwidget->setCurrentIndex(defaultwidget->getStackIndex());
}

QStackedWidget* PropertyWidgetManager::getStackwidget()
{
	return m_stackwidget;
}

void PropertyWidgetManager::onDealClicked(std::shared_ptr<propertySetManager> setmanager)
{
	if (m_propertyMap.find(setmanager->getWidgetType()) == m_propertyMap.end())
		throw std::runtime_error("error");
	auto widget = m_propertyMap[setmanager->getWidgetType()];
	int index = widget->getStackIndex();
	m_stackwidget->setCurrentIndex(index);

	setmanager->dealShowData(widget);
}


void PropertyWidgetManager::addPropertyWidget(myqtsvg::propertyWidgetType type, propertyWidget* widget)
{
	int index = m_stackwidget->addWidget(widget);
	widget->setStackIndex(index);

	m_propertyMap[type] = widget;
}



void propertyWidgetManagerIniter::initmanager(PropertyWidgetManager* widgetmanager)
{
	for (auto& it : m_map)
	{
		widgetmanager->addPropertyWidget(it.first, it.second());
	}
}

propertyWidgetManagerIniter& propertyWidgetManagerIniter::getInstance()
{
	static propertyWidgetManagerIniter instance;
	return instance;
}

void propertyWidgetManagerIniter::add(myqtsvg::propertyWidgetType type, std::function<propertyWidget* ()> func)
{
	m_map[type] = func;
}

propertyWidgetManagerIniter::propertyWidgetManagerIniter()
{
	defaultinit();
}

void propertyWidgetManagerIniter::defaultinit()
{
	m_map[myqtsvg::propertyWidgetType::canvas] = [this]() {
		return this->createCanvas();
		};
	m_map[myqtsvg::propertyWidgetType::defaultWidget] = [this]() {
		return this->createDefault();
		};

	m_map[myqtsvg::propertyWidgetType::diagramRect] = [this]() {
		return this->createDiagramRect();
		};

	m_map[myqtsvg::propertyWidgetType::diagramCircle] = [this]() {
		return this->createDiagramCircle();
		};
	m_map[myqtsvg::propertyWidgetType::diagramTriangle] = [this]() {
		return this->createDiagramTriangle();
		};
	m_map[myqtsvg::propertyWidgetType::diagramLine] = [this]() {
		return this->createDiagramLine();
		};
	m_map[myqtsvg::propertyWidgetType::diagramMouse] = [this]() {
		return this->createDiagramMouse();
		};
	m_map[myqtsvg::propertyWidgetType::diagramText] = [this]() {
		return this->createDiagramText();
		};
	m_map[myqtsvg::propertyWidgetType::diagramPentagon] = [this]() {
		return this->createDiagramPentagon();
		};
	m_map[myqtsvg::propertyWidgetType::diagramHexagon] = [this]() {
		return this->createDiagramHexagon();
		};
	m_map[myqtsvg::propertyWidgetType::diagramStar] = [this]() {
		return this->createDiagramStar();
		};
	m_map[myqtsvg::propertyWidgetType::canvasRect] = [this]() {
		auto widget = createDiagramRect();
		this->buildCenterMove(widget);
		return widget;
		};
	m_map[myqtsvg::propertyWidgetType::canvasCircle] = [this]() {
		auto widget = createDiagramCircle();
		this->buildCenterMove(widget);
		return widget;
		};
	m_map[myqtsvg::propertyWidgetType::canvasTriangle] = [this]() {
		auto widget = createDiagramTriangle();
		this->buildCenterMove(widget);
		return widget;
		};
	m_map[myqtsvg::propertyWidgetType::canvasLine] = [this]() {
		auto widget = createDiagramLine();
		this->buildCenterMove(widget);
		return widget;
		};
	m_map[myqtsvg::propertyWidgetType::canvasMouse] = [this]() {
		auto widget = createDiagramMouse();
		this->buildCenterMove(widget);
		return widget;
		};
	m_map[myqtsvg::propertyWidgetType::canvasText] = [this]() {
		auto widget = createDiagramText();
		this->buildCenterMove(widget);
		return widget;
		};
	m_map[myqtsvg::propertyWidgetType::canvasPentagon] = [this]() {
		auto widget = createDiagramPentagon();
		this->buildCenterMove(widget);
		return widget;
		};
	m_map[myqtsvg::propertyWidgetType::canvasHexagon] = [this]() {
		auto widget = createDiagramHexagon();
		this->buildCenterMove(widget);
		return widget;
		};
	m_map[myqtsvg::propertyWidgetType::canvasStar] = [this]() {
		auto widget = createDiagramStar();
		this->buildCenterMove(widget);
		return widget;
		};
}

propertyWidget* propertyWidgetManagerIniter::createDefault()
{
	auto widget = new propertyWidget();
	buildName(widget);
	return widget;
}

propertyWidget* propertyWidgetManagerIniter::createDiagramRect()
{
	auto widget = new propertyWidget();
	buildName(widget);
	buildRectRadio(widget);
	buildScale(widget);
	buildRotate(widget);
	buildSpacesize(widget);
	buildPen(widget);
	buildBrush(widget);
	return widget;
}

propertyWidget* propertyWidgetManagerIniter::createDiagramCircle()
{
	auto widget = new propertyWidget();
	buildName(widget);
	buildCircleRadio(widget);
	buildScale(widget);
	buildRotate(widget);
	buildSpacesize(widget);
	buildPen(widget);
	buildBrush(widget);
	return widget;
}

propertyWidget* propertyWidgetManagerIniter::createDiagramTriangle()
{
	auto widget = new propertyWidget();
	buildName(widget);
	buildScale(widget);
	buildTriangleRadios(widget);
	buildTriangleEdgeType(widget);
	buildRotate(widget);
	buildSpacesize(widget);
	buildPen(widget);
	buildBrush(widget);
	return widget;
}

propertyWidget* propertyWidgetManagerIniter::createDiagramLine()
{
	auto widget = new propertyWidget();
	buildName(widget);
	buildScale(widget);
	buildRotate(widget);
	buildSpacesize(widget);
	buildPen(widget);
	buildBrush(widget);
	return widget;
}

propertyWidget* propertyWidgetManagerIniter::createDiagramChoose()
{
	auto widget = new propertyWidget();
	buildName(widget);
	return widget;
}

propertyWidget* propertyWidgetManagerIniter::createDiagramMouse()
{
	auto widget = new propertyWidget();
	buildName(widget);
	buildPen(widget);
	return widget;
}

propertyWidget* propertyWidgetManagerIniter::createDiagramText()
{
	auto widget = new propertyWidget();
	buildName(widget);
	buildFont(widget);
	buildPenColor(widget);
	return widget;
}

propertyWidget* propertyWidgetManagerIniter::createDiagramPentagon()
{
	auto widget = new propertyWidget();
	buildName(widget);
	buildRotate(widget);
	buildScale(widget);
	buildSpacesize(widget);
	buildPen(widget);
	buildBrush(widget);
	return widget;
}

propertyWidget* propertyWidgetManagerIniter::createDiagramHexagon()
{
	auto widget = new propertyWidget();
	buildName(widget);
	buildRotate(widget);
	buildScale(widget);
	buildSpacesize(widget);
	buildPen(widget);
	buildBrush(widget);
	return widget;
}

propertyWidget* propertyWidgetManagerIniter::createDiagramStar()
{
	auto widget = new propertyWidget();
	buildName(widget);
	buildRotate(widget);
	buildScale(widget);
	buildSpacesize(widget);
	buildPen(widget);
	buildBrush(widget);
	return widget;

}

propertyWidget* propertyWidgetManagerIniter::createCanvas()
{
	auto widget = new propertyWidget();
	buildName(widget);
	buildCanvasSize(widget);
	buildCanvasScale(widget);
	return widget;
}

void propertyWidgetManagerIniter::buildName(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsString>(myconfig::getInstance().getDefaultName());
	widget->addPropertyDelegate(myconfig::getInstance().getNameName(), params);

}

void propertyWidgetManagerIniter::buildPen(propertyWidget* widget)
{
	auto& config = myconfig::getInstance();
	std::shared_ptr<IdelegatePramas> params = std::make_shared<delegateParamsColor>(config.getPenColor());
	widget->addPropertyDelegate(config.getPenColorName(), params);

	params = std::make_shared<delegateParamsInt>(config.getPenWidthMax(), 1, 1, config.getPenWidth());
	widget->addPropertyDelegate(config.getPenWdithName(), params);

	static QVector<QString> penstylelist{
		myqtsvg::PenStyleToQstring(Qt::PenStyle::SolidLine)
		, myqtsvg::PenStyleToQstring(Qt::PenStyle::DashLine)
		, myqtsvg::PenStyleToQstring(Qt::PenStyle::DotLine)
		, myqtsvg::PenStyleToQstring(Qt::PenStyle::DashDotLine)
		, myqtsvg::PenStyleToQstring(Qt::PenStyle::DashDotDotLine)
		, myqtsvg::PenStyleToQstring(Qt::PenStyle::CustomDashLine)
		, myqtsvg::PenStyleToQstring(Qt::PenStyle::MPenStyle)
	};
	params = std::make_shared<delegateParamsEnum>(penstylelist, config.getPenStyle());
	widget->addPropertyDelegate(config.getPenStyleName(), params);
}

void propertyWidgetManagerIniter::buildBrush(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsColor>(myconfig::getInstance().getBrushColor());
	widget->addPropertyDelegate(myconfig::getInstance().getBrushColorName(), params);
}

void propertyWidgetManagerIniter::buildCenterMove(propertyWidget* widget)
{
	int lengthmax = myconfig::getInstance().getCanvasLengthMax();
	auto params = std::make_shared<delegateParamsInt>(lengthmax, -lengthmax, 1, 0);
	widget->addPropertyDelegate(myconfig::getInstance().getCenterHOffsetName(), params);

	params = std::make_shared<delegateParamsInt>(lengthmax, -lengthmax, 1, 0);
	widget->addPropertyDelegate(myconfig::getInstance().getCenterVOffsetName(), params);
}

void propertyWidgetManagerIniter::buildScale(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsDouble>(myconfig::getInstance().getScaleMax(), 0.01, 0.01, 2, 1.00);
	widget->addPropertyDelegate(myconfig::getInstance().getScaleName(), params);
}

void propertyWidgetManagerIniter::buildRotate(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsInt>(180, -180, 1, 0);
	widget->addPropertyDelegate(myconfig::getInstance().getRotateAngleName(), params);
}

void propertyWidgetManagerIniter::buildSpacesize(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsInt>(myconfig::getInstance().getSpaceLengthMax(), 1, 1, myconfig::getInstance().getSpaceHeight());
	widget->addPropertyDelegate(myconfig::getInstance().getSpaceHeightName(), params);

	params = std::make_shared<delegateParamsInt>(myconfig::getInstance().getSpaceLengthMax(), 1, 1, myconfig::getInstance().getSpaceWidth());
	widget->addPropertyDelegate(myconfig::getInstance().getSpaceWidthName(), params);
}

void propertyWidgetManagerIniter::buildRectRadio(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsDouble>(myconfig::getInstance().getRectRadioMax(), myconfig::getInstance().getRectRadioMin(), 0.01, 2, myconfig::getInstance().getRectRadio());
	widget->addPropertyDelegate(myconfig::getInstance().getRectRadioName(), params);
}

void propertyWidgetManagerIniter::buildCircleRadio(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsDouble>(myconfig::getInstance().getCircleRadioMax(), myconfig::getInstance().getCircleRadioMin(), 0.01, 2, myconfig::getInstance().getCircleRadio());
	widget->addPropertyDelegate(myconfig::getInstance().getCircleRadioName(), params);
}

void propertyWidgetManagerIniter::buildTriangleRadios(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsTriangleSides>(myconfig::getInstance().getTriangleRadioBottomName()
		, myconfig::getInstance().getTriangleRadioLeftName()
		, myconfig::getInstance().getTriangleRadioRightName()
		, myconfig::getInstance().getTriangleRadioMax());
	widget->addPropertyDelegate(myconfig::getInstance().getTriangleRadioName(), params);
}

void propertyWidgetManagerIniter::buildTriangleEdgeType(propertyWidget* widget)
{
	static QVector<QString> edgetype{
		DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Bottom)
		, DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Left)
		, DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Right)
	};
	auto params = std::make_shared<delegateParamsEnum>(edgetype, myconfig::getInstance().getTriangleEdgetype());
	widget->addPropertyDelegate(myconfig::getInstance().getEdgeTypeName(), params);
}

void propertyWidgetManagerIniter::buildCanvasSize(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsInt>(myconfig::getInstance().getCanvasLengthMax(), 1, 1, myconfig::getInstance().getCanvasHeight());
	widget->addPropertyDelegate(myconfig::getInstance().getCanvasHeightName(), params);

	params = std::make_shared<delegateParamsInt>(myconfig::getInstance().getCanvasLengthMax(), 1, 1, myconfig::getInstance().getCanvasWidth());
	widget->addPropertyDelegate(myconfig::getInstance().getCanvasWidthName(), params);
}

void propertyWidgetManagerIniter::buildFont(propertyWidget* widget)
{
	static const QStringList fontFamilies = []() {
		QFontDatabase fontdatabase;
		return fontdatabase.families();
		}();
	std::shared_ptr<IdelegatePramas> params = std::make_shared<delegateParamsEnum>(QVector<QString>::fromList(fontFamilies), myconfig::getInstance().getTextFamily());
	widget->addPropertyDelegate(myconfig::getInstance().getFontFamilyName(), params);

	params = std::make_shared<delegateParamsInt>(myconfig::getInstance().getFontSizeMax(), 1, 1, myconfig::getInstance().getTextSize());
	widget->addPropertyDelegate(myconfig::getInstance().getFontSizeName(), params);
}

void propertyWidgetManagerIniter::buildPenColor(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsColor>(myconfig::getInstance().getPenColor());
	widget->addPropertyDelegate(myconfig::getInstance().getPenColorName(), params);
}

void propertyWidgetManagerIniter::buildCanvasScale(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsDouble>(myconfig::getInstance().getScaleMax(), 0.01, 0.01, 2, 1.00);
	widget->addPropertyDelegate(myconfig::getInstance().getCanvasScaleName(), params);
}

