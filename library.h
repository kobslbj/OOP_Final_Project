#include <bits/stdc++.h>
#include "book.h"
#include "user.h"

using namespace std;

class Library{
public:
    Library();
    ~Library();

    // Keyboard handling
    virtual char getKey();
    
    // Operation logic
    virtual void operation(char opCode);

    // Display functions
    virtual void coutMainPage();
    virtual void coutBookIcon(int bookNum);
    virtual void searchBook(); 
    
    // Utility functions
    virtual int getIdx();
    virtual bool getExit();

    // User management
    void addUser();
    bool borrowBook(const string& userID, const string& bookID, const string& date);
    void returnBook(const string& userID, const string& bookID, const string& date);
    vector<string> getMostPopularBooks();

    void borrowBookInteraction();

    void returnBookInteraction();

protected:
    int idx;
    bool exit;
    vector<Book*> books;
    vector<User*> users;
    map<string, int> bookPopularity; // tracks popularity of books by bookID
};

