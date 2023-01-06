#ifndef TABEDITOR_H
#define TABEDITOR_H
#include<QTabWidget>
#include"editor.h"

class tabEditor:public QTabWidget
{
    Q_OBJECT

public:
    tabEditor(QWidget* parent=nullptr);
    void add(Editor *editor);
public slots:
    void openFile(QString filePath);
    void on_tabClose(int index);
    void on_tabChange(int index);
    void code_formatChange(const QString& format);
    void code_formatIndexChange(int comboxIndex);
private:
    QString getFileName(QString filePath);

signals:
    void tabChanged(int);//change the index of combox list to show curfile's codec name
};

#endif // TABEDITOR_H
