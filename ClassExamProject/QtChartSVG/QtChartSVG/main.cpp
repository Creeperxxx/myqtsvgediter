#include <QtWidgets/QApplication>
#include "mymainwindow.h"
#include "diagramdrawparams.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<DiagramDrawParamsTriangle::sideRadios>("DiagramDrawParamsTriangle::TriangleSizeRadios");

    QCoreApplication::setOrganizationName("svgeditor"); 
    QCoreApplication::setApplicationName("svgeditor");

    MyMainWindow w;
    w.show();
    return a.exec();
}
