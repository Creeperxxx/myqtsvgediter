#include "shuxingwidget.h"
#include "tuxing.h"

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
{
	m_shuxinglayout = new QFormLayout(this);
	m_shuxinglayout->setAlignment(Qt::AlignLeft);
	m_shuxinglayout->setContentsMargins(0, 0, 0, 0);
	m_shuxinglayout->setSpacing(0);
	setLayout(m_shuxinglayout);

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

}

void propertyWidget::addProperty(const QString& name, QWidget* widget)
{
	//propertyItem* item = new propertyItem(name, widget);
	m_shuxinglayout->addRow(name, widget);
	if (m_propertyMap.contains(name))
		throw std::runtime_error("error");
	m_propertyMap[name] = widget;
	//connect(item, &propertyItem::valueChanged, this, &propertyWidget::propertyChanged);
}
void propertyWidget::paintEvent(QPaintEvent* event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.fillRect(rect(), QColor(255, 255, 255));
}

void propertyWidget::setstackwidgetindex(int index)
{
	m_stackwidgetindex = index;
}

int propertyWidget::getstackwidgetindex()
{
	return m_stackwidgetindex;
}



//propertyItem::propertyItem(QString name, QVariant value, propertyType type, QWidget* parent)
//	:m_type(type)
//	, m_name(name)
//	, QWidget(parent)
//	, m_editWidget(nullptr)
//{
//	QHBoxLayout* layout = new QHBoxLayout(this);
//	layout->setContentsMargins(0, 0, 0, 0);
//
//	switch (type)
//	{
//	case propertyItem::propertyType::Text:
//		m_editWidget = createTextWidget();
//		break;
//	case propertyItem::propertyType::Color:
//		break;
//	case propertyItem::propertyType::File:
//		break;
//	case propertyItem::propertyType::Boolean:
//		break;
//	case propertyItem::propertyType::Enum:
//		break;
//	default:
//		break;
//	}
//
//	//switch (type)
//	//{
//	//case propertyItem::propertyType::String:
//	//{
//	//	m_editWidget = new QLineEdit(this);
//	//	dynamic_cast<QLineEdit*>(m_editWidget)->setText(value.toString());
//	//	//连接信号 文本更改发送信号
//	//	connect(dynamic_cast<QLineEdit*>(m_editWidget), &QLineEdit::textChanged, this, &propertyItem::onValueChanged);
//	//}
//	//break;
//	//case propertyItem::propertyType::Int:
//	//{
//	//	QSpinBox* spinbox = new QSpinBox(this);
//	//	spinbox->setValue(value.toInt());
//	//	m_editWidget = spinbox;
//	//	//连接信号 数值更改发送信号
//	//	connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged), this, &propertyItem::onValueChanged);
//	//}
//	//break;
//	//case propertyItem::propertyType::Double:
//	//{
//	//	QDoubleSpinBox* doubleSpinBox = new QDoubleSpinBox(this);
//	//	doubleSpinBox->setValue(value.toDouble());
//	//	m_editWidget = doubleSpinBox;
//	//	//连接信号 数值更改发送信号
//	//	connect(doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &propertyItem::onValueChanged);
//	//}
//	//break;
//	//case propertyItem::propertyType::Boolean:
//	//{
//	//	QComboBox* combo = new QComboBox(this);
//	//	combo->addItem("False", false);
//	//	combo->addItem("True", true);
//	//	combo->setCurrentIndex(value.toBool() ? 1 : 0);
//	//	m_editWidget = combo;
//	//	//连接信号 选择更改发送信号
//	//	connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &propertyItem::onValueChanged);
//	//}
//	//break;
//	//case propertyItem::propertyType::Color:
//	//{
//	//	throw std::runtime_error("error");
//	//}
//	//break;
//	//default:
//	//	throw std::runtime_error("error");
//	//}
//	//layout->addWidget(m_editWidget);
//}

QString PropertyWidgetManager::createPropertyWidget(propertyobjecttype type, QWidget* delegatewidget)
{
	auto widget = createinitPropertyWidget(type, delegatewidget);
	int index = m_propertyStackWidget->addWidget(widget);
	widget->setstackwidgetindex(index);

	QUuid uid = QUuid::createUuid();
	QString keystr = uid.toString();
	m_propertyMap[keystr] = widget;

	return keystr;
}

propertyWidget* PropertyWidgetManager::createinitPropertyWidget(propertyobjecttype type, QWidget* delegatewidget)
{
	propertyWidget* propertywidget = new propertyWidget();
	switch (type)
	{
	case PropertyWidgetManager::propertyobjecttype::diagramRect:
		builddiagramRectPropertyWidget(propertywidget,delegatewidget);
		break;
	case PropertyWidgetManager::propertyobjecttype::diagramCircle:
		break;
	case PropertyWidgetManager::propertyobjecttype::diagramTriangle:
		break;
	case PropertyWidgetManager::propertyobjecttype::diagramLine:
		break;
	case PropertyWidgetManager::propertyobjecttype::huabu:
		break;
	case PropertyWidgetManager::propertyobjecttype::huabuRect:
		break;
	case PropertyWidgetManager::propertyobjecttype::huabuCircle:
		break;
	case PropertyWidgetManager::propertyobjecttype::huabuTriangle:
		break;
	case PropertyWidgetManager::propertyobjecttype::huabuLine:
		break;
	default:
		break;
	}
	//return Widget;
	return propertywidget;
}

void PropertyWidgetManager::builddiagramRectPropertyWidget(propertyWidget* widget, QWidget* delegatewidget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	if (delegatewidget == nullptr)
		throw std::runtime_error("error");
	auto castDelegatewidget = dynamic_cast<DiagramItem*>(delegatewidget);
	if (castDelegatewidget == nullptr)
		throw std::runtime_error("error");

	//比例
	QDoubleSpinBox* radioSpinbox = new QDoubleSpinBox();
	radioSpinbox->setRange(rectRadioMin, rectRadioMax);
	radioSpinbox->setSingleStep(rectRadioStep);
	radioSpinbox->setDecimals(rectRadioDecimals);
	radioSpinbox->setValue(rectRadioInitvlaue);


	QObject::connect(radioSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
		, castDelegatewidget, &DiagramItem::onRectRadioChanged);

	widget->addProperty("比例", radioSpinbox);



	//widget->addProperty("比例", 1.0, propertyItem::propertyType::Double);

	//长和宽
	//widget->addProperty("宽度", 50.0, propertyItem::propertyType::Double);
	//widget->addProperty("高度", 25.0, propertyItem::propertyType::Double);

	//旋转角度
	QSpinBox* rotateSpinbox = new QSpinBox();
	rotateSpinbox->setRange(-180, 180);
	rotateSpinbox->setSingleStep(1);
	rotateSpinbox->setValue(0);

	QObject::connect(rotateSpinbox, QOverload<int>::of(&QSpinBox::valueChanged)
		, castDelegatewidget, &DiagramItem::onRectRotateChanged);

	//widget->addProperty("旋转角度", 0.0, propertyItem::propertyType::Double);

	//画笔颜色 宽度
	//widget->addProperty("画笔颜色", "#000000", propertyItem::propertyType::String);
	//widget->addProperty("画笔宽度", 2, propertyItem::propertyType::Int);

	//填充颜色
	//widget->addProperty("填充颜色", "#FFFFFF", propertyItem::propertyType::String);
}

void PropertyWidgetManager::builddiagramCirclePropertyWidget(propertyWidget* widget)
{
    //比例
	//widget->addProperty("比例", 1.0, propertyItem::propertyType::Double);

	//宽和高


	//旋转角度
	//widget->addProperty("旋转角度", 0.0, propertyItem::propertyType::Double);

	//画笔颜色宽度
	//widget->addProperty("画笔颜色", "#000000", propertyItem::propertyType::String);
	//widget->addProperty("画笔宽度", 2, propertyItem::propertyType::Int);

	//填充颜色
	//widget->addProperty("填充颜色", "#FFFFFF", propertyItem::propertyType::String);
}

void PropertyWidgetManager::builddiagramTrianglePropertyWidget(propertyWidget* widget)
{
	//三边比例
		//bottom
	//widget->addProperty("底边比例", 1.0, propertyItem::propertyType::Double);
		//left
	//widget->addProperty("左边比例", 1.0, propertyItem::propertyType::Double);
		//right
	//widget->addProperty("右边比例", 1.0, propertyItem::propertyType::Double);

	//指定某条边角度
	//widget->addProperty("指定边", );

	//rotate


	//画笔颜色 宽度

	//填充颜色

}


//propertyItem::propertyItem(const QString& name, QWidget* widget, QWidget* parent)
	//:m_name(name)
	//, m_editWidget(widget)
	//, QWidget(parent)
//{
	
//}
