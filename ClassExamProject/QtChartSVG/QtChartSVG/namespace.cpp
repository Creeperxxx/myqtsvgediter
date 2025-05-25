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
	case myqtsvg::ShapeType::Pentagon:
		return myconfig::getInstance().getPentagonName();
		break;
	case myqtsvg::ShapeType::Hexagon:
		return myconfig::getInstance().getHexagonName();
		break;
	case myqtsvg::ShapeType::Star:
		return myconfig::getInstance().getStarName();
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
	else if (type == myconfig::getInstance().getPentagonName())
		return myqtsvg::ShapeType::Pentagon;
	else if (type == myconfig::getInstance().getHexagonName())
		return myqtsvg::ShapeType::Hexagon;
	else if (type == myconfig::getInstance().getStarName())
		return myqtsvg::ShapeType::Star;
	else
		throw std::runtime_error("error");

}

myqtsvg::propertyWidgetType myqtsvg::diagramShapetypeToPropertyWidgetType(ShapeType type)
{
	switch (type)
	{
	case myqtsvg::ShapeType::Rect:
		return propertyWidgetType::diagramRect;
		break;
	case myqtsvg::ShapeType::Circle:
		return propertyWidgetType::diagramCircle;
		break;
	case myqtsvg::ShapeType::Triangle:
		return propertyWidgetType::diagramTriangle;
		break;
	case myqtsvg::ShapeType::Line:
		return propertyWidgetType::diagramLine;
		break;
	case myqtsvg::ShapeType::Mouse:
		return propertyWidgetType::diagramMouse;
		break;
	case myqtsvg::ShapeType::choose:
		return propertyWidgetType::defaultWidget;
		break;
	case myqtsvg::ShapeType::Text:
		return propertyWidgetType::diagramText;
		break;
	case myqtsvg::ShapeType::Pentagon:
		return propertyWidgetType::diagramPentagon;
		break;
	case myqtsvg::ShapeType::Hexagon:
		return propertyWidgetType::diagramHexagon;
		break;
	case myqtsvg::ShapeType::Star:
		return propertyWidgetType::diagramStar;
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}

myqtsvg::propertyWidgetType myqtsvg::canvasShapetypeToPropertyWidgetType(ShapeType type)
{
	switch (type)
	{
	case myqtsvg::ShapeType::Rect:
		return propertyWidgetType::canvasRect;
		break;
	case myqtsvg::ShapeType::Circle:
		return propertyWidgetType::canvasCircle;
		break;
	case myqtsvg::ShapeType::Triangle:
		return propertyWidgetType::canvasTriangle;
		break;
	case myqtsvg::ShapeType::Line:
		return propertyWidgetType::canvasLine;
		break;
	case myqtsvg::ShapeType::Mouse:
		return propertyWidgetType::canvasMouse;
		break;
	case myqtsvg::ShapeType::choose:
		return propertyWidgetType::canvas;
		break;
	case myqtsvg::ShapeType::Text:
		return propertyWidgetType::canvasText;
		break;
	case myqtsvg::ShapeType::Pentagon:
		return propertyWidgetType::canvasPentagon;
		break;
	case myqtsvg::ShapeType::Hexagon:
		return propertyWidgetType::canvasHexagon;
		break;
	case myqtsvg::ShapeType::Star:
		return propertyWidgetType::canvasStar;
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
