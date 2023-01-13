#include "mainwindow.h"
#include <QApplication>
#include"utils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    objMap["MainWindow"]=&w;
    w.show();

    return a.exec();
}
