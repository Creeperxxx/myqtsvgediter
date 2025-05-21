#include <qmessagebox.h>
#include <qcolordialog.h>
#include "propertydelegage.h"
#include "diagramdrawparams.h"

doubleDelegate::doubleDelegate(std::shared_ptr<IdelegatePramas> params)
	: m_editwidget(nullptr)
{
	createWidget(params);
}

IpropertyDelegate::IpropertyDelegate()
	:m_olddata(nullptr)
{
}

void IpropertyDelegate::setData(std::shared_ptr<propertydata> data)
{
	isDataValid(data);
	disconnectValueChangedSignal();

	setValue(data->getValue());
	connectValueChanged(data);
	m_olddata = data;
}


QWidget* doubleDelegate::getEditWidget()
{
	return m_editwidget;
}

void doubleDelegate::createWidget(std::shared_ptr<IdelegatePramas> params)
{
	std::shared_ptr<delegateParamsDouble> p = isParamsCastValid<delegateParamsDouble>(params);

	m_editwidget = new QDoubleSpinBox();
	m_editwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	checkQobjectValid(m_editwidget);

	m_editwidget->setRange(p->m_valuemin, p->m_valuemax);
	m_editwidget->setSingleStep(p->m_valuestep);
	m_editwidget->setDecimals(p->m_valuedecimals);
	m_editwidget->setValue(p->m_initvalue);

	QObject::connect(m_editwidget, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged)
		, this, &IpropertyDelegate::slotValueChanged);
}

QVariant doubleDelegate::value()
{
	return QVariant::fromValue(m_editwidget->value());
}


intDelegate::intDelegate(std::shared_ptr<IdelegatePramas> params)
	:m_editwidget(nullptr)
{
	createWidget(params);
}

void IpropertyDelegate::slotValueChanged()
{
	QVariant v = value();
	emit signalValueChanged(v);
}


QWidget* intDelegate::getEditWidget()
{
	return m_editwidget;
}

void intDelegate::createWidget(std::shared_ptr<IdelegatePramas> params)
{
	std::shared_ptr<delegateParamsInt> p = isParamsCastValid<delegateParamsInt>(params);

	m_editwidget = new QSpinBox();
	m_editwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	checkQobjectValid(m_editwidget);

	m_editwidget->setRange(p->m_valuemin, p->m_valuemax);
	m_editwidget->setSingleStep(p->m_valuestep);
	m_editwidget->setValue(p->m_initvalue);

	QObject::connect(m_editwidget, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &IpropertyDelegate::slotValueChanged);
}

QVariant intDelegate::value()
{
	return QVariant::fromValue(m_editwidget->value());
}

colorDelete::colorDelete(std::shared_ptr<IdelegatePramas> params)
	:m_button(nullptr)
	, m_colorwidget(nullptr)
	, m_colorlayout(nullptr)
	, m_colorlabel(nullptr)
	, m_currentcolor(QColor(Qt::white))
{
	createWidget(params);
}

void IpropertyDelegate::checkQobjectValid(QObject* object)
{
	if (object == nullptr)
		throw std::runtime_error("error");
}

void IpropertyDelegate::isDataValid(std::shared_ptr<propertydata> data)
{
	if (data == nullptr || data.get() == nullptr)
		throw std::runtime_error("error");
}

void IpropertyDelegate::disconnectValueChangedSignal()
{
	if (m_olddata != nullptr && m_olddata.get() != nullptr)
	{
		QObject::disconnect(this, &IpropertyDelegate::signalValueChanged, m_olddata.get(), &propertydata::slotValueChanged);

	}

}

void IpropertyDelegate::connectValueChanged(std::shared_ptr<propertydata> data)
{
	QObject::connect(this, &IpropertyDelegate::signalValueChanged,
		data.get(), &propertydata::slotValueChanged);
}

void doubleDelegate::setValue(QVariant value)
{
	m_editwidget->setValue(isDataCanConvert<double>(value));
}

void intDelegate::setValue(QVariant value)
{
	m_editwidget->setValue(isDataCanConvert<int>(value));
}

QWidget* colorDelete::getEditWidget()
{
	return m_colorwidget;
}

void colorDelete::createWidget(std::shared_ptr<IdelegatePramas> params)
{
	auto p = isParamsCastValid<delegateParamsColor>(params);

	m_colorwidget = new QWidget();
	m_colorwidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_colorlayout = new QHBoxLayout();
	m_colorwidget->setLayout(m_colorlayout);

	m_button = new QPushButton("选择", m_colorwidget);
	m_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	m_colorlabel = new QLabel(m_colorwidget);
	m_currentcolor = p->m_initcolor;
	m_colorlabel->setStyleSheet(QString("QLabel { background-color: rgba(%1,%2,%3,%4);border: 1px solid black; }")
		.arg(m_currentcolor.red()).arg(m_currentcolor.green())
		.arg(m_currentcolor.blue()).arg(m_currentcolor.alpha()));
	m_colorlabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_colorlayout->addWidget(m_colorlabel);
	m_colorlayout->addWidget(m_button);

	QObject::connect(m_button, &QPushButton::clicked, [=]()
		{
			QColor color = QColorDialog::getColor(Qt::white, m_button, "选择颜色");
			if (color.isValid())
			{
				m_currentcolor = color;
				m_colorlabel->setStyleSheet(QString("QLabel { background-color: rgba(%1,%2,%3,%4);border: 1px solid black; }")
					.arg(m_currentcolor.red()).arg(m_currentcolor.green())
					.arg(m_currentcolor.blue()).arg(m_currentcolor.alpha()));
				this->slotValueChanged();
			}
		});
}

void colorDelete::setValue(QVariant value)
{
	m_currentcolor = isDataCanConvert<QColor>(value);
	m_colorlabel->setStyleSheet(QString("QLabel { background-color: rgba(%1,%2,%3,%4);border: 1px solid black; }")
		.arg(m_currentcolor.red()).arg(m_currentcolor.green())
		.arg(m_currentcolor.blue()).arg(m_currentcolor.alpha()));
}

QVariant colorDelete::value()
{
	return QVariant::fromValue(m_currentcolor);
}

stringDelegate::stringDelegate(std::shared_ptr<IdelegatePramas> params)
	: m_label(nullptr)
{
	createWidget(params);
}

QWidget* stringDelegate::getEditWidget()
{
	return m_label;
}

void stringDelegate::createWidget(std::shared_ptr<IdelegatePramas> params)
{
	auto p = isParamsCastValid<delegateParamsString>(params);
	m_label = new QLabel(p->m_initstring);
	m_label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void stringDelegate::setValue(QVariant value)
{
	m_label->setText(isDataCanConvert<QString>(value));
}

QVariant stringDelegate::value()
{
	return QVariant::fromValue(m_label->text());
}

enumDelegate::enumDelegate(std::shared_ptr<IdelegatePramas> params)
	:m_combobox(nullptr)
{
	createWidget(params);
}

QWidget* enumDelegate::getEditWidget()
{
	return m_combobox;
}

void enumDelegate::createWidget(std::shared_ptr<IdelegatePramas> params)
{
	auto p = isParamsCastValid<delegateParamsEnum>(params);

	m_combobox = new QComboBox();
	m_combobox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	for (const auto& str : p->m_vec)
	{
		m_combobox->addItem(str);
	}
	int index = m_combobox->findText(p->m_initstring);
	if (index == -1)
		m_combobox->setCurrentIndex(0);
	else
		m_combobox->setCurrentIndex(index);

	QObject::connect(m_combobox, &QComboBox::currentTextChanged, this, &IpropertyDelegate::slotValueChanged);
}

void enumDelegate::setValue(QVariant value)
{
	int index = m_combobox->findText(isDataCanConvert<QString>(value));
	if (index == -1)
		m_combobox->setCurrentIndex(0);
	else
		m_combobox->setCurrentIndex(index);

}

QVariant enumDelegate::value()
{
	return QVariant::fromValue(m_combobox->currentText());
}


triangleSideRadioDelegate::triangleSideRadioDelegate(std::shared_ptr<IdelegatePramas> params)
	:m_radiowidget(nullptr)
	, m_widget(nullptr)
	, m_bottombox(nullptr)
	, m_leftbox(nullptr)
	, m_rightbox(nullptr)
	, m_formlayout(nullptr)
	, m_button(nullptr)
	, m_vlayout(nullptr)
{
	createWidget(params);
}

QWidget* triangleSideRadioDelegate::getEditWidget()
{
	return m_widget;
}

void triangleSideRadioDelegate::onisValid()
{
	int bottom = m_bottombox->value();
	int left = m_leftbox->value();
	int right = m_rightbox->value();
	if ((bottom + left <= right) || (bottom + right <= left) || (left + right <= bottom))
	{
		QMessageBox::warning(m_button, "warning", "不合法三边");
		return;
	}
	else
	{
		slotValueChanged();
	}
}

void triangleSideRadioDelegate::createWidget(std::shared_ptr<IdelegatePramas> params)
{
	auto castparams = isParamsCastValid<delegateParamsTriangleSides>(params);

	m_radiowidget = new QWidget();
	m_radiowidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_formlayout = new QFormLayout(m_radiowidget);
	m_radiowidget->setLayout(m_formlayout);

	m_bottombox = new QSpinBox();
	m_bottombox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_bottombox->setRange(0, castparams->m_Radiomax);
	m_bottombox->setValue(castparams->m_radios.m_bottom);

	m_leftbox = new QSpinBox();
	m_leftbox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_leftbox->setRange(0, castparams->m_Radiomax);
	m_leftbox->setValue(castparams->m_radios.m_left);

	m_rightbox = new QSpinBox();
	m_rightbox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_rightbox->setRange(0, castparams->m_Radiomax);
	m_rightbox->setValue(castparams->m_radios.m_right);

	m_formlayout->addRow(castparams->m_bottomstr, m_bottombox);
	m_formlayout->addRow(castparams->m_leftstr, m_leftbox);
	m_formlayout->addRow(castparams->m_rightstr, m_rightbox);

	m_button = new QPushButton("确认");
	m_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	QObject::connect(m_button, &QPushButton::clicked, this, &triangleSideRadioDelegate::onisValid);



	m_widget = new QWidget();
	m_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_vlayout = new QVBoxLayout();
	m_widget->setLayout(m_vlayout);

	m_vlayout->addWidget(m_radiowidget);
	m_vlayout->addWidget(m_button);
}

void triangleSideRadioDelegate::setValue(QVariant value)
{
	auto radios = isDataCanConvert<DiagramDrawParamsTriangle::TriangleSizeRadios>(value);
	m_bottombox->setValue(radios.m_bottom);
	m_leftbox->setValue(radios.m_left);
	m_rightbox->setValue(radios.m_right);
}

QVariant triangleSideRadioDelegate::value()
{
	DiagramDrawParamsTriangle::TriangleSizeRadios radios;
	radios.m_bottom = m_bottombox->value();
	radios.m_left = m_leftbox->value();
	radios.m_right = m_rightbox->value();
	QVariant value = QVariant::fromValue(radios);
	return value;
}
