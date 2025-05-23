#pragma once

#include <QGraphicsDropShadowEffect>
#include <qwidget.h>

class ShadowEffectHelper {
public:
    static void applyShadow(QWidget* widget, bool enable = true,
        const QColor& color = Qt::black,
        int xOffset = 2, int yOffset = 2,
        int blurRadius = 8);
};

