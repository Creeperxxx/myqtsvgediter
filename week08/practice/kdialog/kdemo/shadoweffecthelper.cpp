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

bool ShadowEffectHelper::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::Show) {
            QWidget* widget = qobject_cast<QWidget*>(obj);
            if (widget && !widget->property("shadowApplied").toBool()) { // 确保每个窗口只应用一次
                applyShadow(widget);
                widget->setProperty("shadowApplied", true); // 标记已应用
            }
        }
        return QObject::eventFilter(obj, event);
}