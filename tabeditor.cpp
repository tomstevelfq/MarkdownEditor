#include "tabeditor.h"

tabEditor::tabEditor(QWidget *parent):QTabWidget(parent)
{
    add(new Editor());
}

void tabEditor::add(Editor *editor){
    addTab(editor,editor->getName());
    setCurrentWidget(editor);
}
