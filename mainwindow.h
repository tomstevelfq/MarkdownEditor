#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "editor.h"
#include"tabeditor.h"
#include<QLabel>
#include"finddialog.h"

class MarkdownEditor;
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
    bool MarkdownDispalyState=false;
    void toggle_undo(bool);
    void toggle_redo(bool);
    void toggle_copyCut(bool);
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
    QAction *pLangC;
    QAction* pIndent;
    QAction* pWrap;
    QAction *pUndo;
    QAction *pRedo;
    QAction *pCut;
    QAction *pCopy;
    QAction *pLangMarkdown;
    QAction* pLangTest;
    QAction* pMarkdown;
    FindDialog* findDialog;
    MarkdownEditor* meditor;
public slots:
    void on_textChanged(Editor*);
private slots:
    void newFile();
    void open();
    void fontChoose();
    void on_statusBarTrigger();
    void on_langSelected(QAction*);
    void on_indentTrigger();
    void on_wrapTrigger();
    void on_redoTrigger();
    void on_undoTrigger();
    void on_copyTrigger();
    void on_cutTrigger();
    void on_pasteTrigger();
    void on_findTrigger();
    void on_startFindTrigger(QString,bool,bool);
    void on_startReplaceTrigger(QString,QString,bool,bool);
    void on_startReplaceAllTrigger(QString,QString,bool,bool);
    void on_markdownTrigger();
//    void save();
//    void saveAs();
//    void print();
//    void exit();
};

#endif // MAINWINDOW_H
