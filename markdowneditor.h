#ifndef MARKDOWNEDITOR_H
#define MARKDOWNEDITOR_H
#include<QHBoxLayout>
#include<QWidget>
#include"editor.h"

class MarkdownEditor:public QHBoxLayout{
public:
    Editor* editor1;
    Editor* editor2;
    MarkdownEditor(QWidget *parent=nullptr):QHBoxLayout(parent){
        editor1=new Editor();
        editor1->langChanged("markdown");
        editor2=new Editor();
        this->addWidget(editor1);
        this->addWidget(editor2);
    }
    void setVisible(bool hide){
        editor1->setHidden(!hide);
        editor2->setHidden(!hide);
    }
};

#endif // MARKDOWNEDITOR_H
