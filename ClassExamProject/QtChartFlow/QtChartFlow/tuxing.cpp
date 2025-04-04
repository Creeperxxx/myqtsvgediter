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
	drag->setPixmap(drawPixmap());
	//drag->setHotSpot(QPoint(25, 25));
	drag->setHotSpot(drag->pixmap().rect().center());
	drag->exec(Qt::CopyAction);
}

QMimeData* IDiagramItem::createMimedata()
{
	QMimeData* mimeData = new QMimeData();
	mimeData->setData(mymimetype, QByteArray::number(static_cast<int>(m_shapetype)));
	return mimeData;
}

QPixmap IDiagramItem::drawPixmap()
{
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
	return this->grab();
}

IDiagramItem::IDiagramItem(ShapeType type, QString picturePath, QWidget* parent)
	:QWidget(parent)
	, m_shapetype(type)
	, m_picturePath(picturePath)
{
	//initLayout();
	//drawmyself(m_picturePath); //是否设置一个默认路径？
	//initSizePolicy();
	setFixedSize(40, 20);
}

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

void IDiagramItem::initSizePolicy()
{
	setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
}

void IDiagramItem::paintEvent(QPaintEvent* event)
{
	//drawmyself();
	QPainter painter(this);
	QPen pen(Qt::black, 2);
	painter.setPen(pen);
	painter.setBrush(Qt::transparent);
	gettuxignzujian(m_shapetype)->draw(&painter);
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
	else if(m_drawByLoadpic)
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
	painter->drawRect(getSuitableRect());
}

QRect drawtuxingkuzujianjuxing::getSuitableRect()
{
	double scalefactor = qMin(static_cast<double>(m_sourceWidgetSize.width()) / m_rectSize.width()
		, static_cast<double>(m_sourceWidgetSize.width()) / m_rectSize.height());
	QSize newsize = m_rectSize * scalefactor;
	QPoint topleft = QPoint((m_sourceWidgetSize.width() - newsize.width()) / 2, (m_sourceWidgetSize.height() - newsize.height()) / 2);

	return QRect(topleft, newsize);
}

void drawtuxingkuzujian::drawByLoadpic(QPainter* painter)
{

	QPixmap newpixmap = getSuitablePicPixmap(QPixmap(m_picpath));
	QPoint topleft((m_sourceWidgetSize.width() - newpixmap.width()) / 2, (m_sourceWidgetSize.height()- newpixmap.height()) / 2);
	painter->drawPixmap(topleft, newpixmap);
}

QPixmap drawtuxingkuzujian::getSuitablePicPixmap(QPixmap pixmap)
{
	int targetwidth = m_sourceWidgetSize.width();
	int targetheight = m_sourceWidgetSize.height();
	QSize originalsize = pixmap.size();
	double scalefactor = qMin(static_cast<double>(targetwidth) / originalsize.width(), static_cast<double>(targetheight) / originalsize.height());
	QSize newsize = originalsize * scalefactor;
	return pixmap.scaled(newsize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void drawtuxingkuzujian::drawByDraw(QPainter* painter)
{
	painter->drawRect(0, 0, 10, 10);
}

std::unique_ptr<drawtuxingkuzujian> drawtuxingkuzujianfactory::createtuxingzujian(ShapeType type)
{
	switch (type)
	{
	default:
	case ShapeType::juxing:
		return std::make_unique<drawtuxingkuzujianjuxing>();
	}
}

std::unique_ptr<drawtuxingkuzujian> IDiagramItem::gettuxignzujian(ShapeType type)
{
	std::unique_ptr<drawtuxingkuzujian> ptr = drawtuxingkuzujianfactory::createtuxingzujian(type);
	ptr->setPicpath(m_picturePath);
	
}

