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

}

QStackedWidget* PropertyWidgetManager::getstackwidget()
{
	return m_propertyStackWidget;
}

QString PropertyWidgetManager::createPropertyWidget(propertyobjecttype type, QWidget* delegatewidget)
{
	auto widget = createinitPropertyWidget(type, delegatewidget);
	int index = m_propertyStackWidget->addWidget(widget);
	widget->setstackwidgetindex(index);

	QUuid uid = QUuid::createUuid();
	QString keystr = uid.toString();
	m_propertyMap[keystr] = widget;

	DiagramItem* p = dynamic_cast<DiagramItem*>(delegatewidget);
	if (p != nullptr)
	{
		p->setPropertyWidgetKey(keystr);
	}

	return keystr;
}

void PropertyWidgetManager::dealclicked(QString key)
{
	if (!m_propertyMap.contains(key))
	{
		return;
	}
	int index = m_propertyMap[key]->getstackwidgetindex();
	if (index < 0)
	{
		return;
	}
	m_propertyStackWidget->setCurrentIndex(index);

}

propertyWidget* PropertyWidgetManager::createinitPropertyWidget(propertyobjecttype type, QWidget* delegatewidget)
{
	propertyWidget* propertywidget = new propertyWidget();
	switch (type)
	{
	case PropertyWidgetManager::propertyobjecttype::diagramRect:
		builddiagramRectPropertyWidget(propertywidget, delegatewidget);
		break;
	case PropertyWidgetManager::propertyobjecttype::diagramCircle:
		builddiagramCirclePropertyWidget(propertywidget, delegatewidget);
		break;
	case PropertyWidgetManager::propertyobjecttype::diagramTriangle:
		builddiagramTrianglePropertyWidget(propertywidget, delegatewidget);
		break;
	case PropertyWidgetManager::propertyobjecttype::diagramLine:
		builddiagramLinePropertyWidget(propertywidget, delegatewidget);
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

	//名字
	QString name = "diagram " + ShapeTypeTool::shapetypeEnumToQstring(castDelegatewidget->gettype());
	QLabel* nameLabel = new QLabel(name);
	widget->addProperty("名字", nameLabel);
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

	widget->addProperty("旋转角度", rotateSpinbox);

	//widget->addProperty("旋转角度", 0.0, propertyItem::propertyType::Double);

	//画笔颜色 宽度
	QPushButton* penColorButton = new QPushButton("选择");
	QObject::connect(penColorButton, &QPushButton::clicked, [=]() {
		QColor color = QColorDialog::getColor(Qt::black, widget, "选择画笔颜色");
		if (color.isValid()) {
			// 设置画笔颜色
			// castDelegatewidget->setPenColor(color);
			castDelegatewidget->onPenColorChanged(color);
		}
		});

	widget->addProperty("画笔颜色", penColorButton);

	QDoubleSpinBox* penWidthSpinbox = new QDoubleSpinBox();
	penWidthSpinbox->setRange(0, penWidthMax);
	penWidthSpinbox->setSingleStep(penWidthStep);
	penWidthSpinbox->setValue(penWidth);

	QObject::connect(penWidthSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
		, castDelegatewidget, &DiagramItem::onPenWidthChanged);

	widget->addProperty("画笔宽度", penWidthSpinbox);

	QPushButton* fillColorButton = new QPushButton("选择");
	QObject::connect(fillColorButton, &QPushButton::clicked, [=]() {
		QColor color = QColorDialog::getColor(Qt::white, widget, "选择填充颜色");
		if (color.isValid()) {
			// 设置填充颜色
			// castDelegatewidget->setFillColor(color);
			castDelegatewidget->onPenBrushChanged(color);
		}
		});

	widget->addProperty("填充颜色", fillColorButton);

	//widget->addProperty("画笔颜色", "#000000", propertyItem::propertyType::String);
	//widget->addProperty("画笔宽度", 2, propertyItem::propertyType::Int);

	//填充颜色
	//widget->addProperty("填充颜色", "#FFFFFF", propertyItem::propertyType::String);
}

void PropertyWidgetManager::builddiagramCirclePropertyWidget(propertyWidget* widget, QWidget* delegatewidget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	if (delegatewidget == nullptr)
		throw std::runtime_error("error");
	auto castDelegatewidget = dynamic_cast<DiagramItem*>(delegatewidget);
	if (castDelegatewidget == nullptr)
		throw std::runtime_error("error");

	QString name = "diagram " + ShapeTypeTool::shapetypeEnumToQstring(castDelegatewidget->gettype());
	QLabel* nameLabel = new QLabel(name);
	widget->addProperty("名字", nameLabel);
	//比例
	//widget->addProperty("比例", 1.0, propertyItem::propertyType::Double);
	QDoubleSpinBox* radioSpinbox = new QDoubleSpinBox();
	radioSpinbox->setRange(circleRadioMin, circleRadioMax);
	radioSpinbox->setSingleStep(circleRadioStep);
	radioSpinbox->setDecimals(circleRadioDecimals);
	radioSpinbox->setValue(circleRadioInitvlaue);

	widget->addProperty("比例", radioSpinbox);
	QObject::connect(radioSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
		, castDelegatewidget, &DiagramItem::onCircleRadioChanged);


	//宽和高


	//旋转角度
	//widget->addProperty("旋转角度", 0.0, propertyItem::propertyType::Double);
	QSpinBox* rotateSpinbox = new QSpinBox();
	rotateSpinbox->setRange(-180, 180);
	rotateSpinbox->setSingleStep(1);
	rotateSpinbox->setValue(0);
	QObject::connect(rotateSpinbox, QOverload<int>::of(&QSpinBox::valueChanged)
		, castDelegatewidget, &DiagramItem::onCircleRotateChanged);
	widget->addProperty("旋转角度", rotateSpinbox);

	QPushButton* penColorButton = new QPushButton("选择");
	QObject::connect(penColorButton, &QPushButton::clicked, [=]() {
		QColor color = QColorDialog::getColor(Qt::black, widget, "选择画笔颜色");
		if (color.isValid()) {
			// 设置画笔颜色
			// castDelegatewidget->setPenColor(color);
			castDelegatewidget->onPenColorChanged(color);
		}
		});

	widget->addProperty("画笔颜色", penColorButton);

	QDoubleSpinBox* penWidthSpinbox = new QDoubleSpinBox();
	penWidthSpinbox->setRange(0, penWidthMax);
	penWidthSpinbox->setSingleStep(penWidthStep);
	penWidthSpinbox->setValue(penWidth);

	QObject::connect(penWidthSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
		, castDelegatewidget, &DiagramItem::onPenWidthChanged);

	widget->addProperty("画笔宽度", penWidthSpinbox);

	QPushButton* fillColorButton = new QPushButton("选择");
	QObject::connect(fillColorButton, &QPushButton::clicked, [=]() {
		QColor color = QColorDialog::getColor(Qt::white, widget, "选择填充颜色");
		if (color.isValid()) {
			// 设置填充颜色
			// castDelegatewidget->setFillColor(color);
			castDelegatewidget->onPenBrushChanged(color);
		}
		});

	widget->addProperty("填充颜色", fillColorButton);

	//画笔颜色宽度
	//widget->addProperty("画笔颜色", "#000000", propertyItem::propertyType::String);
	//widget->addProperty("画笔宽度", 2, propertyItem::propertyType::Int);


	//填充颜色
	//widget->addProperty("填充颜色", "#FFFFFF", propertyItem::propertyType::String);
}

void PropertyWidgetManager::builddiagramTrianglePropertyWidget(propertyWidget* widget, QWidget* delegatewidget)
{
	if (widget == nullptr)
		throw std::runtime_error("error");

	if (delegatewidget == nullptr)
		throw std::runtime_error("error");
	auto castDelegatewidget = dynamic_cast<DiagramItem*>(delegatewidget);
	if (castDelegatewidget == nullptr)
		throw std::runtime_error("error");

	QString name = "diagram " + ShapeTypeTool::shapetypeEnumToQstring(castDelegatewidget->gettype());
	QLabel* nameLabel = new QLabel(name);
	widget->addProperty("名字", nameLabel);
	//三边比例
		//bottom
	//widget->addProperty("底边比例", 1.0, propertyItem::propertyType::Double);
		//left
	//widget->addProperty("左边比例", 1.0, propertyItem::propertyType::Double);
		//right
	//widget->addProperty("右边比例", 1.0, propertyItem::propertyType::Double);
	QDoubleSpinBox* bottomSpinbox = new QDoubleSpinBox();
	bottomSpinbox->setRange(triangleRadioMin, triangleRadioMax);
	bottomSpinbox->setSingleStep(triangleRadioStep);
	bottomSpinbox->setDecimals(2);
	bottomSpinbox->setValue(1.0);

	widget->addProperty("底边比例", bottomSpinbox);
	QObject::connect(bottomSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
		, castDelegatewidget, &DiagramItem::onTriangleSideRadioChangedBottom);

	QDoubleSpinBox* leftSpinbox = new QDoubleSpinBox();
	leftSpinbox->setRange(triangleRadioMin, triangleRadioMax);
	leftSpinbox->setSingleStep(triangleRadioStep);
	leftSpinbox->setDecimals(2);
	leftSpinbox->setValue(1.0);
	widget->addProperty("左边比例", leftSpinbox);
	QObject::connect(leftSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
		, castDelegatewidget, &DiagramItem::onTriangleSideRadioChangedLeft);

	QDoubleSpinBox* rightSpinbox = new QDoubleSpinBox();
	rightSpinbox->setRange(triangleRadioMin, triangleRadioMax);
	rightSpinbox->setSingleStep(triangleRadioStep);
	rightSpinbox->setDecimals(2);
	rightSpinbox->setValue(1.0);
	widget->addProperty("右边比例", rightSpinbox);
	QObject::connect(rightSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
		, castDelegatewidget, &DiagramItem::onTriangleSideRadioChangedRight);


	//指定某条边角度
	//widget->addProperty("指定边", );
	QComboBox* edgeTypeComboBox = new QComboBox();
	edgeTypeComboBox->addItem(DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Bottom), QVariant::fromValue<int>(static_cast<int>(DiagramDrawParamsTriangle::EdgeType::Bottom)));
	edgeTypeComboBox->addItem(DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Left), QVariant::fromValue<int>(static_cast<int>(DiagramDrawParamsTriangle::EdgeType::Left)));
	edgeTypeComboBox->addItem(DiagramDrawParamsTriangle::edgetypeEnumToString(DiagramDrawParamsTriangle::EdgeType::Right), QVariant::fromValue<int>(static_cast<int>(DiagramDrawParamsTriangle::EdgeType::Right)));

	QObject::connect(edgeTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged)
		, [=]()
		{
			int index = edgeTypeComboBox->currentData().toInt();
			DiagramDrawParamsTriangle::EdgeType edgetype = static_cast<DiagramDrawParamsTriangle::EdgeType>(index);
			castDelegatewidget->onTriangleEdgeTypeChanged(edgetype);
		});
	widget->addProperty("指定边", edgeTypeComboBox);

	//rotate
	QSpinBox* rotateSpinbox = new QSpinBox();
	rotateSpinbox->setRange(-180, 180);
	rotateSpinbox->setSingleStep(1);
	rotateSpinbox->setValue(0);
	QObject::connect(rotateSpinbox, QOverload<int>::of(&QSpinBox::valueChanged)
		, castDelegatewidget, &DiagramItem::onTriangleEdgeRotateChanged);
	widget->addProperty("旋转角度", rotateSpinbox);

	//画笔颜色 宽度
	QPushButton* penColorButton = new QPushButton("选择");
	QObject::connect(penColorButton, &QPushButton::clicked, [=]() {
		QColor color = QColorDialog::getColor(Qt::black, widget, "选择画笔颜色");
		if (color.isValid()) {
			// 设置画笔颜色
			// castDelegatewidget->setPenColor(color);
			castDelegatewidget->onPenColorChanged(color);
		}
		});

	widget->addProperty("画笔颜色", penColorButton);

	QDoubleSpinBox* penWidthSpinbox = new QDoubleSpinBox();
	penWidthSpinbox->setRange(0, penWidthMax);
	penWidthSpinbox->setSingleStep(penWidthStep);
	penWidthSpinbox->setValue(penWidth);

	QObject::connect(penWidthSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
		, castDelegatewidget, &DiagramItem::onPenWidthChanged);

	widget->addProperty("画笔宽度", penWidthSpinbox);

	QPushButton* fillColorButton = new QPushButton("选择");
	QObject::connect(fillColorButton, &QPushButton::clicked, [=]() {
		QColor color = QColorDialog::getColor(Qt::white, widget, "选择填充颜色");
		if (color.isValid()) {
			// 设置填充颜色
			// castDelegatewidget->setFillColor(color);
			castDelegatewidget->onPenBrushChanged(color);
		}
		});

	widget->addProperty("填充颜色", fillColorButton);


}

void PropertyWidgetManager::builddiagramLinePropertyWidget(propertyWidget* widget, QWidget* delegatewidget)
{
	//rotate
	if (widget == nullptr)
		throw std::runtime_error("error");

	if (delegatewidget == nullptr)
		throw std::runtime_error("error");
	auto castDelegatewidget = dynamic_cast<DiagramItem*>(delegatewidget);
	if (castDelegatewidget == nullptr)
		throw std::runtime_error("error");

	QString name = "diagram " + ShapeTypeTool::shapetypeEnumToQstring(castDelegatewidget->gettype());
	QLabel* nameLabel = new QLabel(name);
	widget->addProperty("名字", nameLabel);

	//旋转角度
	QSpinBox* rotateSpinbox = new QSpinBox();
	rotateSpinbox->setRange(-180, 180);
	rotateSpinbox->setSingleStep(1);
	rotateSpinbox->setValue(0);
	QObject::connect(rotateSpinbox, QOverload<int>::of(&QSpinBox::valueChanged)
		, castDelegatewidget, &DiagramItem::onLineRotateChanged);
	widget->addProperty("旋转角度", rotateSpinbox);

	//画笔颜色 宽度
	QPushButton* penColorButton = new QPushButton("选择");
	QObject::connect(penColorButton, &QPushButton::clicked, [=]() {
		QColor color = QColorDialog::getColor(Qt::black, widget, "选择画笔颜色");
		if (color.isValid()) {
			// 设置画笔颜色
			// castDelegatewidget->setPenColor(color);
			castDelegatewidget->onPenColorChanged(color);
		}
		});

	widget->addProperty("画笔颜色", penColorButton);

	QDoubleSpinBox* penWidthSpinbox = new QDoubleSpinBox();
	penWidthSpinbox->setRange(0, penWidthMax);
	penWidthSpinbox->setSingleStep(penWidthStep);
	penWidthSpinbox->setValue(penWidth);

	QObject::connect(penWidthSpinbox, QOverload<double>::of(&QDoubleSpinBox::valueChanged)
		, castDelegatewidget, &DiagramItem::onPenWidthChanged);

	widget->addProperty("画笔宽度", penWidthSpinbox);

	QPushButton* fillColorButton = new QPushButton("选择");
	QObject::connect(fillColorButton, &QPushButton::clicked, [=]() {
		QColor color = QColorDialog::getColor(Qt::white, widget, "选择填充颜色");
		if (color.isValid()) {
			// 设置填充颜色
			// castDelegatewidget->setFillColor(color);
			castDelegatewidget->onPenBrushChanged(color);
		}
		});

	widget->addProperty("填充颜色", fillColorButton);

}



//propertyItem::propertyItem(const QString& name, QWidget* widget, QWidget* parent)
	//:m_name(name)
	//, m_editWidget(widget)
	//, QWidget(parent)
//{

//}
