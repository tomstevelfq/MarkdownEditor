#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H
#include<QSyntaxHighlighter>
#include<QRegularExpression>
class HighLighter:public QSyntaxHighlighter{
    Q_OBJECT
public:
    HighLighter(QTextDocument* parent=nullptr):QSyntaxHighlighter(parent){}
    virtual void addKeywords(QStringList keywords);
    virtual void setClassPattern(QRegularExpression classPattern);
    virtual void setFunctionPattern(QRegularExpression functionPattern);
    virtual void setQuotePattern(QRegularExpression quotePattern);
    virtual void setInlineCommentPattern(QRegularExpression inlineCommentPattern);
    virtual void setBlockCommentStartPattern(QRegularExpression blockCommentStart);
    virtual void setBlockCommentEndPattern(QRegularExpression blockCommentEnd);
    virtual void addRule(QRegularExpression pattern,QTextCharFormat format);
protected:
    virtual void highlightBlock(const QString &text) override;
    virtual void highlightMultilineComments(const QString &text);
    virtual void setKeywordFormat();
    virtual void setClassFormat();
    virtual void setFunctionFormat();
    virtual void setQuoteFormat();
    virtual void setInlineCommentFormat();
    virtual void setBlockCommentFormat();
private:
    struct HighlightingRule{
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> rules;
    QRegularExpression blockCommentStart;
    QRegularExpression blockCommentEnd;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat inlineCommentFormat;
    QTextCharFormat blockCommentFormat;
    QTextCharFormat quoteFormat;
    QTextCharFormat functionFormat;
};

enum BlockState{
    NotInComment,
    InComment
};

HighLighter *cHighlighter(QTextDocument* doc);
HighLighter *cppHighlighter(QTextDocument *doc);
HighLighter *javaHighlighter(QTextDocument *doc);
HighLighter *pythonHighlighter(QTextDocument *doc);
HighLighter *markdownHighlighter(QTextDocument *doc);
HighLighter *testLighter(QTextDocument *doc);

#endif // HIGHLIGHTER_H
