// main.cpp

#include <QCoreApplication>
#include "maincontroller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MainController myMainController;

    myMainController.init();

    return a.exec();
}
