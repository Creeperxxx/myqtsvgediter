#pragma once
#include <qstring.h>
#include <map>
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
		Text
	};
	QString ShapetypeEnumToQstring(ShapeType type);
	ShapeType ShapetypeQstringToEnum(const QString& type);

	enum class propertywidgettype
	{
		defaultWidget,

		diagramRect,
		diagramCircle,
		diagramTriangle,
		diagramLine,
		diagramMouse,
		diagramText,

		huabu,
		huabuRect,
		huabuCircle,
		huabuTriangle,
		huabuLine,
		huabuMouse,
		huabuText
	};

	propertywidgettype diagramShapetypeToPropertyWidgetType(ShapeType type);
	propertywidgettype huabuShapetypeToPropertyWidgetType(ShapeType type);
	
}