#ifndef EDITOR_H
#define EDITOR_H
#include<QPlainTextEdit>
#include<QFile>
#include<QTextCodec>
#include"searchhistory.h"
#include"highlighter.h"
#include<string>
using namespace std;
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
    int tabIndex;
    int currentColumn=1;
    void lineNumberPaint(QPaintEvent *event);
    void setLineNumberFont(Qt::GlobalColor);
    void setCurFilePath(QString);
    void launchFontDialog();
    void loadFont(QString family, QFont::StyleHint styleHint, bool fixedPitch, int pointSize, int tabStopWidth);
    void langChanged(QString);
    QString getLang();
    QString language;
    bool find(QString,bool,bool);
    bool replace(QString,QString,bool,bool);
    void replaceAll(QString,QString,bool,bool);
private:
    void highlightCurrentLine();
    int getLineNumberWidth();
    void moveCursorTo(int position);
    QTextDocument::FindFlags getSearchOptions(bool,bool);
    LineNumberArea *lineNumber;
    int lineNumberPadding=25;
    SearchHistory searchHistory;
    QFont font;
    HighLighter *genHighlighterFor(QString language);
    HighLighter *syntaxHighLighter;
protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void on_cursorPositionChanged();
    void updateLineNumberWidth();
    void updateLineNumberArea(const QRect&,int);
    void toggleRedo(bool);
    void toggleUndo(bool);
    void on_textChange();
signals:
    void columnChanged(int);
    void tabNameChange(int,QString);
    void textedit(int linenum,const string& str);
};

#endif // EDITOR_H
