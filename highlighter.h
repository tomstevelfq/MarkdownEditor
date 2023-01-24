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
    virtual void setDividePattern(QRegularExpression dividePattern);
    virtual void addRule(QRegularExpression pattern,QTextCharFormat format);
    virtual void setDeletePattern(QRegularExpression deletePattern);
    virtual void setUnderlinePattern(QRegularExpression underlinePattern);
    virtual void setFootnotePattern(QRegularExpression footnotePattern);
    virtual void setBlockPattern(QRegularExpression blockPattern);
    virtual void setCodePattern(QRegularExpression codePattern);
    virtual void setLinkPattern(QRegularExpression linkPattern);
protected:
    virtual void highlightBlock(const QString &text) override;
    virtual void highlightMultilineComments(const QString &text);
    virtual void setKeywordFormat();
    virtual void setClassFormat();
    virtual void setFunctionFormat();
    virtual void setQuoteFormat();
    virtual void setInlineCommentFormat();
    virtual void setBlockCommentFormat();
    virtual void setDivideFormat();
    virtual void setDeleteFormat();
    virtual void setUnderlineFormat();
    virtual void setFootnoteFormat();
    virtual void setBlockFormat();
    virtual void setCodeFormat();
    virtual void setLinkFormat();
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
    QTextCharFormat divideFormat;
    QTextCharFormat deleteFormat;
    QTextCharFormat underlineFormat;
    QTextCharFormat footnoteFormat;
    QTextCharFormat blockFormat;
    QTextCharFormat codeFormat;
    QTextCharFormat linkFormat;
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
