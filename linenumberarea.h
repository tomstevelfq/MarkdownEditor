#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H
#include<QWidget>
#include"editor.h"

class LineNumberArea:public QWidget{
    Q_OBJECT

public:
    LineNumberArea(Editor* editor);
    Qt::GlobalColor lineNumberColor=Qt::darkGray;
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    Editor *editor;
};

#endif // LINENUMBERAREA_H
