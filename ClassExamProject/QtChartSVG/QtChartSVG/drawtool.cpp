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

//std::shared_ptr<IDidgramDrawParams> IbuildHuabuParams::build(QWidget* item)
//{
//	huabu* huabuitem = dynamic_cast<huabu*>(item);
//	if (huabuitem == nullptr)
//		throw std::runtime_error("error");
//	std::shared_ptr<IDidgramDrawParams> params = specialbuild(huabuitem);
//	params->m_brush = huabuitem->getbrush();
//	params->m_pen = huabuitem->getpen();
//	params->m_center = huabuitem->getselfdrawcenter();
//	params->m_spacesize = huabuitem->getselfdrawspacesize();
//	params->m_type = huabuitem->gettype();
//	return params;
//}
//
//std::shared_ptr<IDidgramDrawParams> buildHuabuParamsRect::specialbuild(huabu* item)
//{
//	std::shared_ptr<DiagramDrawParamsRect> params = std::make_shared<DiagramDrawParamsRect>();
//	if (!item->getmimedata().m_rectradio.has_value())
//		throw std::runtime_error("error");
//	params->m_boundingrectradio = item->getmimedata().m_rectradio.value();
//	return params;
//}
//
//std::shared_ptr<IDidgramDrawParams> buildHuabuParamsCircle::specialbuild(huabu* item)
//{
//	std::shared_ptr<DiagramDrawParamsCircle> params = std::make_shared<DiagramDrawParamsCircle>();
//	if (!item->getmimedata().m_circleradio.has_value())
//		throw std::runtime_error("error");
//	params->m_boundingrectradio = item->getmimedata().m_circleradio.value();
//	return params;
//}
//
//std::shared_ptr<IDidgramDrawParams> buildHuabuParamsTriangle::specialbuild(huabu* item)
//{
//	std::shared_ptr<DiagramDrawParamsTriangle> params = std::make_shared<DiagramDrawParamsTriangle>();
//	if (!item->getmimedata().m_triangleSideRadios.has_value())
//		throw std::runtime_error("error");
//	params->m_triangleSizeRadios = item->getmimedata().m_triangleSideRadios.value();
//
//	if (!item->getmimedata().m_triangleEdgeType.has_value())
//		throw std::runtime_error("error");
//	params->m_edgetype = item->getmimedata().m_triangleEdgeType.value();
//
//	if (!item->getmimedata().m_triangleRotate.has_value())
//		throw std::runtime_error("error");
//	params->m_rotationAngle = item->getmimedata().m_triangleRotate.value();
//	return params;
//}
//
//std::unique_ptr<IbuildHuabuParams> FactorybuildHuabuParams::create(ShapeType type)
//{
//	switch (type)
//	{
//	case ShapeType::Rect:
//		return std::make_unique<buildHuabuParamsRect>();
//		break;
//	case ShapeType::Circle:
//		return std::make_unique<buildHuabuParamsCircle>();
//		break;
//	case ShapeType::Triangle:
//		return std::make_unique<buildHuabuParamsTriangle>();
//		break;
//	case ShapeType::Line:
//		return std::make_unique<buildHuabuParamsLine>();
//		break;
//	default:
//		throw std::runtime_error("error");
//		break;
//	}
//}
//
//std::shared_ptr<IDidgramDrawParams> buildtuxingjiedianparamsfordiagramline::specialbuild(DiagramItem* item)
//{
//	std::shared_ptr<DiagramDrawParamsLine> params = std::make_shared<DiagramDrawParamsLine>();
//	params->m_rotationAngle = item->getLineRotate();
//	return params;
//}
//
//std::shared_ptr<IDidgramDrawParams> buildDiagramPixmapParamsLine::specialbuild(DiagramItem* item)
//{
//	std::shared_ptr<DiagramDrawParamsLine> params = std::make_shared<DiagramDrawParamsLine>();
//	params->m_rotationAngle = item->getLineRotate();
//	return params;
//}
//
//std::shared_ptr<IDidgramDrawParams> buildHuabuParamsLine::specialbuild(huabu* item)
//{
//	std::shared_ptr<DiagramDrawParamsLine> params = std::make_shared<DiagramDrawParamsLine>();
//	params->m_rotationAngle = item->getlinerotate();
//	return params;
//}

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
		<< m_scale
		<< m_pen.color()
		<< m_pen.widthF()
		<< m_brush.color()
		<< m_rotate;	
}

void IDidgramDrawParams::deserialize(QDataStream& in)
{
	qreal width;
	qreal height;
	in >> width
		>> height;
	m_spacesize = QSizeF(width, height);

	in >> m_scale;

	QColor color;
	qreal penwidth;
	in >> color >> penwidth;
	m_pen = QPen(color, penwidth);

	in >> color;
	m_brush = QBrush(color);

	in >> m_rotate;
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


void CreatePropertyDataRect::buildPropertydDataSpecial(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto p = std::dynamic_pointer_cast<DiagramDrawParamsRect>(params);
	if (p == nullptr)
		throw std::runtime_error("error");
	buildName(p, datavec);
	buildRadio(p, datavec);
}

void CreatePropertyDataRect::onRadioChanged(QVariant newradio, std::shared_ptr<DiagramDrawParamsRect> params)
{
	if (!newradio.canConvert<double>())
		throw std::runtime_error("error");
	params->m_boundingrectradio = newradio.value<double>();
	emit params->SignalParamsChanged();
}

void CreatePropertyDataRect::buildRadio(std::shared_ptr<DiagramDrawParamsRect> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto data = std::make_shared<propertydata>(propertynameradio, QVariant::fromValue(params->m_boundingrectradio));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, [=](QVariant value) {
		onRadioChanged(value, params);
		});
}

void CreatePropertyDataRect::buildName(std::shared_ptr<DiagramDrawParamsRect> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto data = std::make_shared<propertydata>(propertynamename, QVariant::fromValue(QString::fromStdString(rectname)));
    datavec->push_back(data);
}

std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> ICreatePropertyData::createPropertyData(std::shared_ptr<IDidgramDrawParams> params)
{
	if (params == nullptr)
		throw std::runtime_error("error");
	std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> vec = std::make_shared<std::vector<std::shared_ptr<propertydata>>>();
	createPropertyDataAll(params, vec);
	buildPropertydDataSpecial(params, vec);
	return vec;
}

void ICreatePropertyData::onRotateChanged(QVariant rotate, std::shared_ptr<IDidgramDrawParams> params)
{
	if (!rotate.canConvert<double>())
		throw std::runtime_error("error");
	params->m_rotate = rotate.value<double>();
	emit params->SignalParamsChanged();
}

void ICreatePropertyData::onScaleChanged(QVariant scale, std::shared_ptr<IDidgramDrawParams> params)
{
	if (!scale.canConvert<double>())
		throw std::runtime_error("error");
	params->m_scale = scale.value<double>();
	emit params->SignalParamsChanged();
}

void ICreatePropertyData::onPencolorChanged(QVariant color, std::shared_ptr<IDidgramDrawParams> params)
{
	if (!color.canConvert<QColor>())
		throw std::runtime_error("error");
	params->m_pen.setColor(color.value<QColor>());
	emit params->SignalParamsChanged();
}

void ICreatePropertyData::onPenwidthChanged(QVariant width, std::shared_ptr<IDidgramDrawParams> params)
{
	if (!width.canConvert<double>())
		throw std::runtime_error("error");
	params->m_pen.setWidthF(width.value<double>());
	emit params->SignalParamsChanged();
}

void ICreatePropertyData::onBrushChanged(QVariant brush, std::shared_ptr<IDidgramDrawParams> params)
{
	if (!brush.canConvert<QColor>())
		throw std::runtime_error("error");
	params->m_brush.setColor(brush.value<QColor>());
	emit params->SignalParamsChanged();
}

void ICreatePropertyData::onSpacesizeWidthChanged(QVariant width, std::shared_ptr<IDidgramDrawParams> params)
{
	if (!width.canConvert<double>())
		throw std::runtime_error("error");
	params->m_spacesize.setWidth(width.value<double>());
	emit params->SignalParamsChanged();
}

void ICreatePropertyData::onSpacesizeHeightChanged(QVariant height, std::shared_ptr<IDidgramDrawParams> params)
{
	if (!height.canConvert<double>())
		throw std::runtime_error("error");
	params->m_spacesize.setHeight(height.value<double>());
	emit params->SignalParamsChanged();
}

void ICreatePropertyData::createPropertyDataAll(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	buildPenAndBrush(params, datavec);
	buildRotate(params, datavec);
	buildSpacesize(params, datavec);
	buildScale(params, datavec);
}

void ICreatePropertyData::buildPenAndBrush(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = nullptr;
	data = std::make_shared<propertydata>(propertynamepencolor, QVariant::fromValue(params->m_pen.color()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, [=](QVariant value) {
		onPencolorChanged(value, params);
		});
	
	data = std::make_shared<propertydata>(propertynamepenwidth, QVariant::fromValue(params->m_pen.widthF()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, [=](QVariant value) {
		onPenwidthChanged(value, params);
		});

	data = std::make_shared<propertydata>(propertynamebrush, QVariant::fromValue(params->m_brush.color()));
    datavec->push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, [=](QVariant value) {
        onBrushChanged(value, params);
        });
}

void ICreatePropertyData::buildRotate(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(propertynamerotate, QVariant::fromValue(params->m_rotate));
	datavec->push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, [=](QVariant value) {
        onRotateChanged(value, params);
        });
}

void ICreatePropertyData::buildSpacesize(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(propertynametuxingspacewidth, QVariant::fromValue(params->m_spacesize.width()));
    datavec->push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, [=](QVariant value) {
        onSpacesizeWidthChanged(value, params);
        });

	data = std::make_shared<propertydata>(propertynametuxingspaceheight, QVariant::fromValue(params->m_spacesize.height()));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, [=](QVariant value) {
		onSpacesizeHeightChanged(value, params);
		});
}

void ICreatePropertyData::buildScale(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(propertynamescale, QVariant::fromValue(params->m_scale));
	datavec->push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, [=](QVariant value) {
		onScaleChanged(value, params);
		});
}

void CreatePropertyDataCircle::buildPropertydDataSpecial(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto p = std::dynamic_pointer_cast<DiagramDrawParamsCircle>(params);
	if(p == nullptr)
		throw std::runtime_error("error");
	buildName(p, datavec);
	buildRadio(p, datavec);
}

void CreatePropertyDataCircle::onRadioChanged(QVariant newradio, std::shared_ptr<DiagramDrawParamsCircle> params)
{
	if(!newradio.canConvert<double>())
		throw std::runtime_error("error");
	params->m_boundingrectradio = newradio.value<double>();
	emit params->SignalParamsChanged();

}

void CreatePropertyDataCircle::buildRadio(std::shared_ptr<DiagramDrawParamsCircle> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(propertynameradio, QVariant::fromValue(params->m_boundingrectradio));
	datavec->push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, [=](QVariant value) {
        onRadioChanged(value, params);
        });
}

void CreatePropertyDataCircle::buildName(std::shared_ptr<DiagramDrawParamsCircle> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(propertynamename, QVariant::fromValue(circlename));
	datavec->push_back(data);
}

void CreatePropertyDataTriangle::buildPropertydDataSpecial(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto p = std::dynamic_pointer_cast<DiagramDrawParamsTriangle>(params);
	if(p == nullptr)
		throw std::runtime_error("error");
	buildName(p, datavec);
	buildBottomRadio(p, datavec);
	buildLeftRadio(p, datavec);
	buildRightRadio(p, datavec);
	buildEdgetype(p, datavec);
}

void CreatePropertyDataTriangle::onEdgetypeChanged(QVariant edgetype, std::shared_ptr<DiagramDrawParamsTriangle> params)
{
	if(!edgetype.canConvert<QString>())
		throw std::runtime_error("error");
    params->m_edgetype = DiagramDrawParamsTriangle::edgetypeStringToEnum(edgetype.value<QString>());
	emit params->SignalParamsChanged();
}

void CreatePropertyDataTriangle::onBottomRadioChanged(QVariant newradio, std::shared_ptr<DiagramDrawParamsTriangle> params)
{
	if (!newradio.canConvert<double>())
		throw std::runtime_error("error");
	params->m_triangleSizeRadios.m_bottom = newradio.value<double>();
	emit params->SignalParamsChanged();
}

void CreatePropertyDataTriangle::onLeftRadioChanged(QVariant newradio, std::shared_ptr<DiagramDrawParamsTriangle> params)
{
	if(!newradio.canConvert<double>())
		throw std::runtime_error("error");
    params->m_triangleSizeRadios.m_left = newradio.value<double>();
    emit params->SignalParamsChanged();
}

void CreatePropertyDataTriangle::onRightRadioChanged(QVariant newradio, std::shared_ptr<DiagramDrawParamsTriangle> params)
{
	if(!newradio.canConvert<double>())
        throw std::runtime_error("error");
    params->m_triangleSizeRadios.m_right = newradio.value<double>();
    emit params->SignalParamsChanged();
}

void CreatePropertyDataTriangle::buildEdgetype(std::shared_ptr<DiagramDrawParamsTriangle> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(propertynameedgetype, QVariant::fromValue(DiagramDrawParamsTriangle::edgetypeEnumToString(params->m_edgetype)));
	datavec->push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, [=](QVariant value) {
        onEdgetypeChanged(value, params);
        });
}

void CreatePropertyDataTriangle::buildBottomRadio(std::shared_ptr<DiagramDrawParamsTriangle> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(propertynamebottomradio, QVariant::fromValue(params->m_triangleSizeRadios.m_bottom));
	datavec->push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, [=](QVariant value) {
        onBottomRadioChanged(value, params);
        });
}

void CreatePropertyDataTriangle::buildLeftRadio(std::shared_ptr<DiagramDrawParamsTriangle> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(propertynameleftradio, QVariant::fromValue(params->m_triangleSizeRadios.m_left));
	datavec->push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, [=](QVariant value) {
        onLeftRadioChanged(value, params);
        });
}

void CreatePropertyDataTriangle::buildRightRadio(std::shared_ptr<DiagramDrawParamsTriangle> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(propertynamerightradio, QVariant::fromValue(params->m_triangleSizeRadios.m_right));
	datavec->push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, [=](QVariant value) {
        onRightRadioChanged(value, params);
        });
}

void CreatePropertyDataTriangle::buildName(std::shared_ptr<DiagramDrawParamsTriangle> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(propertynamename, QVariant::fromValue(trianglename));
	datavec->push_back(data);
}

void CreatePropertyDataLine::buildPropertydDataSpecial(std::shared_ptr<IDidgramDrawParams> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	auto p = std::dynamic_pointer_cast<DiagramDrawParamsLine>(params);
	if(p == nullptr)
		throw std::runtime_error("error");
    buildName(p, datavec);
}

void CreatePropertyDataLine::buildName(std::shared_ptr<DiagramDrawParamsLine> params, std::shared_ptr<std::vector<std::shared_ptr<propertydata>>> datavec)
{
	std::shared_ptr<propertydata> data = std::make_shared<propertydata>(propertynamename, QVariant::fromValue(linename));
	datavec->push_back(data);
}

PropertyDataInterface& PropertyDataInterface::getInstance()
{
	static PropertyDataInterface instance;
	return instance;
}

PropertyDataInterface& PropertyDataInterface::addCreator(ShapeType type, std::function<std::shared_ptr<ICreatePropertyData>()> creator)
{
	m_creatorMap[type] = creator;
	return *this;
}

std::shared_ptr<ICreatePropertyData> PropertyDataInterface::getCreator(ShapeType type)
{
	if (m_creatorMap.find(type) != m_creatorMap.end())
	{
		return m_creatorMap[type]();
	}
	else
	{
		throw std::runtime_error("error");
	}
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
	params->m_center = QPointF(0, 0);
	params->m_paramChanged = true;
	params->m_pen = QPen(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::color))), cfggetval<double>(qtcf::tuxing::all::painter::pen::width));
	params->m_spacesize = QSizeF(cfggetval<double>(qtcf::tuxing::all::spacesize::width), cfggetval<double>(qtcf::tuxing::all::spacesize::height));
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
	auto p = std::shared_ptr<DiagramDrawParamsCircle>();
	p->m_boundingrectradio = cfggetval<double>(qtcf::tuxing::circle::boundingrectradio);
	p->m_rotate = cfggetval<double>(qtcf::tuxing::circle::rotate);
	p->m_scale = cfggetval<double>(qtcf::tuxing::circle::scale);
	p->m_type = ShapeType::Circle;
	return p;
}

std::shared_ptr<IDidgramDrawParams> createParamsTriangle::createSpecial()
{
	auto p = std::shared_ptr<DiagramDrawParamsTriangle>();
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
