#pragma once
#include <qwidget.h>
#include <qformlayout.h>
#include <qspinbox.h>
#include <qvariant.h>
#include <qboxlayout.h>
#include <qlineedit.h>
#include <qcombobox.h>

class propertyItem : public QWidget
{
	Q_OBJECT

public:
	enum class propertyType
	{
		String,
		Int,
		Double,
		Boolean,
		Color,
	};
	propertyItem(QString name, QVariant value, propertyType type, QWidget* parent = nullptr);
signals:
	//信号
	void valueChanged(const QString& name, const QVariant& value);

private slots:
	void onValueChanged()
	{
		emit valueChanged(m_name, value());
	}



private:
	QVariant value()
	{
		switch (m_type)
		{
		case propertyItem::propertyType::String:
			return dynamic_cast<QLineEdit*>(m_editWidget)->text();
			break;
		case propertyItem::propertyType::Int:
			return dynamic_cast<QSpinBox*>(m_editWidget)->value();
			break;
		case propertyItem::propertyType::Double:
			return dynamic_cast<QDoubleSpinBox*>(m_editWidget)->value();
			break;
		case propertyItem::propertyType::Boolean:
			return dynamic_cast<QComboBox*>(m_editWidget)->currentData();
			break;
		case propertyItem::propertyType::Color:
			throw std::runtime_error("error");
			break;
		default:
			throw std::runtime_error("error");
			break;
		}
	}

	QString m_name;
	propertyType m_type;
	QWidget* m_editWidget;
};




class shuxingwidget : public QWidget
{
	Q_OBJECT
public:
	void huabushuxing();
	shuxingwidget(QWidget* parent = nullptr);
	QFormLayout* m_shuxinglayout;
};

