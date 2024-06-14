#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
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

        string getFilename();
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
class AnimatedBook : public Book {
public:
    AnimatedBook(string filename, string title, string author, string category);
    void readContent() override;
    void playAnimation();
};

class MorseCodeBook : public Book {
    private:
        map<string, char> morseMap; 

    public:
        MorseCodeBook(string filename, string title, string author, string category);
        void initializeMorseMap();
        void readContent() override;
        string decodeMorse(const string& line);
};

class FormulaBook : public Book {
    public:
        FormulaBook(string filename, string title, string author, string category);
        void readContent();
        double evaluateExpression(const string &expression);
};
#endif