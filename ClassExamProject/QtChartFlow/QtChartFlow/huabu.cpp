#include "huabu.h"

huabu::huabu(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

huabu::~huabu()
{}

void huabu::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasText())
	{
		event->acceptProposedAction();
	}
}

void huabu::dropEvent(QDropEvent* event)
{
	//获取信息，这个事件要做什么
	//重绘
	update();
	event->acceptProposedAction();
}

void huabu::dragMoveEvent(QDragMoveEvent* event)
{

}
