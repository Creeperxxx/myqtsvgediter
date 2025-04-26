#include "tuxing.h"
#include "shuxingwidget.h"
#include "config.h"

void DiagramItem::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		dragstartposition = event->pos();
		emit signalMouseClicked(m_propertywidgettype, m_propertyDataVec);
	}
}

void DiagramItem::mouseMoveEvent(QMouseEvent* event)
{
	if (!(event->buttons() & Qt::LeftButton))
	{
		return;
	}
	if ((event->pos() - dragstartposition).manhattanLength() < QApplication::startDragDistance())
		return;

	QDrag* drag = new QDrag(this);
	drag->setMimeData(createDragMimeData());
	drag->setPixmap(drawDiagramPixmap());
	drag->setHotSpot(drag->pixmap().rect().center());
	drag->exec(Qt::CopyAction);
}

QMimeData* DiagramItem::createDragMimeData()
{
	if (m_params.m_mimetype.isEmpty())
	{
		throw std::runtime_error("error");//todo:except
	}
	QMimeData* diagramDragMimedata = new QMimeData();
	QByteArray array = createDiagramMimedataDeliveryparams();
	QDataStream stream(array);
	stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);
	DiagramMimedata data;
	stream >> data;

	diagramDragMimedata->setData(m_params.m_mimetype, array);
	return diagramDragMimedata;
}

QByteArray DiagramItem::createDiagramMimedataDeliveryparams()
{
	QByteArray bytearray;
	QDataStream stream(&bytearray, QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_DefaultCompiledVersion);
	DiagramMimedata data;
	data.m_type = m_params.m_type;
	data.m_scale = m_params.m_scale;
	data.m_pen = m_params.m_pen;
	data.m_brush = m_params.m_brush;
	data.m_spacesize = m_params.m_spacesize;
	switch (m_params.m_type)
	{
	case ShapeType::Rect:
		buildRectMimedata(data);
		break;
	case ShapeType::Circle:
		buildCircleMimedata(data);
		break;
	case ShapeType::Triangle:
		buildTriangleMimedata(data);
		break;
	case ShapeType::Line:
		buildLineMimedata(data);
		break;
	default:
		throw std::runtime_error("error");
	}
	stream << data;
	return bytearray;
}


QSizeF DiagramItem::getPixmapSpaceSize()
{
	return m_params.m_spacesize;
}







QPointF DiagramItem::getPixmapCenter()
{
	return QPointF(m_params.m_spacesize.width() / 2.0, m_params.m_spacesize.height() / 2.0);
}

void DiagramItem::initDiagramPainter(QPainter& painter)
{
	painter.setPen(m_params.m_pen);
	painter.setBrush(m_params.m_brush);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::TextAntialiasing, true);
}

void DiagramItem::initDiagramPixmapPainter(QPainter& painter)
{
	painter.setPen(m_params.m_pen);
	painter.setBrush(m_params.m_brush);
	painter.setRenderHint(QPainter::Antialiasing, true);
}

void DiagramItem::buildRectMimedata(DiagramMimedata& data)
{
	if (!m_params.m_rectRadio.has_value())
		throw std::runtime_error("error");
	data.m_rectradio = m_params.m_rectRadio.value();
	if (!m_params.m_rectRotate.has_value())
		throw std::runtime_error("error");
	data.m_rectRotate = m_params.m_rectRotate.value();
}

void DiagramItem::buildCircleMimedata(DiagramMimedata& data)
{
	if (!m_params.m_circleboundingrectradio.has_value())
		throw std::runtime_error("error");
	data.m_circleradio = m_params.m_circleboundingrectradio;
	if (!m_params.m_circlerotate.has_value())
		throw std::runtime_error("error");
	data.m_circlerotate = m_params.m_circlerotate.value();
}

void DiagramItem::buildTriangleMimedata(DiagramMimedata& data)
{
	if (!m_params.m_triangleSideRadios.has_value())
		throw std::runtime_error("error");
	data.m_triangleSideRadios = m_params.m_triangleSideRadios.value();

	if (!m_params.m_triangleEdgeType.has_value())
		throw std::runtime_error("error");
	data.m_triangleEdgeType = m_params.m_triangleEdgeType.value();

	if (!m_params.m_triangleEdgeRotate.has_value())
		throw std::runtime_error("error");
	data.m_triangleRotate = m_params.m_triangleEdgeRotate.value();
}

void DiagramItem::buildLineMimedata(DiagramMimedata& data)
{
	data.m_linerotate = m_params.m_linerotate;
}

std::shared_ptr<IDidgramDrawParams> DiagramItem::builddrawparams()
{
	return builddrawparamsrest(buildspecialbytype());
}

std::shared_ptr<IDidgramDrawParams> DiagramItem::buildPixmapDrawParams()
{
	return buildPixmapDrawParamsRest(buildspecialbytype());
}

std::shared_ptr<IDidgramDrawParams> DiagramItem::buildspecialbytype()
{
	switch (m_params.m_type)
	{
	case ShapeType::Rect:
		return builddrawparamsrect();
		break;
	case ShapeType::Circle:
		return builddrawparamscircle();
		break;
	case ShapeType::Triangle:
		return builddrawparamstriangle();
		break;
	case ShapeType::Line:
		return builddrawparamsline();
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
}


bool DiagramItem::getdrawbypainter()
{
	return m_params.m_drawByPainter;
}

bool DiagramItem::getdrawbyloadpic()
{
	return m_params.m_drawByloadpic;
}

bool DiagramItem::getisdrawbypainter()
{
	return m_params.m_isdrawByPainter;
}


void DiagramItem::onRectRadioChanged(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");

	m_params.m_rectRadio = value.toDouble();
	update();
}

void DiagramItem::onRectRotateChanged(QVariant value)
{
	if (!value.canConvert<int>())
		throw std::runtime_error("error");
	m_params.m_rectRotate = value.toInt();
	update();
}

void DiagramItem::onCircleRadioChanged(QVariant newradio)
{
	if (!newradio.canConvert<double>())
		throw std::runtime_error("error");
	m_params.m_circleboundingrectradio = newradio.value<double>();
	update();
}

void DiagramItem::onCircleRotateChanged(QVariant newrottate)
{
	if (!newrottate.canConvert<int>())
		throw std::runtime_error("error");
	m_params.m_circlerotate = newrottate.value<int>();
	update();
}

void DiagramItem::onTriangleSideRadioChangedBottom(QVariant bottom)
{
	if (!bottom.canConvert<double>())
		throw std::runtime_error("error");
	m_params.m_triangleSideRadios->m_bottom = bottom.value<double>();
	update();
}

void DiagramItem::onTriangleSideRadioChangedLeft(QVariant left)
{
	if (!left.canConvert<double>())
		throw std::runtime_error("error");
	m_params.m_triangleSideRadios->m_left = left.value<double>();
	update();
}

void DiagramItem::onTriangleSideRadioChangedRight(QVariant right)
{
	if (!right.canConvert<double>())
		throw std::runtime_error("error");
	m_params.m_triangleSideRadios->m_right = right.value<double>();
	update();
}

void DiagramItem::onTriangleEdgeTypeChanged(QVariant type)
{
	if (!type.canConvert<QString>())
		throw std::runtime_error("error");
	m_params.m_triangleEdgeType = DiagramDrawParamsTriangle::edgetypeStringToEnum(type.value<QString>());
	update();
}

void DiagramItem::onTriangleEdgeRotateChanged(QVariant rotate)
{
	if (!rotate.canConvert<int>())
		throw std::runtime_error("error");
	m_params.m_triangleEdgeRotate = rotate.value<int>();
	update();
}

void DiagramItem::onLineRotateChanged(QVariant rotate)
{
	if (!rotate.canConvert<int>())
		throw std::runtime_error("error");
	m_params.m_linerotate = rotate.value<int>();
	update();
}

void DiagramItem::onScaleChanged(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	m_params.m_scale = value.value<double>();
	update();
}

void DiagramItem::onSpacesizeWidthChanged(QVariant value)
{
	if (!value.canConvert<double>())
		throw std::runtime_error("error");
	m_params.m_spacesize.setWidth(value.value<double>());
	update();
}

void DiagramItem::onSpacesizeHeightChanged(QVariant value)
{
	if(!value.canConvert<double>())
		throw std::runtime_error("error");
    m_params.m_spacesize.setHeight(value.value<double>());
    update();
}




void DiagramItem::onPenColorChanged(QVariant newcolor)
{
	if (!newcolor.canConvert<QColor>())
		throw std::runtime_error("error");
	m_params.m_pen.setColor(newcolor.value<QColor>());
	update();
}

void DiagramItem::onPenWidthChanged(QVariant newwidth)
{
	if (!newwidth.canConvert<int>())
		throw std::runtime_error("error");
	m_params.m_pen.setWidth(newwidth.toInt());
	update();
}

void DiagramItem::onPenBrushChanged(QVariant newbrush)
{
	if (!newbrush.canConvert<QColor>())
		throw std::runtime_error("error");
	m_params.m_brush = newbrush.value<QColor>();
	update();
}

//void DiagramItem::setPropertyWidgetManger(PropertyWidgetManager* manager)
//{
	//m_propertyWidgetManger = manager;
//}

void DiagramItem::createPropertyWidget()
{
	switch (m_params.m_type)
	{
	case ShapeType::Rect:
		buildRectPropertyData();
		break;
	case ShapeType::Circle:
		buildCirclePropertyData();
		break;
	case ShapeType::Triangle:
		buildTrianglePropertyData();
		break;
	case ShapeType::Line:
		buildLinePropertyData();
		break;
	default:
		throw std::runtime_error("error");
	}
}

void DiagramItem::buildRectPropertyData()
{
	std::shared_ptr<propertydata> data = nullptr;;
	data = std::make_shared<propertydata>(propertynamename, "diagram rect");
	m_propertyDataVec.push_back(data);

	data = std::make_shared<propertydata>(propertynameradio, QVariant::fromValue(m_params.m_rectRadio.value()));
	m_propertyDataVec.push_back(data);
	QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onRectRadioChanged);

    data = std::make_shared<propertydata>(propertynamerotate, QVariant::fromValue(m_params.m_rectRotate.value()));
	m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onRectRotateChanged);

	data = std::make_shared<propertydata>(propertynamescale, QVariant::fromValue(m_params.m_scale));
	m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onScaleChanged);

	data = std::make_shared<propertydata>(propertynamepencolor, QVariant::fromValue(m_params.m_pen.color()));
	m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onPenColorChanged);

	data = std::make_shared<propertydata>(propertynamepenwidth, QVariant::fromValue(m_params.m_pen.width()));
    m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onPenWidthChanged);

    data = std::make_shared<propertydata>(propertynamebrush, QVariant::fromValue(m_params.m_brush));
    m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onPenBrushChanged);

	data = std::make_shared<propertydata>(propertynametuxingspacewidth, QVariant::fromValue(m_params.m_spacesize.width()));
    m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onSpacesizeWidthChanged);

	data = std::make_shared<propertydata>(propertynametuxingspaceheight, QVariant::fromValue(m_params.m_spacesize.height()));
	m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onSpacesizeHeightChanged);
}

void DiagramItem::buildCirclePropertyData()
{
    std::shared_ptr<propertydata> data = nullptr;
    data = std::make_shared<propertydata>(propertynamename, "diagram circle");
    m_propertyDataVec.push_back(data);

	data = std::make_shared<propertydata>(propertynameradio, QVariant::fromValue(m_params.m_circleboundingrectradio.value()));
	m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onCircleRadioChanged);

	data = std::make_shared<propertydata>(propertynamerotate, QVariant::fromValue(m_params.m_circlerotate.value()));
	m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onCircleRotateChanged);

	data = std::make_shared<propertydata>(propertynamescale, QVariant::fromValue(m_params.m_scale));
    m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onScaleChanged);

	data = std::make_shared<propertydata>(propertynametuxingspacewidth, QVariant::fromValue(m_params.m_spacesize.width()));
    m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onSpacesizeWidthChanged);

	data = std::make_shared<propertydata>(propertynametuxingspaceheight, QVariant::fromValue(m_params.m_spacesize.height()));
    m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onSpacesizeHeightChanged);

	buildPropertyDataPenandBrush();
}

void DiagramItem::buildTrianglePropertyData()
{
	std::shared_ptr<propertydata> data = nullptr;
    data = std::make_shared<propertydata>(propertynamename, "diagram triangle");
    m_propertyDataVec.push_back(data);

    data = std::make_shared<propertydata>(propertynamebottomradio, QVariant::fromValue(m_params.m_triangleSideRadios.value().m_bottom));
    m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onTriangleSideRadioChangedBottom);

	data = std::make_shared<propertydata>(propertynameleftradio, QVariant::fromValue(m_params.m_triangleSideRadios.value().m_left));
	m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onTriangleSideRadioChangedLeft);

	data = std::make_shared<propertydata>(propertynamerightradio, QVariant::fromValue(m_params.m_triangleSideRadios.value().m_right));
    m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onTriangleSideRadioChangedRight);

	data = std::make_shared<propertydata>(propertynameedgetype, QVariant::fromValue(DiagramDrawParamsTriangle::edgetypeEnumToString(m_params.m_triangleEdgeType.value())));
	m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onTriangleEdgeTypeChanged);

	data = std::make_shared<propertydata>(propertynamerotate, QVariant::fromValue(m_params.m_triangleEdgeRotate.value()));
	m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onTriangleEdgeRotateChanged);

	data = std::make_shared<propertydata>(propertynamescale, QVariant::fromValue(m_params.m_scale));
    m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onScaleChanged);

	data = std::make_shared<propertydata>(propertynametuxingspacewidth, QVariant::fromValue(m_params.m_spacesize.width()));
    m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onSpacesizeWidthChanged);

    data = std::make_shared<propertydata>(propertynametuxingspaceheight, QVariant::fromValue(m_params.m_spacesize.height()));
    m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onSpacesizeHeightChanged);

	buildPropertyDataPenandBrush();
}

void DiagramItem::buildLinePropertyData()
{
	std::shared_ptr<propertydata> data = nullptr;
	data = std::make_shared<propertydata>(propertynamename, "diagram line");
    m_propertyDataVec.push_back(data);

	data = std::make_shared<propertydata>(propertynamerotate, QVariant::fromValue(m_params.m_linerotate.value()));
    m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onLineRotateChanged);

    data = std::make_shared<propertydata>(propertynamescale, QVariant::fromValue(m_params.m_scale));
    m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onScaleChanged);

	buildPropertyDataPenandBrush();
}

void DiagramItem::buildPropertyDataPenandBrush()
{
	std::shared_ptr<propertydata> data = nullptr;
	data = std::make_shared<propertydata>(propertynamepencolor, QVariant::fromValue(m_params.m_pen.color()));
	m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onPenColorChanged);

    data = std::make_shared<propertydata>(propertynamepenwidth, QVariant::fromValue(m_params.m_pen.width()));
    m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onPenWidthChanged);

    data = std::make_shared<propertydata>(propertynamebrush, QVariant::fromValue(m_params.m_brush));
    m_propertyDataVec.push_back(data);
    QObject::connect(data.get(), &propertydata::signalValueChanged, this, &DiagramItem::onPenBrushChanged);
}


std::shared_ptr<IDidgramDrawParams> DiagramItem::builddrawparamsrest(std::shared_ptr<IDidgramDrawParams> params)
{
	//params->m_brush = m_params.m_brush;
	//params->m_pen = m_params.m_pen;
	params->m_center = getselfdrawcenter();
	params->m_spacesize = getselfdrawspacesize();
	params->m_type = m_params.m_type;
	params->m_scale = m_params.m_scale;
	params->m_pen = m_params.m_pen;
	params->m_brush = m_params.m_brush;
	return params;
}

std::shared_ptr<IDidgramDrawParams> DiagramItem::builddrawparamsrect()
{
	auto params = std::make_shared<DiagramDrawParamsRect>();
	if (!m_params.m_rectRadio.has_value())
		throw std::runtime_error("error");
	params->m_boundingrectradio = m_params.m_rectRadio.value();
	if (!m_params.m_rectRotate.has_value())
		throw std::runtime_error("error");
	params->m_rectrotate = m_params.m_rectRotate.value();
	return params;
}

std::shared_ptr<IDidgramDrawParams> DiagramItem::builddrawparamscircle()
{
	auto params = std::make_shared<DiagramDrawParamsCircle>();
	if (!m_params.m_circleboundingrectradio.has_value())
		throw std::runtime_error("error");
	params->m_boundingrectradio = m_params.m_circleboundingrectradio.value();
	if (!m_params.m_circlerotate.has_value())
		throw std::runtime_error("error");
	params->m_circlerotate = m_params.m_circlerotate.value();
	return params;
}

std::shared_ptr<IDidgramDrawParams> DiagramItem::builddrawparamstriangle()
{
	auto params = std::make_shared<DiagramDrawParamsTriangle>();
	if (!m_params.m_triangleSideRadios.has_value())
		throw std::runtime_error("error");
	params->m_triangleSizeRadios = m_params.m_triangleSideRadios.value();
	if (!m_params.m_triangleEdgeType.has_value())
		throw std::runtime_error("error");
	params->m_edgetype = m_params.m_triangleEdgeType.value();
	if (!m_params.m_triangleEdgeRotate.has_value())
		throw std::runtime_error("error");
	params->m_rotationAngle = m_params.m_triangleEdgeRotate.value();
	return params;
}

std::shared_ptr<IDidgramDrawParams> DiagramItem::builddrawparamsline()
{
	auto params = std::make_shared<DiagramDrawParamsLine>();
	if (!m_params.m_linerotate.has_value())
		throw std::runtime_error("error");
	params->m_rotationAngle = m_params.m_linerotate.value();
	return params;
}

std::shared_ptr<IDidgramDrawParams> DiagramItem::buildPixmapDrawParamsRest(std::shared_ptr<IDidgramDrawParams> params)
{
	params->m_center = getPixmapCenter();
	params->m_spacesize = getPixmapSpaceSize();
	params->m_type = m_params.m_type;
	params->m_scale = m_params.m_scale;
	params->m_pen = m_params.m_pen;
	params->m_brush = m_params.m_brush;
	return params;
}

QPixmap DiagramItem::drawDiagramPixmap()
{
	QPixmap pixmap(getPixmapSpaceSize().toSize());
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	initDiagramPixmapPainter(painter);

	DiagramDrawInterface::draw(painter, buildPixmapDrawParams());

	return pixmap;

}

void DiagramItem::init()
{
	initWidgetSize();
	createPropertyWidget();
}


void DiagramItem::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	initDiagramPainter(painter);
	painter.fillRect(this->rect(), m_params.m_backgroundcolor);
	GfxLibDiagramitemDrawer::draw(painter, this);
}


GfxLibDiagramitemDrawer::GfxLibDiagramitemDrawer(bool drawByPainter, bool drawByLoadpic, bool isdrawByPainter, std::shared_ptr<IDidgramDrawParams> params)
	:m_drawByPainter(drawByPainter)
	, m_drawByLoadpic(drawByLoadpic)
	, m_isdrawByPainter(isdrawByPainter)
	, m_drawParams(params)
{
}

void GfxLibDiagramitemDrawer::draw(QPainter& painter)
{
	if (m_drawByPainter && m_isdrawByPainter)
		drawByDraw(painter);
	else if(m_drawByLoadpic)
		drawByLoadpic(painter);
	else 
		throw std::runtime_error("error");

	//if (item->getdrawbypainter() && item->getisdrawbypainter())
		//drawByDraw(painter, item);
	//else if (item->getdrawbyloadpic())
		//drawByLoadpic(painter, item);
	//else
		//throw std::runtime_error("error");
}

QString DiagramItem::getpicpath()
{
	if (m_params.m_picpath.has_value())
		return m_params.m_picpath.value();
	else
		return QString();
}

void GfxLibDiagramitemDrawer::init()
{
	if (m_drawParams != nullptr)
	{

	}
}

void GfxLibDiagramitemDrawer::drawByLoadpic(QPainter& painter, DiagramItem* item) {
	QString picpath = item->getpicpath();
	if (picpath.isEmpty())
		throw std::runtime_error("error");

	QPixmap newpixmap = getSuitablePicPixmap(QPixmap(picpath), item);
	QPointF center = item->getselfdrawcenter();
	QPointF topleft = QPointF(center.x() - newpixmap.width() / 2.0, center.y() - newpixmap.height() / 2.0);

	painter.drawPixmap(topleft, newpixmap);
}

QPixmap GfxLibDiagramitemDrawer::getSuitablePicPixmap(QPixmap pixmap, DiagramItem* item)
{
	QSizeF space = item->getselfdrawspacesize();

	double targetwidth = space.width();
	double targetheight = space.height();

	QSizeF originalsize = pixmap.size();
	double scalefactor = qMin(targetwidth / originalsize.width(), targetheight / originalsize.height());
	QSizeF newsize = originalsize * scalefactor;
	return pixmap.scaled(newsize.toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}


//std::shared_ptr<tuxingjiedianparams> buildtuxingjiedianparamsfordiagramcircle::build(DiagramItem* item)
//{
//	std::shared_ptr<tuxingjiedianparamsyuanxing> params = std::make_shared<tuxingjiedianparamsyuanxing>();
//	return params;
//}

ShapeType DiagramItem::gettype()
{
	return m_params.m_type;
}










double DiagramItem::getDiagramItemRectRadio()
{
	if (!m_params.m_rectRadio.has_value())
		throw std::runtime_error("error");
	return m_params.m_rectRadio.value();
}





void GfxLibDiagramitemDrawer::drawByDraw(QPainter& painter)
{
	DiagramDrawInterface::draw(painter);
}

void DiagramItem::initWidgetSize()
{
	if (m_params.m_issizefixed)
	{
		if (m_params.m_fixsize.has_value())
		{
			setFixedSize(m_params.m_fixsize.value().toSize());
		}
		else
		{
			throw std::runtime_error("没有值");//todo:except
		}
	}
	else
	{
		setsizepolicyexpanding();
		initmaxandminsize();
	}
}
void DiagramItem::initmaxandminsize()
{
	if (!m_params.m_maxsize.has_value() || !m_params.m_minsize.has_value())
		throw std::runtime_error("error");//todo:except
	setMaximumSize(m_params.m_maxsize.value().toSize());
	setMinimumSize(m_params.m_minsize.value().toSize());
}

void DiagramItem::setsizepolicyexpanding()
{
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void DiagramItem::resizeEvent(QResizeEvent* event)
{
	//if (!m_iswidgetsizefixed)
	if (!m_params.m_issizefixed)
	{
		QSize allocated = event->size();
		//if (!m_widgetradio.has_value())
			//throw std::runtime_error("error");//todo:except
		if (!m_params.m_widgetradio.has_value())
			throw std::runtime_error("error");//todo:except
		const float ratio = m_params.m_widgetradio.value(); // 宽高比

		// 候选方案1：以宽度为主导
		int w1 = qBound(minimumSize().width(), allocated.width(), maximumSize().width());
		int h1 = qRound(w1 / ratio);
		h1 = qBound(minimumSize().height(), h1, maximumSize().height());
		QSize candidateW(w1, h1);

		// 候选方案2：以高度为主导
		int h2 = qBound(minimumSize().height(), allocated.height(), maximumSize().height());
		int w2 = qRound(h2 * ratio);
		w2 = qBound(minimumSize().width(), w2, maximumSize().width());
		QSize candidateH(w2, h2);

		// 选择更接近原始比例的方案（或面积更大的）
		float ratioDiffW = qAbs(candidateW.width() / (float)candidateW.height() - ratio);
		float ratioDiffH = qAbs(candidateH.width() / (float)candidateH.height() - ratio);

		QSize finalSize = (ratioDiffW < ratioDiffH) ? candidateW : candidateH;
		this->resize(finalSize);
	}
}
QSizeF DiagramItem::getselfdrawspacesize()
{
	return size();
}

QPointF DiagramItem::getselfdrawcenter()
{
	return QPointF(width() / 2.0, height() / 2.0);
}


DiagramItem::DiagramItem(GfxLibDiagramItemParams params, QWidget* parent)
	:m_params(params)
	, QWidget(parent)
	, dragstartposition(QPoint(0, 0))
	//, m_propertyWidgetManger(nullptr)
{
	using Type = PropertyWidgetManager::propertyobjecttype;
	switch (params.m_type)
	{

	case ShapeType::Rect:
		m_propertywidgettype = Type::diagramRect;
		break;
	case ShapeType::Circle:
		m_propertywidgettype = Type::diagramCircle;
		break;
	case ShapeType::Triangle:
		m_propertywidgettype = Type::diagramTriangle;
		break;
	case ShapeType::Line:
		m_propertywidgettype = Type::diagramLine;
		break;
	default:
		throw std::runtime_error("error");
		break;
	}
	init();
}

DiagramDrawParamsTriangle::TriangleSizeRadios DiagramItem::gettrianglesideradio()
{
	if (!m_params.m_triangleSideRadios.has_value())
		throw std::runtime_error("error");
	return m_params.m_triangleSideRadios.value();
}


DiagramDrawParamsTriangle::EdgeType DiagramItem::getedgetype()
{
	if (!m_params.m_triangleEdgeType.has_value())
		throw std::runtime_error("error");
	return m_params.m_triangleEdgeType.value();
}

double DiagramItem::getTriangleRotate()
{
	if (!m_params.m_triangleEdgeRotate.has_value())
		throw std::runtime_error("error");
	return m_params.m_triangleEdgeRotate.value();
}

double DiagramItem::getLineRotate()
{
	if (!m_params.m_linerotate.has_value())
		throw std::runtime_error("error");
	return m_params.m_linerotate.value();
}

double DiagramItem::getCircleBoundingrectradio()
{
	if (!m_params.m_circleboundingrectradio.has_value())
		throw std::runtime_error("error");
	return m_params.m_circleboundingrectradio.value();
}

void GfxLibDiagramItemParams::setLineRotate(double rotate)
{
	m_linerotate = rotate;
}

void GfxLibDiagramItemParams::defaultinit()
{
	//m_issizefixed = cfggetval<bool>(qtcf::tuxingku::diagramwidget::issizefix);
	if (m_issizefixed)
	{
		m_fixsize = QSizeF(cfggetval<double>(qtcf::tuxingku::diagramwidget::fixwidth), cfggetval<float>(qtcf::tuxingku::diagramwidget::fixheight));
	}
	else
	{
		m_widgetradio = cfggetval<double>(qtcf::tuxingku::diagramwidget::widgetradio);
		m_maxsize = QSizeF(cfggetval<double>(qtcf::tuxingku::diagramwidget::maxwidth), cfggetval<float>(qtcf::tuxingku::diagramwidget::maxheight));
		m_minsize = QSizeF(cfggetval<double>(qtcf::tuxingku::diagramwidget::minwidth), cfggetval<float>(qtcf::tuxingku::diagramwidget::minheight));
	}
}


void GfxLibDiagramItemParams::otherInitAfterType()
{
	switch (m_type)
	{
	case ShapeType::Rect:
	{
		m_drawByPainter = cfggetval<bool>(qtcf::tuxing::rectangle::drawbypainter);
		m_drawByloadpic = cfggetval<bool>(qtcf::tuxing::rectangle::drawbyloadpic);
		if (m_drawByloadpic)
			m_picpath = QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::rectangle::imagepath));
		m_rectRadio = cfggetval<double>(qtcf::tuxing::rectangle::radio);
		m_rectRotate = cfggetval<int>(qtcf::tuxing::rectangle::rotate);
		m_scale = cfggetval<double>(qtcf::tuxing::rectangle::scale);
	}
	break;
	case ShapeType::Circle:
	{
		m_drawByPainter = cfggetval<bool>(qtcf::tuxing::circle::drawbypainter);
		m_drawByloadpic = cfggetval<bool>(qtcf::tuxing::circle::drawbyloadpic);
		if (m_drawByloadpic)
			m_picpath = QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::circle::imagepath));
		m_circleboundingrectradio = cfggetval<double>(qtcf::tuxing::circle::boundingrectradio);
		m_circlerotate = cfggetval<int>(qtcf::tuxing::circle::rotate);
		m_scale = cfggetval<double>(qtcf::tuxing::circle::scale);
	}
	break;
	case ShapeType::Triangle:
	{
		m_drawByPainter = cfggetval<bool>(qtcf::tuxing::triangle::drawbypainter);
		m_drawByloadpic = cfggetval<bool>(qtcf::tuxing::triangle::drawbyloadpic);
		if (m_drawByloadpic)
			m_picpath = QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::triangle::imagepath));
		m_triangleSideRadios = DiagramDrawParamsTriangle::TriangleSizeRadios(cfggetval<double>(qtcf::tuxing::triangle::edgeradio::bottom)
			, cfggetval<double>(qtcf::tuxing::triangle::edgeradio::left)
			, cfggetval<double>(qtcf::tuxing::triangle::edgeradio::right));
		m_triangleEdgeType = DiagramDrawParamsTriangle::edgetypeStringToEnum(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::triangle::edgetype)));
		m_triangleEdgeRotate = cfggetval<double>(qtcf::tuxing::triangle::totate);
		m_scale = cfggetval<double>(qtcf::tuxing::triangle::scale);
	}
	break;
	case ShapeType::Line:
	{
		m_drawByPainter = cfggetval<bool>(qtcf::tuxing::line::drawbypainter);
		m_drawByloadpic = cfggetval<bool>(qtcf::tuxing::line::drawbyloadpic);
		if (m_drawByloadpic)
			m_picpath = QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::line::imagepath));
		m_linerotate = cfggetval<double>(qtcf::tuxing::line::rotate);
		m_scale = cfggetval<double>(qtcf::tuxing::line::scale);
	}
	break;
	default:
		throw std::runtime_error("error");
		break;
	}
}


GfxLibDiagramItemParams::GfxLibDiagramItemParams(ShapeType type)
	: m_issizefixed(cfggetval<bool>(qtcf::tuxingku::diagramwidget::issizefix))
	, m_widgetradio(std::nullopt)
	, m_fixsize(std::nullopt)
	, m_maxsize(std::nullopt)
	, m_minsize(std::nullopt)
	, m_pen(QPen(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::pen::color))), cfggetval<double>(qtcf::tuxing::all::painter::pen::width)))
	, m_brush(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::all::painter::brush))))
	, m_mimetype(QString::fromStdString(cfggetval<std::string>(qtcf::mimetype)))
	, m_backgroundcolor(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::tuxingku::diagramwidget::backgroundcolor))))
	, m_type(type)
	, m_picpath(std::nullopt)
	, m_drawByPainter(false)
	, m_drawByloadpic(false)
	, m_isdrawByPainter(cfggetval<bool>(qtcf::tuxingku::diagramwidget::isdrawbypainter))
	, m_rectRadio(std::nullopt)
	, m_triangleSideRadios(std::nullopt)
	, m_triangleEdgeType(std::nullopt)
	, m_triangleEdgeRotate(std::nullopt)
	, m_circleboundingrectradio(std::nullopt)
	, m_linerotate(std::nullopt)
	, m_spacesize(QSizeF(cfggetval<double>(qtcf::tuxing::all::spacesize::width), cfggetval<double>(qtcf::tuxing::all::spacesize::height)))
{
	defaultinit();
	otherInitAfterType();
}

void GfxLibDiagramItemParams::setRectRadio(double radio)
{
	m_rectRadio = radio;
}

void GfxLibDiagramItemParams::setRectRotate(int rotate)
{
	m_rectRotate = rotate;
}

void GfxLibDiagramItemParams::setTriangleSideRadio(double bottom, double left, double right)
{
	m_triangleSideRadios = DiagramDrawParamsTriangle::TriangleSizeRadios(bottom, left, right);
}

void GfxLibDiagramItemParams::setTriangleEdgeType(const QString& edgetype)
{
	m_triangleEdgeType = DiagramDrawParamsTriangle::edgetypeStringToEnum(edgetype);
}

void GfxLibDiagramItemParams::setTriangleRotate(double rotate)
{
	m_triangleEdgeRotate = rotate;
}

void GfxLibDiagramItemParams::setCircleRadio(double radio)
{
	m_circleboundingrectradio = radio;
}

void GfxLibDiagramItemParams::setCircleRotate(int rotate)
{
	m_circlerotate = rotate;
}
