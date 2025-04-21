#pragma once
#include <optional>
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
#include <map>


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

constexpr auto colorProperty = "colorinfo";
constexpr auto colortext = "选择";
constexpr auto colorchoosetitle = "选择颜色";

constexpr auto propertynamename = "名字";
constexpr auto propertynameradio = "比例";
constexpr auto propertynamerotate = "旋转角度";
constexpr auto propertynamebottomradio = "底边比例";
constexpr auto propertynameleftradio = "左边比例";
constexpr auto propertynamerightradio = "右边比例";
constexpr auto propertynameedgetype = "指定边";
constexpr auto propertynamepencolor = "画笔颜色";
constexpr auto propertynamepenwidth = "画笔宽度";
constexpr auto propertynamebrush = "填充颜色";
constexpr auto propertynamescale = "缩放";

constexpr auto propertynamehuabuwidth = "画布宽度";
constexpr auto propertynamehuabuheight = "画布高度";

enum class EditItemDataType
{
	Double,
	Int,
	Enum,
	Color,
	String
};


class propertyItemParams
{
public:

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
	propertyItem(EditItemDataType type);
	QWidget* getEditWidget();
	//QString getName();
	void setData(std::shared_ptr<propertyData> data);
//槽函数
	void onValueChanged();
	//void onSetValue(std::shared_ptr<propertyData> data);
signals:
	void signalValueChanged(EditItemDataType type, QVariant value);
	

private:
	//void createItem(propertyItemParams* params);
	void createWidgetByType(EditItemDataType type);

	void setDataByType(EditItemDataType type, propertyItemParams* params);

	void setDoubleData(propertyItemParams* params);
	void setIntData(propertyItemParams* params);
	void setColorData(propertyItemParams* params);
	void setEnumData(propertyItemParams* params);
	void setStringData(propertyItemParams* params);

	QVariant getValue();

	QWidget* m_editWidget;
	EditItemDataType m_type;
	//propertyItemParams* m_params;
};

class propertyData : public QWidget
{
	
	Q_OBJECT

public:

	void slotValueChanged(EditItemDataType type, QVariant value);

signals:
	void signalValueChangedHuabuTuxing(QString m_objectkey, QString name, EditItemDataType type, QVariant value);
signals:
	void signalValueChanged(QString name, EditItemDataType type, QVariant value);


public:
	EditItemDataType m_datatype;
	QString m_name;
	QVariant m_data;
	std::optional<QString> m_objectkey;

	std::shared_ptr<propertyItemParams> m_params;
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
	void dealclicked(propertyobjecttype type, std::vector<std::shared_ptr<propertyData>> data);
private:
	//propertyWidget* createinitPropertyWidget(propertyobjecttype type, QWidget* widget);

	//void initStackWidget();

	void createonceWidget();
	propertyWidget* createOriginalPropertyWidget();
	void buildDiagramRectPropertyWidget(propertyWidget* widget);
	void buildDiagramCirclePropertyWidget(propertyWidget* widget);
	void buildDiagramTrianglePropertyWidget(propertyWidget* widget);
	void buildDiagramLinePropertyWidget(propertyWidget* widget);
	void buildDiagramHuabuPropertyWidget(propertyWidget* widget);



	void buildPropertywidgetPenandBrush(propertyWidget* widget);
	void buildPropertywidgetname(propertyWidget* widget);
	void buildPropertywidgetRotate(propertyWidget* widget);
	void buildPropertywidgetScale(propertyWidget* widget);

	void addPropertyWidget(propertyobjecttype type, propertyWidget* widget);


	QMap<propertyobjecttype, propertyWidget*> m_propertyMap;
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
	//void addProperty(const QString& name, QWidget* widget);
	void paintEvent(QPaintEvent* event) override;
	void setstackwidgetindex(int index);
	int getstackwidgetindex();

	void addPropertyItem(QString name, EditItemDataType type);
	void addShowingData(std::shared_ptr<propertyData> data);

	//void propertyChanged(const QString& name, const QVariant& value);
private:

	std::map<QString,std::shared_ptr<propertyItem>> m_propertyItemMap;
	QFormLayout* m_shuxinglayout;
	//QMap<QString, QWidget*> m_propertyMap;
	//PropertyWidgetManager::propertyobjecttype m_type;
	int m_stackwidgetindex;
};

