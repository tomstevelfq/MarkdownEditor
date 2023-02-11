#ifndef MARKDOWNEDITOR_H
#define MARKDOWNEDITOR_H
#include<QHBoxLayout>
#include<QWidget>
#include"editor.h"
#include"markdownviewer.h"
#include"markdownrender.h"
#include<string>
using namespace std;

class MarkdownEditor:public QHBoxLayout{
public:
    Editor* editor;
    MarkdownViewer* viewer;
    MarkdownRender* mrender;
    MarkdownEditor(QWidget *parent=nullptr):QHBoxLayout(parent){
        editor=new Editor();
        editor->langChanged("markdown");
        viewer=new MarkdownViewer();
        mrender=new MarkdownRender(viewer);
        mrender->updateHeight(editor->height());
        this->addWidget(editor);
        this->setSpacing(30);
        this->addWidget(viewer);
        this->setStretchFactor(editor,1);
        this->setStretchFactor(viewer,1);
        connect(editor,&Editor::textedit,this,&MarkdownEditor::onedit_trigger);
    }
    void setVisible(bool hide){
        editor->setHidden(!hide);
        viewer->setHidden(!hide);
        //editor2->setHidden(!hide);
    }
public slots:
    void onedit_trigger(int linenum,const string& str){
        mrender->insert(linenum,str);//更新或者插入一个文本行
    }
};

#endif // MARKDOWNEDITOR_H
