#ifndef NAMESPACE_H
#define NAMESPACE_H


#include <qstring.h>
#include <map>
#include <stdexcept>

namespace myqtsvg
{
	enum class ShapeType
	{
		Rect,
		Circle,
		Triangle,
		Line,
		Mouse,
		choose,
		Text,
		Pentagon,
        Hexagon,
		Star
	};
	QString ShapetypeEnumToQstring(ShapeType type);
	ShapeType ShapetypeQstringToEnum(const QString& type);

	enum class propertyWidgetType
	{
		defaultWidget,

		diagramRect,
		diagramCircle,
		diagramTriangle,
		diagramLine,
		diagramMouse,
		diagramText,
		diagramPentagon,
		diagramHexagon,
		diagramStar,

		canvas,
		canvasRect,
		canvasCircle,
		canvasTriangle,
		canvasLine,
		canvasMouse,
		canvasText,
		canvasPentagon,
		canvasHexagon,
		canvasStar
	};

	propertyWidgetType diagramShapetypeToPropertyWidgetType(ShapeType type);
	propertyWidgetType canvasShapetypeToPropertyWidgetType(ShapeType type);

	QString PenStyleToQstring(Qt::PenStyle style);
	Qt::PenStyle QstringToPenStyle(QString style);
}
#endif // NAMESPACE_H
