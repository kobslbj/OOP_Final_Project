#include <iostream>
#include <string>

using namespace std;


#ifndef _PAGE_H_
#define _PAGE_H_

class Page{
    private:
        int page_width;
        int page_height;
        int pageNum;
        char** content;

    public:
        Page(int _pageNum, int _page_width, int _page_height);

        void setPageCont(char** content);
        int getPageNum(); //
        void showPageCont();  // display content pointer
        char** getContent(); 
};

#endif