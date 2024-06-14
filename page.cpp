#include "page.h"

Page::Page(int _pageNum, int _page_width, int _page_height){
    pageNum = _pageNum;
    page_width = _page_width;
    page_height = _page_height;
}

void Page::setPageCont(char** content){
    this->content = content;
}

int Page::getPageNum(){
    return pageNum;
}

void Page::showPageCont(){
    for(int i=0; i<page_height; i++){
        cout << content[i] << endl;
    }
    return;
}

char** Page::getContent() { 
    return content;
}