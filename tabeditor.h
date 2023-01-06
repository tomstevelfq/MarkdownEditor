#ifndef TABEDITOR_H
#define TABEDITOR_H
#include<QTabWidget>
#include"editor.h"

class tabEditor:public QTabWidget
{
public:
    tabEditor(QWidget* parent=nullptr);
private:
    void add(Editor *editor);
};

#endif // TABEDITOR_H
