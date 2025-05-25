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

		canvas,
		huabuRect,
		huabuCircle,
		huabuTriangle,
		huabuLine,
		huabuMouse,
		huabuText
	};

	propertywidgettype diagramShapetypeToPropertyWidgetType(ShapeType type);
	propertywidgettype huabuShapetypeToPropertyWidgetType(ShapeType type);

	QString PenStyleToQstring(Qt::PenStyle style);
	Qt::PenStyle QstringToPenStyle(QString style);
}
#endif // NAMESPACE_H
