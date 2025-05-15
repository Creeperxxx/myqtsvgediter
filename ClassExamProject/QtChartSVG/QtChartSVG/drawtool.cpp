#include "drawtool.h"



DiagramDrawParamsTriangle::TriangleSizeRadios::TriangleSizeRadios()
	:m_bottom(0.0)
	, m_left(0.0)
	, m_right(0.0)
{
}

DiagramDrawParamsTriangle::TriangleSizeRadios::TriangleSizeRadios(qreal bottom, qreal left, qreal right)
	: m_bottom(bottom)
	, m_left(left)
	, m_right(right) {
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
	if (str == "bottom")
		return EdgeType::Bottom;
	else if (str == "left")
		return EdgeType::Left;
	else if (str == "right")
		return EdgeType::Right;
	else
		throw std::runtime_error("error");
}

QString DiagramDrawParamsTriangle::edgetypeEnumToString(EdgeType edgetype)
{
	switch (edgetype)
	{
	case DiagramDrawParamsTriangle::EdgeType::Left:
		return QString("left");
		break;
	case DiagramDrawParamsTriangle::EdgeType::Right:
		return QString("right");
		break;
	case DiagramDrawParamsTriangle::EdgeType::Bottom:
		return QString("bottom");
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}


QString ShapeTypeTool::shapetypeEnumToQstring(ShapeType type)
{
	switch (type)
	{
	case ShapeType::Rect:
		return QString("Rect");
		break;
	case ShapeType::Circle:
		return QString("Circle");
		break;
	case ShapeType::Triangle:
		return QString("Triangle");
		break;
	case ShapeType::Line:
		return QString("Line");
		break;
	default:
		throw std::runtime_error("error");
	}
}

ShapeType ShapeTypeTool::shapetypeQstringToEnum(const QString& type)
{
	if (type == "Rect")
	{
		return ShapeType::Rect;
	}
	else if (type == "Circle")
	{
		return ShapeType::Circle;
	}
	else if (type == "Triangle")
	{
		return ShapeType::Triangle;
	}
	else if (type == "Line")
	{
		return ShapeType::Line;
	}
	else
	{
		throw std::runtime_error("error");
	}
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

void DiagramDrawParamsLine::serialize(QDataStream& out) const
{
	IDidgramDrawParams::serialize(out);
}

void DiagramDrawParamsLine::deserialize(QDataStream& in)
{
	IDidgramDrawParams::deserialize(in);
}




std::shared_ptr<IDidgramDrawParams> ICreateParams::create()
{
	auto p = createSpecial();
	createRest(p);
	return p;
}

void ICreateParams::createRest(std::shared_ptr<IDidgramDrawParams> params)
{
	params->m_brush = QBrush(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::brush))));
	params->m_center = QPoint(0, 0);
	params->m_paramChanged = true;
	params->m_pen = QPen(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::color))), cfggetval<double>(qtcf::tuxing::all::painter::pen::width));
	params->m_spacesize = QSize(cfggetval<double>(qtcf::tuxing::all::spacesize::width), cfggetval<double>(qtcf::tuxing::all::spacesize::height));
}

std::shared_ptr<IDidgramDrawParams> createParamsRect::createSpecial()
{
	auto p = std::make_shared<DiagramDrawParamsRect>();
	p->m_boundingrectradio = cfggetval<double>(qtcf::tuxing::rectangle::radio);
	p->m_rotate = cfggetval<double>(qtcf::tuxing::rectangle::rotate);
	p->m_scale = cfggetval<double>(qtcf::tuxing::rectangle::scale);
	p->m_type = ShapeType::Rect;
	return p;
}

std::shared_ptr<IDidgramDrawParams> createParamsCircle::createSpecial()
{
	auto p = std::make_shared<DiagramDrawParamsCircle>();
	p->m_boundingrectradio = cfggetval<double>(qtcf::tuxing::circle::boundingrectradio);
	p->m_rotate = cfggetval<double>(qtcf::tuxing::circle::rotate);
	p->m_scale = cfggetval<double>(qtcf::tuxing::circle::scale);
	p->m_type = ShapeType::Circle;
	return p;
}

std::shared_ptr<IDidgramDrawParams> createParamsTriangle::createSpecial()
{
	auto p = std::make_shared<DiagramDrawParamsTriangle>();
	p->m_edgetype = DiagramDrawParamsTriangle::edgetypeStringToEnum(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::triangle::edgetype)));
	p->m_rotate = cfggetval<double>(qtcf::tuxing::triangle::totate);
	p->m_scale = cfggetval<double>(qtcf::tuxing::triangle::scale);
	p->m_type = ShapeType::Triangle;
	p->m_triangleSizeRadios.m_bottom = cfggetval<double>(qtcf::tuxing::triangle::edgeradio::bottom);
    p->m_triangleSizeRadios.m_left = cfggetval<double>(qtcf::tuxing::triangle::edgeradio::left);
    p->m_triangleSizeRadios.m_right = cfggetval<double>(qtcf::tuxing::triangle::edgeradio::right);
	return p;

}

std::shared_ptr<IDidgramDrawParams> createParamsLine::createSpecial()
{
	auto p = std::make_shared<DiagramDrawParamsLine>();
	p->m_rotate = cfggetval<double>(qtcf::tuxing::line::rotate);
	p->m_scale = cfggetval<double>(qtcf::tuxing::line::scale); 
	p->m_type = ShapeType::Line;
	return p;
}

createParamsInterface& createParamsInterface::getInstance()
{
	static createParamsInterface p;
	return p;
}

createParamsInterface& createParamsInterface::add(ShapeType type, std::function<std::shared_ptr<ICreateParams>()> creator)
{
	m_map[type] = creator;
	return *this;
}

std::shared_ptr<ICreateParams> createParamsInterface::getParams(ShapeType type)
{
	if (m_map.find(type) != m_map.end())
	{
		return m_map[type]();
	}
	else
	{
		throw std::runtime_error("error");
	}
}

createParamsInterface::createParamsInterface()
{
}

