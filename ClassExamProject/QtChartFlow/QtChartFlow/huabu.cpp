#include "huabu.h"

huabu::huabu(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	init();
}

huabu::~huabu()
{}

void huabu::dragEnterEvent(QDragEnterEvent* event)
{
	if (true == event->mimeData()->hasFormat(mymimetype))
	{
		event->acceptProposedAction();
	}
}

void huabu::dropEvent(QDropEvent* event)
{
	//获取信息，这个事件要做什么

	//重绘
	
	QByteArray array = event->mimeData()->data(mymimetype);
	bool flag;
	int typeint = array.toInt(&flag);
	ShapeType type;
	if (!flag)
	{	
		return;
	}
	type = static_cast<ShapeType>(typeint);
	Ituxingjiedian* tuxingjiedian = tuxingjiedianfactory::createtuxignjiedian(type, event);
	m_tuxingvec.push_back(tuxingjiedian);
	update(); //触发重绘，会调用paintEvent，可以选择重绘部分区域
	event->acceptProposedAction();
}

void huabu::dragMoveEvent(QDragMoveEvent* event)
{
	if (true == event->mimeData()->hasFormat(mymimetype))
	{
		event->acceptProposedAction();
	}
}

void huabu::paintEvent(QPaintEvent* event)
{
	//drawBaseBackground(m_painter);
	QPainter* painter = initPainter();
	//painter->fillRect(this->rect(), Qt::white);
	for (const auto& tuxing : m_tuxingvec)
	{
		tuxing->draw(painter);
	}
	QWidget::paintEvent(event);
}

//void huabu::drawBaseBackground(QPainter* painter)
void huabu::drawBaseBackground()
{
	//this->setAutoFillBackground(true);
	this->setStyleSheet("background-color: white; * { background-color: transparent; border: none; }");

}

void huabu::init()
{
	setAcceptDrops(true);
	drawBaseBackground();
}

QPainter* huabu::initPainter()
{
	QPainter* painter = new QPainter(this);
	// 设置画笔
	painter->setPen(QPen(Qt::black, 2));
	//设置画刷
	painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
	//设置抗锯齿
	painter->setRenderHint(QPainter::Antialiasing, true);
	return painter;
	//设置合成模式
	//m_painter->setCompositionMode()
}

Ituxingjiedian* tuxingjiedianfactory::createtuxignjiedian(ShapeType type, QDropEvent* event)
{
	switch (type)
	{
	default:
	case ShapeType::juxing:
		return new juxingjiedian(event->position().toPoint());
	}
}
