#ifndef MARKDOWNVIEWER_H
#define MARKDOWNVIEWER_H
#include<QWidget>
#include<QLabel>
#include<QVBoxLayout>
#include<QScrollArea>
//class MarkdownViewer:public QScrollArea{
//public:
//    QWidget* wid;
//    QWidget* wid2;
//    QVBoxLayout* layout;
//    QVBoxLayout* layout2;
//    MarkdownViewer(){
//        wid=new QWidget(this);
//        wid2=new QWidget(this);
//        wid->setObjectName("widget");
//        layout=new QVBoxLayout(wid);
//        layout2=new QVBoxLayout(wid);
//        layout2->addWidget(new QLabel("hhhhhhhhh"));
//        layout->addWidget(new QLabel("hello"));
//        layout->setObjectName("layout");
//        this->setWidget(wid2);
//    }
//    QWidget* getWidget(int index){//获取layout中下标为index的widget
//        return layout->itemAt(index)->widget();
//    }
//    int count(){
//        //return layout->count();
//        return 0;
//    }
//    void addWidget(QWidget* wid){
//        layout->addWidget(wid);
//    }
//};

class MarkdownViewer:public QWidget{
public:
    QWidget* wid;
    QVBoxLayout* layout;
    QScrollArea* scrollArea;
    QVBoxLayout* mainLayout;
    MarkdownViewer(){
        mainLayout=new QVBoxLayout(this);
        wid=new QWidget();
        layout=new QVBoxLayout(wid);
        scrollArea=new QScrollArea();
        scrollArea->setWidget(wid);
        mainLayout->addWidget(scrollArea);
        mainLayout->addStretch();
        scrollArea->setStyleSheet("border:none");
        this->setStyleSheet("border:1px solid gray;background-color:#D3D3D3;");
    }
    QWidget* getWidget(int index){//获取layout中下标为index的widget
        return layout->itemAt(index)->widget();
    }
    int count(){
        return layout->count();
    }
    void addWidget(QWidget* wid){
        layout->addWidget(wid);
    }
    void reshape(int height){
        scrollArea->setMinimumHeight(height+200);
    }
    void resizeEvent(QResizeEvent *event) override{
        QWidget::resizeEvent(event);
        scrollArea->setMinimumHeight(this->height()-40);
    }
};
#endif // MARKDOWNVIEWER_H
