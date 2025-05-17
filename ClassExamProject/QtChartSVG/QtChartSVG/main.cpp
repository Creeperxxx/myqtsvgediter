#include <QtWidgets/QApplication>
#include "MyMainWindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<DiagramDrawParamsTriangle::TriangleSizeRadios>("DiagramDrawParamsTriangle::TriangleSizeRadios");

    MyMainWindow w;
    w.show();
    return a.exec();
}
