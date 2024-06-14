#include <bits/stdc++.h>
#include "book.h"

using namespace std;

class Library{
    public:
        Library();
        ~Library();

        // keyboard
        virtual char getKey();
        
        // operation 
        virtual void operation(char opCode);

        // display function
        virtual void coutMainPage();
        virtual void coutBookIcon(int bookNum);
        virtual void searchBook(); 
        
        // get function
        virtual int  getIdx();
        virtual bool getExit();

    protected:
        int idx;
        bool exit;
        vector<Book*> books;
};