//#include "QtChartFlow.h"
#include <QtWidgets/QApplication>
#include "MyMainWindow.h"
#include <qlabel.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QtChartFlow w;
    
    //MyMainWindow w;
    ////w.show();
    //QLabel label;
    //QPixmap pixmap("D:\\Database\\Code\\vs\\c++\\KDevelop-Training\\KDevelop-Training\\ClassExamProject\\QtChartFlow\\QtChartFlow\\juxing.png");
    //if (!pixmap.isNull()) { // 检查图像是否成功加载
    //    label.setPixmap(pixmap);
    //    label.setFixedSize(pixmap.size());
    //}

    //label.show();
    MyMainWindow w;
    w.show();
    return a.exec();
}
