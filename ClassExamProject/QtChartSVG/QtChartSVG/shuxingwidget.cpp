#include "shuxingwidget.h"
#include "tuxing.h"
#include "drawtool.h"
#include "propertydelegage.h"



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


void propertyWidget::setstackwidgetindex(int index)
{
	m_stackwidgetindex = index;
}

void propertyWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	//painter.fillRect(rect(), Qt::white);
}


int propertyWidget::getstackwidgetindex()
{
	return m_stackwidgetindex;
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
	default:
		throw std::runtime_error("error");
		break;
	}
}




PropertyWidgetManager::PropertyWidgetManager(QWidget* parent)
{
	m_propertyStackWidget = new QStackedWidget(parent);
	m_propertyStackWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	//m_propertyStackWidget->setContentsMargins(0, 0, 0, 0);
	//m_propertyStackWidget->setStyleSheet("QStackedWidget{background-color: rgb(255, 255, 255);}");
	//m_propertyStackWidget->setFrameShape(QFrame::NoFrame);
	//m_propertyStackWidget->setFrameShadow(QFrame::Plain);
	//m_propertyStackWidget->setLineWidth(0);
	//m_propertyStackWidget->setMidLineWidth(0);
	//m_propertyStackWidget->setAttribute(Qt::WA_TranslucentBackground, true);
	//m_propertyStackWidget->setAttribute(Qt::WA_NoSystemBackground, true);
	//m_propertyStackWidget->setAttribute(Qt::WA_OpaquePaintEvent, true);

	createonceWidget();
	propertyWidget* defaultwidget = m_propertyMap[propertyobjecttype::defaulttype];
	m_propertyStackWidget->setCurrentIndex(defaultwidget->getstackwidgetindex());
}

QStackedWidget* PropertyWidgetManager::getstackwidget()
{
	return m_propertyStackWidget;
}

//void PropertyWidgetManager::dealclicked(propertyobjecttype type, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> data)
//{
//	if (!m_propertyMap.contains(type))
//		throw std::runtime_error("error");
//	if(data == nullptr)
//		throw std::runtime_error("error");
//
//	propertyWidget* widget = m_propertyMap[type];
//	m_propertyStackWidget->setCurrentIndex(widget->getstackwidgetindex());
//
//	for (const auto& dataptr : *data)
//	{
//		widget->addShowingData(dataptr);
//	}
//}

void PropertyWidgetManager::dealclicked(std::shared_ptr<propertySetManager> setmanager)
{
	if (m_propertyMap.find(setmanager->m_propertyObjectType) == m_propertyMap.end())
		throw std::runtime_error("error");
	auto widget = m_propertyMap[setmanager->m_propertyObjectType];
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
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::colorname)), params);

	QVector<QString> vec;
	vec.push_back("Arial");
	vec.push_back("Times New Roman");
	vec.push_back("Courier New");
	params = std::make_shared<delegateParamsEnum>(vec);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::text::familyname)), params);

	params = std::make_shared<delegateParamsInt>(15, 1, 1, 12);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::text::sizename)), params);
}


void PropertyWidgetManager::buildDiagramHuabuPropertyWidget(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	buildPropertyWidgetName(widget);
	buildPropertyWidgetHuabuSize(widget);

}


void PropertyWidgetManager::buildDefaultPropertyWidget(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");
	buildPropertyWidgetName(widget);
}



void PropertyWidgetManager::addPropertyWidget(propertyobjecttype type, propertyWidget* widget)
{
	if (m_propertyMap.contains(type))
		throw std::runtime_error("error");

	m_propertyMap[type] = widget;
}

void PropertyWidgetManager::buildPropertyWidgetName(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsString>(defaultname);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::namename)), params);
}

void PropertyWidgetManager::buildPropertyWidgetRectRadio(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsDouble>(rectRadioMax, rectRadioMin, rectRadioStep, rectRadioDecimals, rectRadioInitvlaue);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::rectangle::radioname)), params);
}

void PropertyWidgetManager::buildPropertyWidgetRotate(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsInt>(180, -180, 1, 0);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::rotatename)), params);
}

void PropertyWidgetManager::buildProertyWidgetCircleRadio(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsDouble>(circleRadioMax, circleRadioMin, circleRadioStep, circleRadioDecimals, circleRadioInitvlaue);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::circle::radioname)), params);
}

void PropertyWidgetManager::buildPropertyWidgetScale(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsDouble>(1.00, 0.01, 0.01, 2, 1.00);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::scalename)), params);
}

void PropertyWidgetManager::buildPropertyWidgetTriangleSideRadio(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsTriangleSides>(trianglebottom, triangleleft, triangleright, triangleRadiomax);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::triangle::edgeradio::radioname)), params);
}

void PropertyWidgetManager::buildPropertyWidgetTriangleEdgetype(propertyWidget* widget)
{
	QVector<QString> list;
	list.append(DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Bottom));
	list.append(DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Left));
	list.append(DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Right));
	auto params = std::make_shared<delegateParamsEnum>(list);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::triangle::edgetypename)), params);
}

void PropertyWidgetManager::buildPropertyWidgetPenAndBrush(propertyWidget* widget)
{
	std::shared_ptr<IdelegatePramas> params = std::make_shared<delegateParamsColor>(propertyInitColor);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::colorname)), params);

	params = std::make_shared<delegateParamsInt>(penWidthMax, 1, 1, penWidth);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::widthname)), params);

	params = std::make_shared<delegateParamsColor>(propertyInitColor);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::brushcolorname)), params);
}

void PropertyWidgetManager::buildPropertyWidgetSpacesize(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsInt>(tuxingspacewidthMax, tuxingspacewidthMin, 1, tuxingspacewidthvalue);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::widthname)), params);

	params = std::make_shared<delegateParamsInt>(tuxingspaceheightMax, tuxingspaceheightMin, 1, tuxingspaceheightvalue);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::heightname)), params);
}

void PropertyWidgetManager::buildPropertyWidgetHuabuSize(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsInt>(huabuwidthMax, huabuwidthMin, 1, huabuwidthvalue);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::widthname)), params);

	params = std::make_shared<delegateParamsInt>(huabuheightMax, huabuheightMin, 1, huabuheightvalue);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::heightname)), params);
}

void PropertyWidgetManager::buildPropertyWidgetCentermove(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsInt>(centervmax, centervmin, 1, 0);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centervoffsetname)), params);

	params = std::make_shared<delegateParamsInt>(centerhmax, centerhmin, 1, 0);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centerhoffsetname)), params);
}

void PropertyWidgetManager::buildPropertyWidgetPen(propertyWidget* widget)
{
	std::shared_ptr<IdelegatePramas> params = std::make_shared<delegateParamsColor>(propertyInitColor);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::colorname)), params);

	params = std::make_shared<delegateParamsInt>(penWidthMax, 1, 1, penWidth);
	widget->addPropertyItem(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::widthname)), params);
}



propertydata::propertydata(QString name, QVariant data)
	: m_name(name)
	, m_data(data)
{
}

void propertydata::slotValueChanged(QVariant value)
{
	m_data = value;
	emit signalValueChanged(value);
}



doubleDelegate::~doubleDelegate()
{
}

doubleDelegate::doubleDelegate(std::shared_ptr<IdelegatePramas> params)
	: m_editwidget(nullptr)
{
	createWidget(params);
}



void doubleDelegate::setData(std::shared_ptr<propertydata> data)
{
	if (!data->m_data.canConvert<double>())
		throw std::runtime_error("error");
	if (m_olddata != nullptr && m_olddata.get() != nullptr)
	{
		QObject::disconnect(this, &IpropertyDelegate::signalValueChanged, m_olddata.get(), &propertydata::slotValueChanged);
	}
	m_editwidget->setValue(data->m_data.toDouble());

	QObject::connect(this, &IpropertyDelegate::signalValueChanged, data.get(), &propertydata::slotValueChanged);
	m_olddata = data;

}


QWidget* doubleDelegate::getEditWidget()
{
	return m_editwidget;
}

void doubleDelegate::createWidget(std::shared_ptr<IdelegatePramas> params)
{
	delegateParamsDouble* p = dynamic_cast<delegateParamsDouble*>(params.get());

	m_editwidget = new QDoubleSpinBox();
	if (m_editwidget == nullptr)
		throw std::runtime_error("error");

	m_editwidget->setRange(p->m_valuemin, p->m_valuemax);
	m_editwidget->setSingleStep(p->m_valuestep);
	m_editwidget->setDecimals(p->m_valuedecimals);
	m_editwidget->setValue(p->m_initvalue);

	QObject::connect(m_editwidget, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &IpropertyDelegate::slotValueChanged);
}

QVariant doubleDelegate::value()
{
	if (m_editwidget == nullptr)
		throw std::runtime_error("error");

	return QVariant::fromValue(m_editwidget->value());
}

IpropertyDelegate::~IpropertyDelegate()
{
}


void IpropertyDelegate::slotValueChanged()
{
	emit signalValueChanged(value());
}

void IpropertyDelegate::isDataValid()
{
}





intDelegate::~intDelegate()
{
}

intDelegate::intDelegate(std::shared_ptr<IdelegatePramas> params)
	:m_editwidget(nullptr)
{
	createWidget(params);
}




QWidget* intDelegate::getEditWidget()
{
	return m_editwidget;
}

void intDelegate::createWidget(std::shared_ptr<IdelegatePramas> params)
{
	auto p = dynamic_cast<delegateParamsInt*>(params.get());
	if (p == nullptr)
		throw std::runtime_error("error");

	m_editwidget = new QSpinBox();
	m_editwidget->setRange(p->m_valuemin, p->m_valuemax);
	m_editwidget->setSingleStep(p->m_valuestep);
	m_editwidget->setValue(p->m_initvalue);

	QObject::connect(m_editwidget, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IpropertyDelegate::slotValueChanged);
}

QVariant intDelegate::value()
{
	if (m_editwidget == nullptr)
		throw std::runtime_error("error");
	return QVariant::fromValue(m_editwidget->value());
}

colorDelete::~colorDelete()
{
}

colorDelete::colorDelete(std::shared_ptr<IdelegatePramas> params)
	:m_button(nullptr)
	, m_colorwidget(nullptr)
	, m_colorlayout(nullptr)
	, m_colorlabel(nullptr)
	, m_currentcolor(QColor(Qt::white))
{
	createWidget(params);
}




QWidget* colorDelete::getEditWidget()
{
	return m_colorwidget;
}

void colorDelete::createWidget(std::shared_ptr<IdelegatePramas> params)
{
	auto p = isParamsCastValid<delegateParamsColor>(params);

	m_colorwidget = new QWidget();
	m_colorlayout = new QHBoxLayout();
	m_colorwidget->setLayout(m_colorlayout);

	m_button = new QPushButton(propertyColorText, m_colorwidget);

	m_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_colorlabel = new QLabel(m_colorwidget);
	m_currentcolor = p->m_initcolor;
	m_colorlabel->setStyleSheet(QString("QLabel { background-color: rgba(%1,%2,%3,%4);border: 1px solid black; }")
		.arg(m_currentcolor.red()).arg(m_currentcolor.green())
		.arg(m_currentcolor.blue()).arg(m_currentcolor.alpha()));
	m_colorlabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_colorlayout->addWidget(m_colorlabel);
	m_colorlayout->addWidget(m_button);

	QObject::connect(m_button, &QPushButton::clicked, [=]()
		{
			QColor color = QColorDialog::getColor(Qt::white, m_button, propertyColorDialogTitle);
			if (color.isValid())
			{
				m_currentcolor = color;
				m_colorlabel->setStyleSheet(QString("QLabel { background-color: rgba(%1,%2,%3,%4);border: 1px solid black; }")
					.arg(m_currentcolor.red()).arg(m_currentcolor.green())
					.arg(m_currentcolor.blue()).arg(m_currentcolor.alpha()));
				this->slotValueChanged();
			}
		});
}

void colorDelete::setValue(QVariant value)
{
	m_currentcolor = isDataCanConvert<QColor>(value);
	m_colorlabel->setStyleSheet(QString("QLabel { background-color: rgba(%1,%2,%3,%4);border: 1px solid black; }")
		.arg(m_currentcolor.red()).arg(m_currentcolor.green())
		.arg(m_currentcolor.blue()).arg(m_currentcolor.alpha()));
}

QVariant colorDelete::value()
{
	return QVariant::fromValue(m_currentcolor);
}


delegateParamsDouble::~delegateParamsDouble()
{
}

delegateParamsDouble::delegateParamsDouble(double max, double min, double step, int decimals, double initvalue)
	:m_valuemax(max)
	, m_valuemin(min)
	, m_valuestep(step)
	, m_valuedecimals(decimals)
	, m_initvalue(initvalue)
	, IdelegatePramas(delegateType::Double)
{
}

delegateParamsInt::~delegateParamsInt()
{
}

delegateParamsInt::delegateParamsInt(int max, int min, int step, int initvalue)
	:m_valuemax(max)
	, m_valuemin(min)
	, m_valuestep(step)
	, m_initvalue(initvalue)
	, IdelegatePramas(delegateType::Int)
{
}

delegateParamsColor::~delegateParamsColor()
{
}

delegateParamsColor::delegateParamsColor(QColor initcolor)
	:m_initcolor(initcolor)
	, IdelegatePramas(delegateType::Color)
{
}

delegateParamsEnum::~delegateParamsEnum()
{
}

delegateParamsEnum::delegateParamsEnum(std::vector<QString> vec, QString initstr)
	: IdelegatePramas(delegateType::Enum)
	, m_vec(vec)
	, m_initstring(initstr)
{
}

delegateParamsString::~delegateParamsString()
{
}

delegateParamsString::delegateParamsString(QString str)
	:m_initstring(str)
	, IdelegatePramas(delegateType::String)
{
}

stringDelegate::~stringDelegate()
{
}

stringDelegate::stringDelegate(std::shared_ptr<IdelegatePramas> params)
	: m_label(nullptr)
{
	createWidget(params);
}




QWidget* stringDelegate::getEditWidget()
{
	return m_label;
}


void stringDelegate::createWidget(std::shared_ptr<IdelegatePramas> params)
{
	auto p = isParamsCastValid<delegateParamsString>(params);
	m_label = new QLabel(p->m_initstring);
}

void stringDelegate::setValue(QVariant value)
{
	m_label->setText(isDataCanConvert<QString>(value));
}

QVariant stringDelegate::value()
{
	return QVariant::fromValue(m_label->text());
}


enumDelegate::enumDelegate(std::shared_ptr<IdelegatePramas> params)
	:m_combobox(nullptr)
{
	createWidget(params);
}




QWidget* enumDelegate::getEditWidget()
{
	return m_combobox;
}

void enumDelegate::createWidget(std::shared_ptr<IdelegatePramas> params)
{
	if (params == nullptr)
		throw std::runtime_error("error");
	auto p = dynamic_cast<delegateParamsEnum*>(params.get());
	if (p == nullptr)
		throw std::runtime_error("error");

	m_combobox = new QComboBox();
	for (const auto& str : p->m_vec)
	{
		m_combobox->addItem(str);
	}
	int index = m_combobox->findText(p->m_initstring);
	if (index == -1)
		m_combobox->setCurrentIndex(0);
	else
		m_combobox->setCurrentIndex(index);

	QObject::connect(m_combobox, &QComboBox::currentTextChanged, this, &IpropertyDelegate::slotValueChanged);
}

void enumDelegate::setValue(QVariant value)
{
	int index = m_combobox->findText(isDataCanConvert<QString>(value));
	if (index == -1)
		throw std::runtime_error("error");
	m_combobox->setCurrentIndex(index);
}

QVariant enumDelegate::value()
{
	return QVariant::fromValue(m_combobox->currentText());
}

IdelegatePramas::IdelegatePramas(delegateType type)
	:m_type(type)
{
}

IdelegatePramas::~IdelegatePramas()
{
}








drawParamsPropertySet::~drawParamsPropertySet()
{
}

void drawParamsPropertySet::onPenColorChanged(QVariant value)
{
	if (!value.canConvert<QColor>())
		throw std::runtime_error("error");
	m_params->m_pen.setColor(value.value<QColor>());
	m_params->m_paramChanged = true;
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onPenWidthChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_params->m_pen.setWidth(value.value<int>());
	m_params->m_paramChanged = true;
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onBrushColorChanged(QVariant value)
{
	if (!value.canConvert<QColor>())
		throw std::runtime_error("error");
	m_params->m_brush.setColor(value.value<QColor>());
	m_params->m_paramChanged = true;
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onRotateChanged(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	m_params->m_rotate = value.value<double>();
	m_params->m_paramChanged = true;
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onSpacewidthChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_params->m_spacesize.setWidth(value.value<int>());
	m_params->m_paramChanged = true;
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onSpaceHeightChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_params->m_spacesize.setHeight(value.value<int>());
	m_params->m_paramChanged = true;
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onScaleChanged(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	m_params->m_scale = value.value<double>();
	m_params->m_paramChanged = true;
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onCenterHOffset(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_params->m_centerHoffset = value.value<int>();
	m_params->m_paramChanged = true;
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onCenterVOffset(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_params->m_centerVoffset = value.value<int>();
	m_params->m_paramChanged = true;
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onRectRadioChanged(QVariant value)
{
	if (m_params->m_type != ShapeType::Rect)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsRect>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	castparams->m_boundingrectradio = value.value<double>();
	m_params->m_paramChanged = true;
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onCricleRadioChanged(QVariant value)
{
	if (m_params->m_type != ShapeType::Circle)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsCircle>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	castparams->m_boundingrectradio = value.value<double>();
	castparams->m_paramChanged = true;
	emit SignalValueChangedByData();

}

void drawParamsPropertySet::onTriangleRadioChanged(QVariant value)
{
	if (m_params->m_type != ShapeType::Triangle)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	if (!value.canConvert<DiagramDrawParamsTriangle::TriangleSizeRadios>())
		throw std::runtime_error("error");
	castparams->m_triangleSizeRadios = value.value<DiagramDrawParamsTriangle::TriangleSizeRadios>();
	castparams->m_paramChanged = true;
	emit SignalValueChangedByData();
}


void drawParamsPropertySet::onTriangleEdgetypeRadioChanged(QVariant value)
{
	if (m_params->m_type != ShapeType::Triangle)
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	if (!value.canConvert<QString>())
		throw std::runtime_error("error");
	castparams->m_edgetype = DiagramDrawParamsTriangle::edgetypeStringToEnum(value.value<QString>());
	castparams->m_paramChanged = true;
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onTextFamilyChanged(QVariant value)
{
	if (!value.canConvert<QString>())
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsText>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	castparams->m_font.setFamily(value.value<QString>());
	emit SignalValueChangedByData();
}

void drawParamsPropertySet::onTextSizeChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsText>(m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	castparams->m_font.setPointSize(value.value<int>());
	emit SignalValueChangedByData();

}


void IDrawParamsPropertyDataBuilder::build(std::shared_ptr<IpropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	if (set == nullptr || set.get() == nullptr
		|| datavec == nullptr || datavec.get() == nullptr)
		throw std::runtime_error("error");
	auto castset = std::dynamic_pointer_cast<drawParamsPropertySet>(set);
	if (castset == nullptr)
		throw std::runtime_error("error");

	probuild(castset, datavec);
}

void PenColorDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::colorname)), QVariant(set->m_params->m_pen.color()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onPenColorChanged);
}

void PenWidthDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::widthname)), QVariant::fromValue(set->m_params->m_pen.width()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onPenWidthChanged);
}

void BrushDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::brushcolorname)), QVariant::fromValue(set->m_params->m_brush));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onBrushColorChanged);
}

void RotateDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::rotatename)), QVariant::fromValue(set->m_params->m_rotate));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onRotateChanged);
}

void SpacewidthDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::widthname)), set->m_params->m_spacesize.width());
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onSpacewidthChanged);
}

std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> propertyDataVecOfPropertySetCreator::create(std::shared_ptr<IpropertySet> set)
{
	auto datavec = std::make_shared<std::vector<std::shared_ptr<propertydata>>>();

	for (auto& builder : m_builders)
	{
		builder->build(set, datavec);
	}
	return datavec;
}

void propertyDataVecOfPropertySetCreator::addBuilder(std::shared_ptr<IpropertyDataBuilder> builder)
{
	if (builder == nullptr || builder.get() == nullptr)
		throw std::runtime_error("error");
	m_builders.push_back(builder);
}

propertyDataVecOfPropertySetCreatorFactor& propertyDataVecOfPropertySetCreatorFactor::getInstance()
{
	static propertyDataVecOfPropertySetCreatorFactor instance;
	return instance;
}

std::shared_ptr<propertyDataVecOfPropertySetCreator> propertyDataVecOfPropertySetCreatorFactor::create(std::shared_ptr<std::vector<QString>> propertynamevec)
{
	if (propertynamevec == nullptr || propertynamevec.get() == nullptr)
		throw std::runtime_error("error");

	std::shared_ptr<propertyDataVecOfPropertySetCreator> creator = std::make_shared<propertyDataVecOfPropertySetCreator>();
	for (const auto& name : *propertynamevec)
	{
		if (m_builderCreatefunc.find(name) != m_builderCreatefunc.end())
		{
			creator->addBuilder(m_builderCreatefunc[name]());
		}
		else
		{
			throw std::runtime_error("error");
		}
	}
	return creator;
}

propertyDataVecOfPropertySetCreatorFactor& propertyDataVecOfPropertySetCreatorFactor::addCreator(QString name, std::function<std::shared_ptr<IpropertyDataBuilder>()> func)
{
	if (m_builderCreatefunc.find(name) != m_builderCreatefunc.end())
	{
		throw std::runtime_error("error");
	}
	else
	{
		m_builderCreatefunc[name] = func;
	}
	return *this;
}

propertyDataVecOfPropertySetCreatorFactor::propertyDataVecOfPropertySetCreatorFactor()
{
}

void SpaceheightDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::spacesize::heightname)), QVariant::fromValue(set->m_params->m_spacesize.height()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onSpaceHeightChanged);
}

void ScaleDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::scalename)), QVariant::fromValue(set->m_params->m_scale));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onScaleChanged);
}

void CenterHoffsetDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centerhoffsetname)), QVariant(set->m_params->m_centerHoffset));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onCenterHOffset);
}

void CenterVoffsetDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::centervoffsetname)), QVariant::fromValue(set->m_params->m_centerVoffset));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onCenterVOffset);
}



IpropertySet::~IpropertySet()
{
}

void IpropertySet::addShowData(propertyWidget* widget)
{
	for (auto& data : *m_propertyDataVec)
	{
		widget->addShowingData(data);
	}
}

void propertySetManager::addPropertySet(QString name, std::shared_ptr<IpropertySet> set)
{
	if (m_propertySetMap.find(name) != m_propertySetMap.end())
		throw std::runtime_error("error");
	m_propertySetMap[name] = set;
}

std::shared_ptr<IpropertySet> propertySetManager::getPropertySet(QString name)
{
	if (m_propertySetMap.find(name) == m_propertySetMap.end())
		throw std::runtime_error("error");
	return m_propertySetMap[name];
}

void propertySetManager::dealShowData(propertyWidget* widget)
{
	for (auto& data : m_propertySetMap)
	{
		data.second->addShowData(widget);
	}
}


otherPropertySet::~otherPropertySet()
{
}

void otherPropertySet::onHuabuHeightChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_huabuwidth = value.value<int>();
	emit signalHuabuWidthChanged(m_huabuwidth);
}

void otherPropertySet::onHuabuWidthChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_huabuheight = value.value<int>();
	emit signalHuabuHeightChanged(m_huabuheight);
}

void IOtherPropertyDataBuilder::build(std::shared_ptr<IpropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	if (set == nullptr || set.get() == nullptr || datavec == nullptr || datavec.get() == nullptr)
		throw std::runtime_error("error");
	auto castset = std::dynamic_pointer_cast<otherPropertySet>(set);
	if (castset == nullptr)
		throw std::runtime_error("error");
	probuild(castset, datavec);
}

void NamePropertyDataBuilder::probuild(std::shared_ptr<otherPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::namename)), QVariant::fromValue(set->m_name));
	datavec->push_back(data);

}

void RectRadioDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsRect>(set->m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::rectangle::radioname)), QVariant::fromValue(castparams->m_boundingrectradio));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onRectRadioChanged);
}



void CircleRadioDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsCircle>(set->m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::circle::radioname)), QVariant::fromValue(castparams->m_boundingrectradio));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onCricleRadioChanged);
}

void TriangleRadioDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(set->m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::triangle::edgeradio::radioname)), QVariant::fromValue(castparams->m_triangleSizeRadios));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onTriangleRadioChanged);
}


void TriangleEdgetypeDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(set->m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	QString edgetype = DiagramDrawParamsTriangle::edgetypeEnumToString(castparams->m_edgetype);
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::triangle::edgetypename)), QVariant::fromValue(edgetype));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onTriangleEdgetypeRadioChanged);
}


triangleSideRadioDelegate::triangleSideRadioDelegate(std::shared_ptr<IdelegatePramas> params)
	:m_radiowidget(nullptr)
	, m_widget(nullptr)
	, m_bottombox(nullptr)
	, m_leftbox(nullptr)
	, m_rightbox(nullptr)
	, m_formlayout(nullptr)
	, m_button(nullptr)
	, m_vlayout(nullptr)
{
	createWidget(params);
}

QWidget* triangleSideRadioDelegate::getEditWidget()
{
	return m_widget;
}

void triangleSideRadioDelegate::onisValid()
{
	int bottom = m_bottombox->value();
	int left = m_leftbox->value();
	int right = m_rightbox->value();
	if ((bottom + left <= right) || (bottom + right <= left) || (left + right <= bottom))
	{
		QMessageBox::warning(m_button, "warning", invalidTriangleRadios);
		return;
	}
	else
	{
		slotValueChanged();
	}
}


void triangleSideRadioDelegate::createWidget(std::shared_ptr<IdelegatePramas> params)
{
	auto castparams = isParamsCastValid<delegateParamsTriangleSides>(params);

	m_radiowidget = new QWidget();
	m_formlayout = new QFormLayout(m_radiowidget);
	m_radiowidget->setLayout(m_formlayout);

	m_bottombox = new QSpinBox();
	m_bottombox->setRange(0, castparams->m_Radiomax);
	m_bottombox->setValue(castparams->m_radios.m_bottom);

	m_leftbox = new QSpinBox();
	m_leftbox->setRange(0, castparams->m_Radiomax);
	m_leftbox->setValue(castparams->m_radios.m_left);

	m_rightbox = new QSpinBox();
	m_rightbox->setRange(0, castparams->m_Radiomax);
	m_rightbox->setValue(castparams->m_radios.m_right);

	m_formlayout->addRow(castparams->m_bottomstr, m_bottombox);
	m_formlayout->addRow(castparams->m_leftstr, m_leftbox);
	m_formlayout->addRow(castparams->m_rightstr, m_rightbox);

	m_button = new QPushButton("确认");
	QObject::connect(m_button, &QPushButton::clicked, this, &triangleSideRadioDelegate::onisValid);



	m_widget = new QWidget();
	m_vlayout = new QVBoxLayout();
	m_widget->setLayout(m_vlayout);

	m_vlayout->addWidget(m_radiowidget);
	m_vlayout->addWidget(m_button);
}

void triangleSideRadioDelegate::setValue(QVariant value)
{
	auto radios = isDataCanConvert<DiagramDrawParamsTriangle::TriangleSizeRadios>(value);
	m_bottombox->setValue(radios.m_bottom);
	m_leftbox->setValue(radios.m_left);
	m_rightbox->setValue(radios.m_right);
}

QVariant triangleSideRadioDelegate::value()
{
	DiagramDrawParamsTriangle::TriangleSizeRadios radios;
	radios.m_bottom = m_bottombox->value();
	radios.m_left = m_leftbox->value();
	radios.m_right = m_rightbox->value();
	QVariant value = QVariant::fromValue(radios);
	return value;
}


delegateParamsTriangleSides::delegateParamsTriangleSides(QString bottomstr, QString leftstr, QString rightstr, int radiomax)
	:IdelegatePramas(delegateType::TriangleSides)
	, m_bottomstr(bottomstr)
	, m_leftstr(leftstr)
	, m_rightstr(rightstr)
	, m_Radiomax(radiomax)
{

}

delegateParamsTriangleSides::~delegateParamsTriangleSides()
{
}

void TextFontFamilyDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsText>(set->m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	auto data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::text::familyname)), QVariant::fromValue(castparams->m_font.family()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onTextFamilyChanged);
}

void TextFontSizeDrawParamsPropertyDataBuilder::probuild(std::shared_ptr<drawParamsPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto castparams = std::dynamic_pointer_cast<DiagramDrawParamsText>(set->m_params);
	if (castparams == nullptr)
		throw std::runtime_error("error");
	auto data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::text::sizename)), QVariant::fromValue(castparams->m_font.pointSize()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &drawParamsPropertySet::onTextSizeChanged);
}

void HuabuHeightPropertyDataBuilder::probuild(std::shared_ptr<otherPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::heightname)), QVariant::fromValue(set->m_huabuheight));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &otherPropertySet::onHuabuHeightChanged);
}

void HuabuWidthPropertyDataBuilder::probuild(std::shared_ptr<otherPropertySet> set, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto data = std::make_shared<propertydata>(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::widthname)), QVariant::fromValue(set->m_huabuwidth));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, set.get(), &otherPropertySet::onHuabuWidthChanged);
}
