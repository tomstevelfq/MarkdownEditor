#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editor.h"
#include"tabeditor.h"
#include<QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Editor *editor;
    QLabel* languageLabel;
    QLabel* wordCountLabel;
    QLabel* charCountLabel;
    QStatusBar* statusBar;
    QLabel* wordLabel;
    QLabel* charLabel;
    QAction *pStatus;
    void toggle_undo(bool);
    void toggle_redo(bool);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    tabEditor *tab;
    void initStatusBar();
    QMap<QAction*,QString> langMap;
    void initLangMap();
    QAction *pLangPython;
    QAction *pLangCpp;
    QAction *pLangJava;
    QAction* pIndent;
    QAction* pWrap;
    QAction *pUndo;
    QAction *pRedo;
private slots:
    void newFile();
    void open();
    void fontChoose();
    void on_textChanged(Editor*);
    void on_statusBarTrigger();
    void on_langSelected(QAction*);
    void on_indentTrigger();
    void on_wrapTrigger();
    void on_redoTrigger();
    void on_undoTrigger();
//    void save();
//    void saveAs();
//    void print();
//    void exit();
};

#endif // MAINWINDOW_H
