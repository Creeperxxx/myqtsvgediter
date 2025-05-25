#ifndef PROPERTYDELEGATE_H
#define PROPERTYDELEGATE_H

#include <qobject.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qboxlayout.h>
#include <qcombobox.h>
#include <qformlayout.h>
#include "propertydelegateparams.h"
#include "propertydata.h"



class IpropertyDelegate : public QObject
{
	Q_OBJECT
signals:
	void signalValueChanged(QVariant value);

public:
	IpropertyDelegate();
	void setData(std::shared_ptr<propertydata> data);
	virtual QWidget* getEditWidget() = 0;

	void slotValueChanged();
protected:
	virtual void createWidget(std::shared_ptr<IdelegatePramas> params) = 0;
	virtual QVariant value() = 0;
	virtual void setValue(QVariant value) = 0;

	template <typename T>
	T isDataCanConvert(QVariant value);

	template <typename T>
	std::shared_ptr<T> isParamsCastValid(std::shared_ptr<IdelegatePramas> params);
	void checkQobjectValid(QObject* object);
private:
	void isDataValid(std::shared_ptr<propertydata> data);
	void disconnectValueChangedSignal();
	void connectValueChanged(std::shared_ptr<propertydata> data);

	std::shared_ptr<propertydata> m_olddata;
};

template <typename T>
inline T IpropertyDelegate::isDataCanConvert(QVariant value)
{
	if (!value.canConvert<T>())
		throw std::runtime_error("error");
	return value.value<T>();
}

template<typename T>
inline std::shared_ptr<T> IpropertyDelegate::isParamsCastValid(std::shared_ptr<IdelegatePramas> params)
{
	if(params == nullptr || params.get() == nullptr)
		throw std::runtime_error("error");
	std::shared_ptr<T> p = std::dynamic_pointer_cast<T>(params);
	if (p == nullptr)
		throw std::runtime_error("error");
	return p;
}













class doubleDelegate : public IpropertyDelegate
{
public:
	doubleDelegate(std::shared_ptr<IdelegatePramas> params);
	QWidget* getEditWidget() override;

protected:
	void createWidget(std::shared_ptr<IdelegatePramas> params) override;
    QVariant value() override;
	void setValue(QVariant value) override;

private:
	QDoubleSpinBox* m_editwidget;
};

class intDelegate : public IpropertyDelegate
{
public:
	intDelegate(std::shared_ptr<IdelegatePramas> params);
	QWidget* getEditWidget() override;

protected:
	void createWidget(std::shared_ptr<IdelegatePramas> params) override;
	void setValue(QVariant value) override;
    QVariant value() override;

private:
	QSpinBox* m_editwidget;
};

class colorDelete : public IpropertyDelegate
{
public:
	colorDelete(std::shared_ptr<IdelegatePramas> params);
	QWidget* getEditWidget() override;

protected:
	void createWidget(std::shared_ptr<IdelegatePramas> params) override;
	void setValue(QVariant value) override;
    QVariant value() override;

private:

	QColor m_currentcolor;
	QPushButton* m_button;
	QLabel* m_colorlabel;
	QWidget* m_colorwidget;
	QHBoxLayout* m_colorlayout;
};

class stringDelegate : public IpropertyDelegate
{
public:
	stringDelegate(std::shared_ptr<IdelegatePramas> params);
	QWidget* getEditWidget() override;
protected:
	void createWidget(std::shared_ptr<IdelegatePramas> params) override;
	void setValue(QVariant value) override;
    QVariant value()override;

private:

	QLabel* m_label;
};

class enumDelegate : public IpropertyDelegate
{
public:
	enumDelegate(std::shared_ptr<IdelegatePramas> params);
	QWidget* getEditWidget() override;
protected:
	void createWidget(std::shared_ptr<IdelegatePramas> params) override;
	void setValue(QVariant value) override;
    QVariant value() override;
private:

	QComboBox* m_combobox;
};

class triangleSideRadioDelegate : public IpropertyDelegate
{
public:
	triangleSideRadioDelegate(std::shared_ptr<IdelegatePramas> params);
	QWidget* getEditWidget() override;
	void onisValid();


protected:
	void createWidget(std::shared_ptr<IdelegatePramas> params) override;
	void setValue(QVariant value) override;
    QVariant value() override;
private:


	QWidget* m_radiowidget;
	QWidget* m_widget;
	QSpinBox* m_bottombox;
	QSpinBox* m_leftbox;
	QSpinBox* m_rightbox;
	QFormLayout* m_formlayout;
	QPushButton* m_button;
	QVBoxLayout* m_vlayout;

};


#endif // PROPERTYDELEGATE_H
