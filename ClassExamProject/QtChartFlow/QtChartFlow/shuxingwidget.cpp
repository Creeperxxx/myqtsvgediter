#include "shuxingwidget.h"

void shuxingwidget::huabushuxing()
{
	QSpinBox* widthspinbox = new QSpinBox(this);
	widthspinbox->setRange(200, 8000);
	m_shuxinglayout->addRow("宽度：", widthspinbox);

	QSpinBox* heightspinbox = new QSpinBox(this);
	heightspinbox->setRange(200, 8000);
	m_shuxinglayout->addRow("高度：", heightspinbox);

}

shuxingwidget::shuxingwidget(QWidget* parent)
	: QWidget(parent)
{
	m_shuxinglayout = new QFormLayout(this);
	m_shuxinglayout->setContentsMargins(0, 0, 0, 0);
	m_shuxinglayout->setSpacing(0);
	setLayout(m_shuxinglayout);

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

	huabushuxing();
}

propertyItem::propertyItem(QString name, QVariant value, propertyType type, QWidget* parent)
	:m_type(type)
	, m_name(name)
	, QWidget(parent)
	, m_editWidget(nullptr)
{
	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);

	switch (type)
	{
	case propertyItem::propertyType::String:
	{
		m_editWidget = new QLineEdit(this);
		dynamic_cast<QLineEdit*>(m_editWidget)->setText(value.toString());

		//连接信号 文本更改发送信号
	}
		break;
	case propertyItem::propertyType::Int:
	{
		QSpinBox* spinbox = new QSpinBox(this);
		spinbox->setValue(value.toInt());
		m_editWidget = spinbox;
		//连接信号 数值更改发送信号
	}
		break;
	case propertyItem::propertyType::Double:
	{
		QDoubleSpinBox* doubleSpinBox = new QDoubleSpinBox(this);
		doubleSpinBox->setValue(value.toDouble());
		m_editWidget = doubleSpinBox;
		//连接信号 数值更改发送信号
	}
		break;
	case propertyItem::propertyType::Boolean:
	{
		QComboBox* combo = new QComboBox(this);
		combo->addItem("False", false);
		combo->addItem("True", true);
		combo->setCurrentIndex(value.toBool() ? 1 : 0);
		m_editWidget = combo;
		//连接信号 选择更改发送信号
	}
		break;
	case propertyItem::propertyType::Color:
	{
		throw std::runtime_error("error");
	}
		break;
	default:
		throw std::runtime_error("error");
	}
	layout->addWidget(m_editWidget);
}
