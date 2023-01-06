#include "tabeditor.h"

tabEditor::tabEditor(QWidget *parent):QTabWidget(parent)
{
    setTabsClosable(true);
    connect(this,&tabEditor::tabCloseRequested,this,&tabEditor::on_tabClose);
    connect(this,&tabEditor::currentChanged,this,&tabEditor::on_tabChange);
    add(new Editor());
}

void tabEditor::add(Editor *editor){
    addTab(editor,editor->getName());
    setCurrentWidget(editor);
}

void tabEditor::openFile(QString filePath){
    Editor* curEditor=static_cast<Editor*>(currentWidget());
    if(curEditor->getName()=="untitled"){
        curEditor->loadFile(filePath);
        setTabText(currentIndex(),getFileName(filePath));
    }else{
        add(new Editor());
        Editor* curEditor=static_cast<Editor*>(currentWidget());
        curEditor->loadFile(filePath);
        setTabText(currentIndex(),getFileName(filePath));
    }
}

QString tabEditor::getFileName(QString filePath){
    QStringList lis=filePath.split("/");
    return lis.back();
}

void tabEditor::on_tabClose(int index){
    removeTab(index);
}

void tabEditor::code_formatChange(const QString& format){
    static_cast<Editor*>(currentWidget())->codeFormatChange(format);
}

void tabEditor::code_formatIndexChange(int comboxIndex){
    static_cast<Editor*>(currentWidget())->formatIndex=comboxIndex;
}

void tabEditor::on_tabChange(int comboxIndex){
    Editor *editor=static_cast<Editor*>(widget(comboxIndex));
    emit(tabChanged(editor->formatIndex));
}
