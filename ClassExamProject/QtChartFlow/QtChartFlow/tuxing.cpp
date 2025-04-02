#include "tuxing.h"

	
void juxingDiagramItem::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		dragstartposition = event->pos();
	}
}

void juxingDiagramItem::mouseMoveEvent(QMouseEvent* event)
{
	if (!(event->buttons() & Qt::LeftButton))
	{
		return;
	}
	if ((event->pos() - dragstartposition).manhattanLength()< QApplication::startDragDistance())
		return;

	QDrag* drag = new QDrag(this);
	QMimeData* mimeData = new QMimeData;

	mimeData->setText("Custom Drag Data");
	drag->setMimeData(mimeData);
	drag->exec(Qt::CopyAction);
}