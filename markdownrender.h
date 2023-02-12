#ifndef MARKDOWNRENDER_H
#define MARKDOWNRENDER_H
#include<QString>
#include<vector>
#include<iostream>
#include<string>
#include<QMap>
#include"markdownviewer.h"
#include<QVariant>
#include<QFontMetrics>
using namespace std;
enum Type{TITLE,LINK,PARA};
class Markdown{
public:
    int level=0;
    string pstr;
    int pos;
    Type type;
    string content;
    Markdown(string pstr){
        this->pstr=pstr;
        this->pos=0;//下标初始化
    }
    virtual int parser()=0;
    bool gettoken(char token){
        if(pos<pstr.size()&&token==pstr[pos]){
            pos++;
            return true;
        }else{
            return false;
        }
    }
    void delblank(){
        if(pstr[pos]==' '||pstr[pos]=='\t'||pstr[pos]=='\n'){
            pos++;
            delblank();
        }
    }
    void getsentence(){//获取一个句子
        while(pos<pstr.size()&&pstr[pos]!='\n'){
            pos++;
        }
        pos++;
    }
};

class HeadTitle:public Markdown{
public:
    HeadTitle(string pstr):Markdown(pstr){
        type=TITLE;
    }
    int parser(){
        while(gettoken('#')){//确定标题级别
            level++;
        }
        if(!gettoken(' ')){
            return -1;//解析失败返回-1
        }
        int sta=pos;
        getsentence();
        content=pstr.substr(sta,pos-sta);
        return pos;//解析成功
    }
};

class Link:public Markdown{
public:
    Link(string pstr):Markdown(pstr){
        type=LINK;
    }
    string name;
    string linkstr;
    int parser(){
        gettoken('[');
        int sta=pos;
        while(pstr[pos]!=']'){
            pos++;
        }
        name=pstr.substr(sta,pos-sta);
        gettoken(']');
        gettoken('(');
        sta=pos;
        while(pstr[pos]!=')'){
            pos++;
        }
        linkstr=pstr.substr(sta,pos-sta);
        gettoken(')');
        return 0;
    }
};

class Paragraph:public Markdown{
public:
    Paragraph(string str):Markdown(str){
        type=PARA;
    }
    int parser(){
        if(pstr.back()=='\n'){
            pstr.pop_back();
        }
        content=pstr;
        return 0;
    }
};

class WidgetFactory{
public:
    static QWidget* genWidget(Markdown* md,QWidget* wid){
        if(md->type==TITLE){
            static_cast<QLabel*>(wid)->setText(QString::fromStdString(md->content));
            wid->setProperty("line",1);
            QFont font;
            if(md->level==1){
                font.setPixelSize(80);
                font.setBold(true);
                wid->setFont(font);
                QFontMetrics fm(font);//用QFontMetrics计算label的高度
                wid->setProperty("heightadd",fm.height()*1.5);
            }else if(md->level==2){
                font.setPixelSize(70);
                font.setBold(true);
                wid->setFont(font);
                QFontMetrics fm(font);//用QFontMetrics计算label的高度
                wid->setProperty("heightadd",fm.height()*1.5);
            }else if(md->level==3){
                font.setPixelSize(60);
                font.setBold(true);
                wid->setFont(font);
                QFontMetrics fm(font);//用QFontMetrics计算label的高度
                wid->setProperty("heightadd",fm.height()*1.5);
            }else if(md->level==4){
                font.setPixelSize(50);
                font.setBold(true);
                wid->setFont(font);
                QFontMetrics fm(font);//用QFontMetrics计算label的高度
                wid->setProperty("heightadd",fm.height()*1.5);
            }
            return wid;
        }else if(md->type==LINK){
            static_cast<QLabel*>(wid)->setText(QString::fromStdString(md->content));
            wid->setProperty("heightadd",35);
            return wid;
        }else if(md->type==PARA){
            static_cast<QLabel*>(wid)->setText(QString::fromStdString(md->content));
            wid->setProperty("heightadd",35);
            return wid;
        }
    }
};

class MarkdownRender{
public:
    int pos=0;
    int lines=0;
    int height=0;
    //MarkdownRoot* root;
    MarkdownViewer* viewer;
    vector<pair<int,Markdown*>> documents;
    MarkdownRender(MarkdownViewer* pview=nullptr){
        viewer=pview;
    }

    void insert(int linenum,const string& str){
        cout<<linenum<<"  "<<viewer->count()-1<<endl;
        if(linenum<viewer->count()-1){
            QWidget* wid=viewer->getWidget(linenum);
            process(str,wid);
            viewer->wid->adjustSize();
        }else{
            QLabel* wid=new QLabel();
            if(process(str,wid)){
                QVBoxLayout* lout=viewer->layout;
                QLayoutItem* item=lout->itemAt(lout->count()-1);
                lout->removeItem(item);//删除弹簧
                lout->addWidget(wid);
                cout<<static_cast<QLabel*>(wid)->height()<<endl;

                lout->addStretch();//重新添加弹簧
                cout<<wid->property("heightadd").toInt()<<endl;
                height+=wid->property("heightadd").toInt();
                cout<<wid->height()<<endl;
                viewer->wid->setMinimumHeight(height);
                viewer->wid->setMaximumHeight(height);
                viewer->wid->adjustSize();
            }
        }
    }
    void updateHeight(int height){
        viewer->reshape(height);
    }
    bool process(const string& str,QWidget* wid){
        wid->setProperty("line",1);
        wid->setProperty("heightadd",0);
        if(str.size()==0){
            return false;
        }
        if(str[0]=='#'){
            HeadTitle* title=new HeadTitle(str);
            if(title->parser()!=-1){
                WidgetFactory::genWidget(title,wid);
                return true;
            }else{
                cout<<wid->property("heightadd").toInt()<<endl;
                return false;
            }

        }else if(str[0]=='['){
            Link* link=new Link(str);
            if(link->parser()!=-1){
                WidgetFactory::genWidget(link,wid);
                return true;
            }else{
                return false;
            }
        }else{
            Paragraph* para=new Paragraph(str);
            if(para->parser()!=-1){
                WidgetFactory::genWidget(para,wid);
            }
        }
    }
};

#endif // MARKDOWNRENDER_H
