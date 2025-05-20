#pragma once
#include <qcolor.h>
#include <vector>
#include <memory>
#include "diagramdrawparams.h"


enum class delegateType
{
	Double,
	Int,
	Enum,
	Color,
	String,
	TriangleSides
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
	~delegateParamsDouble();
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
	~delegateParamsInt();
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
	~delegateParamsColor();
	delegateParamsColor(QColor initcolor);
	QColor m_initcolor;
};

class delegateParamsEnum : public IdelegatePramas
{
public:
	~delegateParamsEnum();
	delegateParamsEnum(std::vector<QString> vec, QString initstr);
	std::vector<QString> m_vec;
	QString m_initstring;
};

class delegateParamsString : public IdelegatePramas
{
public:
	~delegateParamsString();
	delegateParamsString(QString str);
	QString m_initstring;
};

class delegateParamsTriangleSides : public IdelegatePramas
{
public:
	delegateParamsTriangleSides(QString bottomstr
		, QString leftstr
		, QString rightstr
		, int radiomax);
	~delegateParamsTriangleSides();
	QString m_bottomstr;
	QString m_leftstr;
	QString m_rightstr;
	int m_Radiomax;
	DiagramDrawParamsTriangle::TriangleSizeRadios m_radios;
};
