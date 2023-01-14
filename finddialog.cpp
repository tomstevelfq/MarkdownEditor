#include"finddialog.h"

FindDialog::FindDialog(QWidget* parent):QDialog(parent){
    findLabel=new QLabel("find what:    ");
    replaceLabel=new QLabel("replace with:");
    findLineEdit=new QLineEdit();
    replaceLineEdit=new QLineEdit();
    findNextButton=new QPushButton("find next");
    replaceButton=new QPushButton("replace");
    replaceAllButton=new QPushButton("replace all");
    caseSensitiveBox=new QCheckBox("match case");
    wholeWordsBox=new QCheckBox("whole words");

    setFocusProxy(findLineEdit);

    findHorizontal=new QHBoxLayout();
    replaceHorizontal=new QHBoxLayout();
    optionsLayout=new QHBoxLayout();
    verticalLayout=new QVBoxLayout();

    verticalLayout->addLayout(findHorizontal);
    verticalLayout->addLayout(replaceHorizontal);
    verticalLayout->addLayout(optionsLayout);

    findHorizontal->addWidget(findLabel);
    findHorizontal->addWidget(findLineEdit);
    replaceHorizontal->addWidget(replaceLabel);
    replaceHorizontal->addWidget(replaceLineEdit);
    optionsLayout->addWidget(caseSensitiveBox);
    optionsLayout->addWidget(wholeWordsBox);
    optionsLayout->addWidget(findNextButton);
    optionsLayout->addWidget(replaceButton);
    optionsLayout->addWidget(replaceAllButton);

    setLayout(verticalLayout);
    setWindowTitle("find and replace");

    connect(findNextButton,&QPushButton::clicked,this,&FindDialog::on_findNext);
    connect(replaceButton,&QPushButton::clicked,this,&FindDialog::on_replace);
    connect(replaceAllButton,&QPushButton::clicked,this,&FindDialog::on_replace);
}

void FindDialog::on_findNext(){
    QString query=findLineEdit->text();
    if(query.isEmpty()){
        QMessageBox::information(this,"输入为空","请输入一条查询");
        return;
    }
    bool caseSensitive=caseSensitiveBox->isChecked();
    bool wholeWords=wholeWordsBox->isChecked();
    emit(start_find(query,caseSensitive,wholeWords));
}

void FindDialog::on_replace(){
    QString what=findLineEdit->text();
    if(what.isEmpty()){
        QMessageBox::information(this,"输入为空","请输入一条查询");
        return;
    }
    QString with=replaceLineEdit->text();
    bool caseSensitive=caseSensitiveBox->isChecked();
    bool wholeWords=wholeWordsBox->isChecked();
    bool replace=sender()==replaceButton;
    if(replace){
        emit(start_replace(what,with,caseSensitive,wholeWords));
    }else{
        emit(start_replaceAll(what,with,caseSensitive,wholeWords));
    }
}

void FindDialog::on_findRequestReady(QString message){
    QMessageBox::information(this,"find and replace",message);
}


