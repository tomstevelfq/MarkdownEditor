#include "mainwindow.h"
#include <QApplication>
#include"utils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QApplication::setStyle("fusion");
    objMap["MainWindow"]=&w;
    w.show();

    return a.exec();
}
