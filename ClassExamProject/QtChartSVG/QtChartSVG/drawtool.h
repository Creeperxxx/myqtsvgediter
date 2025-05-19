#pragma once
#include <memory>
#include <qpoint.h>
#include <qbrush.h>
#include <qpen.h>
#include <stdexcept>
#include <qstring.h>
#include <qfont.h>
#include <qlineedit.h>
#include "configmanager.h"
#include "config.h"


constexpr auto rectname = "rect";
constexpr auto circlename = "circle";
constexpr auto trianglename = "triangle";
constexpr auto linename = "line";


enum class ShapeType
{
	Rect,
	Circle,
	Triangle,
	Line,
	Mouse,
	choose,
	Text
};

namespace ShapeTypeTool
{
	QString shapetypeEnumToQstring(ShapeType type);
	ShapeType shapetypeQstringToEnum(const QString& type);
}

//旋转角度由double变int


class IDidgramDrawParams : QObject
{
	Q_OBJECT
public:
signals:
	void SignalParamsChanged();


public:
	IDidgramDrawParams(const IDidgramDrawParams& other);
	IDidgramDrawParams() {}
	virtual ~IDidgramDrawParams() {}

	friend QDataStream& operator<<(QDataStream& out, const IDidgramDrawParams& params);
	friend QDataStream& operator>>(QDataStream& in, IDidgramDrawParams& params);
	virtual void serialize(QDataStream& out) const;
	virtual void deserialize(QDataStream& in);


	QPoint m_center;
	QSize m_spacesize;
	ShapeType m_type;
	double m_scale;
	QPen m_pen;
	QBrush m_brush;
	int m_rotate;
	int m_centerHoffset;
	int m_centerVoffset;
	bool m_paramChanged = true;
	bool m_isdrawInHuabu;
	
	bool m_ischoosed = false;
};

class DiagramDrawParamsRect : public IDidgramDrawParams
{
public:
	~DiagramDrawParamsRect() {}
	DiagramDrawParamsRect(const DiagramDrawParamsRect& other);
	DiagramDrawParamsRect() {}
	void serialize(QDataStream& out) const override;
    void deserialize(QDataStream& in) override;


	double m_boundingrectradio;
};

class DiagramDrawParamsCircle :public IDidgramDrawParams
{
public:
	double m_boundingrectradio;
	void serialize(QDataStream& out) const override;
    void deserialize(QDataStream& in) override;

	~DiagramDrawParamsCircle() {}
	DiagramDrawParamsCircle(const DiagramDrawParamsCircle& other);
	DiagramDrawParamsCircle() {}
};

class DiagramDrawParamsTriangle : public IDidgramDrawParams
{
public:
	void serialize(QDataStream& out) const override;
    void deserialize(QDataStream& in) override;

	enum class EdgeType
	{
		Left,
		Right,
		Bottom
	};
	static EdgeType edgetypeStringToEnum(const QString& edgetype);
	static QString edgetypeEnumToString(EdgeType edgetype);
	~DiagramDrawParamsTriangle() {}
	DiagramDrawParamsTriangle(const DiagramDrawParamsTriangle& other);
	DiagramDrawParamsTriangle() {}
	class TriangleSizeRadios
	{
	public:
		TriangleSizeRadios();
		TriangleSizeRadios(qreal bottom, qreal left, qreal right);
		qreal m_bottom;
		qreal m_left;
		qreal m_right;
	};
	TriangleSizeRadios m_triangleSizeRadios;
	EdgeType m_edgetype;

};

Q_DECLARE_METATYPE(DiagramDrawParamsTriangle::TriangleSizeRadios)

class DiagramDrawParamsLine : public IDidgramDrawParams
{
public:
	~DiagramDrawParamsLine() {}
	DiagramDrawParamsLine(const DiagramDrawParamsLine& other);
	DiagramDrawParamsLine() {}
	void serialize(QDataStream& out) const override;
    void deserialize(QDataStream& in) override;
};

class DiagramDrawParamsMouse : public IDidgramDrawParams 
{
public:
	~DiagramDrawParamsMouse() {}
	DiagramDrawParamsMouse(const DiagramDrawParamsMouse& other);
	DiagramDrawParamsMouse() {}
	void serialize(QDataStream& out) const override;
    void deserialize(QDataStream& in) override;


};

class DiagramDrawParamsChoose : public IDidgramDrawParams
{
public:
	~DiagramDrawParamsChoose() {}
	DiagramDrawParamsChoose(const DiagramDrawParamsChoose& other);
	DiagramDrawParamsChoose() {}
	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;
    
};


class TextLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	TextLineEdit(QWidget* parent = nullptr);
	void setTextColor(QColor textcolor);
	void setBackGroundColor(QColor color);

	void showEvent(QShowEvent* event)override;
	void setFont(const QFont& font);

	void focusOutEvent(QFocusEvent* event)override;

	void adjustsize();
signals:
	void signalHasFocusOut();
};

class DiagramDrawParamsText : public IDidgramDrawParams
{
public:
	~DiagramDrawParamsText() {}
	DiagramDrawParamsText(const DiagramDrawParamsText& other);
	DiagramDrawParamsText() {}

	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;

	QFont m_font;
	TextLineEdit* m_textedit;
};






class ICreateParams
{
public:
	std::shared_ptr<IDidgramDrawParams> create();
protected:
	virtual std::shared_ptr<IDidgramDrawParams> createSpecial() = 0;
private:
	void createRest(std::shared_ptr<IDidgramDrawParams> params);
};

class createParamsRect : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial() override;
};

class createParamsCircle : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial() override;
};

class createParamsTriangle : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial() override;
};

class createParamsLine : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial() override;
};

class createParamsMouse : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial()override;
};

class createParamsChoose : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial()override;
};

class createParamsText : public ICreateParams
{
protected:
	std::shared_ptr<IDidgramDrawParams> createSpecial()override;
};

class createParamsInterface
{
public:
	static createParamsInterface& getInstance();
	createParamsInterface& add(ShapeType type, std::function<std::shared_ptr<ICreateParams>()> creator);
	std::shared_ptr<ICreateParams> getParams(ShapeType type);
private:
	createParamsInterface();

	std::map<ShapeType, std::function<std::shared_ptr<ICreateParams>()>> m_map;

};

























