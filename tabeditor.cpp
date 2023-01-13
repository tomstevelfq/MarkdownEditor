#include "tabeditor.h"
#include<QMessageBox>
#include<QTextStream>
#include<QFileDialog>

tabEditor::tabEditor(QWidget *parent):QTabWidget(parent)
{
    setTabsClosable(true);
    connect(this,&tabEditor::tabCloseRequested,this,&tabEditor::on_tabClose);
    connect(this,&tabEditor::currentChanged,this,&tabEditor::on_tabChange);
    add(new Editor());
}

void tabEditor::add(Editor *editor){
    connect(editor,&Editor::tabNameChange,this,&tabEditor::on_tabNameChange);
    connect(editor,&Editor::textChanged,this,&tabEditor::on_textChanged);
    addTab(editor,editor->getName());
    setCurrentWidget(editor);
    editor->tabIndex=currentIndex();
}

void tabEditor::openFile(QString filePath){
    if(filePath==""){//file has not been opened
        return;
    }
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
    if(count()>1){
        removeTab(index);//remove tab only when tabs are not only one
    }
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

void tabEditor::on_tabNameChange(int index,QString newName){
    setTabText(index,newName);
}

void tabEditor::saveFile(){
    Editor *editor=static_cast<Editor*>(currentWidget());
    QString filePath=editor->getName();
    if(filePath!="untitled"){
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QFile::Text))
        {
            QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
            return;
        }
        QTextStream qout(&file);
        qout.setCodec(editor->code);
        QString str=editor->toPlainText();
        qout<<editor->toPlainText();
        file.close();
    }
}

void tabEditor::saveFileAs(){
    Editor *editor=static_cast<Editor*>(currentWidget());
    QString filePath=QFileDialog::getSaveFileName(this,"save as");
    if(filePath.isNull()){
        return;
    }

    QString originPath=editor->getName();
    editor->setCurFilePath(filePath);
    QFile *pFile;
    if(originPath=="untitled"){
        pFile=new QFile(filePath);
    }else{
        pFile=new QFile(originPath);
        pFile->rename(filePath);
    }
    if (!pFile->open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + pFile->errorString());
        return;
    }
    QTextStream qout(pFile);
    qout.setCodec(editor->code);
    QString str=editor->toPlainText();
    qout<<str;
    pFile->close();
    setTabText(currentIndex(),filePath);
}

void tabEditor::on_textChanged(){
    QTextStream qout(stdout);
    qout<<"hello haha"<<endl;
    emit(textChanged(static_cast<Editor*>(currentWidget())));
}

void tabEditor::on_langChanged(QString lang){
    static_cast<Editor*>(currentWidget())->langChanged(lang);
}
