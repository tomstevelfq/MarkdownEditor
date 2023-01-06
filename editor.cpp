#include "editor.h"
#include<QTextCodec>
#include<QTextStream>
#include<QMessageBox>
#include<QTextStream>
#include<QFont>

Editor::Editor(QWidget *parent):QPlainTextEdit(parent){
    code=QTextCodec::codecForName("utf-8");
    QFont font;
    font.setPointSize(10);
    this->setFont(font);
    highlightCurrentLine();
    connect(this,&Editor::cursorPositionChanged,this,&Editor::on_cursorPositionChanged);
}

void Editor::loadFile(QString filePath){
    curFilePath=filePath;
    loadFile();
}

void Editor::loadFile(){
    QFile curFile(curFilePath);
    if (!curFile.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + curFile.errorString());
        return;
    }
    QTextStream qin(&curFile);
    qin.setCodec(code);
    setPlainText(qin.readAll());
}

void Editor::codeFormatChange(const QString &format){
    code=QTextCodec::codecForName(format.toStdString().c_str());
    loadFile(curFilePath);//update file loading after the format was changed
}

void Editor::highlightCurrentLine(){
    QList<QTextEdit::ExtraSelection> selections;
    QTextEdit::ExtraSelection extraSelection;
    QColor color(235,235,235);
    extraSelection.format.setBackground(color);
    extraSelection.format.setProperty(QTextFormat::FullWidthSelection,true);
    extraSelection.cursor=textCursor();
    extraSelection.cursor.clearSelection();
    selections.append(extraSelection);
    setExtraSelections(selections);
}

void Editor::on_cursorPositionChanged(){
    highlightCurrentLine();
}

QString Editor::getName(){
    if(curFilePath.isEmpty()){
        return "untitled";
    }
    return curFilePath;
}
