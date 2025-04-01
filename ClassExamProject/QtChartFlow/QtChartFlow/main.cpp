//#include "QtChartFlow.h"
#include <QtWidgets/QApplication>
#include "MyMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QtChartFlow w;
    
    MyMainWindow w;
    w.show();
    return a.exec();
}
