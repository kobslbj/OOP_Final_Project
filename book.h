#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "page.h"

using namespace std;

#ifndef _BOOK_H_
#define _BOOK_H_

class Book{
    protected:
        string filename; 
        string title;
        string author;
        string category;
        vector<Page*> page_vec;

        int PAGE_W = 100;
        int PAGE_H = 40;


    public:
        Book(string filename, string title, string author, string category);

        virtual void readContent();
        virtual void preview();
        void goToPage(int pageNum);

        // keyboard
        virtual char getKey();

        string getTitle();
        string getAuthor();
        string getCategory();
};


class TxtBook : public Book{
    public:
        TxtBook(string filename, string title, string author, string category);
};

class FigBook : public Book{
    public:
        FigBook(string filename, string title, string author, string category);
        void readContent();
        char** get_figure(fstream& fin, int* fig_h);
        // void preview();
};

#endif