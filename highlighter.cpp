#include"highlighter.h"
#include<QTextStream>

//修改成读取配置文件的模式
void HighLighter::addKeywords(QStringList keywords){
    setKeywordFormat();
    foreach(const QString &keyword,keywords){
        addRule(QRegularExpression(keyword),keywordFormat);
    }
}

void HighLighter::setKeywordFormat(){
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
}

void HighLighter::setClassPattern(QRegularExpression classPattern){
    setClassFormat();
    addRule(classPattern,classFormat);
}

void HighLighter::setClassFormat(){
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
}

void HighLighter::setFunctionPattern(QRegularExpression functionPattern){
    setFunctionFormat();
    addRule(functionPattern,functionFormat);
}

void HighLighter::setFunctionFormat(){
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
}

void HighLighter::setQuotePattern(QRegularExpression quotePattern){
    setQuoteFormat();
    addRule(quotePattern,quoteFormat);
}

void HighLighter::setQuoteFormat(){
    quoteFormat.setForeground(Qt::darkGreen);
}

void HighLighter::setInlineCommentPattern(QRegularExpression inlineCommentPattern)
{
    setInlineCommentFormat();
    addRule(inlineCommentPattern, inlineCommentFormat);
}


void HighLighter::setInlineCommentFormat()
{
    inlineCommentFormat.setForeground(Qt::darkGreen);
}


void HighLighter::setBlockCommentStartPattern(QRegularExpression blockCommentStart)
{
    setBlockCommentFormat();
    this->blockCommentStart = blockCommentStart;
}


void HighLighter::setBlockCommentEndPattern(QRegularExpression blockCommentEnd)
{
    this->blockCommentEnd = blockCommentEnd;
}


void HighLighter::setBlockCommentFormat()
{
    blockCommentFormat.setForeground(Qt::darkGreen);
}


void HighLighter::addRule(QRegularExpression pattern, QTextCharFormat format)
{
    HighlightingRule rule;
    rule.pattern = pattern;
    rule.format = format;
    rules.append(rule);
}

void HighLighter::highlightBlock(const QString &text){
    int a=3;
    a=4;
    QTextStream qout(stdout);

    foreach(const HighlightingRule& rule,rules){
        QRegularExpressionMatchIterator iterator=rule.pattern.globalMatch(text);
        while(iterator.hasNext()){
            QRegularExpressionMatch match=iterator.next();
            setFormat(match.capturedStart(),match.capturedLength(),rule.format);
        }
    }
    highlightMultilineComments(text);//多行注释
}

void HighLighter::highlightMultilineComments(const QString &text){
    setCurrentBlockState(BlockState::NotInComment);
    int startIndex=0;
    if(previousBlockState()!=BlockState::InComment){
        startIndex=text.indexOf(blockCommentStart);
    }
    while(startIndex>=0){
        QRegularExpressionMatch match=blockCommentEnd.match(text,startIndex);
        int endIndex=match.capturedStart();
        int commentLength=0;
        if(endIndex==-1){
            setCurrentBlockState(BlockState::InComment);
            commentLength=text.length()-startIndex;
        }else{
            commentLength=endIndex-startIndex+match.capturedLength();
        }
        setFormat(startIndex,commentLength,blockCommentFormat);
        startIndex=text.indexOf(blockCommentStart,startIndex+commentLength);
    }
}

HighLighter *cHighlighter(QTextDocument* doc){
    QStringList keywords;
    //c语言中的关键字
    keywords << "\\bauto\\b" << "\\bbreak\\b" << "\\bcase\\b" << "\\bchar\\b" << "\\bconst\\b"
             << "\\bcontinue\\b" << "\\bdefault\\b" << "\\bdo\\b" << "\\bdouble\\b" << "\\belse\\b"
             << "\\benum\\b" << "\\bextern\\b" << "\\bfloat\\b" << "\\bfor\\b" << "\\bgoto\\b"
             << "\\bif\\b" << "\\bint\\b" << "\\blong\\b" << "\\bregister\\b" << "\\breturn\\b"
             << "\\bshort\\b" << "\\bsigned\\b" << "\\bsizeof\\b" << "\\bstatic\\b" << "\\bstruct\\b"
             << "\\bswitch\\b" << "\\btypedef\\b" << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvoid\\b"
             << "\\bvolatile\\b" << "\\bwhile\\b";

    QRegularExpression classPattern("\\b[A-Z_][a-zA-Z0-9_]*\\b");
    QRegularExpression quotePattern("(\".*\")|('\\\\.')|('.{0,1}')");
    QRegularExpression functionPattern("\\b[A-Za-z_][A-Za-z0-9_]*(?=\\()");
    QRegularExpression inlineCommentPattern("//.*");
    QRegularExpression blockCommentStart("/\\*");
    QRegularExpression blockCommentEnd("\\*/");

    HighLighter *highlighter=new HighLighter(doc);
    highlighter->addKeywords(keywords);
    highlighter->setClassPattern(classPattern);
    highlighter->setQuotePattern(quotePattern);
    highlighter->setFunctionPattern(functionPattern);
    highlighter->setInlineCommentPattern(inlineCommentPattern);
    highlighter->setBlockCommentStartPattern(blockCommentStart);
    highlighter->setBlockCommentEndPattern(blockCommentEnd);
    return highlighter;
}

/* Returns a Highlighter object specific to the C++ language and its grammar and syntax.
 */
HighLighter *cppHighlighter(QTextDocument *doc)
{
    HighLighter *cLanguage = cHighlighter(doc);
    QStringList cppOnlyKeywords;

    cppOnlyKeywords <<  "\\basm\\b" << "\\bbool\\b" << "\\bcatch\\b" <<
                        "\\bclass\\b" << "\\bconst_cast\\b" << "\\bdelete\\b" <<
                        "\\bdynamic_cast\\b" << "\\bexplicit\\b" << "\\bfalse\\b" <<
                        "\\bfriend\\b" << "\\binline\\b" << "\\bmutable\\b" <<
                        "\\bnamespace\\b" << "\\bnew\\b" << "\\boperator\\b" <<
                        "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b" <<
                        "\\breinterpret_cast\\b" << "\\bstatic_cast\\b" <<
                        "\\btemplate\\b" << "\\bthis\\b" << "\\bthrow\\b" <<
                        "\\btrue\\b" << "\\btry\\b" << "\\btypeid\\b" << "\\btypename\\b" <<
                        "\\bvirtual\\b" << "\\busing\\b" << "\\bwchar_t\\b";

    cLanguage->addKeywords(cppOnlyKeywords);
    return cLanguage;
}


/* Returns a Highlighter object specific to the Java language and its grammar and syntax.
 */
HighLighter *javaHighlighter(QTextDocument *doc)
{
    QStringList keywords;

    keywords << "\\babstract\\b" << "\\bassert\\b" << "\\bboolean\\b" << "\\bbreak\\b" << "\\bbyte\\b"
             << "\\bcase\\b" << "\\bcatch\\b" << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b" << "\\bcontinue\\b"
             << "\\bdefault\\b" << "\\bdo\\b" << "\\bdouble\\b" << "\\belse\\b" << "\\benum\\b" << "\\bextends\\b"
             << "\\bfinal\\b" << "\\bfinally\\b" << "\\bfloat\\b" << "\\bfor\\b" << "\\bgoto\\b" << "\\bif\\b"
             << "\\bimplements\\b" << "\\bimport\\b" << "\\binstanceof\\b" << "\\bint\\b" << "\\binterface\\b"
             << "\\blong\\b" << "\\bnative\\b" << "\\bnew\\b" << "\\bpackage\\b" << "\\bprivate\\b" << "\\bprotected\\b"
             << "\\bpublic\\b" << "\\breturn\\b" << "\\bshort\\b" << "\\bstatic\\b" << "\\bstrictfp\\b" << "\\bsuper\\b"
             << "\\bswitch\\b" << "\\bsynchronized\\b" << "\\bthis\\b" << "\\bthrow\\b" << "\\bthrows\\b" << "\\btransient\\b"
             << "\\btry\\b" << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwhile\\b" << "\\btrue\\b" << "\\bfalse\\b" << "\\bnull\\b";

    QRegularExpression classPattern("\\b[A-Z_][a-zA-Z0-9_]*\\b");
    QRegularExpression quotePattern("(\".*\")|('\\\\.')|('.{0,1}')");
    QRegularExpression functionPattern("\\b[A-Za-z_][A-Za-z0-9_]*(?=\\()");
    QRegularExpression inlineCommentPattern("//.*");
    QRegularExpression blockCommentStart("/\\*");
    QRegularExpression blockCommentEnd("\\*/");

    HighLighter *highlighter = new HighLighter(doc);
    highlighter->addKeywords(keywords);
    highlighter->setClassPattern(classPattern);
    highlighter->setQuotePattern(quotePattern);
    highlighter->setFunctionPattern(functionPattern);
    highlighter->setInlineCommentPattern(inlineCommentPattern);
    highlighter->setBlockCommentStartPattern(blockCommentStart);
    highlighter->setBlockCommentEndPattern(blockCommentEnd);

    return highlighter;
}


/* Returns a Highlighter object specific to the Python language and its grammar and syntax.
 */
HighLighter *pythonHighlighter(QTextDocument *doc)
{
    QStringList keywords;

    keywords << "\\band\\b" << "\\bas\\b" << "\\bassert\\b" << "\\bbreak\\b" << "\\bclass\\b" << "\\bcontinue\\b"
             << "\\bdef\\b" << "\\bdel\\b" << "\\belif\\b" << "\\belse\\b" << "\\bexcept\\b" << "\\bFalse\\b"
             << "\\bfinally\\b" << "\\bfor\\b" << "\\bfrom\\b" << "\\bglobal\\b" << "\\bif\\b" << "\\bimport\\b"
             << "\\bin\\b" << "\\bis\\b" << "\\blambda\\b" << "\\bNone\\b" << "\\bnonlocal\\b" << "\\bnot\\b"
             << "\\bor\\b" << "\\bpass\\b" << "\\braise\\b" << "\\breturn\\b" << "\\bTrue\\b" << "\\btry\\b"
             << "\\bwhile\\b" << "\\bwith\\b" << "\\byield\\b";

    QRegularExpression classPattern("\\b[A-Z_][a-zA-Z0-9_]*\\b");
    QRegularExpression quotePattern("(\".*\")|('.*')");
    QRegularExpression functionPattern("\\b[A-Za-z_][A-Za-z0-9_]*(?=\\()");
    QRegularExpression inlineCommentPattern("#.*");
    // TODO change for Python
    QRegularExpression blockCommentStart("'''");
    QRegularExpression blockCommentEnd("'''");

    HighLighter *highlighter = new HighLighter(doc);
    highlighter->addKeywords(keywords);
    highlighter->setClassPattern(classPattern);
    highlighter->setQuotePattern(quotePattern);
    highlighter->setFunctionPattern(functionPattern);
    highlighter->setInlineCommentPattern(inlineCommentPattern);
    highlighter->setBlockCommentStartPattern(blockCommentStart);
    highlighter->setBlockCommentEndPattern(blockCommentEnd);

    return highlighter;
}

HighLighter *markdownHighlighter(QTextDocument *doc){
    return nullptr;
}

HighLighter *testLighter(QTextDocument *doc){
    QStringList keywords;
    //c语言中的关键字
    keywords << "\\bint\\b";
    QRegularExpression classPattern("\\b[A-Z_][a-zA-Z0-9_]*\\b");
    QRegularExpression quotePattern("(\".*\")|('\\\\.')|('.{0,1}')");
    QRegularExpression functionPattern("\\b[A-Za-z_][A-Za-z0-9_]*(?=\\()");
    QRegularExpression inlineCommentPattern("//.*");
    QRegularExpression blockCommentStart("/\\*");
    QRegularExpression blockCommentEnd("\\*/");

    HighLighter *highlighter=new HighLighter(doc);
    highlighter->addKeywords(keywords);
    highlighter->setClassPattern(classPattern);
    highlighter->setQuotePattern(quotePattern);
    highlighter->setFunctionPattern(functionPattern);
    highlighter->setInlineCommentPattern(inlineCommentPattern);
    highlighter->setBlockCommentStartPattern(blockCommentStart);
    highlighter->setBlockCommentEndPattern(blockCommentEnd);
    return highlighter;
}

