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
    QMenu *pFromat=mBar->addMenu("格式");
    QMenu *pView=mBar->addMenu("视图");

    QAction* pNew=pFile->addAction("新建");
    QAction* pOpen=pFile->addAction("打开");
    QAction* pSave=pFile->addAction("保存");
    QAction* pSaveAs=pFile->addAction("另存为");
    QAction* pPrint=pFile->addAction("打印");
    QAction* pExit=pFile->addAction("退出");

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

    QWidget *centerWidget=centralWidget();
    QVBoxLayout *verticalLayout=new QVBoxLayout(centerWidget);
    tab=new tabEditor();
    Editor* editor=new Editor();
    verticalLayout->addWidget(tab);
    connect(combox,static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),tab,&tabEditor::code_formatChange);
    connect(combox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),tab,&tabEditor::code_formatIndexChange);
    connect(tab,&tabEditor::tabChanged,combox,&QComboBox::setCurrentIndex);
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
