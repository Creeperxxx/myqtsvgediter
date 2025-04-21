#include "tuxing.h"
#include "shuxingwidget.h"

void DiagramItem::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		dragstartposition = event->pos();

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
	//drag->setHotSpot(QPoint(25, 25));
	drag->setHotSpot(drag->pixmap().rect().center());
	drag->exec(Qt::CopyAction);
}
//
//void IDiagramItem::initmimetype(QString mimetype)
//{
//	m_mimetype = mimetype;
//}

QMimeData* DiagramItem::createDragMimeData()
{
	if (m_params.m_mimetype.isEmpty())
	{
		throw std::runtime_error("error");//todo:except
	}
	QMimeData* diagramDragMimedata = new QMimeData();
	//diagramDragMimedata->setData(m_params.m_mimetype, QByteArray::number(static_cast<int>(m_params.m_type)));
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

//std::shared_ptr<tuxingjiedianparams> DiagramItem::createpixmapparams()
//{
//	switch (m_params.m_type)
//	{
//	default:
//	case ShapeType::juxing:
//	{
//		std::shared_ptr<tuxingjiedianparamsjuxing> p = std::make_shared<tuxingjiedianparamsjuxing>();
//		if (!m_params.m_juxingradio.has_value())
//			throw std::runtime_error("error");//todo:except
//
//		p->m_radio = m_params.m_juxingradio.value();
//		return p;
//	}
//	break;
//	case ShapeType::yuanxing:
//	{
//		std::shared_ptr<tuxingjiedianparamsyuanxing> p = std::make_shared<tuxingjiedianparamsyuanxing>();
//		return p;
//	}
//	break;
//	}
//}

//QPainter* DiagramItem::getDiagramItemPainter()
//{
//	QPainter* painter = new QPainter(this);
//	painter->setPen(m_params.m_pen);
//	painter->setBrush(m_params.m_brush);
//	painter->setRenderHint(QPainter::Antialiasing, true);
//	return painter;
//}

//float DiagramItem::getHuabuRectRadio()
//{
//	if (!m_params.m_juxingradio.has_value())
//		throw std::runtime_error("error");
//	return m_params.m_juxingradio.value();
//}







QSizeF DiagramItem::getPixmapSpaceSize()
{
	return m_huabuspacesize;
}

//qreal DiagramItem::getPixmapScale()
//{
//	return m_pixmapScale;
//}



QPen DiagramItem::gethuabupen()
{
	//return m_params.m_huabutuxingpen;
	return m_huabupen;
}

QBrush DiagramItem::gethuabubrush()
{
	//return m_params.m_huabutuxingbrush;
	return m_huabubrush;
}

QPen DiagramItem::getdiagrampen()
{
	return m_params.m_pen;
}

QBrush DiagramItem::getdiagrambrush()
{
	return m_params.m_brush;
}







QPoint DiagramItem::getPixmapCenter()
{
	//return QPoint(m_diagramDragPixmap->width() / 2, m_diagramDragPixmap->height() / 2);
	return QPoint(m_params.m_huabutuxingspacesize.width() / 2, m_params.m_huabutuxingspacesize.height() / 2);
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

void DiagramItem::onValueChanged(QString name, EditItemDataType type, QVariant value)
{
	if (name == propertynameradio)
	{
		if (type != EditItemDataType::Double)
			throw std::runtime_error("error");
		double realvalue = value.toDouble();
		switch (m_params.m_type)
		{
		case ShapeType::Rect:
			onRectRadioChanged(realvalue);
			break;
		case ShapeType::Circle:
			onCircleRadioChanged(realvalue);
			break;
		default:
			throw std::runtime_error("error");
			break;
		}
	}
	else
	{

	}
}

void DiagramItem::onRectRadioChanged(double newradio)
{
	m_params.m_rectRadio = newradio;
	update();
}

void DiagramItem::onRectRotateChanged(int newrotate)
{
	m_params.m_rectRotate = newrotate;
	update();
}

void DiagramItem::onCircleRadioChanged(double newradio)
{
	m_params.m_circleboundingrectradio = newradio;
	update();
}

void DiagramItem::onCircleRotateChanged(int newrottate)
{
	m_params.m_circlerotate = newrottate;
	update();
}

void DiagramItem::onTriangleSideRadioChangedBottom(double bottom)
{
	m_params.m_triangleSideRadios->m_bottom = bottom;
	update();
}

void DiagramItem::onTriangleSideRadioChangedLeft(double left)
{
	m_params.m_triangleSideRadios->m_left = left;
	update();
}

void DiagramItem::onTriangleSideRadioChangedRight(double right)
{
	m_params.m_triangleSideRadios->m_right = right;
	update();
}

void DiagramItem::onTriangleEdgeTypeChanged(DiagramDrawParamsTriangle::EdgeType type)
{
	m_params.m_triangleEdgeType = type;
	update();
}

void DiagramItem::onTriangleEdgeRotateChanged(int rotate)
{
	m_params.m_triangleEdgeRotate = rotate;
	update();
}

void DiagramItem::onLineRotateChanged(int rotate)
{
	m_params.m_linerotate = rotate;
	update();
}




void DiagramItem::onPenColorChanged(QColor newcolor)
{
	m_params.m_pen.setColor(newcolor);
	update();
}

void DiagramItem::onPenWidthChanged(double newwidth)
{
	m_params.m_pen.setWidth(newwidth);
	update();
}

void DiagramItem::onPenBrushChanged(QBrush newbrush)
{
	m_params.m_brush = newbrush;
	update();
}

void DiagramItem::setPropertyWidgetManger(PropertyWidgetManager* manager)
{
	m_propertyWidgetManger = manager;
}

void DiagramItem::createPropertyWidget()
{
	switch (m_params.m_type)
	{
	case ShapeType::Rect:
		m_propertyWidgetManger->createPropertyWidget(PropertyWidgetManager::propertyobjecttype::diagramRect, this);
		break;
	case ShapeType::Circle:
		m_propertyWidgetManger->createPropertyWidget(PropertyWidgetManager::propertyobjecttype::diagramCircle, this);
		break;
	case ShapeType::Triangle:
		m_propertyWidgetManger->createPropertyWidget(PropertyWidgetManager::propertyobjecttype::diagramTriangle, this);
		break;
	case ShapeType::Line:
		m_propertyWidgetManger->createPropertyWidget(PropertyWidgetManager::propertyobjecttype::diagramLine, this);
		break;
	default:
		throw std::runtime_error("error");
	}
}

void DiagramItem::setPropertyWidgetKey(QString key)
{
	m_propertyWidgetKey = key;
}

std::shared_ptr<IDidgramDrawParams> DiagramItem::builddrawparamsrest(std::shared_ptr<IDidgramDrawParams> params)
{
	//params->m_brush = m_params.m_brush;
	//params->m_pen = m_params.m_pen;
	params->m_center = getselfdrawcenter();
	params->m_spacesize = getselfdrawspacesize();
	params->m_type = m_params.m_type;
	params->m_scale = m_params.m_scale;
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
	//params->m_brush = m_huabubrush;
	//params->m_pen = m_huabupen;
	params->m_center = getPixmapCenter();
	params->m_spacesize = getPixmapSpaceSize();
	params->m_type = m_params.m_type;
	params->m_scale = m_params.m_scale;
	return params;
}

//QPixmap IDiagramItem::createPixmap(QSize targetwidgetsize, QPen targetpen, QBrush targetbrush, QColor targetbackgroundcolor)
//void DiagramItem::drawDiagramPixmap()
QPixmap DiagramItem::drawDiagramPixmap()
{
	//initDiagramPixmap();
	QPixmap pixmap(getPixmapSpaceSize().toSize());
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	initDiagramPixmapPainter(painter);

	//DiagramDrawInterface::draw(painter, FactoryBuildDiagramPixmapParams::create(m_params.m_type)->build(this));
	//DiagramDrawInterface::draw(painter, factoryall::create(DiagramItemType::tuxingkupixmap, m_params.m_type)->build(this));
	DiagramDrawInterface::draw(painter, buildPixmapDrawParams());

	return pixmap;

	//QPixmap pixmap(m_params.m_huabutuxingspacesize);
	////pixmap.fill(m_params.m_huabubackgroundcolor);
	//pixmap.fill(Qt::transparent);
	//QPainter painter(&pixmap);
	//painter.setPen(m_params.m_huabutuxingpen);
	//painter.setBrush(m_params.m_huabutuxingbrush);
	//painter.setRenderHint(QPainter::Antialiasing, true);
	////std::shared_ptr<tuxingjiedianparams> params = factorytuxingparams::create(m_params.m_type);
	//std::shared_ptr<tuxingjiedianparams> params = createpixmapparams();
	//params->m_center = QPoint(pixmap.rect().x() + pixmap.width() / 2, pixmap.rect().y() + pixmap.height() / 2);
	//params->m_spacesize = (m_params.m_huabutuxingspacesize);
	//params->m_type = m_params.m_type;
	//params->m_painter = &painter;

	//DiagramDrawInterface::draw(params);
	////painter.fillRect(pixmap.rect(), Qt::white);
	//return pixmap;
}
//
//void DiagramItem::initDiagramPixmap()
//{
//	m_diagramDragPixmap = new QPixmap(m_params.m_huabutuxingspacesize);
//}
//QPixmap IDiagramItem::drawPixmap()
//{
	//QImage image("D:/Database/Code/vs/c++/KDevelop-Training/KDevelop-Training/ClassExamProject/QtChartFlow/QtChartFlow/juxing.png");
	//QPixmap pixmap = QPixmap::fromImage(image);
	//QPixmap preview = this->grab(); //捕获当前控件
	//preview = preview.scaled(200, 200, Qt::KeepAspectRatio);//缩放
	//QPixmap translucentPreview(preview.size());
	//translucentPreview.fill(Qt::transparent);
	//QPainter painter(&translucentPreview);
	////painter.setOpacity(0.9); //70%不透明
	//painter.setRenderHint(QPainter::Antialiasing, true);
	//painter.drawPixmap(0, 0, preview);
	//painter.end();
	//return translucentPreview;
	//return this->grab();
	//QPixmap pixmap;
	//pixmap.fill(Qt::white);
	//QPainter painter(&pixmap);
	//painter.setPen(QPen(Qt::black, 3));
	//painter.setBrush(Qt::transparent);

	//return pixmap;
	//return this->grab();
//}
//IDiagramItem::IDiagramItem(ShapeType type, QString picturepath, QColor color, QWidget* parent)
//	: QWidget(parent)
//	, m_shapetype(type)
//	, m_picturePath(picturepath)
//	, m_backgroundcolor(color)
//{
//	init();
//}

void DiagramItem::init()
{
	initWidgetSize();
	//initDiagramDrawer();
	//createDragMimeData();
	//initDiagramPixmap();
}

//void IDiagramItem::init(QColor pencolor, int penwidth, QBrush penbrush, QSize widgetsize)
//void IDiagramItem::init(bool issizefixed, std::optional<QSize> fixsize, std::optional<QSize> maxsize, std::optional<QSize> minsize
//	, QColor color, int penwidth, QBrush brush, QString mimetype, QColor backgroundcolor, std::optional<float> widgetradio)
//{
//	m_mimetype = mimetype;
//	m_iswidgetsizefixed = issizefixed;
//	m_widgetradio = widgetradio;
//	m_backgroundcolor = backgroundcolor;
//	initWidgetSize(fixsize, maxsize, minsize);
//	initpenandbrush(color, penwidth, brush);
//	inittuxingzujian();
//
//}

//IDiagramItem::IDiagramItem(ShapeType type, QString picturePath, QWidget* parent)
//	:QWidget(parent)
//	, m_shapetype(type)
//	, m_picturePath(picturePath)
//{
//	init();
//	//initLayout();
//	//drawmyself(m_picturePath); //是否设置一个默认路径？
//	//initSizePolicy();
//	//setFixedSize(tuxingkubujianwidth, tuxingkubujianwidth / 2);
//	initpenandbrush();
//	initWidgetSize();
//	inittuxingzujian();
//	//setStyleSheet("background-color: white;");
//	//autoFillBackground();
//	//setAutoFillBackground(true);
//	//raise();
//}

//void IDiagramItem::initLayout()
//{
//	m_layout = new QHBoxLayout(this);
//	this->setLayout(m_layout);
//}

////juxingDiagramItem::juxingDiagramItem() :m_shapetype(ShapeType::juxing) {}
//juxingDiagramItem::juxingDiagramItem(QWidget* parent)
//	: IDiagramItem(ShapeType::juxing, parent)
//{
//	pixtureshow();
//}
//
//juxingDiagramItem::~juxingDiagramItem() {}

//void juxingDiagramItem::pixtureshow()
//{
//	QLabel* label = new QLabel(this);
//
//	// 加载资源文件中的图片
//	QPixmap pixmap(":/tuxingku/juxing.png");
//
//	// 设置标签的图片
//	label->setPixmap(pixmap);
//
//	// 如果需要调整标签大小以适应图片大小
//	label->setFixedSize(pixmap.width(), pixmap.height());
//	layout->addWidget(label);
//	//m_hlayout->addWidget(label);
//}

//void IDiagramItem::mysetlayout()
//{
//	m_hlayout = new QHBoxLayout();
//	setLayout(m_hlayout);
//}

//void IDiagramItem::drawmyself()
//{
//	QPainter painter(this);
//	QPen pen(Qt::black, 2);
//	painter.setPen(pen);
//	painter.setBrush(Qt::transparent);
//	QRect recv(0, 0, 40, 20);
//	painter.drawRect(recv);
//	this->setFixedSize(40, 20);
//}

//void IDiagramItem::initSizePolicy()
//{
//	setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
//}

void DiagramItem::paintEvent(QPaintEvent* event)
{
	//drawmyself();
	//QWidget::paintEvent(event);
	//updatetuxingsize();
	//QPainter* painter = getDiagramItemPainter();
	QPainter painter(this);
	initDiagramPainter(painter);
	painter.fillRect(this->rect(), m_params.m_backgroundcolor);
	GfxLibDiagramitemDrawer::draw(painter, this);
	//m_diagramDrawer->draw(painter, this);
}

//QPainter* DiagramItem::getDiagramItemPixmapPainter()
//{
//	QPainter* painter = new QPainter(m_diagramDragPixmap);
//	painter->setPen(m_params.m_huabutuxingpen);
//	painter->setBrush(m_params.m_huabutuxingbrush);
//	painter->setRenderHint(QPainter::Antialiasing, true);
//	return painter;
//}

//void IDiagramItem::loadpictureasmyself(QString path)
//{
//	QLabel* label = new QLabel(this);
//
//	// 加载资源文件中的图片
//	QPixmap pixmap(path);
//
//	// 设置标签的图片
//	label->setPixmap(pixmap);
//
//	// 如果需要调整标签大小以适应图片大小
//	label->setFixedSize(pixmap.width(), pixmap.height());
//	m_layout->addWidget(label);
//}

//void drawtuxingkuzujian::drawByDraw(QPainter* painter)
//{
//}

//void drawtuxingkuzujian::drawByLoadpic(QPainter* painter)
//{
//	QPixmap pixmap(picPath);
//	int targetwidth = sourceWidget.width();
//	int targetheight = sourceWidget.height();
//	QSize originalsize = pixmap.size();
//	double scalefactor = qMin(static_cast<double>(targetwidth) / originalsize.width(), static_cast<double>(targetheight) / originalsize.height());
//	QSize newsize = originalsize * scalefactor;
//	QPixmap newpixmap = pixmap.scaled(newsize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//	QPoint topleft((targetwidth - newsize.width()) / 2, (targetheight - newsize.height()) / 2);
//	painter->drawPixmap(topleft, newpixmap);
//}

//void GfxLibDiagramitemDrawer::draw(QPainter* painter, std::shared_ptr<GfxLibDiagramItemParams> params)
void GfxLibDiagramitemDrawer::draw(QPainter& painter, DiagramItem* item)
{
	if (!item)
		throw std::runtime_error("error");//todo
	//if (m_isdrawByPainter && m_drawByPainter)
	if (item->getdrawbypainter() && item->getisdrawbypainter())
		drawByDraw(painter, item);
	else if (item->getdrawbyloadpic())
		drawByLoadpic(painter, item);
	else
		throw std::runtime_error("error");
}

//m_rect = calcusuitablerect(painter->pen().width());
//painter->drawRect(m_rect);

//QRect drawtuxingkuzujianjuxing::getSuitableRect()
//{
//	double scalefactor = qMin(static_cast<double>(m_sourceWidgetSize.width()) / m_rectSize.width()
//		, static_cast<double>(m_sourceWidgetSize.width()) / m_rectSize.height());
//	QSize newsize = m_rectSize * scalefactor;
//	QPoint topleft = QPoint((m_sourceWidgetSize.width() - newsize.width()) / 2, (m_sourceWidgetSize.height() - newsize.height()) / 2);
//
//	return QRect(topleft, newsize);
//}

QString DiagramItem::getpicpath()
{
	if (m_params.m_picpath.has_value())
		return m_params.m_picpath.value();
	else
		return QString();
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




//std::shared_ptr<IDidgramDrawParams> GfxLibDiagramitemDrawer::buildparams(DiagramItem* item)
//{
//	//return factorybuildtuxingjiedianparamsfordiagram::create(item->gettype())->build(item);
//	//return factoryall::create(DiagramItemType::tuxingku, item->gettype())->build(item);
//
//	return item->builddrawparams();
//}

void GfxLibDiagramitemDrawer::drawByDraw(QPainter& painter, DiagramItem* item)
{
	DiagramDrawInterface::draw(painter, item->builddrawparams());
}

//GfxLibDiagramitemDrawer::GfxLibDiagramitemDrawer(bool drawbypainter, bool drawbyloadpic, bool isdrawbypainter)
	//:m_drawByPainter(drawbypainter)
	//, m_drawByLoadpic(drawbyloadpic)
	//, m_isdrawByPainter(isdrawbypainter) {
//}

//void GfxLibDiagramitemDrawer::drawByDraw(QPainter* painter)
//{
//	painter->drawRect(0, 0, 10, 10);
//}

//std::unique_ptr<drawtuxingkuzujian> drawtuxingkuzujianfactory::createtuxingzujian(ShapeType type)
//{
//	switch (type)
//	{
//	default:
//	case ShapeType::juxing:
//		return std::make_unique<drawtuxingkuzujianjuxing>();
//	case ShapeType::yuanxing:
//		return std::make_unique<drawtuxingkuzujianyuanxing>();
//	}
//}

//std::unique_ptr<drawtuxingkuzujian> IDiagramItem::gettuxignzujian(ShapeType type)
//{
//	std::unique_ptr<drawtuxingkuzujian> ptr = drawtuxingkuzujianfactory::createtuxingzujian(type);
//	return ptr;
//}
//drawtuxingkuzujian::drawtuxingkuzujian(QSize sourceWidgetSize, QString picpath = imagepathjuxing, bool drawbypainter = false, bool drawbyloadpic = false)
//	: m_sourceWidgetSize(m_sourceWidgetSize)
//	, m_picpath(picpath)
//	, m_drawByPainter(drawbypainter)
//	, m_drawByLoadpic(drawbyloadpic) {}

//drawtuxingkuzujianjuxing::drawtuxingkuzujianjuxing()
//{
//	drawtuxingkuzujian::m_drawByPainter = true;
//	drawtuxingkuzujian::m_drawByLoadpic = true;
//}

//drawtuxingkuzujianyuanxing::drawtuxingkuzujianyuanxing()
//{
//	drawtuxingkuzujian::m_drawByPainter = true;
//}

//void IDiagramItem::initWidgetSize(QSize size)
//void IDiagramItem::initWidgetSize(std::optional<QSize> fixsize, std::optional<QSize> maxsize, std::optional<QSize> minsize)
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
		//if (maxsize.has_value() && minsize.has_value())
		//if(m_params.m_maxsize.has_value() && m_params.m_minsize.has_value())
		//{
			//initmaxandminsize(m_params.m_maxsize.value(), m_params.m_minsize.value());
		//}
		//else
		//{
			//throw std::runtime_error("没有值");//todo:except
		//}
	}
	//if (isdiagramitemsizefix == true)
	//{
	//	this->setFixedSize(diagramitemwidth, diagramitemheight);
	//}
	//else
	//{
	//	// 设置size policy
	//	this->setsizepolicyexpanding();
	//	this->initmaxandminsize()
	//	//this->setMinimumSize(diatramitemminwidth, diatramitemminheight);
	//	//this->setMaximumSize(diagramitemmaxwidth, diagramitemmaxheight);
	//}
}

//void IDiagramItem::initmaxandminsize(QSize maxsize, QSize minsize)
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

//void drawtuxingkuzujian::setSourcewidgetsize(QSize size)
//{
//	m_sourceWidgetSize = size;
//}
//
//void drawtuxingkuzujian::setpicpath(QString path)
//{
//	m_picpath = path;
//}


//std::unique_ptr<drawtuxingkuzujian> factorydrawtuxingkuzujian::create(ShapeType type)
//{
//	switch (type)
//	{
//	default:
//	case ShapeType::juxing:
//		return std::make_unique<drawtuxingkuzujianjuxing>();
//		break;
//	case ShapeType::yuanxing:
//		return std::make_unique<drawtuxingkuzujianyuanxing>();
//		break;
//	}
//}
QSizeF DiagramItem::getselfdrawspacesize()
{
	return size();
}

QPointF DiagramItem::getselfdrawcenter()
{
	return QPointF(width() / 2.0, height() / 2.0);
}

//void DiagramItem::initDiagramDrawer()
//{
	//m_diagramDrawer = std::make_shared<GfxLibDiagramitemDrawer>(m_params.m_drawByPainter, m_params.m_drawByloadpic, m_params.m_isdrawByPainter);
//}

//std::shared_ptr<GfxLibDiagramitemDrawer> DiagramItem::createtuxing()
//{
//	switch (m_params.m_type)
//	{
//	default:
//	case ShapeType::juxing:
//	{
//		std::shared_ptr<GfxLibDiagramItemDrawerRect> p = std::make_shared<GfxLibDiagramItemDrawerRect>();
//		p->m_params = std::make_shared<tuxingjiedianparamsjuxing>();
//		dynamic_cast<tuxingjiedianparamsjuxing*>(p->m_params.get())->m_radio = m_params.m_juxingradio.value_or(diagramitemjuxingradio);
//		return p;
//	}
//	break;
//	case ShapeType::yuanxing:
//	{
//		std::shared_ptr<GfxLibDiagramItemDrawerCircle> p = std::make_shared<GfxLibDiagramItemDrawerCircle>();
//		p->m_params = std::make_shared<tuxingjiedianparamsyuanxing>();
//		return p;
//	}
//	break;
//	}
//}

//void DiagramItem::inittuxingzujian()
//{
//	m_diagramDrawer = createtuxing();
//	m_diagramDrawer->m_picpath = m_params.m_picpath.value_or(imagepathjuxing);
//	m_diagramDrawer->m_params->m_center = getcenter();
//	m_diagramDrawer->m_params->m_spacesize = getspacesize();
//	m_diagramDrawer->m_params->m_type = m_params.m_type;
//	//m_tuxing = factorydrawtuxingkuzujian::create(m_shapetype);
//	//m_tuxing->m_picpath = m_picturePath;
//	//m_tuxing->m_params->m_spacesize = getspacesize();
//	//m_tuxing->m_params->m_center = calculatebujiancentor();
//	//m_tuxing->m_params->m_type = m_shapetype;
//	//m_tuxing = drawtuxingkuzujianfactory::createtuxingzujian(m_shapetype);
//	//m_tuxing->setSourcewidgetsize(this->size());
//	//m_tuxing->setpicpath(m_picturePath);
//}

DiagramItem::DiagramItem(GfxLibDiagramItemParams params, QWidget* parent)
	:m_params(params)
	, QWidget(parent)
	, dragstartposition(QPoint(0, 0))
	, m_propertyWidgetManger(nullptr)
	, m_propertyWidgetKey("")
{
	m_huabupen = params.m_pen;
	m_huabubrush = params.m_brush;
	m_huabuspacesize = params.m_huabutuxingspacesize;
	m_pixmapScale = m_params.m_scale;
	init();
}

//GfxLibDiagramItemDrawerRect::GfxLibDiagramItemDrawerRect()
//{
//	m_drawByPainter = true;
//	m_drawByLoadpic = true;
//}

//// 建议的改进版本
//QRect drawtuxingkuzujianjuxing::calcusuitablerect(int penWidth)
//{
//	const int windowWidth = m_sourceWidgetSize.width();
//	const int windowHeight = m_sourceWidgetSize.height();
//
//	// 验证输入参数
//	if (windowWidth <= 0 || windowHeight <= 0 || penWidth < 0) {
//		return QRect();
//	}
//
//	// 确保矩形比例是合理的正值
//	const double rectangleAspectRatio = diagramitemjuxingradio;
//	if (rectangleAspectRatio <= 0.0) {
//		return QRect();
//	}
//
//	// 计算有效绘制区域
//	const int effectiveWidth = std::max(0, windowWidth - penWidth * 2);
//	const int effectiveHeight = std::max(0, windowHeight - penWidth * 2);
//
//	if (effectiveWidth == 0 || effectiveHeight == 0) {
//		return QRect();
//	}
//
//	// 计算适合的矩形尺寸
//	QSize rectSize;
//	const double effectiveRatio = static_cast<double>(effectiveWidth) / effectiveHeight;
//
//	if (effectiveRatio > rectangleAspectRatio) {
//		// 宽度受限
//		rectSize.setHeight(effectiveHeight);
//		rectSize.setWidth(static_cast<int>(std::round(effectiveHeight * rectangleAspectRatio)));
//	}
//	else {
//		// 高度受限
//		rectSize.setWidth(effectiveWidth);
//		rectSize.setHeight(static_cast<int>(std::round(effectiveWidth / rectangleAspectRatio)));
//	}
//
//	// 验证计算结果
//	if (rectSize.width() <= 0 || rectSize.height() <= 0) {
//		return QRect();
//	}
//
//	// 计算中心位置（不需要再加penWidth，因为effective已经考虑了）
//	const QPoint center(
//		(windowWidth - rectSize.width()) / 2,
//		(windowHeight - rectSize.height()) / 2
//	);
//
//	return QRect(center, rectSize);
//}
//
//int drawtuxingkuzujianyuanxing::calcusuitable(int penwidth)
//{
//	return (qMin(m_sourceWidgetSize.height(), m_sourceWidgetSize.width()) - penwidth) / 2;
//}

//void GfxLibDiagramItemDrawerCircle::drawByDraw(QPainter* painter)
//{
	//m_params->m_painter = painter;
	//DiagramDrawInterface::draw(m_params);
	//m_r = calcusuitable(painter->pen().width());
	//QPoint center = QPoint(m_sourceWidgetSize.width() / 2, m_sourceWidgetSize.height() / 2);
	//painter->drawEllipse(center, m_r, m_r);
//}

//GfxLibDiagramItemDrawerCircle::GfxLibDiagramItemDrawerCircle()
//{
	//m_drawByPainter = true;
//}

//void IDiagramItem::initpenandbrush(QColor color, int penwidth, QBrush brush)
//void IDiagramItem::initpenandbrush()
//{
	//m_pen = QPen(diagramitempencolor, diagramitempenwidth);
	//m_brush = diagramitembrush;
	//m_pen = QPen(color, penwidth);
	//m_brush = brush;

//}

//void DiagramItem::updatetuxingsize()
//{
	//if (!m_iswidgetsizefixe)
	//if (!m_params.m_issizefixed)
	//{
		//m_tuxing->m_params->m_spacesize = getspacesize();
		//m_tuxing->m_params->m_center = calculatebujiancentor();
	//}
//}


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
	//m_pen = QPen(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::tuxingku::diagramwidget::pen::color))), cfggetval<int>(qtcf::tuxingku::diagramwidget::pen::width));
	//m_brush = QBrush(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::tuxingku::diagramwidget::brush))));
	//m_mimetype = QString::fromStdString(cfggetval<std::string>(qtcf::mimetype));
	//m_backgroundcolor = QColor(QString::fromStdString(cfggetval<std::string>(qtcf::tuxingku::diagramwidget::backgroundcolor)));
	//m_huabutuxingspacesize = QSizeF(cfggetval<double>(qtcf::huabu::tuxingspace::spacewidth), cfggetval<double>(qtcf::huabu::tuxingspace::spaceheight));
	//m_huabutuxingpen = QPen(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::tuxingspace::pen::color))), cfggetval<int>(qtcf::huabu::tuxingspace::pen::width));
	//m_huabutuxingbrush = QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::tuxingspace::brush)));
	//m_isdrawByPainter = cfggetval<bool>(qtcf::tuxingku::diagramwidget::isdrawbypainter);
}

//GfxLibDiagramItemParams::GfxLibDiagramItemParams(bool issizefixed
//	, std::optional<QSizeF> fixsize
//	, std::optional<QSizeF> maxsize
//	, std::optional<QSizeF> minsize
//	, QPen pen
//	, QBrush brush
//	, QString mimetype
//	, QColor backgroundcolor
//	, std::optional<QString> picpath
//	, std::optional<float> juxingradio
//	, QSizeF huabutuxingspacesize
//	, QPen huabutuxingpen
//	, QBrush huabutuxingbrush
//	//, QColor haububackgroundcolor
//	//, std::optional<float> widgetradio
//	, ShapeType type
//	, bool drawbypainter
//	, bool drawbyloadpic
//	, bool isdrawbypainter)
//	: m_issizefixed(issizefixed)
//	, m_fixsize(fixsize)
//	, m_maxsize(maxsize)
//	, m_minsize(minsize)
//	, m_pen(pen)
//	, m_brush(brush)
//	, m_mimetype(mimetype)
//	, m_backgroundcolor(backgroundcolor)
//	//, m_widgetradio(widgetradio)
//	, m_type(type)
//	, m_picpath(picpath)
//	, m_juxingradio(juxingradio)
//	, m_huabutuxingspacesize(huabutuxingspacesize)
//	, m_huabutuxingpen(huabutuxingpen)
//	, m_huabutuxingbrush(huabutuxingbrush)
//	, m_drawByPainter(drawbypainter)
//	, m_drawByloadpic(drawbyloadpic)
//	, m_isdrawByPainter(isdrawbypainter) {
//}
//, m_huabubackgroundcolor(huabubackgroundcolor) {}

//GfxLibDiagramItemParams::builder::builder()
//{
//	defaultinit();
//}
//
//void GfxLibDiagramItemParams::builder::defaultinit()
//{
//	m_issizefixed = cfggetval<bool>(qtcf::tuxingku::diagramwidget::issizefix);
//	if (m_issizefixed)
//	{
//		m_fixsize = QSizeF(cfggetval<double>(qtcf::tuxingku::diagramwidget::fixwidth), cfggetval<float>(qtcf::tuxingku::diagramwidget::fixheight));
//	}
//	else
//	{
//		m_maxsize = QSizeF(cfggetval<double>(qtcf::tuxingku::diagramwidget::maxwidth), cfggetval<float>(qtcf::tuxingku::diagramwidget::maxheight));
//		m_minsize = QSizeF(cfggetval<double>(qtcf::tuxingku::diagramwidget::minwidth), cfggetval<float>(qtcf::tuxingku::diagramwidget::minheight));
//	}
//	m_pen = QPen(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::tuxingku::diagramwidget::pen::color))), cfggetval<int>(qtcf::tuxingku::diagramwidget::pen::width));
//	m_brush = QBrush(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::tuxingku::diagramwidget::brush))));
//	m_mimetype = QString::fromStdString(cfggetval<std::string>(qtcf::mimetype));
//	m_backgroundcolor = QColor(QString::fromStdString(cfggetval<std::string>(qtcf::tuxingku::diagramwidget::backgroundcolor)));
//	m_huabutuxingspacesize = QSizeF(cfggetval<double>(qtcf::huabu::tuxingspace::spacewidth), cfggetval<double>(qtcf::huabu::tuxingspace::spaceheight));
//	m_huabutuxingpen = QPen(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::tuxingspace::pen::color))), cfggetval<int>(qtcf::huabu::tuxingspace::pen::width));
//	m_huabutuxingbrush = QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::tuxingspace::brush)));
//	m_isdrawByPainter = cfggetval<bool>(qtcf::tuxingku::diagramwidget::isdrawbypainter);
//
//
//	//m_issizefixed = cfggetval<bool>(qtcf::diagramWidgetIsSizeFix);
//	//m_fixsize = QSize(cfggetval<int>(qtcf::diagramWidgetFixWidth), cfggetval<int>(qtcf::diagramWidgetFixWidth) / cfggetval<float>(qtcf::diagramWidgetRadio));
//	//m_maxsize = QSize(cfggetval<int>(qtcf::diagramWidgetMaxWidth), cfggetval<int>(qtcf::diagramWidgetMaxWidth) / cfggetval<float>(qtcf::diagramWidgetRadio));
//	//m_minsize = QSize(cfggetval<int>(qtcf::diagramWidgetMinWidth), cfggetval<int>(qtcf::diagramWidgetMinWidth) / cfggetval<float>(qtcf::diagramWidgetRadio));
//	//m_pen = QPen(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::diagramWidgetPenColor))), cfggetval<int>(qtcf::diagramWidgetPenWidth));
//	//m_brush = QBrush(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::diagramWidgetBrush))));
//	//m_mimetype = QString::fromStdString(cfggetval<std::string>(qtcf::mimeType));
//	//m_backgroundcolor = QColor(QString::fromStdString(cfggetval<std::string>(qtcf::diagramWidgetBackgroundColor)));
//	//m_widgetradio = cfggetval<float>(qtcf::diagramWidgetRadio);
//	//m_huabutuxingspacesize = QSize(cfggetval<int>(qtcf::huabuTuxingSpaceSpaceWidth), cfggetval<int>(qtcf::huabuTuxingSpaceSpaceWidth) / cfggetval<float>(qtcf::huabuTuxingSpaceSpaceradio));
//	//m_huabutuxingpen = QPen(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabuTuxingSpacePenColor))), cfggetval<int>(qtcf::huabuTuxingSpacePenWidth));
//	//m_huabutuxingbrush = QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabuTuxingSpaceBrush)));
//	//m_isdrawByPainter = cfggetval<bool>(qtcf::diagramWidgetIsDrawByPainter);
//}

//void GfxLibDiagramItemParams::builder::setsomethingaftertype(ShapeType type)
//{
//	switch (type)
//	{
//	case ShapeType::Rect:
//	{
//		m_drawByPainter = cfggetval<bool>(qtcf::tuxing::rectangle::drawbypainter);
//		m_drawByloadpic = cfggetval<bool>(qtcf::tuxing::rectangle::drawbyloadpic);
//		if (m_drawByloadpic)
//			m_picpath = QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::rectangle::imagepath));
//		m_juxingradio = cfggetval<double>(qtcf::tuxing::rectangle::radio);
//	}
//	break;
//	case ShapeType::Circle:
//	{
//		m_drawByPainter = cfggetval<bool>(qtcf::tuxing::circle::drawbypainter);
//		m_drawByloadpic = cfggetval<bool>(qtcf::tuxing::circle::drawbyloadpic);
//		if (m_drawByloadpic)
//			m_picpath = QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::circle::imagepath));
//
//	}
//	break;
//	case ShapeType::Triangle:
//	{
//		m_drawByPainter = cfggetval<bool>(qtcf::tuxing::triangle::drawbypainter);
//		m_drawByloadpic = cfggetval<bool>(qtcf::tuxing::triangle::drawbyloadpic);
//		if (m_drawByloadpic)
//			m_picpath = QString::fromStdString(cfggetval<std::string>(qtcf::tuxing::triangle::imagepath));
//	}
//	break;
//	default:
//		qDebug() << "void GfxLibDiagramItemParams::builder::setDrawByPainterandloadpicandis(ShapeType type)";
//		break;
//	}
//}

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

//GfxLibDiagramItemParams::builder& GfxLibDiagramItemParams::builder::settype(ShapeType type)
//{
//	m_type = type;
//	return *this;
//}

//GfxLibDiagramItemParams::builder& GfxLibDiagramItemParams::builder::setpicpath(QString path)
//{
//	m_picpath = path;
//	return *this;
//}

//GfxLibDiagramItemParams::builder& GfxLibDiagramItemParams::builder::setjuxingradio(float radio)
//{
//	m_juxingradio = radio;
//	return *this;
//}
//
//GfxLibDiagramItemParams::builder& GfxLibDiagramItemParams::builder::setdrawbypainter(bool flag)
//{
//	m_drawByPainter = flag;
//	return *this;
//}
//
//GfxLibDiagramItemParams::builder& GfxLibDiagramItemParams::builder::setdrawbyloadpic(bool flag)
//{
//	m_drawByloadpic = flag;
//	return *this;
//}
//
//GfxLibDiagramItemParams GfxLibDiagramItemParams::builder::build()
//{
//	return GfxLibDiagramItemParams(m_issizefixed,
//		m_fixsize,
//		m_maxsize,
//		m_minsize,
//		m_pen,
//		m_brush,
//		m_mimetype,
//		m_backgroundcolor,
//		m_picpath,
//		m_juxingradio,
//		m_huabutuxingspacesize,
//		m_huabutuxingpen,
//		m_huabutuxingbrush,
//		m_widgetradio,
//		m_type,
//		m_drawByPainter,
//		m_drawByloadpic,
//		m_isdrawByPainter);
//}

GfxLibDiagramItemParams::GfxLibDiagramItemParams(ShapeType type)
	: m_issizefixed(cfggetval<bool>(qtcf::tuxingku::diagramwidget::issizefix))
	, m_widgetradio(std::nullopt)
	, m_fixsize(std::nullopt)
	, m_maxsize(std::nullopt)
	, m_minsize(std::nullopt)
	//, m_pen(QPen(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::tuxingku::diagramwidget::pen::color))), cfggetval<int>(qtcf::tuxingku::diagramwidget::pen::width)))
	//, m_brush(QBrush(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::tuxingku::diagramwidget::brush)))))
	, m_pen(QPen(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::painter::pen::color))), cfggetval<double>(qtcf::painter::pen::width)))
	, m_brush(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::painter::brush))))
	, m_mimetype(QString::fromStdString(cfggetval<std::string>(qtcf::mimetype)))
	, m_backgroundcolor(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::tuxingku::diagramwidget::backgroundcolor))))
	, m_type(type)
	, m_picpath(std::nullopt)
	, m_huabutuxingspacesize(QSizeF(cfggetval<double>(qtcf::huabu::tuxingspace::spacewidth), cfggetval<double>(qtcf::huabu::tuxingspace::spaceheight)))
	//, m_huabutuxingpen(QPen(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::tuxingspace::pen::color))), cfggetval<int>(qtcf::huabu::tuxingspace::pen::width)))
	//, m_huabutuxingbrush(QColor(QString::fromStdString(cfggetval<std::string>(qtcf::huabu::tuxingspace::brush))))
	, m_drawByPainter(false)
	, m_drawByloadpic(false)
	, m_isdrawByPainter(cfggetval<bool>(qtcf::tuxingku::diagramwidget::isdrawbypainter))
	, m_rectRadio(std::nullopt)
	, m_triangleSideRadios(std::nullopt)
	, m_triangleEdgeType(std::nullopt)
	, m_triangleEdgeRotate(std::nullopt)
	, m_circleboundingrectradio(std::nullopt)
	, m_linerotate(std::nullopt)
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
