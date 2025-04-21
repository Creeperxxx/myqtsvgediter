#include <QtGui>
#include <QApplication>
#include "mycalculatorEngine.h"
#include "mycalculatorInterface.h"

int main(int argc, char* argv[])
{

    QApplication app(argc, argv);
    mycalculatorInterface* ui = mycalculatorInterface::getInstance();
    if (ui == nullptr)
        throw std::runtime_error("error");

    ui->show();
    return app.exec();
}