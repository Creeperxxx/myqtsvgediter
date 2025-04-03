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
	if ((event->pos() - dragstartposition).manhattanLength()< QApplication::startDragDistance())
		return;

	QDrag* drag = new QDrag(this);
	drag->setMimeData(createMimedata());
	drag->setPixmap(drawPixmap());
	//drag->setHotSpot(QPoint(25, 25));
	drag->setHotSpot(event->pos());
	drag->exec(Qt::CopyAction);
}

QMimeData* IDiagramItem::createMimedata()
{
	QMimeData* mimeData = new QMimeData();
	mimeData->setData("application/x-custom-shape", QByteArray::number(static_cast<int>(m_shapetype)));
	return mimeData;
}

QPixmap juxingDiagramItem::drawPixmap()
{
	//QImage image("D:/Database/Code/vs/c++/KDevelop-Training/KDevelop-Training/ClassExamProject/QtChartFlow/QtChartFlow/juxing.png");
	//QPixmap pixmap = QPixmap::fromImage(image);
	QPixmap preview = this->grab(); //捕获当前控件
	preview = preview.scaled(200, 200, Qt::KeepAspectRatio);//缩放
	QPixmap translucentPreview(preview.size());
	translucentPreview.fill(Qt::transparent);
	QPainter painter(&translucentPreview);
	painter.setOpacity(0.9); //70%不透明
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.drawPixmap(0, 0, preview);
	painter.end();
	return translucentPreview;
}

IDiagramItem::IDiagramItem(QWidget* parent) :QWidget(parent) {}

//juxingDiagramItem::juxingDiagramItem() :m_shapetype(ShapeType::juxing) {}
juxingDiagramItem::juxingDiagramItem(QWidget* parent)
	: IDiagramItem(parent)
{
	ui.setupUi(this);
	m_shapetype = ShapeType::juxing;
}

juxingDiagramItem::~juxingDiagramItem() {}
