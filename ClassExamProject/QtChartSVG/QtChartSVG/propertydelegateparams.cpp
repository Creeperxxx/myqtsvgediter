#include "propertydelegateparams.h"

IdelegatePramas::IdelegatePramas(delegateType type)
	:m_type(type)
{
}

IdelegatePramas::~IdelegatePramas()
{
}

delegateParamsDouble::~delegateParamsDouble()
{
}

delegateParamsInt::~delegateParamsInt()
{
}

delegateParamsColor::~delegateParamsColor()
{
}

delegateParamsEnum::~delegateParamsEnum()
{
}

delegateParamsString::~delegateParamsString()
{
}

delegateParamsTriangleSides::~delegateParamsTriangleSides()
{
}


delegateParamsDouble::delegateParamsDouble(double max, double min, double step, int decimals, double initvalue)
	:m_valuemax(max)
	, m_valuemin(min)
	, m_valuestep(step)
	, m_valuedecimals(decimals)
	, m_initvalue(initvalue)
	, IdelegatePramas(delegateType::Double)
{
}

delegateParamsInt::delegateParamsInt(int max, int min, int step, int initvalue)
	:m_valuemax(max)
	, m_valuemin(min)
	, m_valuestep(step)
	, m_initvalue(initvalue)
	, IdelegatePramas(delegateType::Int)
{
}

delegateParamsColor::delegateParamsColor(QColor initcolor)
	:m_initcolor(initcolor)
	, IdelegatePramas(delegateType::Color)
{
}

delegateParamsEnum::delegateParamsEnum(QVector<QString> vec, QString initstr)
	: IdelegatePramas(delegateType::Enum)
	, m_vec(vec)
	, m_initstring(initstr)
{
}

delegateParamsString::delegateParamsString(QString str)
	:m_initstring(str)
	, IdelegatePramas(delegateType::String)
{
}

delegateParamsTriangleSides::delegateParamsTriangleSides(QString bottomstr, QString leftstr, QString rightstr, int radiomax)
	:IdelegatePramas(delegateType::TriangleSides)
	, m_bottomstr(bottomstr)
	, m_leftstr(leftstr)
	, m_rightstr(rightstr)
	, m_Radiomax(radiomax)
{

}
