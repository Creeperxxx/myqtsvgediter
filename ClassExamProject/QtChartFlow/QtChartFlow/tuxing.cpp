#include "tuxing.h"


void IDiagramItem::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		dragstartposition = event->pos();
	}
}

void IDiagramItem::mouseMoveEvent(QMouseEvent* event)
{
	if (!(event->buttons() & Qt::LeftButton))
	{
		return;
	}
	if ((event->pos() - dragstartposition).manhattanLength() < QApplication::startDragDistance())
		return;

	QDrag* drag = new QDrag(this);
	drag->setMimeData(createMimedata());
	drag->setPixmap(createPixmap());
	//drag->setHotSpot(QPoint(25, 25));
	drag->setHotSpot(drag->pixmap().rect().center());
	drag->exec(Qt::CopyAction);
}
//
//void IDiagramItem::initmimetype(QString mimetype)
//{
//	m_mimetype = mimetype;
//}

QMimeData* IDiagramItem::createMimedata()
{
	if (m_params.m_mimetype.isEmpty())
	{
		throw std::runtime_error("error");//todo:except
	}
	QMimeData* mimeData = new QMimeData();
	mimeData->setData(m_params.m_mimetype, QByteArray::number(static_cast<int>(m_params.m_type)));
	return mimeData;
}

std::shared_ptr<tuxingjiedianparams> IDiagramItem::createpixmapparams()
{
	switch (m_params.m_type)
	{
	default:
	case ShapeType::juxing:
	{
		std::shared_ptr<tuxingjiedianparamsjuxing> p = std::make_shared<tuxingjiedianparamsjuxing>();
		if (!m_params.m_juxingradio.has_value())
			throw std::runtime_error("error");//todo:except

		p->m_radio = m_params.m_juxingradio.value();
		return p;
	}
	break;
	case ShapeType::yuanxing:
	{
		std::shared_ptr<tuxingjiedianparamsyuanxing> p = std::make_shared<tuxingjiedianparamsyuanxing>();
		return p;
	}
	break;
	}
}


//QPixmap IDiagramItem::createPixmap(QSize targetwidgetsize, QPen targetpen, QBrush targetbrush, QColor targetbackgroundcolor)
QPixmap IDiagramItem::createPixmap()
{
	QPixmap pixmap(m_params.m_huabutuxingspacesize);
	//pixmap.fill(m_params.m_huabubackgroundcolor);
	pixmap.fill(Qt::transparent);
	QPainter painter(&pixmap);
	painter.setPen(m_params.m_huabutuxingpen);
	painter.setBrush(m_params.m_huabutuxingbrush);
	painter.setRenderHint(QPainter::Antialiasing, true);
	//std::shared_ptr<tuxingjiedianparams> params = factorytuxingparams::create(m_params.m_type);
	std::shared_ptr<tuxingjiedianparams> params = createpixmapparams();
	params->m_center = QPoint(pixmap.rect().x() + pixmap.width() / 2, pixmap.rect().y() + pixmap.height() / 2);
	params->m_spacesize = (m_params.m_huabutuxingspacesize);
	params->m_type = m_params.m_type;
	params->m_painter = &painter;

	factorytuxingjiedian::draw(params);
	//painter.fillRect(pixmap.rect(), Qt::white);
	return pixmap;
}
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

void IDiagramItem::init()
{
	initWidgetSize();
	inittuxingzujian();
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

void IDiagramItem::paintEvent(QPaintEvent* event)
{
	//drawmyself();

	//QWidget::paintEvent(event);
	QPainter* painter = new QPainter(this);
	painter->setPen(m_params.m_pen);
	painter->setBrush(m_params.m_brush);
	painter->setRenderHint(QPainter::Antialiasing, true);
	updatetuxingsize();
	painter->fillRect(this->rect(), m_params.m_backgroundcolor);
	m_tuxing->draw(painter);
}

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

void drawtuxingkuzujian::draw(QPainter* painter)
{
	if (m_drawByPainter)
	{
		drawByDraw(painter);
	}
	else if (m_drawByLoadpic)
	{
		drawByLoadpic(painter);
	}
	else
	{
		return;
	}
}

void drawtuxingkuzujianjuxing::drawByDraw(QPainter* painter)
{
	m_params->m_painter = painter;
	factorytuxingjiedian::draw(m_params);
	//m_rect = calcusuitablerect(painter->pen().width());
	//painter->drawRect(m_rect);
}

//QRect drawtuxingkuzujianjuxing::getSuitableRect()
//{
//	double scalefactor = qMin(static_cast<double>(m_sourceWidgetSize.width()) / m_rectSize.width()
//		, static_cast<double>(m_sourceWidgetSize.width()) / m_rectSize.height());
//	QSize newsize = m_rectSize * scalefactor;
//	QPoint topleft = QPoint((m_sourceWidgetSize.width() - newsize.width()) / 2, (m_sourceWidgetSize.height() - newsize.height()) / 2);
//
//	return QRect(topleft, newsize);
//}

void drawtuxingkuzujian::drawByLoadpic(QPainter* painter)
{

	QPixmap newpixmap = getSuitablePicPixmap(QPixmap(m_picpath));
	if (!m_params)
		throw std::runtime_error("error");//todo:except
	QPoint topleft = QPoint((m_params->m_spacesize.width() - newpixmap.width()) / 2, (m_params->m_spacesize.height() - newpixmap.height()) / 2);
	painter->drawPixmap(topleft, newpixmap);
}

QPixmap drawtuxingkuzujian::getSuitablePicPixmap(QPixmap pixmap)
{
	if (!m_params)
		throw std::runtime_error("error");//todo:except

	int targetwidth = m_params->m_spacesize.width();
	int targetheight = m_params->m_spacesize.height();

	QSize originalsize = pixmap.size();
	double scalefactor = qMin(static_cast<double>(targetwidth) / originalsize.width(), static_cast<double>(targetheight) / originalsize.height());
	QSize newsize = originalsize * scalefactor;
	return pixmap.scaled(newsize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void drawtuxingkuzujian::drawByDraw(QPainter* painter)
{
	painter->drawRect(0, 0, 10, 10);
}

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
void IDiagramItem::initWidgetSize()
{
	if (m_params.m_issizefixed)
	{
		if (m_params.m_fixsize.has_value())
		{
			setFixedSize(m_params.m_fixsize.value());
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
void IDiagramItem::initmaxandminsize()
{
	if (!m_params.m_maxsize.has_value() || !m_params.m_minsize.has_value())
		throw std::runtime_error("error");//todo:except
	setMaximumSize(m_params.m_maxsize.value());
	setMinimumSize(m_params.m_minsize.value());
}

void IDiagramItem::setsizepolicyexpanding()
{
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void IDiagramItem::resizeEvent(QResizeEvent* event)
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
QSize IDiagramItem::getspacesize()
{
	return size();
}

QPoint IDiagramItem::calculatebujiancentor()
{
	return QPoint(this->rect().x() + width() / 2, rect().y() + height() / 2);
}

std::shared_ptr<drawtuxingkuzujian> IDiagramItem::createtuxing()
{
	switch (m_params.m_type)
	{
	default:
	case ShapeType::juxing:
	{
		std::shared_ptr<drawtuxingkuzujianjuxing> p = std::make_shared<drawtuxingkuzujianjuxing>();
		p->m_params = std::make_shared<tuxingjiedianparamsjuxing>();
		dynamic_cast<tuxingjiedianparamsjuxing*>(p->m_params.get())->m_radio = m_params.m_juxingradio.value_or(diagramitemjuxingradio);
		return p;
	}
	break;
	case ShapeType::yuanxing:
	{
		std::shared_ptr<drawtuxingkuzujianyuanxing> p = std::make_shared<drawtuxingkuzujianyuanxing>();
		p->m_params = std::make_shared<tuxingjiedianparamsyuanxing>();
		return p;
	}
	break;
	}
}

void IDiagramItem::inittuxingzujian()
{
	m_tuxing = createtuxing();
	m_tuxing->m_picpath = m_params.m_picpath.value_or(imagepathjuxing);
	m_tuxing->m_params->m_center = calculatebujiancentor();
	m_tuxing->m_params->m_spacesize = getspacesize();
	m_tuxing->m_params->m_type = m_params.m_type;
	//m_tuxing = factorydrawtuxingkuzujian::create(m_shapetype);
	//m_tuxing->m_picpath = m_picturePath;
	//m_tuxing->m_params->m_spacesize = getspacesize();
	//m_tuxing->m_params->m_center = calculatebujiancentor();
	//m_tuxing->m_params->m_type = m_shapetype;
	//m_tuxing = drawtuxingkuzujianfactory::createtuxingzujian(m_shapetype);
	//m_tuxing->setSourcewidgetsize(this->size());
	//m_tuxing->setpicpath(m_picturePath);
}

IDiagramItem::IDiagramItem(diagramitemparams params)
	:m_params(params)
{
	init();
}

drawtuxingkuzujianjuxing::drawtuxingkuzujianjuxing()
{
	m_drawByPainter = true;
	m_drawByLoadpic = true;
}

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

void drawtuxingkuzujianyuanxing::drawByDraw(QPainter* painter)
{
	m_params->m_painter = painter;
	factorytuxingjiedian::draw(m_params);
	//m_r = calcusuitable(painter->pen().width());
	//QPoint center = QPoint(m_sourceWidgetSize.width() / 2, m_sourceWidgetSize.height() / 2);
	//painter->drawEllipse(center, m_r, m_r);
}

drawtuxingkuzujianyuanxing::drawtuxingkuzujianyuanxing()
{
	m_drawByPainter = true;
}

//void IDiagramItem::initpenandbrush(QColor color, int penwidth, QBrush brush)
//void IDiagramItem::initpenandbrush()
//{
	//m_pen = QPen(diagramitempencolor, diagramitempenwidth);
	//m_brush = diagramitembrush;
	//m_pen = QPen(color, penwidth);
	//m_brush = brush;

//}

void IDiagramItem::updatetuxingsize()
{
	//if (!m_iswidgetsizefixe)
	if (!m_params.m_issizefixed)
	{
		m_tuxing->m_params->m_spacesize = getspacesize();
		m_tuxing->m_params->m_center = calculatebujiancentor();
	}
}
diagramitemparams::diagramitemparams(bool issizefixed
	, std::optional<QSize> fixsize
	, std::optional<QSize> maxsize
	, std::optional<QSize> minsize
	, QPen pen
	, QBrush brush
	, QString mimetype
	, QColor backgroundcolor
	, std::optional<QString> picpath
	, std::optional<float> juxingradio
	, QSize huabutuxingspacesize
	, QPen huabutuxingpen
	, QBrush huabutuxingbrush
	//, QColor haububackgroundcolor
	, std::optional<float> widgetradio
	, ShapeType type)
	: m_issizefixed(issizefixed)
	, m_fixsize(fixsize)
	, m_maxsize(maxsize)
	, m_minsize(minsize)
	, m_pen(pen)
	, m_brush(brush)
	, m_mimetype(mimetype)
	, m_backgroundcolor(backgroundcolor)
	, m_widgetradio(widgetradio)
	, m_type(type)
	, m_picpath(picpath)
	, m_juxingradio(juxingradio)
	, m_huabutuxingspacesize(huabutuxingspacesize)
	, m_huabutuxingpen(huabutuxingpen)
	, m_huabutuxingbrush(huabutuxingbrush) {}
	//, m_huabubackgroundcolor(huabubackgroundcolor) {}
