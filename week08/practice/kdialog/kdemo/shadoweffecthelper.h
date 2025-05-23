#pragma once

#include <QGraphicsDropShadowEffect>
#include <qwidget.h>
#include <qevent.h>

class ShadowEffectHelper : public QObject
{
    Q_OBJECT
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
public:
    static void applyShadow(QWidget* widget, bool enable = true,
        const QColor& color = Qt::black,
        int xOffset = 2, int yOffset = 2,
        int blurRadius = 8);
};

