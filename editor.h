#ifndef EDITOR_H
#define EDITOR_H
#include<QPlainTextEdit>
#include<QFile>
#include<QTextCodec>
class LineNumberArea;
class Editor:public QPlainTextEdit{
    Q_OBJECT

public:
    Editor(QWidget *parent=nullptr);
    QString curFilePath;
    void loadFile(QString filePath);  //load the specific file
    void loadFile();  //load the file named by curFilePath
    QString getName();
    QTextCodec *code;
    void codeFormatChange(const QString &format);
    int formatIndex=0;
    int currentColumn=1;
    void lineNumberPaint(QPaintEvent *event);
    void setLineNumberFont(Qt::GlobalColor);
private:
    void highlightCurrentLine();
    int getLineNumberWidth();
    LineNumberArea *lineNumber;
    int lineNumberPadding=25;

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void on_cursorPositionChanged();
    void updateLineNumberWidth();
    void updateLineNumberArea(const QRect&,int);
    void saveAndSaveAs();
signals:
    columnChanged(int);
};

#endif // EDITOR_H
