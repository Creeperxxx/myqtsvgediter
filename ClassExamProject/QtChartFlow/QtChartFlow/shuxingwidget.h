#pragma once
#include <qwidget.h>
#include <qformlayout.h>
#include <qspinbox.h>
#include <qvariant.h>
#include <qboxlayout.h>
#include <qcombobox.h>
#include <qstackedwidget.h>
#include <qpushbutton.h>
#include <qcolordialog.h>
#include <map>
#include <optional>
#include <qlabel.h>


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
constexpr const int triangleRadioDecimals = 1.00;
constexpr const double triangleInitBottomValue = 1;
constexpr const double triangleInitLeftValue = 1;
constexpr const double triangleInitRightValue = 1;

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
constexpr auto propertynametuxingspacewidth = "图形宽度";
constexpr auto propertynametuxingspaceheight = "图形高度";

constexpr auto propertynamehuabuwidth = "画布宽度";
constexpr const int huabuwidthMax = 8000;
constexpr const int huabuwidthMin = 100;
constexpr const int huabuwidthvalue = 900;
constexpr auto propertynamehuabuheight = "画布高度";
constexpr const int huabuheightMax = 8000;
constexpr const int huabuheightMin = 100;
constexpr const int huabuheightvalue = 900;

constexpr const int tuxingspacewidthMax = 1000;
constexpr const int tuxingspacewidthMin = 10;

constexpr const int tuxingspaceheightMax = 1000;
constexpr const int tuxingspaceheightMin = 10;
constexpr const int tuxingspacewidthvalue = 500;
constexpr const int tuxingspaceheightvalue = 500;


constexpr auto propertyColorText = "选择";
constexpr auto propertyColorDialogTitle = "选择一个颜色";
constexpr auto propertyInitColor = Qt::white;

constexpr auto defaultname = "default name";
constexpr auto propertynamehmove = "水平偏移";
constexpr auto propertynamevmove = "垂直偏移";
constexpr auto centerhmax = 4000;
constexpr auto centerhmin = -4000;
constexpr auto centervmax = 4000;
constexpr auto centervmin = -4000;
//constexpr auto property

enum class delegateType
{
	Double,
	Int,
	Enum,
	Color,
	String
};



class IdelegatePramas
{
public:
	delegateType m_type;
	IdelegatePramas(delegateType type);
	virtual ~IdelegatePramas() = 0;
};

class delegateParamsDouble : public IdelegatePramas
{
public:
	~delegateParamsDouble() {}
	delegateParamsDouble(double max
		, double min
		, double step
		, int decimals
		, double initvalue);
	double m_valuemax;
	double m_valuemin;
	double m_valuestep;
	int m_valuedecimals;
	double m_initvalue;

};

class delegateParamsInt : public IdelegatePramas
{
public:
	~delegateParamsInt() {}
	delegateParamsInt(int max
		, int min
		, int step
		, int initvalue);
	int m_valuemax;
	int m_valuemin;
	int m_valuestep;
	int m_initvalue;
};

class delegateParamsColor : public IdelegatePramas
{
public:
    ~delegateParamsColor() {}
	delegateParamsColor(QColor initcolor);
	QColor m_initcolor;
};

class delegateParamsEnum : public IdelegatePramas
{
public:
    ~delegateParamsEnum() {}
	delegateParamsEnum(QVector<QString> vec);
	QVector<QString> m_vec;
	QString m_initstring;
};

class delegateParamsString : public IdelegatePramas
{
public:
    ~delegateParamsString() {}
	delegateParamsString(QString str);
	QString m_initstring;
};



class propertydata;











class IpropertyDelegate : public QWidget
{
	Q_OBJECT

public:
	void slotValueChanged();
signals:
    void signalValueChanged(QVariant value);

	virtual ~IpropertyDelegate() = 0;
	virtual void setData(std::shared_ptr<propertydata> data) = 0;
	virtual QWidget* getEditWidget() = 0;

protected:
	virtual void createWidget(std::shared_ptr<IdelegatePramas> params) = 0;
	virtual QVariant value() = 0;

};

class doubleDelegate : public IpropertyDelegate
{
public:
	~doubleDelegate() override = default;
	doubleDelegate(std::shared_ptr<IdelegatePramas> params);
	void setData(std::shared_ptr<propertydata> data) override;
	QWidget* getEditWidget() override;
private:
	void createWidget(std::shared_ptr<IdelegatePramas> params) override;
	QVariant value();

	QDoubleSpinBox* m_editwidget;
};

class intDelegate : public IpropertyDelegate
{
    ~intDelegate() override = default;
	intDelegate(std::shared_ptr<IdelegatePramas> params);
    void setData(std::shared_ptr<propertydata> data) override;
	QWidget* getEditWidget() override;
private:
	void createWidget(std::shared_ptr<IdelegatePramas> params) override;
    QVariant value();

    QSpinBox* m_editwidget;
};

class colorDelete : public IpropertyDelegate
{
public:
    ~colorDelete() override = default;
	colorDelete(std::shared_ptr<IdelegatePramas> params);
    void setData(std::shared_ptr<propertydata> data) override;
	QWidget* getEditWidget() override;
private:
	void createWidget(std::shared_ptr<IdelegatePramas> params) override;

    QVariant value();
	
	QColor m_currentcolor;
    QPushButton* m_button;
	QLabel* m_colorlabel;
	QWidget* m_colorwidget;
	QHBoxLayout* m_colorlayout;
};

class stringDelegate : public IpropertyDelegate
{
public:
	~stringDelegate() override = default;
	stringDelegate(std::shared_ptr<IdelegatePramas> params);
	void setData(std::shared_ptr<propertydata> data) override;
private:
	void createWidget(std::shared_ptr<IdelegatePramas> params) override;

	QVariant value();

	QLabel* m_label;
};

class enumDelegate : public IpropertyDelegate
{
    ~enumDelegate() override = default;
	enumDelegate(std::shared_ptr<IdelegatePramas> params);
    void setData(std::shared_ptr<propertydata> data) override;
private:
	void createWidget(std::shared_ptr<IdelegatePramas> params) override;
    QVariant value();

    QComboBox* m_combobox;
};









class propertydata : public QWidget
{
	Q_OBJECT
public:
	propertydata(QString name, QVariant data);

signals:
	void signalValueChanged(QVariant value);
public:
	QString m_name;
	QVariant m_data;

};
class propertyWidget;

















class PropertyWidgetManager : public QWidget
{
	Q_OBJECT
public:
	enum class propertyobjecttype
	{
		defaulttype,
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
	void dealclicked(propertyobjecttype type, std::vector<std::shared_ptr<propertydata>> data);
private:

	void createonceWidget();
	propertyWidget* createOriginalPropertyWidget();
	void buildDiagramRectPropertyWidget(propertyWidget* widget);
	void buildDiagramCirclePropertyWidget(propertyWidget* widget);
	void buildDiagramTrianglePropertyWidget(propertyWidget* widget);
	void buildDiagramLinePropertyWidget(propertyWidget* widget);
	void buildDiagramHuabuPropertyWidget(propertyWidget* widget);
	void buildDiagramHuabuRectPropertyWidget(propertyWidget* widget);
    void buildDiagramHuabuCirclePropertyWidget(propertyWidget* widget);
    void buildDiagramHuabuTrianglePropertyWidget(propertyWidget* widget);
    void buildDiagramHuabuLinePropertyWidget(propertyWidget* widget);
	void buildDefaultPropertyWidget(propertyWidget* widget);


	void addPropertyWidget(propertyobjecttype type, propertyWidget* widget);


	void buildPropertyWidgetName(propertyWidget* widget);
	void buildPropertyWidgetRectRadio(propertyWidget* widget);
	void buildPropertyWidgetRotate(propertyWidget* widget);
	void buildProertyWidgetCircleRadio(propertyWidget* widget);
	void buildPropertyWidgetScale(propertyWidget* widget);
	void buildPropertyWidgetTriangleSideRadio(propertyWidget* widget);
	void buildPropertyWidgetTriangleEdgetype(propertyWidget* widget);
	void buildPropertyWidgetPenAndBrush(propertyWidget* widget);
	void buildPropertyWidgetSpacesize(propertyWidget* widget);
	void buildPropertyWidgetHuabuSize(propertyWidget* widget);
	void buildPropertyWidgetCentermove(propertyWidget* widget);





	QMap<propertyobjecttype, propertyWidget*> m_propertyMap;
	QStackedWidget* m_propertyStackWidget;
	
};




















class propertyWidget : public QWidget
{
	Q_OBJECT
public:
	propertyWidget(QWidget* parent = nullptr);
	void setstackwidgetindex(int index);
	int getstackwidgetindex();

	void addPropertyItem(QString name, std::shared_ptr<IdelegatePramas> params);
	void addShowingData(std::shared_ptr<propertydata> data);

private:
	std::shared_ptr<IpropertyDelegate> createDelegate(std::shared_ptr<IdelegatePramas> params);
	std::shared_ptr<IpropertyDelegate> createDelegateDouble(std::shared_ptr<IdelegatePramas> params);
	std::shared_ptr<IpropertyDelegate> createDelegateInt(std::shared_ptr<IdelegatePramas> params);
	std::shared_ptr<IpropertyDelegate> createDelegateColor(std::shared_ptr<IdelegatePramas> params);
	std::shared_ptr<IpropertyDelegate> createDelegateEnum(std::shared_ptr<IdelegatePramas> params);
	std::shared_ptr<IpropertyDelegate> createDelegateString(std::shared_ptr<IdelegatePramas> params);



	std::map<QString, std::shared_ptr<IpropertyDelegate>> m_propertyDelegateMap;
	QFormLayout* m_shuxinglayout;
	int m_stackwidgetindex;
};

