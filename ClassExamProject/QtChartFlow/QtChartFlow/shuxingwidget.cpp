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
{
	m_shuxinglayout = new QFormLayout(this);
	m_shuxinglayout->setAlignment(Qt::AlignLeft);
	m_shuxinglayout->setContentsMargins(0, 0, 0, 0);
	m_shuxinglayout->setSpacing(0);
	setLayout(m_shuxinglayout);

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

}

void propertyWidget::addPropertyItem(QString name, EditItemDataType type)
{
	std::shared_ptr<propertyItem> item = std::make_shared<propertyItem>(type);
	if (m_propertyItemMap.find(name) != m_propertyItemMap.end())
		throw std::runtime_error("error");
	m_propertyItemMap[name] = item;

	QWidget* editwidget = item->getEditWidget();
	m_shuxinglayout->addRow(name, editwidget);

}

//void propertyWidget::addProperty(const QString& name, QWidget* widget)
//{
//	//propertyItem* item = new propertyItem(name, widget);
//	m_shuxinglayout->addRow(name, widget);
//	if (m_propertyMap.contains(name))
//		throw std::runtime_error("error");
//	m_propertyMap[name] = widget;
//	//connect(item, &propertyItem::valueChanged, this, &propertyWidget::propertyChanged);
//}
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

void propertyWidget::addShowingData(std::shared_ptr<propertyData> data)
{
	if (data == nullptr)
		throw std::runtime_error("error");
	if (m_propertyItemMap.find(data->m_name) == m_propertyItemMap.end())
		throw std::runtime_error("error");

	m_propertyItemMap[data->m_name]->setData(data);
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

PropertyWidgetManager::PropertyWidgetManager(QWidget* parent)
{
	m_propertyStackWidget = new QStackedWidget(parent);
	m_propertyStackWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	m_propertyStackWidget->setContentsMargins(0, 0, 0, 0);
	m_propertyStackWidget->setStyleSheet("QStackedWidget{background-color: rgb(255, 255, 255);}");
	m_propertyStackWidget->setFrameShape(QFrame::NoFrame);
	m_propertyStackWidget->setFrameShadow(QFrame::Plain);
	m_propertyStackWidget->setLineWidth(0);
	m_propertyStackWidget->setMidLineWidth(0);
	m_propertyStackWidget->setAttribute(Qt::WA_TranslucentBackground, true);
	m_propertyStackWidget->setAttribute(Qt::WA_NoSystemBackground, true);
	m_propertyStackWidget->setAttribute(Qt::WA_OpaquePaintEvent, true);

	createonceWidget();
}

QStackedWidget* PropertyWidgetManager::getstackwidget()
{
	return m_propertyStackWidget;
}

void PropertyWidgetManager::dealclicked(propertyobjecttype type, std::vector<std::shared_ptr<propertyData>> data)
{
	if (!m_propertyMap.contains(type))
		throw std::runtime_error("error");
	propertyWidget* widget = m_propertyMap[type];
	
	for (const auto& dataptr : data)
	{
		widget->addShowingData(dataptr);
	}
}

//QString PropertyWidgetManager::createPropertyWidget(propertyobjecttype type, QWidget* delegatewidget)
//{
//	auto widget = createinitPropertyWidget(type, delegatewidget);
//	int index = m_propertyStackWidget->addWidget(widget);
//	widget->setstackwidgetindex(index);
//
//	QUuid uid = QUuid::createUuid();
//	QString keystr = uid.toString();
//	m_propertyMap[keystr] = widget;
//
//	DiagramItem* p = dynamic_cast<DiagramItem*>(delegatewidget);
//	if (p != nullptr)
//	{
//		p->setPropertyWidgetKey(keystr);
//	}
//
//	return keystr;
//}

//void PropertyWidgetManager::/*dealclicked*/(QString key)
//{
//	if (!m_propertyMap.contains(key))
//	{
//		return;
//	}
//	int index = m_propertyMap[key]->getstackwidgetindex();
//	if (index < 0)
//	{
//		return;
//	}
//	m_propertyStackWidget->setCurrentIndex(index);
//
//}

//propertyWidget* PropertyWidgetManager::createinitPropertyWidget(propertyobjecttype type, QWidget* delegatewidget)
//{
//	propertyWidget* propertywidget = new propertyWidget();
//	switch (type)
//	{
//	case PropertyWidgetManager::propertyobjecttype::diagramRect:
//		builddiagramRectPropertyWidget(propertywidget, delegatewidget);
//		break;
//	case PropertyWidgetManager::propertyobjecttype::diagramCircle:
//		builddiagramCirclePropertyWidget(propertywidget, delegatewidget);
//		break;
//	case PropertyWidgetManager::propertyobjecttype::diagramTriangle:
//		builddiagramTrianglePropertyWidget(propertywidget, delegatewidget);
//		break;
//	case PropertyWidgetManager::propertyobjecttype::diagramLine:
//		builddiagramLinePropertyWidget(propertywidget, delegatewidget);
//		break;
//	case PropertyWidgetManager::propertyobjecttype::huabu:
//		break;
//	case PropertyWidgetManager::propertyobjecttype::huabuRect:
//		break;
//	case PropertyWidgetManager::propertyobjecttype::huabuCircle:
//		break;
//	case PropertyWidgetManager::propertyobjecttype::huabuTriangle:
//		break;
//	case PropertyWidgetManager::propertyobjecttype::huabuLine:
//		break;
//	default:
//		break;
//	}
//	//return Widget;
//	return propertywidget;
//}


//void PropertyWidgetManager::builddiagramRectPropertyWidget(propertyWidget* widget, QWidget* delegatewidget)
//{
//	if (widget == nullptr)
//		throw std::runtime_error("error");
//
//	if (delegatewidget == nullptr)
//		throw std::runtime_error("error");
//	auto castDelegatewidget = dynamic_cast<DiagramItem*>(delegatewidget);
//	if (castDelegatewidget == nullptr)
//		throw std::runtime_error("error");
//
//	//名字
//	QString name = "diagram " + ShapeTypeTool::shapetypeEnumToQstring(castDelegatewidget->gettype());
//	QLabel* nameLabel = new QLabel(name);
//	widget->addProperty("名字", nameLabel);
//	//比例
//	QDoubleSpinBox* radioSpinbox = new QDoubleSpinBox();
//	radioSpinbox->setRange(rectRadioMin, rectRadioMax);
//	radioSpinbox->setSingleStep(rectRadioStep);
//	radioSpinbox->setDecimals(rectRadioDecimals);
//	radioSpinbox->setValue(rectRadioInitvlaue);
//
//
//	QObject::connect(radioSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
//		, castDelegatewidget, &DiagramItem::onRectRadioChanged);
//
//	widget->addProperty("比例", radioSpinbox);
//
//
//
//	//widget->addProperty("比例", 1.0, propertyItem::propertyType::Double);
//
//	//长和宽
//	//widget->addProperty("宽度", 50.0, propertyItem::propertyType::Double);
//	//widget->addProperty("高度", 25.0, propertyItem::propertyType::Double);
//
//	//旋转角度
//	QSpinBox* rotateSpinbox = new QSpinBox();
//	rotateSpinbox->setRange(-180, 180);
//	rotateSpinbox->setSingleStep(1);
//	rotateSpinbox->setValue(0);
//
//	QObject::connect(rotateSpinbox, QOverload<int>::of(&QSpinBox::valueChanged)
//		, castDelegatewidget, &DiagramItem::onRectRotateChanged);
//
//	widget->addProperty("旋转角度", rotateSpinbox);
//
//	//widget->addProperty("旋转角度", 0.0, propertyItem::propertyType::Double);
//
//	//画笔颜色 宽度
//	QPushButton* penColorButton = new QPushButton("选择");
//	QObject::connect(penColorButton, &QPushButton::clicked, [=]() {
//		QColor color = QColorDialog::getColor(Qt::black, widget, "选择画笔颜色");
//		if (color.isValid()) {
//			// 设置画笔颜色
//			// castDelegatewidget->setPenColor(color);
//			castDelegatewidget->onPenColorChanged(color);
//		}
//		});
//
//	widget->addProperty("画笔颜色", penColorButton);
//
//	QDoubleSpinBox* penWidthSpinbox = new QDoubleSpinBox();
//	penWidthSpinbox->setRange(0, penWidthMax);
//	penWidthSpinbox->setSingleStep(penWidthStep);
//	penWidthSpinbox->setValue(penWidth);
//
//	QObject::connect(penWidthSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
//		, castDelegatewidget, &DiagramItem::onPenWidthChanged);
//
//	widget->addProperty("画笔宽度", penWidthSpinbox);
//
//	QPushButton* fillColorButton = new QPushButton("选择");
//	QObject::connect(fillColorButton, &QPushButton::clicked, [=]() {
//		QColor color = QColorDialog::getColor(Qt::white, widget, "选择填充颜色");
//		if (color.isValid()) {
//			// 设置填充颜色
//			// castDelegatewidget->setFillColor(color);
//			castDelegatewidget->onPenBrushChanged(color);
//		}
//		});
//
//	widget->addProperty("填充颜色", fillColorButton);
//
//	//widget->addProperty("画笔颜色", "#000000", propertyItem::propertyType::String);
//	//widget->addProperty("画笔宽度", 2, propertyItem::propertyType::Int);
//
//	//填充颜色
//	//widget->addProperty("填充颜色", "#FFFFFF", propertyItem::propertyType::String);
//}
//
//void PropertyWidgetManager::builddiagramCirclePropertyWidget(propertyWidget* widget, QWidget* delegatewidget)
//{
//	if (widget == nullptr)
//		throw std::runtime_error("error");
//
//	if (delegatewidget == nullptr)
//		throw std::runtime_error("error");
//	auto castDelegatewidget = dynamic_cast<DiagramItem*>(delegatewidget);
//	if (castDelegatewidget == nullptr)
//		throw std::runtime_error("error");
//
//	QString name = "diagram " + ShapeTypeTool::shapetypeEnumToQstring(castDelegatewidget->gettype());
//	QLabel* nameLabel = new QLabel(name);
//	widget->addProperty("名字", nameLabel);
//	//比例
//	//widget->addProperty("比例", 1.0, propertyItem::propertyType::Double);
//	QDoubleSpinBox* radioSpinbox = new QDoubleSpinBox();
//	radioSpinbox->setRange(circleRadioMin, circleRadioMax);
//	radioSpinbox->setSingleStep(circleRadioStep);
//	radioSpinbox->setDecimals(circleRadioDecimals);
//	radioSpinbox->setValue(circleRadioInitvlaue);
//
//	widget->addProperty("比例", radioSpinbox);
//	QObject::connect(radioSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
//		, castDelegatewidget, &DiagramItem::onCircleRadioChanged);
//
//
//	//宽和高
//
//
//	//旋转角度
//	//widget->addProperty("旋转角度", 0.0, propertyItem::propertyType::Double);
//	QSpinBox* rotateSpinbox = new QSpinBox();
//	rotateSpinbox->setRange(-180, 180);
//	rotateSpinbox->setSingleStep(1);
//	rotateSpinbox->setValue(0);
//	QObject::connect(rotateSpinbox, QOverload<int>::of(&QSpinBox::valueChanged)
//		, castDelegatewidget, &DiagramItem::onCircleRotateChanged);
//	widget->addProperty("旋转角度", rotateSpinbox);
//
//	QPushButton* penColorButton = new QPushButton("选择");
//	QObject::connect(penColorButton, &QPushButton::clicked, [=]() {
//		QColor color = QColorDialog::getColor(Qt::black, widget, "选择画笔颜色");
//		if (color.isValid()) {
//			// 设置画笔颜色
//			// castDelegatewidget->setPenColor(color);
//			castDelegatewidget->onPenColorChanged(color);
//		}
//		});
//
//	widget->addProperty("画笔颜色", penColorButton);
//
//	QDoubleSpinBox* penWidthSpinbox = new QDoubleSpinBox();
//	penWidthSpinbox->setRange(0, penWidthMax);
//	penWidthSpinbox->setSingleStep(penWidthStep);
//	penWidthSpinbox->setValue(penWidth);
//
//	QObject::connect(penWidthSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
//		, castDelegatewidget, &DiagramItem::onPenWidthChanged);
//
//	widget->addProperty("画笔宽度", penWidthSpinbox);
//
//	QPushButton* fillColorButton = new QPushButton("选择");
//	QObject::connect(fillColorButton, &QPushButton::clicked, [=]() {
//		QColor color = QColorDialog::getColor(Qt::white, widget, "选择填充颜色");
//		if (color.isValid()) {
//			// 设置填充颜色
//			// castDelegatewidget->setFillColor(color);
//			castDelegatewidget->onPenBrushChanged(color);
//		}
//		});
//
//	widget->addProperty("填充颜色", fillColorButton);
//
//	//画笔颜色宽度
//	//widget->addProperty("画笔颜色", "#000000", propertyItem::propertyType::String);
//	//widget->addProperty("画笔宽度", 2, propertyItem::propertyType::Int);
//
//
//	//填充颜色
//	//widget->addProperty("填充颜色", "#FFFFFF", propertyItem::propertyType::String);
//}
//
//void PropertyWidgetManager::builddiagramTrianglePropertyWidget(propertyWidget* widget, QWidget* delegatewidget)
//{
//	if (widget == nullptr)
//		throw std::runtime_error("error");
//
//	if (delegatewidget == nullptr)
//		throw std::runtime_error("error");
//	auto castDelegatewidget = dynamic_cast<DiagramItem*>(delegatewidget);
//	if (castDelegatewidget == nullptr)
//		throw std::runtime_error("error");
//
//	QString name = "diagram " + ShapeTypeTool::shapetypeEnumToQstring(castDelegatewidget->gettype());
//	QLabel* nameLabel = new QLabel(name);
//	widget->addProperty("名字", nameLabel);
//	//三边比例
//		//bottom
//	//widget->addProperty("底边比例", 1.0, propertyItem::propertyType::Double);
//		//left
//	//widget->addProperty("左边比例", 1.0, propertyItem::propertyType::Double);
//		//right
//	//widget->addProperty("右边比例", 1.0, propertyItem::propertyType::Double);
//	QDoubleSpinBox* bottomSpinbox = new QDoubleSpinBox();
//	bottomSpinbox->setRange(triangleRadioMin, triangleRadioMax);
//	bottomSpinbox->setSingleStep(triangleRadioStep);
//	bottomSpinbox->setDecimals(2);
//	bottomSpinbox->setValue(1.0);
//
//	widget->addProperty("底边比例", bottomSpinbox);
//	QObject::connect(bottomSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
//		, castDelegatewidget, &DiagramItem::onTriangleSideRadioChangedBottom);
//
//	QDoubleSpinBox* leftSpinbox = new QDoubleSpinBox();
//	leftSpinbox->setRange(triangleRadioMin, triangleRadioMax);
//	leftSpinbox->setSingleStep(triangleRadioStep);
//	leftSpinbox->setDecimals(2);
//	leftSpinbox->setValue(1.0);
//	widget->addProperty("左边比例", leftSpinbox);
//	QObject::connect(leftSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
//		, castDelegatewidget, &DiagramItem::onTriangleSideRadioChangedLeft);
//
//	QDoubleSpinBox* rightSpinbox = new QDoubleSpinBox();
//	rightSpinbox->setRange(triangleRadioMin, triangleRadioMax);
//	rightSpinbox->setSingleStep(triangleRadioStep);
//	rightSpinbox->setDecimals(2);
//	rightSpinbox->setValue(1.0);
//	widget->addProperty("右边比例", rightSpinbox);
//	QObject::connect(rightSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
//		, castDelegatewidget, &DiagramItem::onTriangleSideRadioChangedRight);
//
//
//	//指定某条边角度
//	//widget->addProperty("指定边", );
//	QComboBox* edgeTypeComboBox = new QComboBox();
//	edgeTypeComboBox->addItem(DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Bottom), QVariant::fromValue<int>(static_cast<int>(DiagramDrawParamsTriangle::EdgeType::Bottom)));
//	edgeTypeComboBox->addItem(DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Left), QVariant::fromValue<int>(static_cast<int>(DiagramDrawParamsTriangle::EdgeType::Left)));
//	edgeTypeComboBox->addItem(DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Right), QVariant::fromValue<int>(static_cast<int>(DiagramDrawParamsTriangle::EdgeType::Right)));
//
//	QObject::connect(edgeTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged)
//		, [=]()
//		{
//			int index = edgeTypeComboBox->currentData().toInt();
//			DiagramDrawParamsTriangle::EdgeType edgetype = static_cast<DiagramDrawParamsTriangle::EdgeType>(index);
//			castDelegatewidget->onTriangleEdgeTypeChanged(edgetype);
//		});
//	widget->addProperty("指定边", edgeTypeComboBox);
//
//	//rotate
//	QSpinBox* rotateSpinbox = new QSpinBox();
//	rotateSpinbox->setRange(-180, 180);
//	rotateSpinbox->setSingleStep(1);
//	rotateSpinbox->setValue(0);
//	QObject::connect(rotateSpinbox, QOverload<int>::of(&QSpinBox::valueChanged)
//		, castDelegatewidget, &DiagramItem::onTriangleEdgeRotateChanged);
//	widget->addProperty("旋转角度", rotateSpinbox);
//
//	//画笔颜色 宽度
//	QPushButton* penColorButton = new QPushButton("选择");
//	QObject::connect(penColorButton, &QPushButton::clicked, [=]() {
//		QColor color = QColorDialog::getColor(Qt::black, widget, "选择画笔颜色");
//		if (color.isValid()) {
//			// 设置画笔颜色
//			// castDelegatewidget->setPenColor(color);
//			castDelegatewidget->onPenColorChanged(color);
//		}
//		});
//
//	widget->addProperty("画笔颜色", penColorButton);
//
//	QDoubleSpinBox* penWidthSpinbox = new QDoubleSpinBox();
//	penWidthSpinbox->setRange(0, penWidthMax);
//	penWidthSpinbox->setSingleStep(penWidthStep);
//	penWidthSpinbox->setValue(penWidth);
//
//	QObject::connect(penWidthSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
//		, castDelegatewidget, &DiagramItem::onPenWidthChanged);
//
//	widget->addProperty("画笔宽度", penWidthSpinbox);
//
//	QPushButton* fillColorButton = new QPushButton("选择");
//	QObject::connect(fillColorButton, &QPushButton::clicked, [=]() {
//		QColor color = QColorDialog::getColor(Qt::white, widget, "选择填充颜色");
//		if (color.isValid()) {
//			// 设置填充颜色
//			// castDelegatewidget->setFillColor(color);
//			castDelegatewidget->onPenBrushChanged(color);
//		}
//		});
//
//	widget->addProperty("填充颜色", fillColorButton);
//
//
//}
//
//void PropertyWidgetManager::builddiagramLinePropertyWidget(propertyWidget* widget, QWidget* delegatewidget)
//{
//	//rotate
//	if (widget == nullptr)
//		throw std::runtime_error("error");
//
//	if (delegatewidget == nullptr)
//		throw std::runtime_error("error");
//	auto castDelegatewidget = dynamic_cast<DiagramItem*>(delegatewidget);
//	if (castDelegatewidget == nullptr)
//		throw std::runtime_error("error");
//
//	QString name = "diagram " + ShapeTypeTool::shapetypeEnumToQstring(castDelegatewidget->gettype());
//	QLabel* nameLabel = new QLabel(name);
//	widget->addProperty("名字", nameLabel);
//
//	//旋转角度
//	QSpinBox* rotateSpinbox = new QSpinBox();
//	rotateSpinbox->setRange(-180, 180);
//	rotateSpinbox->setSingleStep(1);
//	rotateSpinbox->setValue(0);
//	QObject::connect(rotateSpinbox, QOverload<int>::of(&QSpinBox::valueChanged)
//		, castDelegatewidget, &DiagramItem::onLineRotateChanged);
//	widget->addProperty("旋转角度", rotateSpinbox);
//
//	//画笔颜色 宽度
//	QPushButton* penColorButton = new QPushButton("选择");
//	QObject::connect(penColorButton, &QPushButton::clicked, [=]() {
//		QColor color = QColorDialog::getColor(Qt::black, widget, "选择画笔颜色");
//		if (color.isValid()) {
//			// 设置画笔颜色
//			// castDelegatewidget->setPenColor(color);
//			castDelegatewidget->onPenColorChanged(color);
//		}
//		});
//
//	widget->addProperty("画笔颜色", penColorButton);
//
//	QDoubleSpinBox* penWidthSpinbox = new QDoubleSpinBox();
//	penWidthSpinbox->setRange(0, penWidthMax);
//	penWidthSpinbox->setSingleStep(penWidthStep);
//	penWidthSpinbox->setValue(penWidth);
//
//	QObject::connect(penWidthSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
//		, castDelegatewidget, &DiagramItem::onPenWidthChanged);
//
//	widget->addProperty("画笔宽度", penWidthSpinbox);
//
//	QPushButton* fillColorButton = new QPushButton("选择");
//	QObject::connect(fillColorButton, &QPushButton::clicked, [=]() {
//		QColor color = QColorDialog::getColor(Qt::white, widget, "选择填充颜色");
//		if (color.isValid()) {
//			// 设置填充颜色
//			// castDelegatewidget->setFillColor(color);
//			castDelegatewidget->onPenBrushChanged(color);
//		}
//		});
//
//	widget->addProperty("填充颜色", fillColorButton);
//
//}
//
//void PropertyWidgetManager::buildhuabuPropertyWidget(propertyWidget* widget, QWidget* delegatewidget)
//{
//
//}

//void PropertyWidgetManager::initStackWidget()
//{
//	auto widget = createOriginalPropertyWidget();
//}

void PropertyWidgetManager::createonceWidget()
{
	propertyWidget* widget = createOriginalPropertyWidget();
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
}

propertyWidget* PropertyWidgetManager::createOriginalPropertyWidget()
{
	propertyWidget* widget = new propertyWidget();
	int index = m_propertyStackWidget->addWidget(widget);
	widget->setstackwidgetindex(index);

}

void PropertyWidgetManager::buildDiagramRectPropertyWidget(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	buildPropertywidgetname(widget);
	widget->addPropertyItem(propertynameradio, EditItemDataType::Double);
	buildPropertywidgetRotate(widget);
	buildPropertywidgetPenandBrush(widget);
	
	
}

void PropertyWidgetManager::buildDiagramCirclePropertyWidget(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	buildPropertywidgetname(widget);
	widget->addPropertyItem(propertynameradio, EditItemDataType::Double);
	buildPropertywidgetRotate(widget);
	buildPropertywidgetPenandBrush(widget);

}

void PropertyWidgetManager::buildDiagramTrianglePropertyWidget(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	buildPropertywidgetname(widget);
	widget->addPropertyItem(propertynamebottomradio, EditItemDataType::Double);
	widget->addPropertyItem(propertynameleftradio, EditItemDataType::Double);
	widget->addPropertyItem(propertynamerightradio, EditItemDataType::Double);
	widget->addPropertyItem(propertynameedgetype, EditItemDataType::Enum);
	buildPropertywidgetRotate(widget);
	buildPropertywidgetPenandBrush(widget);

}

void PropertyWidgetManager::buildDiagramLinePropertyWidget(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	buildPropertywidgetname(widget);
	buildPropertywidgetRotate(widget);
	buildPropertywidgetPenandBrush(widget);
}

void PropertyWidgetManager::buildDiagramHuabuPropertyWidget(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	widget->addPropertyItem(propertynamename, EditItemDataType::String);
	widget->addPropertyItem(propertynamehuabuwidth, EditItemDataType::Int);
	widget->addPropertyItem(propertynamehuabuheight, EditItemDataType::Int);
}

void PropertyWidgetManager::buildPropertywidgetPenandBrush(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	widget->addPropertyItem(propertynamepencolor, EditItemDataType::Color);
	widget->addPropertyItem(propertynamepenwidth, EditItemDataType::Int);
	widget->addPropertyItem(propertynamebrush, EditItemDataType::Color);
}

void PropertyWidgetManager::buildPropertywidgetname(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	widget->addPropertyItem(propertynamename, EditItemDataType::String);
}

void PropertyWidgetManager::buildPropertywidgetRotate(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	widget->addPropertyItem(propertynamerotate, EditItemDataType::Int);
}

void PropertyWidgetManager::buildPropertywidgetScale(propertyWidget* widget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	widget->addPropertyItem(propertynamescale, EditItemDataType::Double);
}

void PropertyWidgetManager::addPropertyWidget(propertyobjecttype type, propertyWidget* widget)
{
	if (m_propertyMap.contains(type))
		throw std::runtime_error("error");

	m_propertyMap[type] = widget;
}

//void PropertyWidgetManager::createDiagramRectPropertyWidget()
//{
//	propertyWidget* widget = createOriginalPropertyWidget();
//	std::shared_ptr<propertyItemParams> params = std::make_shared<propertyItemParams>();
//
//	using type = propertyItemParams::EditItemDataType;
//	params->m_editDataType = propertyItemParams::EditItemDataType::String;
//	params->m_name = "对象";
//	widget->addPropertyItem(params.get());
//
//	params->m_editDataType = type::Double;
//	params->m_name = "比例";
//	params->m_doubleRangeMin = rectRadioMin;
//	params->m_doubleRangeMax = rectRadioMax;
//	params->m_doubleSingleStep = rectRadioStep;
//	params->m_doubleDecimals = rectRadioDecimals;
//	params->m_doubleDefaultValue = rectRadioInitvlaue;
//	widget->addPropertyItem(params.get());
//
//	params->m_editDataType = type::Int;
//	params->m_name = "旋转角度";
//	params->m_intRangeMin = -180;
//	params->m_intRangeMax = 180;
//	params->m_intSingleStep = 1;
//	params->m_intDefaultValue = 0;
//	widget->addPropertyItem(params.get());
//
//	params->m_editDataType = type::Color;
//	params->m_name = "画笔颜色";
//	widget->addPropertyItem(params.get());
//
//	params->m_editDataType = type::Int;
//	params->m_name = "画笔宽度";
//	params->m_intRangeMin = 1;
//	params->m_intRangeMax = penWidthMax;
//	params->m_intDefaultValue = penWidth;
//	params->m_intSingleStep = 1;
//	widget->addPropertyItem(params.get());
//
//	params->m_editDataType = type::Color;
//	params->m_name = "填充颜色";
//	widget->addPropertyItem(params.get());
//}



//propertyItem::propertyItem(const QString& name, QWidget* widget, QWidget* parent)
	//:m_name(name)
	//, m_editWidget(widget)
	//, QWidget(parent)
//{

//}

propertyItem::propertyItem(EditItemDataType type)
	:m_editWidget(nullptr)
	, m_type(type)
	//, m_name(data->m_name)
	//, m_choosedColor(nullptr)
	//, m_type(data->m_datatype)
{
	createWidgetByType(type);
}

QWidget* propertyItem::getEditWidget()
{
	return m_editWidget;
}

void propertyItem::setData(std::shared_ptr<propertyData> data)
{
	if (data == nullptr)
		throw std::runtime_error("error");
	if (data->m_datatype != m_type)
		throw std::runtime_error("error");
	if (data->m_params == nullptr)
		throw std::runtime_error("error");
	
	setDataByType(data->m_datatype, data->m_params.get());

	QObject::connect(this, &propertyItem::signalValueChanged
		, data.get(), &propertyData::slotValueChanged);
}

void propertyItem::onValueChanged()
{
	//获取值

	emit signalValueChanged(m_type, getValue());
}

//QString propertyItem::getName()
//{
//	return m_name;	
//}
//
//void propertyItem::onValueChanged()
//{
//	emit signalValueChanged(m_name, m_type, getValue());
//}

//void propertyItem::onSetValue(std::shared_ptr<propertyData> data)
//{
//	switch (m_type)
//	{
//	case propertyItemParams::EditItemDataType::Double:
//		
//		break;
//	case propertyItemParams::EditItemDataType::Int:
//		break;
//	case propertyItemParams::EditItemDataType::Enum:
//		break;
//	case propertyItemParams::EditItemDataType::Color:
//		break;
//	case propertyItemParams::EditItemDataType::String:
//		break;
//	default:
//		break;
//	}
//}

//void propertyItem::createItem(propertyItemParams* data)
//{
//	switch (data)
//	{
//	case propertyItemParams::EditItemDataType::Double:
//		buildDoubleItem(params);
//		break;
//	case propertyItemParams::EditItemDataType::Int:
//		buildIntItem(params);
//		break;
//	case propertyItemParams::EditItemDataType::Enum:
//		buildEnumItem(params);
//		break;
//	case propertyItemParams::EditItemDataType::Color:
//		buildColorItem(params);
//		break;
//	case propertyItemParams::EditItemDataType::String:
//		buildStringItem(params);
//		break;
//	default:
//		throw std::runtime_error("error");
//		break;
//	}
//}



void propertyItem::createWidgetByType(EditItemDataType type)
{
	switch (type)
	{
	case EditItemDataType::Double:
	{
		auto ptr = new QDoubleSpinBox();
		QObject::connect(ptr, &QDoubleSpinBox::valueChanged
			, this, &propertyItem::onValueChanged);
		m_editWidget = ptr;
	}
		break;
	case EditItemDataType::Int:
	{
		auto ptr = new QSpinBox();
		QObject::connect(ptr, &QSpinBox::valueChanged
			, this, &propertyItem::onValueChanged);
		m_editWidget = ptr;
	}
		break;
	case EditItemDataType::Enum:
	{
		auto ptr = new QComboBox();
		QObject::connect(ptr, &QComboBox::currentTextChanged
			, this, &propertyItem::onValueChanged);
		m_editWidget = ptr;
	}
		break;
	case EditItemDataType::Color:
	{
		auto ptr = new QPushButton(colortext);
		QObject::connect(ptr, &QPushButton::clicked, [=]()
			{
				QColor color = QColorDialog::getColor(Qt::white, ptr, colorchoosetitle);
				if (color.isValid())
				{
					ptr->setProperty(colorProperty, QVariant::fromValue(color));
					this->onValueChanged();
				}
			});
		m_editWidget = ptr;
	}
		break;
	case EditItemDataType::String:
	{
		auto ptr = new QLabel();
		m_editWidget = ptr;
	}
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

void propertyItem::setDataByType(EditItemDataType type, propertyItemParams* params)
{
	switch (type)
	{
	case EditItemDataType::Double:
		setDoubleData(params);
		break;
	case EditItemDataType::Int:
		setIntData(params);
		break;
	case EditItemDataType::Enum:
		setEnumData(params);
		break;
	case EditItemDataType::Color:
		setColorData(params);
		break;
	case EditItemDataType::String:
		setStringData(params);
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

void propertyItem::setDoubleData(propertyItemParams* params)
{
	if (params == nullptr)
		throw std::runtime_error("error");
	if (!params->isDoubleDataValid())
		throw std::runtime_error("error");
	if (m_editWidget == nullptr)
		throw std::runtime_error("error");

	QDoubleSpinBox* spinbox = dynamic_cast<QDoubleSpinBox*>(m_editWidget);
	if (spinbox == nullptr)
		throw std::runtime_error("error");

	spinbox->setRange(params->m_doubleRangeMin.value(), params->m_doubleRangeMax.value());
	spinbox->setSingleStep(params->m_doubleSingleStep.value());
	spinbox->setDecimals(params->m_doubleDecimals.value());
	spinbox->setValue(params->m_doubleDefaultValue.value());

}

void propertyItem::setIntData(propertyItemParams* params)
{
	if (params == nullptr)
		throw std::runtime_error("error");
	if (!params->isIntDataValid())
		throw std::runtime_error("error");
	if (m_editWidget == nullptr)
		throw std::runtime_error("error");

	auto spinbox = dynamic_cast<QSpinBox*>(m_editWidget);
	if (spinbox == nullptr)
		throw std::runtime_error("error");

	spinbox->setRange(params->m_intRangeMin.value(), params->m_intRangeMax.value());
	spinbox->setSingleStep(params->m_intSingleStep.value());
	spinbox->setValue(params->m_intDefaultValue.value());
	
}

void propertyItem::setColorData(propertyItemParams* params)
{

}

void propertyItem::setEnumData(propertyItemParams* params)
{
	if (params == nullptr)
		throw std::runtime_error("error");
	if (!params->isEnumDataValid())
		throw std::runtime_error("error");
	if (m_editWidget == nullptr)
		throw std::runtime_error("error");

	auto combobox = dynamic_cast<QComboBox*>(m_editWidget);
	if (combobox == nullptr)
		throw std::runtime_error("error");

	combobox->clear();
	for (const QString& str : params->m_enumStringVec.value())
	{
		combobox->addItem(str);
	}

}

void propertyItem::setStringData(propertyItemParams* params)
{
	if (params == nullptr)
		throw std::runtime_error("error");
	if (!params->isStringItemValid())
		throw std::runtime_error("error");
	if (m_editWidget == nullptr)
		throw std::runtime_error("error");

	auto label = dynamic_cast<QLabel*>(m_editWidget);
	if (label == nullptr)
		throw std::runtime_error("error");

	if (!params->isStringItemValid())
		throw std::runtime_error("error");
	label->setText(params->m_stringStr.value());

}

QVariant propertyItem::getValue()
{
	switch (m_type)
	{
	case EditItemDataType::Double:
	{
		auto ptr = dynamic_cast<QDoubleSpinBox*>(m_editWidget);
		if (ptr == nullptr)
			throw std::runtime_error("error");
		return QVariant(ptr->value());
	}
		break;
	case EditItemDataType::Int:
	{
		auto ptr = dynamic_cast<QSpinBox*>(m_editWidget);
		if (ptr == nullptr)
			throw std::runtime_error("error");
		return QVariant(ptr->value());
	}
		break;
	case EditItemDataType::Enum:
	{
		auto ptr = dynamic_cast<QComboBox*>(m_editWidget);
		if (ptr == nullptr)
			throw std::runtime_error("error");
		return QVariant(ptr->currentText());
	}
		break;
	case EditItemDataType::Color:
	{
		auto ptr = dynamic_cast<QPushButton*>(m_editWidget);
		if (ptr == nullptr)
			throw std::runtime_error("error");
		return ptr->property(colorProperty);
	}
		break;
	case EditItemDataType::String:
	{
		auto ptr = dynamic_cast<QLabel*>(m_editWidget);
		if (ptr == nullptr)
			throw std::runtime_error("error");
		return QVariant(ptr->text());
	}
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

//QVariant propertyItem::getValue()
//{
//	switch (m_type)
//	{
//	case propertyItemParams::EditItemDataType::Double:
//		return QVariant(dynamic_cast<QDoubleSpinBox*>(m_editWidget)->value());
//		break;
//	case propertyItemParams::EditItemDataType::Int:
//		return QVariant(dynamic_cast<QSpinBox*>(m_editWidget)->value());
//		break;
//	case propertyItemParams::EditItemDataType::Enum:
//		return QVariant(dynamic_cast<QComboBox*>(m_editWidget)->currentText());
//		break;
//	case propertyItemParams::EditItemDataType::Color:
//		return QVariant(m_choosedColor.value_or(Qt::white));
//		break;
//	case propertyItemParams::EditItemDataType::String:
//		return QVariant(dynamic_cast<QLabel*>(m_editWidget)->text());//好像不会被触发
//		break;
//	default:
//		throw std::runtime_error("error");
//		break;
//	}
//}

bool propertyItemParams::isDoubleDataValid()
{
	return m_doubleRangeMax.has_value() && m_doubleRangeMin.has_value()
		&& m_doubleSingleStep.has_value() && m_doubleDecimals.has_value()
		&& m_doubleDefaultValue.has_value();
}

bool propertyItemParams::isIntDataValid()
{
	return m_intRangeMax.has_value() && m_intDefaultValue.has_value()
		&& m_intRangeMin.has_value() && m_intSingleStep.has_value();
}

bool propertyItemParams::isEnumDataValid()
{
	return m_enumStringVec.has_value();
}

bool propertyItemParams::isStringItemValid()
{
	return m_stringStr.has_value();
}

void propertyData::slotValueChanged(EditItemDataType type, QVariant value)
{
	//objectkey
	if (m_objectkey.has_value())
	{
		emit signalValueChangedHuabuTuxing(m_objectkey.value(), m_name, type, value);
	}
	else
	{
		emit signalValueChanged(m_name, type, value);
	}
}
