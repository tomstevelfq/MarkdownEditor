#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QToolBar>
#include<QLayout>
#include<QFileDialog>
#include<QTextStream>
#include<QLabel>
#include<QFile>
#include<QVBoxLayout>
#include<QMessageBox>
#include<QTextCodec>
#include<QComboBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIcon icon(":/files/icons/MarkDownEditor.svg");
    this->setWindowIcon(icon);
    QMenuBar* mBar=menuBar();
    QMenu* pFile=mBar->addMenu("文件");
    QMenu *pEdit=mBar->addMenu("编辑");
    QMenu *pFormat=mBar->addMenu("格式");
    QMenu *pView=mBar->addMenu("视图");

    QAction* pNew=pFile->addAction("新建");
    QAction* pOpen=pFile->addAction("打开");
    QAction* pSave=pFile->addAction("保存");
    QAction* pSaveAs=pFile->addAction("另存为");
    QAction* pPrint=pFile->addAction("打印");
    QAction* pExit=pFile->addAction("退出");
    QAction *pUndo=pEdit->addAction("撤销");
    QAction *pRedo=pEdit->addAction("重做");
    QAction *pCut=pEdit->addAction("剪切");
    QAction *pCopy=pEdit->addAction("复制");
    QAction *pPaste=pEdit->addAction("粘贴");
    QAction *pFind=pEdit->addAction("查找");
    QAction *pReplace=pEdit->addAction("替换...");
    QAction *pFont=pFormat->addAction("字体");
    QMenu *menuLanguage=pFormat->addMenu("语言");
    pIndent=pFormat->addAction("缩进");
    pWrap=pFormat->addAction("换行");
    pLangPython=menuLanguage->addAction("python");
    pLangCpp=menuLanguage->addAction("cpp");
    pLangJava=menuLanguage->addAction("java");
    pLangPython->setCheckable(true);
    pLangCpp->setCheckable(true);
    pLangJava->setCheckable(true);
    pFormat->addAction(menuLanguage->menuAction());
    QActionGroup *langGroup=new QActionGroup(this);
    langGroup->setExclusive(true);
    langGroup->addAction(pLangPython);
    langGroup->addAction(pLangCpp);
    langGroup->addAction(pLangJava);
    connect(langGroup,&QActionGroup::triggered,this,&MainWindow::on_langSelected);
    pStatus=pView->addAction("状态栏");
    pIndent->setCheckable(true);
    pIndent->setChecked(true);
    pWrap->setCheckable(true);
    pStatus->setCheckable(true);
    pStatus->setChecked(true);
    pWrap->setChecked(false);

    QToolBar* toolBar=new QToolBar("toolBar",this);
    addToolBar(toolBar);

    QIcon icon1(":/files/icons/新建.svg");
    pNew->setIcon(icon1);
    QIcon icon2(":/files/icons/文件夹.svg");
    pOpen->setIcon(icon2);
    QIcon icon3(":/files/icons/保存.svg");
    pSave->setIcon(icon3);
    QIcon icon4(":/files/icons/另存为.svg");
    pSaveAs->setIcon(icon4);
    QIcon icon5(":/files/icons/打印.svg");
    pPrint->setIcon(icon5);
    QIcon icon6(":/files/icons/退出.svg");
    pExit->setIcon(icon6);
    QIcon icon7(":/files/icons/撤销.svg");
    pUndo->setIcon(icon7);
    QIcon icon8(":/files/icons/重做.png");
    pRedo->setIcon(icon8);
    QIcon icon9(":/files/icons/剪切.svg");
    pCut->setIcon(icon9);
    QIcon icon10(":/files/icons/复制.svg");
    pCopy->setIcon(icon10);
    QIcon icon11(":/files/icons/粘贴.svg");
    pPaste->setIcon(icon11);
    QIcon icon12(":/files/icons/搜索.svg");
    pFind->setIcon(icon12);
    //QIcon icon13(":/files/icons/替换.svg");
    //pReplace->setIcon(icon13);


    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->addAction(pNew);
    toolBar->addAction(pOpen);
    toolBar->addAction(pSave);
    toolBar->addAction(pSaveAs);
    toolBar->addSeparator();
    toolBar->addAction(pPrint);
    toolBar->addAction(pExit);
    toolBar->layout()->setContentsMargins(15,15,20,20);
    toolBar->layout()->setSpacing(20);
    toolBar->addSeparator();
    QComboBox* combox=new QComboBox();
    combox->setCurrentText("编码");
    combox->addItem("UTF-8");
    combox->addItem("GBK");
    combox->addItem("ANSI");
    toolBar->addWidget(combox);

    connect(pNew,&QAction::triggered,this,&MainWindow::newFile);
    connect(pOpen,&QAction::triggered,this,&MainWindow::open);
    connect(pExit,&QAction::triggered,this,&MainWindow::close);
    connect(pIndent,&QAction::triggered,this,&MainWindow::on_indentTrigger);
    connect(pWrap,&QAction::triggered,this,&MainWindow::on_wrapTrigger);

    QWidget *centerWidget=centralWidget();
    QVBoxLayout *verticalLayout=new QVBoxLayout(centerWidget);
    tab=new tabEditor();
    Editor* editor=new Editor();
    verticalLayout->addWidget(tab);
    connect(combox,static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),tab,&tabEditor::code_formatChange);
    connect(combox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),tab,&tabEditor::code_formatIndexChange);
    connect(tab,&tabEditor::tabChanged,combox,&QComboBox::setCurrentIndex);
    connect(pSave,&QAction::triggered,tab,&tabEditor::saveFile);
    connect(pSaveAs,&QAction::triggered,tab,&tabEditor::saveFileAs);
    connect(pFont,&QAction::triggered,this,&MainWindow::fontChoose);
    connect(pStatus,&QAction::triggered,this,&MainWindow::on_statusBarTrigger);

    statusBar=new QStatusBar(this);
    this->setStatusBar(statusBar);
    initStatusBar();
    connect(tab,&tabEditor::textChanged,this,&MainWindow::on_textChanged);
    on_textChanged(static_cast<Editor*>(tab->currentWidget()));
    initLangMap();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile(){
    tab->add(new Editor());
}

void MainWindow::open(){
    QString curFilePath=QFileDialog::getOpenFileName(this,"请选择文件","./","text (*.txt)");
    tab->openFile(curFilePath);
}

void MainWindow::fontChoose(){
    Editor *editor=static_cast<Editor*>(tab->currentWidget());
    editor->launchFontDialog();
}

void MainWindow::initStatusBar(){
    languageLabel=new QLabel("语言：未选择");
    wordLabel=new QLabel("字数:");
    charLabel=new QLabel("字符数:");
    wordCountLabel=new QLabel();
    charCountLabel=new QLabel();
    statusBar->setStyleSheet("margin-left:15px");
    statusBar->addWidget(languageLabel);
    statusBar->addPermanentWidget(wordLabel);
    statusBar->addPermanentWidget(wordCountLabel);
    statusBar->addPermanentWidget(charLabel);
    statusBar->addPermanentWidget(charCountLabel);
}

void MainWindow::initLangMap(){
    langMap[pLangPython]="python";
    langMap[pLangCpp]="cpp";
    langMap[pLangJava]="java";
}

void MainWindow::on_textChanged(Editor *editor){
    int letterNum=editor->toPlainText().length();
    wordCountLabel->setText(QString::number(letterNum));
    charCountLabel->setText(QString::number(letterNum));
}

void MainWindow::on_statusBarTrigger(){
    if(pStatus->isChecked()){
        statusBar->setVisible(true);
    }else{
        statusBar->setVisible(false);
    }
}

void MainWindow::on_langSelected(QAction* langAction){
    QString lang=langMap[langAction];
    if(lang==static_cast<Editor*>(tab->currentWidget())->getLang()){
        return;
    }else{
        static_cast<Editor*>(tab->currentWidget())->langChanged(lang);
        languageLabel->setText(lang);
    }
}

void MainWindow::on_wrapTrigger(){
    if(pWrap->isChecked()){
        static_cast<Editor*>(tab->currentWidget())->setWordWrapMode(QTextOption::WordWrap);
        static_cast<Editor*>(tab->currentWidget())->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    }else{
        static_cast<Editor*>(tab->currentWidget())->setWordWrapMode(QTextOption::NoWrap);
    }
}

void MainWindow::on_indentTrigger(){

}
