#include "propertywidget.h"
#include "propertyset.h"
#include "propertydata.h"
#include "propertydelegage.h"
#include "myconfig.h"
#include <qfontdatabase.h>


propertyWidget::propertyWidget(QWidget* parent)
	: QWidget(parent)
	, m_stackwidgetindex(0)
{
	m_shuxinglayout = new QFormLayout(this);
	m_shuxinglayout->setAlignment(Qt::AlignLeft);
	m_shuxinglayout->setContentsMargins(0, 0, 0, 0);
	m_shuxinglayout->setSpacing(0);
	setLayout(m_shuxinglayout);

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

void propertyWidget::setstackwidgetindex(int index)
{
	m_stackwidgetindex = index;
}

int propertyWidget::getstackwidgetindex()
{
	return m_stackwidgetindex;
}

void propertyWidget::addPropertyItem(QString name, std::shared_ptr<IdelegatePramas> params)
{
	std::shared_ptr<IpropertyDelegate> delegate = createDelegate(params);

	if (delegate == nullptr)
		throw std::runtime_error("error");
	if (m_propertyDelegateMap.find(name) != m_propertyDelegateMap.end())
		throw std::runtime_error("error");

	QWidget* delegatewidget = delegate->getEditWidget();
	if (delegatewidget == nullptr)
		throw std::runtime_error("error");
	m_shuxinglayout->addRow(name, delegatewidget);
	m_propertyDelegateMap[name] = delegate;
}

void propertyWidget::addShowingData(std::shared_ptr<propertydata> data)
{
	if (m_propertyDelegateMap.find(data->m_name) == m_propertyDelegateMap.end())
		throw std::runtime_error("error");

	m_propertyDelegateMap[data->m_name]->setData(data);
}

std::shared_ptr<IpropertyDelegate> propertyWidget::createDelegate(std::shared_ptr<IdelegatePramas> params)
{
	switch (params->m_type)
	{
	case delegateType::Double:
		return std::make_shared<doubleDelegate>(params);
		break;
	case delegateType::Int:
		return std::make_shared<intDelegate>(params);
		break;
	case delegateType::Enum:
		return std::make_shared<enumDelegate>(params);
		break;
	case delegateType::Color:
		return std::make_shared<colorDelete>(params);
		break;
	case delegateType::String:
		return std::make_shared<stringDelegate>(params);
		break;
	case delegateType::TriangleSides:
		return std::make_shared<triangleSideRadioDelegate>(params);
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

PropertyWidgetManager::PropertyWidgetManager(QWidget* parent)
{
	m_propertyStackWidget = new QStackedWidget(parent);
	m_propertyStackWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_propertyStackWidget->setContentsMargins(0, 0, 0, 0);
	m_propertyStackWidget->setFrameShape(QFrame::NoFrame);
	m_propertyStackWidget->setFrameShadow(QFrame::Plain);

	createonceWidget();
	propertyWidget* defaultwidget = m_propertyMap[propertyobjecttype::defaulttype];
	m_propertyStackWidget->setCurrentIndex(defaultwidget->getstackwidgetindex());
}

QStackedWidget* PropertyWidgetManager::getstackwidget()
{
	return m_propertyStackWidget;
}

void PropertyWidgetManager::dealclicked(std::shared_ptr<propertySetManager> setmanager)
{
	if (m_propertyMap.find(setmanager->m_propertyWidgetType) == m_propertyMap.end())
		throw std::runtime_error("error");
	auto widget = m_propertyMap[setmanager->m_propertyWidgetType];
	int index = widget->getstackwidgetindex();
	m_propertyStackWidget->setCurrentIndex(index);

	setmanager->dealShowData(widget);
}

void PropertyWidgetManager::createonceWidget()
{
	propertyWidget* widget = createOriginalPropertyWidget();
	buildDefaultPropertyWidget(widget);
	addPropertyWidget(propertyobjecttype::defaulttype, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramRectPropertyWidget(widget);
	addPropertyWidget(propertyobjecttype::diagramRect, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramCirclePropertyWidget(widget);
	addPropertyWidget(propertyobjecttype::diagramCircle, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramTrianglePropertyWidget(widget);
	addPropertyWidget(propertyobjecttype::diagramTriangle, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramLinePropertyWidget(widget);
	addPropertyWidget(propertyobjecttype::diagramLine, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramMousePropertyWidget(widget);
	addPropertyWidget(propertyobjecttype::diagramMouse, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramTextPropertyWidget(widget);
	addPropertyWidget(propertyobjecttype::diagramText, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramHuabuPropertyWidget(widget);
	addPropertyWidget(propertyobjecttype::huabu, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramRectPropertyWidget(widget);
	buildPropertyWidgetCentermove(widget);
	addPropertyWidget(propertyobjecttype::huabuRect, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramCirclePropertyWidget(widget);
	buildPropertyWidgetCentermove(widget);
	addPropertyWidget(propertyobjecttype::huabuCircle, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramTrianglePropertyWidget(widget);
	buildPropertyWidgetCentermove(widget);
	addPropertyWidget(propertyobjecttype::huabuTriangle, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramLinePropertyWidget(widget);
	buildPropertyWidgetCentermove(widget);
	addPropertyWidget(propertyobjecttype::huabuLine, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramMousePropertyWidget(widget);
	buildPropertyWidgetCentermove(widget);
	addPropertyWidget(propertyobjecttype::huabuMouse, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramTextPropertyWidget(widget);
	buildPropertyWidgetCentermove(widget);
	addPropertyWidget(propertyobjecttype::huabuText, widget);
}

propertyWidget* PropertyWidgetManager::createOriginalPropertyWidget()
{
	propertyWidget* widget = new propertyWidget();
	int index = m_propertyStackWidget->addWidget(widget);
	widget->setstackwidgetindex(index);
	return widget;
}

void PropertyWidgetManager::buildDiagramRectPropertyWidget(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	buildPropertyWidgetName(widget);
	buildPropertyWidgetRectRadio(widget);
	buildPropertyWidgetRotate(widget);
	buildPropertyWidgetScale(widget);
	buildPropertyWidgetSpacesize(widget);
	buildPropertyWidgetPenAndBrush(widget);
}

void PropertyWidgetManager::buildDiagramCirclePropertyWidget(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");
	buildPropertyWidgetName(widget);
	buildProertyWidgetCircleRadio(widget);
	buildPropertyWidgetRotate(widget);
	buildPropertyWidgetScale(widget);
	buildPropertyWidgetSpacesize(widget);
	buildPropertyWidgetPenAndBrush(widget);
}

void PropertyWidgetManager::buildDiagramTrianglePropertyWidget(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	buildPropertyWidgetName(widget);
	buildPropertyWidgetTriangleSideRadio(widget);
	buildPropertyWidgetTriangleEdgetype(widget);
	buildPropertyWidgetRotate(widget);
	buildPropertyWidgetScale(widget);
	buildPropertyWidgetSpacesize(widget);
	buildPropertyWidgetPenAndBrush(widget);
}

void PropertyWidgetManager::buildDiagramLinePropertyWidget(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	buildPropertyWidgetName(widget);
	buildPropertyWidgetRotate(widget);
	buildPropertyWidgetScale(widget);
	buildPropertyWidgetSpacesize(widget);
	buildPropertyWidgetPenAndBrush(widget);
}

void PropertyWidgetManager::buildDiagramMousePropertyWidget(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	buildPropertyWidgetName(widget);
	buildPropertyWidgetPen(widget);
}

void PropertyWidgetManager::buildDiagramTextPropertyWidget(propertyWidget* widget)
{
	buildPropertyWidgetName(widget);
	std::shared_ptr<IdelegatePramas> params = std::make_shared<delegateParamsColor>(Qt::black);
	widget->addPropertyItem(myconfig::getInstance().getPenColorName()
		, params);

	QFontDatabase fontDatabase;
	QStringList fontFamilies = fontDatabase.families();
	params = std::make_shared<delegateParamsEnum>(QVector<QString>::fromList(fontFamilies), fontFamilies[0]);
	widget->addPropertyItem(myconfig::getInstance().getFontFamilyName()
		, params);

	params = std::make_shared<delegateParamsInt>(15, 1, 1, myconfig::getInstance().getTextSize());
	widget->addPropertyItem(myconfig::getInstance().getFontSizeName()
		, params);
}

void PropertyWidgetManager::buildDiagramHuabuPropertyWidget(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	buildPropertyWidgetName(widget);
	buildPropertyWidgetHuabuSize(widget);

	auto params = std::make_shared<delegateParamsDouble>(3.00, 0.01, 0.01, 2, 1);
	widget->addPropertyItem(myconfig::getInstance().getCanvasScaleName(), params);
}

void PropertyWidgetManager::buildDefaultPropertyWidget(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");
	buildPropertyWidgetName(widget);
}

void PropertyWidgetManager::addPropertyWidget(propertyobjecttype type, propertyWidget* widget)
{
	if (m_propertyMap.find(type) != m_propertyMap.end())
		throw std::runtime_error("error");

	m_propertyMap[type] = widget;
}


void PropertyWidgetManager::buildPropertyWidgetName(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsString>(myconfig::getInstance().getDefaultName());
	widget->addPropertyItem(myconfig::getInstance().getNameName(), params);
}

void PropertyWidgetManager::buildPropertyWidgetRectRadio(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsDouble>(myconfig::getInstance().getRectRadioMax()
		, myconfig::getInstance().getRectRadioMin()
		, myconfig::getInstance().getRectRadioStep()
		, 1
		, myconfig::getInstance().getRectRadio());

	widget->addPropertyItem(myconfig::getInstance().getRectRadioName()
		, params);
}

void PropertyWidgetManager::buildPropertyWidgetRotate(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsInt>(180, -180, 1, 0);
	widget->addPropertyItem(myconfig::getInstance().getRotateAngleName()
		, params);
}

void PropertyWidgetManager::buildProertyWidgetCircleRadio(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsDouble>(myconfig::getInstance().getCircleRadioMax()
		, myconfig::getInstance().getCircleRadioMin()
		, myconfig::getInstance().getCircleRadioStep()
		, 2
		, myconfig::getInstance().getCircleRadio());
	widget->addPropertyItem(myconfig::getInstance().getCircleRadioName()
		, params);
}

void PropertyWidgetManager::buildPropertyWidgetScale(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsDouble>(1.00, 0.01, 0.01, 2, 1.00);
	widget->addPropertyItem(myconfig::getInstance().getScaleName()
		, params);
}

void PropertyWidgetManager::buildPropertyWidgetTriangleSideRadio(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsTriangleSides>(myconfig::getInstance().getTriangleRadioBottomName()
		, myconfig::getInstance().getTriangleRadioLeftName()
		, myconfig::getInstance().getTriangleRadioRightName()
		, myconfig::getInstance().getTriangleRadioMax());
	widget->addPropertyItem(myconfig::getInstance().getTriangleRadioName()
		, params);
}

void PropertyWidgetManager::buildPropertyWidgetTriangleEdgetype(propertyWidget* widget)
{
	QVector<QString> list;
	list.append(DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Bottom));
	list.append(DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Left));
	list.append(DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Right));
	auto params = std::make_shared<delegateParamsEnum>(list, list[0]);
	widget->addPropertyItem(myconfig::getInstance().getEdgeTypeName()
		, params);
}

void PropertyWidgetManager::buildPropertyWidgetPenAndBrush(propertyWidget* widget)
{
	std::shared_ptr<IdelegatePramas> params = std::make_shared<delegateParamsColor>(myconfig::getInstance().getPenColor());
	widget->addPropertyItem(myconfig::getInstance().getPenColorName(), params);

	params = std::make_shared<delegateParamsInt>(myconfig::getInstance().getPenWidthMax()
		, 1
		, 1
		, myconfig::getInstance().getPenWidth());
	widget->addPropertyItem(myconfig::getInstance().getPenWdithName()
		, params);

	QVector<QString> vec;
	vec.push_back(PenStyleToQstring(Qt::PenStyle::SolidLine));
	vec.push_back(PenStyleToQstring(Qt::PenStyle::DashLine));
	vec.push_back(PenStyleToQstring(Qt::PenStyle::DotLine));
	vec.push_back(PenStyleToQstring(Qt::PenStyle::DashDotLine));
	vec.push_back(PenStyleToQstring(Qt::PenStyle::DashDotDotLine));
	vec.push_back(PenStyleToQstring(Qt::PenStyle::CustomDashLine));
	vec.push_back(PenStyleToQstring(Qt::PenStyle::MPenStyle));
	params = std::make_shared<delegateParamsEnum>(vec, myconfig::getInstance().getPenStyle());
	widget->addPropertyItem(myconfig::getInstance().getPenStyleName(), params);


	params = std::make_shared<delegateParamsColor>(myconfig::getInstance().getBrushColor());
	widget->addPropertyItem(myconfig::getInstance().getBrushColorName()
		, params);
}

void PropertyWidgetManager::buildPropertyWidgetSpacesize(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsInt>(myconfig::getInstance().getSpaceLengthMax(), 1, 1, myconfig::getInstance().getSpaceWidth());
	widget->addPropertyItem(myconfig::getInstance().getSpaceWidthName(), params);

	params = std::make_shared<delegateParamsInt>(myconfig::getInstance().getSpaceLengthMax(), 1, 1, myconfig::getInstance().getSpaceHeight());
	widget->addPropertyItem(myconfig::getInstance().getSpaceHeightName(), params);
}

void PropertyWidgetManager::buildPropertyWidgetHuabuSize(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsInt>(myconfig::getInstance().getCanvasLengthMax()
		, 1
		, 1
		, myconfig::getInstance().getCanvasWidth());
	widget->addPropertyItem(myconfig::getInstance().getCanvasWidthName()
		, params);

	params = std::make_shared<delegateParamsInt>(myconfig::getInstance().getCanvasHeight()
		, 1
		, 1
		, myconfig::getInstance().getCanvasHeight());
	widget->addPropertyItem(myconfig::getInstance().getCanvasHeightName()
		, params);
}

void PropertyWidgetManager::buildPropertyWidgetCentermove(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsInt>(myconfig::getInstance().getCanvasHeight(), 0, 1, 0);
	widget->addPropertyItem(myconfig::getInstance().getCenterVOffsetName()
		, params);

	params = std::make_shared<delegateParamsInt>(myconfig::getInstance().getCanvasWidth(), 0, 1, 0);
	widget->addPropertyItem(myconfig::getInstance().getCenterHOffsetName(), params);
}

void PropertyWidgetManager::buildPropertyWidgetPen(propertyWidget* widget)
{
	std::shared_ptr<IdelegatePramas> params = std::make_shared<delegateParamsColor>(myconfig::getInstance().getPenColor());
	widget->addPropertyItem(myconfig::getInstance().getPenColorName(), params);

	QVector<QString> vec;
	vec.push_back(PenStyleToQstring(Qt::PenStyle::SolidLine));
	vec.push_back(PenStyleToQstring(Qt::PenStyle::DashLine));
	vec.push_back(PenStyleToQstring(Qt::PenStyle::DotLine));
	vec.push_back(PenStyleToQstring(Qt::PenStyle::DashDotLine));
	vec.push_back(PenStyleToQstring(Qt::PenStyle::DashDotDotLine));
	vec.push_back(PenStyleToQstring(Qt::PenStyle::CustomDashLine));
	vec.push_back(PenStyleToQstring(Qt::PenStyle::MPenStyle));
	params = std::make_shared<delegateParamsEnum>(vec, myconfig::getInstance().getPenStyle());
	widget->addPropertyItem(myconfig::getInstance().getPenStyleName(), params);

	params = std::make_shared<delegateParamsInt>(myconfig::getInstance().getPenWidthMax(), 1, 1, myconfig::getInstance().getPenWidth());
	widget->addPropertyItem(myconfig::getInstance().getPenWdithName(), params);
}

