#include "kdialog.h"
#include "shadoweffecthelper.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ShadowEffectHelper* shadowHelper = new ShadowEffectHelper();
    a.installEventFilter(shadowHelper);
    
    kdialog w;
    w.show();
    return a.exec();
}
