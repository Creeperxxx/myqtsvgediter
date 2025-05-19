#include <QtWidgets/QApplication>
#include "MyMainWindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<DiagramDrawParamsTriangle::TriangleSizeRadios>("DiagramDrawParamsTriangle::TriangleSizeRadios");

    QCoreApplication::setOrganizationName("svgeditor"); 
    QCoreApplication::setApplicationName("svgeditor");

    MyMainWindow w;
    w.show();
    return a.exec();
}
