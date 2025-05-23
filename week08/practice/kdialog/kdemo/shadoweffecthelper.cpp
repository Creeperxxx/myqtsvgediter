#include "shadoweffecthelper.h"


void ShadowEffectHelper::applyShadow(QWidget* widget, bool enable, const QColor& color, int xOffset, int yOffset, int blurRadius)
{
	if (!enable) 
		return;
	auto effect = new QGraphicsDropShadowEffect(widget);
	effect->setColor(color);
	effect->setOffset(QPoint(xOffset, yOffset));
	effect->setBlurRadius(blurRadius);
	widget->setGraphicsEffect(effect);
}
