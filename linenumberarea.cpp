#include"linenumberarea.h"
#include<QTextStream>

LineNumberArea::LineNumberArea(Editor *editor):QWidget(editor){
    this->editor=editor;
}

void LineNumberArea::paintEvent(QPaintEvent *event){
    editor->lineNumberPaint(event);
}
