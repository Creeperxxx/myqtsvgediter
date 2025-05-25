
#include "myconfig.h"
#include "diagramdrawparams.h"
#include "qpainterpath.h"

//QString ShapeTypeTool::shapetypeEnumToQstring(ShapeType type)
//{
	//switch (type)
	//{
//	case ShapeType::Rect:
//		return myconfig::getInstance().getRectName();
//		break;
//	case ShapeType::Circle:
//		return myconfig::getInstance().getCircleName();
//		break;
//	case ShapeType::Triangle:
//		return myconfig::getInstance().getTriangleName();
//		break;
//	case ShapeType::Line:
//		return myconfig::getInstance().getLineName();
//		break;
//	case ShapeType::Mouse:
//		return myconfig::getInstance().getMouseName();
//		break;
//	case ShapeType::Text:
//		return myconfig::getInstance().getTextName();
//		break;
//	default:
//		throw std::runtime_error("error");
//		break;
//	}
//}
//
//ShapeType ShapeTypeTool::shapetypeQstringToEnum(const QString& type)
//{
//	if (type == myconfig::getInstance().getRectName())
//	{
//		return ShapeType::Rect;
//	}
//	else if (type == myconfig::getInstance().getCircleName())
//	{
//		return ShapeType::Circle;
//	}
//	else if (type == myconfig::getInstance().getTriangleName())
//	{
//		return ShapeType::Triangle;
//	}
//	else if (type == myconfig::getInstance().getLineName())
//	{
//		return ShapeType::Line;
//	}
//	else if (type == myconfig::getInstance().getMouseName())
//	{
//		return ShapeType::Mouse;
//	}
//	else if (type == myconfig::getInstance().getChooseName())
//	{
//		return ShapeType::choose;
//	}
//	else if (type == myconfig::getInstance().getTextName())
//	{
//		return ShapeType::Text;
//	}
//	else
//	{
//		throw std::runtime_error("error");
//	}
//}

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
	m_type = static_cast<myqtsvg::ShapeType>(inttype);

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

QPoint IDidgramDrawParams::getCenter()
{
	return m_center;
}

void IDidgramDrawParams::setCenter(QPoint center)
{
	m_center = center;
}

QSize IDidgramDrawParams::getSpacesize()
{
	return m_spacesize;
}

void IDidgramDrawParams::setSpacesize(QSize size)
{
	m_spacesize = size;
}

void IDidgramDrawParams::setType(myqtsvg::ShapeType type)
{
	m_type = type;
}

myqtsvg::ShapeType IDidgramDrawParams::getType()
{
	return m_type;
}

void IDidgramDrawParams::setScale(double scale)
{
	m_scale = scale;
}

double IDidgramDrawParams::getScale()
{
	return m_scale;
}

void IDidgramDrawParams::setPenColor(QColor color)
{
	m_pen.setColor(color);
}

QColor IDidgramDrawParams::getPenColor()
{
	return m_pen.color();
}

void IDidgramDrawParams::setPenwidth(int width)
{
	m_pen.setWidth(width);
}

int IDidgramDrawParams::getPenwidth()
{
	return m_pen.width();
}

void IDidgramDrawParams::setPenStyle(Qt::PenStyle style)
{
	m_pen.setStyle(style);
}

Qt::PenStyle IDidgramDrawParams::getPenstyle()
{
	return m_pen.style();
}

void IDidgramDrawParams::setPen(QPen pen)
{
	m_pen = pen;
}

QPen IDidgramDrawParams::getPen()
{
	return m_pen;
}

void IDidgramDrawParams::setBrushColor(QColor color)
{
	m_brush.setColor(color);
}

QColor IDidgramDrawParams::getBrushColor()
{
	return m_brush.color();
}

void IDidgramDrawParams::setRotate(int rotate)
{
	m_rotate = rotate;
}

int IDidgramDrawParams::getRotate()
{
	return m_rotate;
}

void IDidgramDrawParams::setCenterHOffset(int offset)
{
	m_centerHoffset = offset;
}

int IDidgramDrawParams::getCenterHOffset()
{
	return m_centerHoffset;
}

void IDidgramDrawParams::setCenterVOffset(int offset)
{
	m_centerVoffset = offset;
}

int IDidgramDrawParams::getCenterVOffset()
{
	return m_centerVoffset;
}

void IDidgramDrawParams::setIsDrawInCanvas(bool flag)
{
	m_isdrawInHuabu = flag;
}

bool IDidgramDrawParams::getIsDrawInCanvas()
{
	return m_isdrawInHuabu;
}



DiagramDrawParamsRect::DiagramDrawParamsRect(const DiagramDrawParamsRect& other)
	:IDidgramDrawParams(other)
{
	m_radio = other.m_radio;
}

DiagramDrawParamsRect::DiagramDrawParamsRect()
{

}

std::shared_ptr<IDidgramDrawParams> DiagramDrawParamsRect::clone()
{
	return std::make_shared<DiagramDrawParamsRect>(*this);
}

void DiagramDrawParamsRect::serialize(QDataStream& out) const
{
	IDidgramDrawParams::serialize(out);
	out << m_radio;
}

void DiagramDrawParamsRect::deserialize(QDataStream& in)
{
	IDidgramDrawParams::deserialize(in);
	in >> m_radio;
}

void DiagramDrawParamsRect::setRadio(double radio)
{
	m_radio = radio;
}

double DiagramDrawParamsRect::getRadio()
{
	return m_radio;
}

void DiagramDrawParamsCircle::serialize(QDataStream& out) const
{
	IDidgramDrawParams::serialize(out);
	out << m_radio;
}

void DiagramDrawParamsCircle::deserialize(QDataStream& in)
{
	IDidgramDrawParams::deserialize(in);
	in >> m_radio;
}

std::shared_ptr<IDidgramDrawParams> DiagramDrawParamsCircle::clone()
{
	return std::make_shared<DiagramDrawParamsCircle>(*this);
}

DiagramDrawParamsCircle::DiagramDrawParamsCircle(const DiagramDrawParamsCircle& other)
	:IDidgramDrawParams(other)
{
	m_radio = other.m_radio;
}

DiagramDrawParamsCircle::DiagramDrawParamsCircle()
{

}

void DiagramDrawParamsCircle::setRadio(double radio)
{
	m_radio = radio;
}

double DiagramDrawParamsCircle::getRadio()
{
	return m_radio;
}

void DiagramDrawParamsTriangle::serialize(QDataStream& out) const
{
	IDidgramDrawParams::serialize(out);
	out << m_sideRadios.m_bottom
		<< m_sideRadios.m_left
		<< m_sideRadios.m_right
		<< static_cast<int>(m_edgetype);
}

void DiagramDrawParamsTriangle::deserialize(QDataStream& in)
{
	IDidgramDrawParams::deserialize(in);
	in >> m_sideRadios.m_bottom
		>> m_sideRadios.m_left
		>> m_sideRadios.m_right;
	int value;
	in >> value;
	m_edgetype = static_cast<EdgeType>(value);
}

std::shared_ptr<IDidgramDrawParams> DiagramDrawParamsTriangle::clone()
{
	return std::make_shared<DiagramDrawParamsTriangle>(*this);
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
	m_sideRadios.m_bottom = other.m_sideRadios.m_bottom;
	m_sideRadios.m_left = other.m_sideRadios.m_left;
	m_sideRadios.m_right = other.m_sideRadios.m_right;
	m_edgetype = other.m_edgetype;
}

DiagramDrawParamsTriangle::DiagramDrawParamsTriangle()
{

}

DiagramDrawParamsTriangle::sideRadios::sideRadios()
	:m_bottom(0)
	, m_left(0)
	, m_right(0)
{
}

DiagramDrawParamsTriangle::sideRadios::sideRadios(int bottom, int left, int right)
	: m_bottom(bottom)
	, m_left(left)
	, m_right(right) {
}

void DiagramDrawParamsTriangle::sideRadios::setBottom(int bottom)
{
	m_bottom = bottom;
}

int DiagramDrawParamsTriangle::sideRadios::getBottom()
{
	return m_bottom;
}

void DiagramDrawParamsTriangle::sideRadios::setLeft(int left)
{
	m_left = left;
}

int DiagramDrawParamsTriangle::sideRadios::getLeft()
{
	return m_left;
}

void DiagramDrawParamsTriangle::sideRadios::setRight(int right)
{
	m_right = right;
}

int DiagramDrawParamsTriangle::sideRadios::getRight()
{
	return m_right;
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

std::shared_ptr<IDidgramDrawParams> DiagramDrawParamsLine::clone()
{
	return std::make_shared<DiagramDrawParamsLine>(*this);
}

DiagramDrawParamsMouse::DiagramDrawParamsMouse(const DiagramDrawParamsMouse& other)
	:IDidgramDrawParams(other)
	//, m_path(std::make_shared<QPainterPath>(*other.m_path))
	, m_path(other.m_path == nullptr ? std::make_shared<QPainterPath>() : std::make_shared<QPainterPath>(*other.m_path))
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

std::shared_ptr<IDidgramDrawParams> DiagramDrawParamsMouse::clone()
{
	return std::make_shared<DiagramDrawParamsMouse>(*this);
}

void DiagramDrawParamsMouse::setPath(std::shared_ptr<QPainterPath> path)
{
	m_path = path;
}

std::shared_ptr<QPainterPath> DiagramDrawParamsMouse::getPaht()
{
	return m_path;
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

std::shared_ptr<IDidgramDrawParams> DiagramDrawParamsChoose::clone()
{
	return std::make_shared<DiagramDrawParamsChoose>(*this);
}

TextLineEdit::TextLineEdit(QWidget* parent)
	:QLineEdit(parent)
{
	setFrame(false);
	setAlignment(Qt::AlignCenter);

	setStyleSheet("background:transparent;");

	QObject::connect(this, &TextLineEdit::textChanged, this, &TextLineEdit::adjustsize);
	QObject::connect(this, &TextLineEdit::editingFinished, this, &TextLineEdit::signalHasFocusOut);
	QObject::connect(this, &TextLineEdit::signalHasFocusOut, this, [this]() {
		this->hide();
		});
}

TextLineEdit::TextLineEdit(const TextLineEdit& other)
{
	if (other.parentWidget() != nullptr)
		setParent(other.parentWidget());
	setTextColor(other.m_textcolor);
	setText(other.text());
	setFont(other.font());
	adjustsize();
	
	move(other.pos());
}


void TextLineEdit::setTextColor(QColor textcolor)
{
	setStyleSheet(QString("color: rgba(%1,%2,%3,%4);")
		.arg(textcolor.red()).arg(textcolor.green())
		.arg(textcolor.blue()).arg(textcolor.alpha()));
	m_textcolor = textcolor;
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
	emit signalHasFocusOut();
	QLineEdit::focusOutEvent(event);
}

void TextLineEdit::adjustsize()
{
	QFontMetrics fm(font());
	int width = fm.horizontalAdvance(text()) + 10;
	int height = fm.height() + 5;

	setFixedSize(width, height);
}

void TextLineEdit::createTextLineEdit(std::shared_ptr<IDidgramDrawParams> params, QWidget* parent)
{
	if (params == nullptr)
		throw std::runtime_error("error");
	auto p = std::dynamic_pointer_cast<DiagramDrawParamsText>(params);
	if (p == nullptr)
		throw std::runtime_error("error");

	auto textedit = new TextLineEdit(parent);
	textedit->setTextColor(p->getPenColor());
	textedit->setText("编辑");
	textedit->setFont(p->getFont());
	textedit->adjustsize();
	
	QSize size = textedit->size();
	QPoint topleft = QPoint(p->getCenter().x() - size.width() / 2, p->getCenter().y() - size.height() / 2);
	textedit->move(topleft);

    textedit->show();
	p->setTextEdit(textedit);

}

DiagramDrawParamsText::DiagramDrawParamsText(const DiagramDrawParamsText& other)
	:IDidgramDrawParams(other)
{
	m_font = other.m_font;
	if (other.m_textedit == nullptr)
		m_textedit = nullptr;
	else
		m_textedit = new TextLineEdit(*other.m_textedit);
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

std::shared_ptr<IDidgramDrawParams> DiagramDrawParamsText::clone()
{
	return std::make_shared<DiagramDrawParamsText>(*this);
}

void DiagramDrawParamsText::setFontFamily(QString family)
{
	m_font.setFamily(family);
}

QString DiagramDrawParamsText::getFontFamily()
{
	return m_font.family();
}

void DiagramDrawParamsText::setFontSize(int size)
{
	m_font.setPointSize(size);
}

int DiagramDrawParamsText::getFontSize()
{
	return m_font.pointSize();
}

void DiagramDrawParamsText::setFont(QFont font)
{
	m_font = font;
}

QFont DiagramDrawParamsText::getFont()
{
	return m_font;
}

void DiagramDrawParamsText::setTextEdit(TextLineEdit* edit)
{
	m_textedit = edit;
}

TextLineEdit* DiagramDrawParamsText::getTextEdit()
{
	return m_textedit;
}

void DiagramDrawParamsTriangle::setBottomRadio(int bottom)
{
	m_sideRadios.setBottom(bottom);
}

void DiagramDrawParamsTriangle::setLeftRadio(int left)
{
	return m_sideRadios.setLeft(left);
}

void DiagramDrawParamsTriangle::setRightRadio(int right)
{
	m_sideRadios.setRight(right);
}

int DiagramDrawParamsTriangle::getBottomRadio()
{
	return m_sideRadios.getBottom();
}

int DiagramDrawParamsTriangle::getLeftRadio()
{
	return m_sideRadios.getLeft();
}

int DiagramDrawParamsTriangle::getRightRadio()
{
	return m_sideRadios.getRight();
}

void DiagramDrawParamsTriangle::setRadios(sideRadios radios)
{
	m_sideRadios = radios;
}

DiagramDrawParamsTriangle::sideRadios DiagramDrawParamsTriangle::getRadios()
{
	return m_sideRadios;
}

void DiagramDrawParamsTriangle::setEdgeType(EdgeType type)
{
	m_edgetype = type;
}

DiagramDrawParamsTriangle::EdgeType DiagramDrawParamsTriangle::getEdgeType()
{
	return m_edgetype;
}
