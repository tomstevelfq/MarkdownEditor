#ifndef EDITOR_H
#define EDITOR_H
#include<QPlainTextEdit>
#include<QFile>
#include<QTextCodec>
class Editor:public QPlainTextEdit{
public:
    Editor(QWidget *parent=nullptr);
    QString curFilePath;
    void loadFile(QString filePath);  //load the specific file
    void loadFile();  //load the file named by curFilePath
    QString getName();
    QTextCodec *code;
private:
    void highlightCurrentLine();
public slots:
    void codeFormatChange(const QString &format);
    void on_cursorPositionChanged();
};

#endif // EDITOR_H
