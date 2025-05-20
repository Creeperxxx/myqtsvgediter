#pragma once
#include <qstring.h>
#include <qobject.h>
#include <qsize.h>
#include <qpen.h>
#include <qbrush.h>
#include <qlineedit.h>
#include <qdatastream.h>

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



class IDidgramDrawParams : QObject
{
	Q_OBJECT
signals:
	void SignalParamsChanged();

public:
	IDidgramDrawParams(const IDidgramDrawParams& other);
	IDidgramDrawParams();

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
	bool m_isdrawInHuabu;

	bool m_ischoosed = false;
};

class DiagramDrawParamsRect : public IDidgramDrawParams
{
public:
	DiagramDrawParamsRect(const DiagramDrawParamsRect& other);
	DiagramDrawParamsRect();
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

	DiagramDrawParamsCircle(const DiagramDrawParamsCircle& other);
	DiagramDrawParamsCircle();
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
	DiagramDrawParamsTriangle(const DiagramDrawParamsTriangle& other);
	DiagramDrawParamsTriangle();
	class TriangleSizeRadios
	{
	public:
		TriangleSizeRadios();
		TriangleSizeRadios(int bottom, int left, int right);
		int m_bottom;
		int m_left;
		int m_right;
	};


	TriangleSizeRadios m_triangleSizeRadios;
	EdgeType m_edgetype;

};

Q_DECLARE_METATYPE(DiagramDrawParamsTriangle::TriangleSizeRadios)

class DiagramDrawParamsLine : public IDidgramDrawParams
{
public:
	DiagramDrawParamsLine(const DiagramDrawParamsLine& other);
	DiagramDrawParamsLine();
	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;
};

class DiagramDrawParamsMouse : public IDidgramDrawParams
{
public:
	DiagramDrawParamsMouse(const DiagramDrawParamsMouse& other);
	DiagramDrawParamsMouse();
	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;

};

class DiagramDrawParamsChoose : public IDidgramDrawParams
{
public:
	DiagramDrawParamsChoose(const DiagramDrawParamsChoose& other);
	DiagramDrawParamsChoose();
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
	DiagramDrawParamsText(const DiagramDrawParamsText& other);
	DiagramDrawParamsText();

	void serialize(QDataStream& out) const override;
	void deserialize(QDataStream& in) override;

	QFont m_font;
	TextLineEdit* m_textedit;
};
