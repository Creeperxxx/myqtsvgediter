#pragma once
#include <qwidget.h>
#include <qformlayout.h>
#include <qspinbox.h>
#include <qvariant.h>
#include <qboxlayout.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qmap.h>
#include <qpainter.h>
#include <qstackedwidget.h>
#include <quuid.h>
#include <qpushbutton.h>
#include <qcolordialog.h>


//rect radio
constexpr const double rectRadioMin = 0.1;
constexpr const double rectRadioMax = 10;
constexpr const double rectRadioStep = 0.1;
constexpr const int rectRadioDecimals = 2;
constexpr const double rectRadioInitvlaue = 2;

constexpr const int penWidth = 2;
constexpr const double penWidthStep = 0.5;
constexpr const int penWidthMax = 10;

constexpr const double circleRadioMin = 0.1;
constexpr const double circleRadioMax = 10;
constexpr const double circleRadioStep = 0.1;
constexpr const int circleRadioDecimals = 2;
constexpr const double circleRadioInitvlaue = 2;

constexpr const double triangleRadioMin = 0.1;
constexpr const double triangleRadioMax = 10;
constexpr const double triangleRadioStep = 0.1;



class propertyWidget;

class PropertyWidgetManager
{
public:
	enum class propertyobjecttype
	{
		diagramRect,
		diagramCircle,
		diagramTriangle,
		diagramLine,

		huabu,
		huabuRect,
		huabuCircle,
		huabuTriangle,
		huabuLine
	};
	PropertyWidgetManager(QWidget* parent);
	QStackedWidget* getstackwidget();
	QString createPropertyWidget(propertyobjecttype type, QWidget* widget);
	void dealclicked(QString key);
private:
	propertyWidget* createinitPropertyWidget(propertyobjecttype type, QWidget* widget);
	void builddiagramRectPropertyWidget(propertyWidget* propertywidget, QWidget* delegatewidget);
	void builddiagramCirclePropertyWidget(propertyWidget* widget, QWidget* delegatewidget);
	void builddiagramTrianglePropertyWidget(propertyWidget* widget, QWidget* delegatewidget);
	void builddiagramLinePropertyWidget(propertyWidget* widget, QWidget* delegatewidget);


	QMap<QString, propertyWidget*> m_propertyMap;
	QStackedWidget* m_propertyStackWidget;
	
};




//class propertyItem : public QWidget
//{
//	Q_OBJECT
//
//public:
//	enum class propertyType
//	{
//		//String,
//		//Int,
//		//Double,
//		//Boolean,
//		//Color,
//		Text,
//		Color, //QColorDialog::getColor(Qt::white, this, "Choose a color");
//		File, //QFileDialog::getOpenFileName(this,"Open File","/home","All Files (*);;Text Files (*.txt)");
//        Boolean,
//		Enum
//		//date,
//		//image
//		//password
//		//wenjianjia
//	};
//	//propertyItem(QString name, QWidget* widget, propertyType type, QWidget* parent = nullptr);
//
//	propertyItem(const QString& name, QWidget* widget, QWidget* parent = nullptr);
////signals:
//	//信号
//	//void valueChanged(const QString& name, const QVariant& value);
//
////private slots:
//	//void onValueChanged();
//	//{
//		//emit valueChanged(m_name, value());
//	//}
//
//
//
//private:
//	//QWidget* createTextwidget();
//
//
//
//
//	//QVariant value();
//	//{
//	//	switch (m_type)
//	//	{
//	//	case propertyItem::propertyType::String:
//	//		return dynamic_cast<QLineEdit*>(m_editWidget)->text();
//	//		break;
//	//	case propertyItem::propertyType::Int:
//	//		return dynamic_cast<QSpinBox*>(m_editWidget)->value();
//	//		break;
//	//	case propertyItem::propertyType::Double:
//	//		return dynamic_cast<QDoubleSpinBox*>(m_editWidget)->value();
//	//		break;
//	//	case propertyItem::propertyType::Boolean:
//	//		return dynamic_cast<QComboBox*>(m_editWidget)->currentData();
//	//		break;
//	//	case propertyItem::propertyType::Color:
//	//		throw std::runtime_error("error");
//	//		break;
//	//	default:
//	//		throw std::runtime_error("error");
//	//		break;
//	//	}
//	//}
//
//	QString m_name;
//	//propertyType m_type;
//	QWidget* m_editWidget;
//};




class propertyWidget : public QWidget
{
	Q_OBJECT
public:
	//void huabushuxing();
	propertyWidget(QWidget* parent = nullptr);
	//class propertyParams
	//{
	//public:
		//QString m_name;
		//QVariant m_value;
		//propertyItem::propertyType m_type;
	//};
	void addProperty(const QString& name, QWidget* widget);
	void paintEvent(QPaintEvent* event) override;
	void setstackwidgetindex(int index);
	int getstackwidgetindex();
//signals:
	//void propertyChanged(const QString& name, const QVariant& value);
private:
	QFormLayout* m_shuxinglayout;
	QMap<QString, QWidget*> m_propertyMap;
	//PropertyWidgetManager::propertyobjecttype m_type;
	int m_stackwidgetindex;
};

