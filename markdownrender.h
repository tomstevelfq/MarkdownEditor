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
#include"editor.h"
using namespace std;
enum Type{TITLE,LINK,PARA,BLOCK};
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
        while(pos<pstr.size()&&pstr[pos]!=']'){
            pos++;
        }
        name=pstr.substr(sta,pos-sta);
        if(!gettoken(']')||name.size()==0){
            return -1;
        }
        if(!gettoken('(')){
            return -1;
        }
        sta=pos;
        while(pos<pstr.size()&&pstr[pos]!=')'){
            pos++;
        }
        linkstr=pstr.substr(sta,pos-sta);
        if(!gettoken(')')){
            return -1;
        }
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

class Block:public Markdown{
public:
    vector<string> contents;
    Block(string str):Markdown(str){
        type=BLOCK;
    }
    void setContents(const vector<string>& v){
        contents=v;
    }
    void setContent(const string& str){
        content=str;
    }
    int parser(){
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
            static_cast<QLabel*>(wid)->setText(QString::fromStdString(static_cast<Link*>(md)->name));
            wid->setProperty("heightadd",35*1.5);
            wid->setStyleSheet("text-decoration:underline;color:#1E90FF;");
            return wid;
        }else if(md->type==PARA){
            static_cast<QLabel*>(wid)->setText(QString::fromStdString(md->content));
            QFont font;
            font.setPixelSize(30);
            wid->setFont(font);
            QFontMetrics fm(font);
            wid->setProperty("heightadd",fm.height()*1.5);
            return wid;
        }else if(md->type==BLOCK){
            static_cast<QLabel*>(wid)->setText(QString::fromStdString(md->content));
            QFont font;
            font.setPixelSize(30);
            wid->setFont(font);
            QFontMetrics fm(font);
            static_cast<QLabel*>(wid)->setStyleSheet("border:5px solid gray;border-width:0 0 0 5px;");
            wid->setProperty("heightadd",fm.height()*2);
            return wid;
        }
    }
};

class MarkdownRender{
public:
    int pos=0;
    int lines=0;
    int height=0;
    Editor* editor;
    //MarkdownRoot* root;
    MarkdownViewer* viewer;
    vector<pair<int,Markdown*>> documents;
    MarkdownRender(MarkdownViewer* pview=nullptr){
        viewer=pview;
    }

    void insert(int linenum,const string& str){
        cout<<linenum<<"  "<<viewer->count()<<endl;
        if(linenum<viewer->count()){
            QWidget* wid=viewer->getWidget(linenum);
            process(str,wid,linenum);
            viewer->wid->adjustSize();
        }else{
            QLabel* wid=new QLabel();
            if(process(str,wid,linenum)){
                QVBoxLayout* lout=viewer->layout;
                lout->insertWidget(lout->count(),wid);
                cout<<wid->property("heightadd").toInt()<<endl;
                height+=wid->property("heightadd").toInt();
                cout<<wid->height()<<endl;
                viewer->wid->setMinimumHeight(max(height,100));
                viewer->wid->setMaximumHeight(max(height,100));
                viewer->wid->setMinimumWidth(200);
                viewer->wid->adjustSize();
            }
        }
    }
    void updateHeight(int height){
        viewer->reshape(height);
    }

    bool process(const string& str,QWidget* wid,int linenum){
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
        }else if(str[0]=='>'){
            auto doc=editor->document();
            while(true){
                if(linenum>=0&&doc->findBlockByNumber(linenum).text()[0]=='>'){
                    linenum--;
                }else{
                    linenum++;
                    break;
                }
            }
            string con;
            while(true){
                if(linenum>doc->lineCount()||!(doc->findBlockByNumber(linenum).text()[0]=='>')){
                    break;
                }
                con+=doc->findBlockByNumber(linenum).text().toStdString()+'\n';
                linenum++;
            }
            Block* block=new Block(str);
            block->setContent(con);
            if(block->parser()!=-1){
                WidgetFactory::genWidget(block,wid);
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
