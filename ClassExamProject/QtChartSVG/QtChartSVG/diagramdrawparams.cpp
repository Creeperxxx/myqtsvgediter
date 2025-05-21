#pragma once
#include "myconfig.h"
#include "diagramdrawparams.h"

QString ShapeTypeTool::shapetypeEnumToQstring(ShapeType type)
{
	switch (type)
	{
	case ShapeType::Rect:
		return myconfig::getInstance().getRectName();
		break;
	case ShapeType::Circle:
		return myconfig::getInstance().getCircleName();
		break;
	case ShapeType::Triangle:
		return myconfig::getInstance().getTriangleName();
		break;
	case ShapeType::Line:
		return myconfig::getInstance().getLineName();
		break;
	case ShapeType::Mouse:
		return myconfig::getInstance().getMouseName();
		break;
	case ShapeType::Text:
		return myconfig::getInstance().getTextName();
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

ShapeType ShapeTypeTool::shapetypeQstringToEnum(const QString& type)
{
	if (type == myconfig::getInstance().getRectName())
	{
		return ShapeType::Rect;
	}
	else if (type == myconfig::getInstance().getCircleName())
	{
		return ShapeType::Circle;
	}
	else if (type == myconfig::getInstance().getTriangleName())
	{
		return ShapeType::Triangle;
	}
	else if (type == myconfig::getInstance().getLineName())
	{
		return ShapeType::Line;
	}
	else if (type == myconfig::getInstance().getMouseName())
	{
		return ShapeType::Mouse;
	}
	else if (type == myconfig::getInstance().getChooseName())
	{
		return ShapeType::choose;
	}
	else if (type == myconfig::getInstance().getTextName())
	{
		return ShapeType::Text;
	}
	else
	{
		throw std::runtime_error("error");
	}
}

IDidgramDrawParams::IDidgramDrawParams(const IDidgramDrawParams& other)
{
	m_center = other.m_center;
	m_spacesize = other.m_spacesize;
	m_type = other.m_type;
	m_scale = other.m_scale;
	m_pen = other.m_pen;
	m_brush = other.m_brush;
	m_rotate = other.m_rotate;
	m_centerHoffset = other.m_centerHoffset;
	m_centerVoffset = other.m_centerVoffset;
	m_isdrawInHuabu = other.m_isdrawInHuabu;
}
IDidgramDrawParams::IDidgramDrawParams()
{

}

QDataStream& operator<<(QDataStream& out, const IDidgramDrawParams& params)
{
	params.serialize(out);
	return out;
}

QDataStream& operator>>(QDataStream& in, IDidgramDrawParams& params)
{
	params.deserialize(in);
	return in;
}

void IDidgramDrawParams::serialize(QDataStream& out) const
{
	out << m_spacesize.width()
		<< m_spacesize.height()
		<< static_cast<int>(m_type)
		<< m_scale
		<< m_pen.color()
		<< m_pen.width()
		<< m_brush.color()
		<< m_rotate
		<< m_centerHoffset
		<< m_centerVoffset;
}

void IDidgramDrawParams::deserialize(QDataStream& in)
{
	int width = 0;
	int height = 0;
	in >> width
		>> height;
	m_spacesize = QSize(width, height);

	int inttype = 0;
	in >> inttype;
	m_type = static_cast<ShapeType>(inttype);

	in >> m_scale;

	QColor color;
	int penwidth;
	in >> color >> penwidth;
	m_pen = QPen(color, penwidth);

	in >> color;
	m_brush = QBrush(color);

	in >> m_rotate;

	in >> m_centerHoffset;
	in >> m_centerVoffset;


}

DiagramDrawParamsRect::DiagramDrawParamsRect(const DiagramDrawParamsRect& other)
	:IDidgramDrawParams(other)
{
	m_boundingrectradio = other.m_boundingrectradio;
}

DiagramDrawParamsRect::DiagramDrawParamsRect()
{

}

void DiagramDrawParamsRect::serialize(QDataStream& out) const
{
	IDidgramDrawParams::serialize(out);
	out << m_boundingrectradio;
}

void DiagramDrawParamsRect::deserialize(QDataStream& in)
{
	IDidgramDrawParams::deserialize(in);
	in >> m_boundingrectradio;
}

void DiagramDrawParamsCircle::serialize(QDataStream& out) const
{
	IDidgramDrawParams::serialize(out);
	out << m_boundingrectradio;
}

void DiagramDrawParamsCircle::deserialize(QDataStream& in)
{
	IDidgramDrawParams::deserialize(in);
	in >> m_boundingrectradio;
}

DiagramDrawParamsCircle::DiagramDrawParamsCircle(const DiagramDrawParamsCircle& other)
	:IDidgramDrawParams(other)
{
	m_boundingrectradio = other.m_boundingrectradio;
}

DiagramDrawParamsCircle::DiagramDrawParamsCircle()
{

}

void DiagramDrawParamsTriangle::serialize(QDataStream& out) const
{
	IDidgramDrawParams::serialize(out);
	out << m_triangleSizeRadios.m_bottom
		<< m_triangleSizeRadios.m_left
		<< m_triangleSizeRadios.m_right
		<< static_cast<int>(m_edgetype);
}

void DiagramDrawParamsTriangle::deserialize(QDataStream& in)
{
	IDidgramDrawParams::deserialize(in);
	in >> m_triangleSizeRadios.m_bottom
		>> m_triangleSizeRadios.m_left
		>> m_triangleSizeRadios.m_right;
	int value;
	in >> value;
	m_edgetype = static_cast<EdgeType>(value);
}

DiagramDrawParamsTriangle::EdgeType DiagramDrawParamsTriangle::edgetypeStringToEnum(const QString& str)
{
	if (str == myconfig::getInstance().getTriangleRadioBottomName())
		return EdgeType::Bottom;
	else if (str == myconfig::getInstance().getTriangleRadioLeftName())
		return EdgeType::Left;
	else if (str == myconfig::getInstance().getTriangleRadioRightName())
		return EdgeType::Right;
	else
		throw std::runtime_error("error");
}

QString DiagramDrawParamsTriangle::edgetypeEnumToString(EdgeType edgetype)
{
	switch (edgetype)
	{
	case DiagramDrawParamsTriangle::EdgeType::Left:
		return myconfig::getInstance().getTriangleRadioLeftName();
		break;
	case DiagramDrawParamsTriangle::EdgeType::Right:
		return myconfig::getInstance().getTriangleRadioRightName();
		break;
	case DiagramDrawParamsTriangle::EdgeType::Bottom:
		return myconfig::getInstance().getTriangleRadioBottomName();
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

DiagramDrawParamsTriangle::DiagramDrawParamsTriangle(const DiagramDrawParamsTriangle& other)
	:IDidgramDrawParams(other)
{
	m_triangleSizeRadios.m_bottom = other.m_triangleSizeRadios.m_bottom;
	m_triangleSizeRadios.m_left = other.m_triangleSizeRadios.m_left;
	m_triangleSizeRadios.m_right = other.m_triangleSizeRadios.m_right;
	m_edgetype = other.m_edgetype;
}

DiagramDrawParamsTriangle::DiagramDrawParamsTriangle()
{

}

DiagramDrawParamsTriangle::TriangleSizeRadios::TriangleSizeRadios()
	:m_bottom(0)
	, m_left(0)
	, m_right(0)
{
}

DiagramDrawParamsTriangle::TriangleSizeRadios::TriangleSizeRadios(int bottom, int left, int right)
	: m_bottom(bottom)
	, m_left(left)
	, m_right(right) {
}

DiagramDrawParamsLine::DiagramDrawParamsLine(const DiagramDrawParamsLine& other)
	:IDidgramDrawParams(other)
{
}


DiagramDrawParamsLine::DiagramDrawParamsLine()
{

}

void DiagramDrawParamsLine::serialize(QDataStream& out) const
{
	IDidgramDrawParams::serialize(out);
}

void DiagramDrawParamsLine::deserialize(QDataStream& in)
{
	IDidgramDrawParams::deserialize(in);
}

DiagramDrawParamsMouse::DiagramDrawParamsMouse(const DiagramDrawParamsMouse& other)
	:IDidgramDrawParams(other)
{

}

DiagramDrawParamsMouse::DiagramDrawParamsMouse()
{

}

void DiagramDrawParamsMouse::serialize(QDataStream& out) const
{
	IDidgramDrawParams::serialize(out);
}

void DiagramDrawParamsMouse::deserialize(QDataStream& in)
{
	IDidgramDrawParams::deserialize(in);
}

DiagramDrawParamsChoose::DiagramDrawParamsChoose(const DiagramDrawParamsChoose& other)
	:IDidgramDrawParams(other)
{

}

DiagramDrawParamsChoose::DiagramDrawParamsChoose()
{

}

void DiagramDrawParamsChoose::serialize(QDataStream& out) const
{
	IDidgramDrawParams::serialize(out);
}

void DiagramDrawParamsChoose::deserialize(QDataStream& in)
{
	IDidgramDrawParams::deserialize(in);
}

TextLineEdit::TextLineEdit(QWidget* parent)
{
	setFrame(false);
	setAlignment(Qt::AlignCenter);

	QObject::connect(this, &TextLineEdit::textChanged, this, &TextLineEdit::adjustsize);
	QObject::connect(this, &TextLineEdit::editingFinished, this, &TextLineEdit::signalHasFocusOut);
}

void TextLineEdit::setTextColor(QColor textcolor)
{
	setStyleSheet(QString("color: rgba(%1,%2,%3,%4);")
		.arg(textcolor.red()).arg(textcolor.green())
		.arg(textcolor.blue()).arg(textcolor.alpha()));
}

void TextLineEdit::setBackGroundColor(QColor color)
{
	setStyleSheet(QString("background-color: rgba(%1,%2,%3,%4);")
		.arg(color.red()).arg(color.green())
		.arg(color.blue()).arg(color.alpha()));
}

void TextLineEdit::showEvent(QShowEvent* event)
{
	adjustsize();
	QLineEdit::showEvent(event);
}

void TextLineEdit::setFont(const QFont& font)
{
	QLineEdit::setFont(font);
	adjustSize();
}

void TextLineEdit::focusOutEvent(QFocusEvent* event)
{
	hide();
	QLineEdit::focusOutEvent(event);
}

void TextLineEdit::adjustsize()
{
	QFontMetrics fm(font());
	int width = fm.horizontalAdvance(text()) + 10;
	int height = fm.height() + 5;

	setFixedSize(width, height);
}

DiagramDrawParamsText::DiagramDrawParamsText(const DiagramDrawParamsText& other)
	:IDidgramDrawParams(other)
{
	m_font = other.m_font;
}

DiagramDrawParamsText::DiagramDrawParamsText()
{

}

void DiagramDrawParamsText::serialize(QDataStream& out) const
{
	IDidgramDrawParams::serialize(out);
	out << m_font.family()
		<< m_font.pointSize()
		<< m_font.bold()
		<< m_font.italic()
		<< m_font.underline()
		<< m_font.strikeOut();
}

void DiagramDrawParamsText::deserialize(QDataStream& in)
{
	IDidgramDrawParams::deserialize(in);
	QString family;
	int pointSize;
	bool bold;
	bool italic;
	bool underline;
	bool strikeOut;

	in >> family
		>> pointSize
		>> bold
		>> italic
		>> underline
		>> strikeOut;

	m_font = QFont(family, pointSize);
	m_font.setBold(bold);
	m_font.setItalic(italic);
	m_font.setUnderline(underline);
	m_font.setStrikeOut(strikeOut);

}
