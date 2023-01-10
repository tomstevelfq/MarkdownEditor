#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editor.h"
#include"tabeditor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Editor *editor;
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    tabEditor *tab;
private slots:
    void newFile();
    void open();
    void fontChoose();
//    void save();
//    void saveAs();
//    void print();
//    void exit();
};

#endif // MAINWINDOW_H
