#ifndef MARKDOWNRENDER_H
#define MARKDOWNRENDER_H
#include<QString>
#include<vector>
#include<iostream>
#include<string>
#include<QMap>
#include"markdownviewer.h"
using namespace std;
enum Type{TITLE,LINK};
class Markdown{
public:
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
    int level=0;
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

//class MarkdownRoot:public Markdown{
//public:
//    MarkdownRoot(string pstr):Markdown(pstr){}
//    vector<Markdown*> sons;//存储根节点的孩子节点
//    int parser(){
//        while(pos<pstr.size()){
//            delblank();
//            if(pstr[pos]=='#'){
//                HeadTitle* title=new HeadTitle(pstr);
//                sons.push_back(title);
//                title->parser();
//            }else if(pstr[pos]=='['){
//                Link* link=new Link(pstr);
//                sons.push_back(link);
//                link->parser();
//            }
//        }
//    }
//};

class WidgetFactory{
public:
    static QWidget* genWidget(Markdown* md,QWidget* wid){
        if(md->type==TITLE){
            static_cast<QLabel*>(wid)->setText(QString::fromStdString(md->content));
            return wid;
        }else if(md->type==LINK){
            return nullptr;
        }
    }
};

class MarkdownRender{
public:
    int pos=0;
    int lines=10;
    //MarkdownRoot* root;
    MarkdownViewer* viewer;
    vector<pair<int,Markdown*>> documents;
    MarkdownRender(MarkdownViewer* pview=nullptr){
        viewer=pview;
    }
//    void render(){
//        root=new MarkdownRoot(parsestr);//文档根节点
//        root->parser();
//    }
    void insert(int linenum,const string& str){
        //QWidget* wid=viewer->wid;
        QVBoxLayout* lout=viewer->layout;
        lines+=10;
        viewer->wid->adjustSize();
        viewer->wid->setMinimumHeight(lines*35);
        viewer->wid->setMaximumHeight(lines*35);
        //QLabel* ll=new QLabel("dfdsf");
        //viewer->setWidget(ll);
//        if(linenum<viewer->count()){
//            QWidget* wid=viewer->getWidget(linenum);
//            if(wid){
//                process(str,wid);
//            }
////            delete(wid);
////            if(!wid){
////                wid=process(str);
////            }
////            QLabel* label=new QLabel("hello world");
////            wid=label;
////            wid->deleteLater();
//        }else{
//            QWidget* wid=new QWidget();
//            process(str,wid);
//            viewer->addWidget(wid);
//        }
    }
    void updateHeight(int height){
        viewer->reshape(height);
    }
    bool process(const string& str,QWidget* wid){
        if(str[0]=='#'){
            HeadTitle* title=new HeadTitle(str);
            if(title->parser()!=-1){
                WidgetFactory::genWidget(title,wid);
                return true;
            }else{
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
        }
    }
};

#endif // MARKDOWNRENDER_H
