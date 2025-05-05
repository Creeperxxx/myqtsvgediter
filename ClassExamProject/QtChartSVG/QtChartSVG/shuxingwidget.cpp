#include "shuxingwidget.h"
#include "tuxing.h"
#include "drawtool.h"

//void shuxingwidget::huabushuxing()
//{
//	QSpinBox* widthspinbox = new QSpinBox(this);
//	widthspinbox->setRange(200, 8000);
//	m_shuxinglayout->addRow("宽度：", widthspinbox);
//
//	QSpinBox* heightspinbox = new QSpinBox(this);
//	heightspinbox->setRange(200, 8000);
//	m_shuxinglayout->addRow("高度：", heightspinbox);
//
//}

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

void PropertyWidgetManager::dealclicked(propertyobjecttype type, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> data)
{
	if (!m_propertyMap.contains(type))
		throw std::runtime_error("error");
	if(data == nullptr)
		throw std::runtime_error("error");

	propertyWidget* widget = m_propertyMap[type];
	m_propertyStackWidget->setCurrentIndex(widget->getstackwidgetindex());

	for (const auto& dataptr : *data)
	{
		widget->addShowingData(dataptr);
	}
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
	buildDiagramHuabuPropertyWidget(widget);
	addPropertyWidget(propertyobjecttype::huabu, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramHuabuRectPropertyWidget(widget);
	addPropertyWidget(propertyobjecttype::huabuRect, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramHuabuCirclePropertyWidget(widget);
	addPropertyWidget(propertyobjecttype::huabuCircle, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramHuabuTrianglePropertyWidget(widget);
	addPropertyWidget(propertyobjecttype::huabuTriangle, widget);

	widget = createOriginalPropertyWidget();
	buildDiagramHuabuLinePropertyWidget(widget);
	addPropertyWidget(propertyobjecttype::huabuLine, widget);

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
	if(widget == nullptr)
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
	buildPropertyWidgetPenAndBrush(widget);
}

void PropertyWidgetManager::buildDiagramHuabuPropertyWidget(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	buildPropertyWidgetName(widget);
	buildPropertyWidgetHuabuSize(widget);

}

void PropertyWidgetManager::buildDiagramHuabuRectPropertyWidget(propertyWidget* widget)
{
	buildDiagramRectPropertyWidget(widget);
	buildPropertyWidgetCentermove(widget);
}

void PropertyWidgetManager::buildDiagramHuabuCirclePropertyWidget(propertyWidget* widget)
{
	buildDiagramCirclePropertyWidget(widget);
	buildPropertyWidgetCentermove(widget);
}

void PropertyWidgetManager::buildDiagramHuabuTrianglePropertyWidget(propertyWidget* widget)
{
	buildDiagramTrianglePropertyWidget(widget);
	buildPropertyWidgetCentermove(widget);
}

void PropertyWidgetManager::buildDiagramHuabuLinePropertyWidget(propertyWidget* widget)
{
	buildDiagramTrianglePropertyWidget(widget);
	buildPropertyWidgetCentermove(widget);
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
	widget->addPropertyItem(propertynamename, params);
}

void PropertyWidgetManager::buildPropertyWidgetRectRadio(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsDouble>(rectRadioMax, rectRadioMin, rectRadioStep, rectRadioDecimals, rectRadioInitvlaue);
	widget->addPropertyItem(propertynameradio, params);
}

void PropertyWidgetManager::buildPropertyWidgetRotate(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsInt>(180, -180, 1, 0);
	widget->addPropertyItem(propertynamerotate, params);
}

void PropertyWidgetManager::buildProertyWidgetCircleRadio(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsDouble>(circleRadioMax, circleRadioMin, circleRadioStep, circleRadioDecimals, circleRadioInitvlaue);
	widget->addPropertyItem(propertynameradio, params);
}

void PropertyWidgetManager::buildPropertyWidgetScale(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsDouble>(1.00, 0.01, 0.01, 2, 1.00);
	widget->addPropertyItem(propertynamescale, params);
}

void PropertyWidgetManager::buildPropertyWidgetTriangleSideRadio(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsDouble>(triangleRadioMax, triangleRadioMin, triangleRadioStep, triangleRadioDecimals, triangleInitBottomValue);
	widget->addPropertyItem(propertynamebottomradio, params);

	params = std::make_shared<delegateParamsDouble>(triangleRadioMax, triangleRadioMin, triangleRadioStep, triangleRadioDecimals, triangleInitLeftValue);
	widget->addPropertyItem(propertynameleftradio, params);

	params = std::make_shared<delegateParamsDouble>(triangleRadioMax, triangleRadioMin, triangleRadioStep, triangleRadioDecimals, triangleInitRightValue);
	widget->addPropertyItem(propertynamerightradio, params);
}

void PropertyWidgetManager::buildPropertyWidgetTriangleEdgetype(propertyWidget* widget)
{
	QVector<QString> list;
	list.append(DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Bottom));
	list.append(DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Left));
	list.append(DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Right));
	auto params = std::make_shared<delegateParamsEnum>(list);
	widget->addPropertyItem(propertynameedgetype, params);
}

void PropertyWidgetManager::buildPropertyWidgetPenAndBrush(propertyWidget* widget)
{
	std::shared_ptr<IdelegatePramas> params = std::make_shared<delegateParamsColor>(propertyInitColor);
	widget->addPropertyItem(propertynamepencolor, params);

	params = std::make_shared<delegateParamsInt>(penWidthMax, 1, 1, penWidth);
	widget->addPropertyItem(propertynamepenwidth, params);

	params = std::make_shared<delegateParamsColor>(propertyInitColor);
	widget->addPropertyItem(propertynamebrush, params);
}

void PropertyWidgetManager::buildPropertyWidgetSpacesize(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsInt>(tuxingspacewidthMax, tuxingspacewidthMin, 1, tuxingspacewidthvalue);
	widget->addPropertyItem(propertynametuxingspacewidth, params);

	params = std::make_shared<delegateParamsInt>(tuxingspaceheightMax, tuxingspaceheightMin, 1, tuxingspaceheightvalue);
	widget->addPropertyItem(propertynametuxingspaceheight, params);
}

void PropertyWidgetManager::buildPropertyWidgetHuabuSize(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsInt>(huabuwidthMax, huabuwidthMin, 1, huabuwidthvalue);
	widget->addPropertyItem(propertynamehuabuwidth, params);

	params = std::make_shared<delegateParamsInt>(huabuheightMax, huabuheightMin, 1, huabuheightvalue);
	widget->addPropertyItem(propertynamehuabuheight, params);
}

void PropertyWidgetManager::buildPropertyWidgetCentermove(propertyWidget* widget)
{
	auto params = std::make_shared<delegateParamsInt>(centervmax, centervmin, 1, 0);
	widget->addPropertyItem(propertynamevmove, params);
	
	params = std::make_shared<delegateParamsInt>(centerhmax, centerhmin, 1, 0);
	widget->addPropertyItem(propertynamehmove, params);
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
	m_editwidget->setValue(data->m_data.toDouble());

	QObject::connect(this, &IpropertyDelegate::signalValueChanged, data.get(), &propertydata::slotValueChanged);
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

	m_editwidget->setRange(p->m_valuemin,p->m_valuemax);
	m_editwidget->setSingleStep(p->m_valuestep);
	m_editwidget->setDecimals(p->m_valuedecimals);
	m_editwidget->setValue(p->m_initvalue);

	QObject::connect(m_editwidget,static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &IpropertyDelegate::slotValueChanged);
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





intDelegate::~intDelegate()
{
}

intDelegate::intDelegate(std::shared_ptr<IdelegatePramas> params)
	:m_editwidget(nullptr)
{
	createWidget(params);
}

void intDelegate::setData(std::shared_ptr<propertydata> data)
{
	if (data == nullptr)
		throw std::runtime_error("error");

	if (!data->m_data.canConvert<int>())
		throw std::runtime_error("error");
	m_editwidget->setValue(data->m_data.toInt());

	QObject::connect(this, &IpropertyDelegate::signalValueChanged, data.get(), &propertydata::slotValueChanged);
}

QWidget* intDelegate::getEditWidget()
{
	return m_editwidget;
}

void intDelegate::createWidget(std::shared_ptr<IdelegatePramas> params)
{
	auto p = dynamic_cast<delegateParamsInt*>(params.get());
	if(p == nullptr )
		throw std::runtime_error("error");

	m_editwidget = new QSpinBox();
	m_editwidget->setRange(p->m_valuemin,p->m_valuemax);
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

void colorDelete::setData(std::shared_ptr<propertydata> data)
{
	if(!data->m_data.canConvert<QColor>())
		throw std::runtime_error("error");
		
	m_currentcolor = data->m_data.value<QColor>();
    m_colorlabel->setStyleSheet(QString("QLabel { background-color: rgba(%1,%2,%3,%4);border: 1px solid black; }")
		.arg(m_currentcolor.red()).arg(m_currentcolor.green())
		.arg(m_currentcolor.blue()).arg(m_currentcolor.alpha()));
	QObject::connect(this, &IpropertyDelegate::signalValueChanged, data.get(), &propertydata::slotValueChanged);
}

QWidget* colorDelete::getEditWidget()
{
	return m_colorwidget;
}

void colorDelete::createWidget(std::shared_ptr<IdelegatePramas> params)
{
	auto p = dynamic_cast<delegateParamsColor*>(params.get());
	if(p == nullptr) 
		throw std::runtime_error("error");

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
	,IdelegatePramas(delegateType::Color)
{
}

delegateParamsEnum::~delegateParamsEnum()
{
}

delegateParamsEnum::delegateParamsEnum(QVector<QString> vec)
	:m_vec(vec)
	, IdelegatePramas(delegateType::Enum)
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

void stringDelegate::setData(std::shared_ptr<propertydata> data)
{
	if(data == nullptr)
		throw std::runtime_error("error");
	if(!data->m_data.canConvert<QString>())
		throw std::runtime_error("error");
	m_label->setText(data->m_data.toString());
	QObject::connect(this, &IpropertyDelegate::signalValueChanged, data.get(), &propertydata::slotValueChanged);
}

QWidget* stringDelegate::getEditWidget()
{
	return m_label;
}


void stringDelegate::createWidget(std::shared_ptr<IdelegatePramas> params)
{
	if(params == nullptr) 
		throw std::runtime_error("error");
	auto p = dynamic_cast<delegateParamsString*>(params.get());
	if (p == nullptr)
		throw std::runtime_error("error");
    m_label = new QLabel(p->m_initstring);
	//信号绑定？
}

QVariant stringDelegate::value()
{
	return QVariant::fromValue(m_label->text());
}

enumDelegate::~enumDelegate()
{
}

enumDelegate::enumDelegate(std::shared_ptr<IdelegatePramas> params)
	:m_combobox(nullptr)
{
	createWidget(params);
}

void enumDelegate::setData(std::shared_ptr<propertydata> data)
{
	if(data == nullptr)
		throw std::runtime_error("error");

	if (!data->m_data.canConvert<QString>())
		throw std::runtime_error("error");
	int index = m_combobox->findText(data->m_data.toString());
	if(index == -1)
		throw std::runtime_error("error");

	m_combobox->setCurrentIndex(index);
	QObject::connect(this, &IpropertyDelegate::signalValueChanged, data.get(), &propertydata::slotValueChanged);
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
	if(p == nullptr)
		throw std::runtime_error("error");

    m_combobox = new QComboBox();
	for(const auto& str : p->m_vec)
	{
		m_combobox->addItem(str);
	}	
	int index = m_combobox->findText(p->m_initstring);
	if(index == -1)
		m_combobox->setCurrentIndex(0);
	else
        m_combobox->setCurrentIndex(index);
	
    QObject::connect(m_combobox, &QComboBox::currentTextChanged, this, &IpropertyDelegate::slotValueChanged);
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
