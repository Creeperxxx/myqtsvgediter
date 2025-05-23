#include "namespace.h"
#include "myconfig.h"

QString myqtsvg::ShapetypeEnumToQstring(ShapeType type)
{
	switch (type)
	{
	case myqtsvg::ShapeType::Rect:
		return myconfig::getInstance().getRectName();
		break;
	case myqtsvg::ShapeType::Circle:
		return myconfig::getInstance().getCircleName();
		break;
	case myqtsvg::ShapeType::Triangle:
		return myconfig::getInstance().getTriangleName();
		break;
	case myqtsvg::ShapeType::Line:
		return myconfig::getInstance().getLineName();
		break;
	case myqtsvg::ShapeType::Mouse:
		return myconfig::getInstance().getMouseName();
		break;
	case myqtsvg::ShapeType::choose:
		return myconfig::getInstance().getChooseName();
		break;
	case myqtsvg::ShapeType::Text:
		return myconfig::getInstance().getTextName();
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

myqtsvg::ShapeType myqtsvg::ShapetypeQstringToEnum(const QString& type)
{
	if (type == myconfig::getInstance().getRectName())
		return myqtsvg::ShapeType::Rect;
	else if (type == myconfig::getInstance().getCircleName())
		return myqtsvg::ShapeType::Circle;
	else if (type == myconfig::getInstance().getTriangleName())
		return myqtsvg::ShapeType::Triangle;
	else if (type == myconfig::getInstance().getLineName())
		return myqtsvg::ShapeType::Line;
	else if (type == myconfig::getInstance().getMouseName())
		return myqtsvg::ShapeType::Mouse;
	else if (type == myconfig::getInstance().getChooseName())
		return myqtsvg::ShapeType::choose;
	else if (type == myconfig::getInstance().getTextName())
		return myqtsvg::ShapeType::Text;
	else
		throw std::runtime_error("error");


}

myqtsvg::propertywidgettype myqtsvg::diagramShapetypeToPropertyWidgetType(ShapeType type)
{
	switch (type)
	{
	case myqtsvg::ShapeType::Rect:
		return propertywidgettype::diagramRect;
		break;
	case myqtsvg::ShapeType::Circle:
		return propertywidgettype::diagramCircle;
		break;
	case myqtsvg::ShapeType::Triangle:
		return propertywidgettype::diagramTriangle;
		break;
	case myqtsvg::ShapeType::Line:
		return propertywidgettype::diagramLine;
		break;
	case myqtsvg::ShapeType::Mouse:
		return propertywidgettype::diagramMouse;
		break;
	case myqtsvg::ShapeType::choose:
		return propertywidgettype::defaultWidget;
		break;
	case myqtsvg::ShapeType::Text:
		return propertywidgettype::diagramText;
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

myqtsvg::propertywidgettype myqtsvg::huabuShapetypeToPropertyWidgetType(ShapeType type)
{
	switch (type)
	{
	case myqtsvg::ShapeType::Rect:
		return propertywidgettype::huabuRect;
		break;
	case myqtsvg::ShapeType::Circle:
		return propertywidgettype::huabuCircle;
		break;
	case myqtsvg::ShapeType::Triangle:
		return propertywidgettype::huabuTriangle;
		break;
	case myqtsvg::ShapeType::Line:
		return propertywidgettype::huabuLine;
		break;
	case myqtsvg::ShapeType::Mouse:
		return propertywidgettype::huabuMouse;
		break;
	case myqtsvg::ShapeType::choose:
		return propertywidgettype::huabu;
		break;
	case myqtsvg::ShapeType::Text:
		return propertywidgettype::huabuText;
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

QString myqtsvg::PenStyleToQstring(Qt::PenStyle style)
{
	switch (style)
	{
	default:
	case Qt::NoPen:
		return "NoPen";
		break;
	case Qt::SolidLine:
		return "SolidLine";
		break;
	case Qt::DashLine:
		return "DashLine";
		break;
	case Qt::DotLine:
		return "DotLine";
		break;
	case Qt::DashDotLine:
		return "DashDotLine";
		break;
	case Qt::DashDotDotLine:
		return "DashDotDotLine";
		break;
	case Qt::CustomDashLine:
		return "CustomDashLine";
		break;
	case Qt::MPenStyle:
		return "MPenStyle";
		break;

	}
}

Qt::PenStyle myqtsvg::QstringToPenStyle(QString style)
{
	if (style == "SolidLine")
		return Qt::SolidLine;
	else if (style == "DashLine")
		return Qt::DashLine;
	else if (style == "DotLine")
		return Qt::DotLine;
	else if (style == "DashDotLine")
		return Qt::DashDotLine;
	else if (style == "DashDotDotLine")
		return Qt::DashDotDotLine;
	else if (style == "CustomDashLine")
		return Qt::CustomDashLine;
	else if (style == "MPenStyle")
		return Qt::MPenStyle;
	else
		return Qt::NoPen;
}
