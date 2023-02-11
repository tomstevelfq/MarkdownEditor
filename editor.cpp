#include "editor.h"
#include<QTextCodec>
#include<QTextStream>
#include<QMessageBox>
#include<QTextStream>
#include<QFont>
#include<QPainter>
#include<QTextBlock>
#include"linenumberarea.h"
#include<QFileDialog>
#include<QFontDialog>
#include"mainwindow.h"
#include"utils.h"
#include"finddialog.h"
#include"searchhistory.h"

Editor::Editor(QWidget *parent):QPlainTextEdit(parent){
    setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
    code=QTextCodec::codecForName("utf-8");
    QFont font;
    font.setPointSize(10);
    this->setFont(font);
    highlightCurrentLine();
    lineNumber=new LineNumberArea(this);
    connect(this,&Editor::cursorPositionChanged,this,&Editor::on_cursorPositionChanged);
    connect(this,&Editor::updateRequest,this,&Editor::updateLineNumberArea);
    connect(this,&Editor::blockCountChanged,this,&Editor::updateLineNumberWidth);
    connect(this,&Editor::redoAvailable,this,&Editor::toggleRedo);
    connect(this,&Editor::undoAvailable,this,&Editor::toggleUndo);
    connect(this,&Editor::textChanged,this,&Editor::on_textChange);
    //this->setStyleSheet("color:darkMagenta");

    updateLineNumberWidth();
    on_cursorPositionChanged();
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
    QTextStream qout(stdout);
    currentColumn=textCursor().positionInBlock();
    //qout<<toPlainText()<<endl;
}

QString Editor::getName(){
    if(curFilePath.isEmpty()){
        return "untitled";
    }
    return curFilePath;
}

int Editor::getLineNumberWidth(){
    int lastLineNumber=blockCount();
    int numberLength=QString::number(lastLineNumber).length();
    int charWidth=fontMetrics().width('9');
    return charWidth*numberLength+lineNumberPadding;
}

void Editor::lineNumberPaint(QPaintEvent *event){
    QPainter painter(lineNumber);
    QTextBlock block=firstVisibleBlock();
    int blockNumber=block.blockNumber();
    int top=(int)(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom=(int)(blockBoundingGeometry(block).height());
    while(block.isValid()&&top<event->rect().bottom()){
        if(block.isVisible()&&bottom>=event->rect().top()){
            QString num=QString::number(blockNumber+1);
            painter.setPen(lineNumber->lineNumberColor);
            painter.drawText(0,top,lineNumber->width()*1,fontMetrics().height(),Qt::AlignRight|Qt::AlignBottom,num);
        }
        block=block.next();
        top=bottom;
        bottom=top+(int)(blockBoundingRect(block).height());
        blockNumber++;
    }
}

void Editor::updateLineNumberWidth(){
    setViewportMargins(getLineNumberWidth()+lineNumberPadding/3,0,0,0);
}

void Editor::updateLineNumberArea(const QRect& drawnRect,int pixels){
    QTextStream qout(stdout);
    if(pixels!=0){
        lineNumber->scroll(0,pixels);
    }else{
        lineNumber->update(0,drawnRect.y(),lineNumber->width(),drawnRect.height());
    }
    if(drawnRect.contains(viewport()->rect())){
        updateLineNumberWidth();
    }

}

void Editor::resizeEvent(QResizeEvent *event){
    QPlainTextEdit::resizeEvent(event);
    QRect cr=contentsRect();
    lineNumber->setGeometry(QRect(cr.left(),cr.top(),getLineNumberWidth(),cr.height()));
}

void Editor::setLineNumberFont(Qt::GlobalColor color){
    lineNumber->lineNumberColor=color;
}

void Editor::setCurFilePath(QString filePath){
    this->curFilePath=filePath;
}

void Editor::launchFontDialog(){
    bool userChoose;
    QFont font=QFontDialog::getFont(&userChoose,QFont("Courier", 10), this);
    if(userChoose){
        loadFont(font.family(),QFont::Monospace,true,font.pointSize(),5);
    }
}

void Editor::loadFont(QString family,QFont::StyleHint styleHint,bool fixedPitch,int fontSize,int tabWidth){
    font.setFamily(family);
    font.setStyleHint(styleHint);
    font.setFixedPitch(fixedPitch);
    font.setPointSize(fontSize);
    setFont(font);
}

void Editor::langChanged(QString lang){
    if(language==lang){
        return;
    }
    language=lang;
    syntaxHighLighter=genHighlighterFor(lang);
}

QString Editor::getLang(){
    return language;
}

void Editor::toggleRedo(bool available){
    MainWindow *wid=static_cast<MainWindow*>(objMap["MainWindow"]);
    wid->toggle_redo(available);
}

void Editor::toggleUndo(bool available){
    MainWindow *wid=static_cast<MainWindow*>(objMap["MainWindow"]);
    wid->toggle_undo(available);
}

bool Editor::find(QString query,bool caseSensitive,bool wholeWords){
    int curCursorPosition=textCursor().position();
    QTextDocument::FindFlags searchOptions=getSearchOptions(caseSensitive,wholeWords);
    bool matchFound=QPlainTextEdit::find(query,searchOptions);
    if(!matchFound){
        moveCursor(QTextCursor::Start);
        matchFound=QPlainTextEdit::find(query,searchOptions);
    }
    if(matchFound){
        int foundPosition=textCursor().position();
        bool previousFound=searchHistory.previousFound(query);
        if(!previousFound){
            searchHistory.add(query,curCursorPosition,foundPosition);
        }else{
            bool loopBackMatch=foundPosition==searchHistory.firstFoundAt(query);
            if(loopBackMatch){
                matchFound=false;
                int cursorPosBeforeSearch=searchHistory.curPosBeforeFirstSearchFor(query);
                moveCursorTo(cursorPosBeforeSearch);
                searchHistory.clear();
                static_cast<FindDialog*>(objMap["findDialog"])->on_findRequestReady("no more results found");
            }
        }
    }else{
        moveCursorTo(curCursorPosition);
        static_cast<FindDialog*>(objMap["findDialog"])->on_findRequestReady("no results found");
    }
    return matchFound;
}

bool Editor::replace(QString what,QString with,bool caseSensitive,bool wholeWords){
    bool found=find(what,caseSensitive,wholeWords);
    if(found){
        QTextCursor cursor=textCursor();
        cursor.beginEditBlock();
        cursor.insertText(with);
        cursor.endEditBlock();
    }
    return found;
}

void Editor::replaceAll(QString what,QString with,bool caseSensitive,bool wholeWords){
    moveCursorTo(0);
    QTextDocument::FindFlags searchOptions=getSearchOptions(caseSensitive,wholeWords);
    bool found=QPlainTextEdit::find(what,searchOptions);
    int replacements=0;
    QTextCursor cursor(document());
    cursor.beginEditBlock();
    while(found){
        QTextCursor curPos=textCursor();
        curPos.insertText(with);
        replacements++;
        found=QPlainTextEdit::find(what,searchOptions);
    }
    cursor.endEditBlock();
    if(replacements==0){
        static_cast<FindDialog*>(objMap["findDialog"])->on_findRequestReady("no results found");
    }else{
        static_cast<FindDialog*>(objMap["findDialog"])->on_findRequestReady("document searched, replaced "+QString::number(replacements)+" instances");
    }
}

void Editor::moveCursorTo(int position){
    QTextCursor newCursor=textCursor();
    newCursor.setPosition(position);
    setTextCursor(newCursor);
}

QTextDocument::FindFlags Editor::getSearchOptions(bool caseSensitive,bool wholeWords){
    QTextDocument::FindFlags searchOptions=QTextDocument::FindFlags();
    if(caseSensitive){
        searchOptions |=QTextDocument::FindCaseSensitively;
    }
    if(wholeWords){
        searchOptions |=QTextDocument::FindWholeWords;
    }
    return searchOptions;
}

void Editor::on_textChange(){
    searchHistory.clear();
    MainWindow* wind=static_cast<MainWindow*>(objMap["MainWindow"]);
    if(wind->MarkdownDispalyState){
        QTextCursor tc=this->textCursor();
        int linenum=tc.blockNumber();
        string str=this->document()->findBlockByNumber(linenum).text().toStdString();
        emit textedit(linenum,str);
    }
    static_cast<MainWindow*>(objMap["MainWindow"])->on_textChanged(this);
}

HighLighter *Editor::genHighlighterFor(QString lang){
    QTextDocument *doc=document();
    if(lang=="c"){
        return cHighlighter(doc);
    }else if(lang=="cpp"){
        return cppHighlighter(doc);
    }else if(lang=="java"){
        return javaHighlighter(doc);
    }else if(lang=="python"){
        return pythonHighlighter(doc);
    }else if(lang=="markdown"){
        return markdownHighlighter(doc);
    }else if(lang=="test"){
        return testLighter(doc);
    }
    return nullptr;
}
