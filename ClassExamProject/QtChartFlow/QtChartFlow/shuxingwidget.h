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
#include <functional>
#include <qvector.h>


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


class propertyItemParams
{
public:
	QString m_name;
	enum class EditItemDataType
	{
		Double,
		Int,
		Enum,
		Color,
		String
	};
	EditItemDataType m_editDataType;
	//QWidget* m_delegateWidget;
	//std::function<void(QVariant)> m_slotFunction;

	//double
	bool isDoubleDataValid();
	std::optional<double> m_doubleRangeMin;
	std::optional<double> m_doubleRangeMax;
	std::optional<double> m_doubleSingleStep;
	std::optional<int> m_doubleDecimals;
	std::optional<double> m_doubleDefaultValue;

	bool isIntDataValid();
	std::optional<int> m_intRangeMin;
	std::optional<int> m_intRangeMax;
	std::optional<int> m_intSingleStep;
	std::optional<int> m_intDefaultValue;

	bool isEnumDataValid();
	std::optional<QVector<QString>> m_enumStringVec;

	bool isStringItemValid();
	std::optional<QString> m_stringStr;

};

class propertyItem : public QWidget
{
	Q_OBJECT
public:
	propertyItem(propertyItemParams* params);
	QWidget* getEditWidget();
	QString getName();
	void onValueChanged();
signals:
	void signalValueChanged(QString name, propertyItemParams::EditItemDataType type
		, QVariant value);
private:
	void createItem(propertyItemParams* params);
	void buildDoubleItem(propertyItemParams* params);
	void buildIntItem(propertyItemParams* params);
	void buildColorItem(propertyItemParams* params);
	void buildEnumItem(propertyItemParams* params);
	void buildStringItem(propertyItemParams* params);

	QVariant getValue();

	QWidget* m_editWidget;
	QString m_name;
	propertyItemParams::EditItemDataType m_type;
	std::optional<QColor> m_choosedColor;

	//propertyItemParams* m_params;
};

class propertyData : public QWidget
{
	
	Q_OBJECT

public:
	void slotValueChanged(propertyItemParams::EditItemDataType type, QVariant value);

signals:
	void signalValueChanged(QString m_objectkey, QString name, propertyItemParams::EditItemDataType type, QVariant value);




	QWidget* m_delegatewidget;
	QString m_objectkey;
};


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
	void buildhuabuPropertyWidget(propertyWidget* widget, QWidget* delegatewidget);
	void buildhuabuRectPropertyWidget(propertyWidget* widget, QWidget* delegatewidget);
	void buildhuabuCirclePropertyWidget(propertyWidget* widget, QWidget* delegatewidget);
	void buildhuabuTrianglePropertyWidget(propertyWidget* widget, QWidget* delegatewidget);
	void buildhuabuLinePropertyWidget(propertyWidget* widget, QWidget* delegatewidget);

	void initStackWidget();
	propertyWidget* createOriginalPropertyWidget();
	void createDiagramRectPropertyWidget();


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
	void addPropertyItem(propertyItemParams* params);
	//void addProperty(const QString& name, QWidget* widget);
	void paintEvent(QPaintEvent* event) override;
	void setstackwidgetindex(int index);
	int getstackwidgetindex();

	
signals:
	void signalValueChanged(QString name, propertyItemParams::EditItemDataType type, QVariant value);


	//void propertyChanged(const QString& name, const QVariant& value);
private:

	QString m_datashowingobjectflag;
	std::vector<std::shared_ptr<propertyItem>> m_propertyItemVec;
	QFormLayout* m_shuxinglayout;
	//QMap<QString, QWidget*> m_propertyMap;
	//PropertyWidgetManager::propertyobjecttype m_type;
	int m_stackwidgetindex;
	propertyData* m_data;
};

