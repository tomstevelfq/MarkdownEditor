#ifndef SEARCHHISTORY_H
#define SEARCHHISTORY_H
#include<QMap>
class SearchHistory{
public:
    SearchHistory(){}
    inline void clear(){searchHistory.clear();}
    void add(QString term,int curPosBeforeFirstSearch,int firstFoundAt){//curPosBeforeFirstSearch是查找操作之前的光标位置
        if(!previousFound(term)){
            searchHistory.clear();
        }
        QPair<int,int> location;
        location.first=curPosBeforeFirstSearch;
        location.second=firstFoundAt;
        searchHistory.insert(term,location);
    }
    inline bool previousFound(QString term){return searchHistory.find(term)!=searchHistory.end();}
    inline int curPosBeforeFirstSearchFor(QString term){return searchHistory[term].first;}
    inline int firstFoundAt(QString term){return searchHistory[term].second;}
    inline QMap<QString,QPair<int,int>>* getSearchHistory(){return &searchHistory;}

private:
    QMap<QString,QPair<int,int>> searchHistory;
};

#endif // SEARCHHISTORY_H
