#include "kdialog.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    kdialog w;
    w.show();
    return a.exec();
}
