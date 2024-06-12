#include "library.h"

// txt file path
#define PATH "./TXT/"

// color setting
#define NONE    "\e[0m"
#define WHITE_B "\e[47m"

Library::Library(){
    idx = 0;
    exit = false;
    string path = PATH;

    ifstream inList;
    inList.open(path + "bookList.txt");

    string filename;
    char   type;

    while(!inList.eof()){
        inList >> filename >> type;

        ifstream inBook;
        inBook.open(path + filename);

        string str;
        string title;
        string author;
        string category;

        // Book Category
        while(1){
            getline(inBook, str);
            const auto find = str.find("type:");
            if(find != string::npos){
                str.erase(find, 5);
                str.erase(str.length()-1, 1);
                while(str[0] == ' ') str.erase(0, 1);
                category = str;
                break;
            }
        }

        // Book Title
        while(1){
            getline(inBook, str);
            const auto find = str.find("Title:");
            if(find != string::npos){
                str.erase(find, 6);
                str.erase(str.length()-1, 1);
                while(str[0] == ' ')  str.erase(0, 1);
                title = str;
                break;
            }
        }

        // Book Title
        while(1){
            getline(inBook, str);
            const auto find = str.find("Author:");
            if(find != string::npos){
                str.erase(find, 7);
                str.erase(str.length()-1, 1);
                while(str[0] == ' ')  str.erase(0, 1);
                author = str;
                break;
            }
        }

        if(type == 'T'){
            TxtBook *tb = new TxtBook(filename, title, author, category);
            books.push_back(tb);            
        }
        else if(type == 'F'){
            FigBook *fb = new FigBook(filename, title, author, category);
            books.push_back(fb);        
        }
    }
    inList.close();
};

Library::~Library(){
    for(auto& b: books) delete b;
    books.clear();
};

char Library::getKey(){
    char key;

    while(true){
        system("stty raw");
        key = getchar();
        system("stty cooked");

        if (key == '\x1b') {
            if (getchar() == '\x5b') {
                switch(getchar()) {
                    case 'A': return 'U'; // up    (esc + [ + A)
                    case 'B': return 'D'; // down  (esc + [ + B)
                    case 'C': return 'R'; // right (esc + [ + C)
                    case 'D': return 'L'; // left  (esc + [ + D)
                }
            }
        }
        else if (key == '\x0d') return 'E'; // enter (cd)
    }
}

void Library::operation(char opCode){
    switch (opCode)
    {
        case 'L': 
            if(idx > 1 && idx < books.size())     idx--; 
            break;
        case 'R': 
            if(idx > 0 && idx < books.size() - 1) idx++; 
            break;
        case 'U': 
            if(idx > books.size() - 1) idx = books.size() - 1;
            else if(idx < 5)           idx = 0;
            else                       idx = idx - 5;
            break;
        case 'D': 
            if(idx > books.size() - 5) idx = books.size();
            else if(idx == 0)          idx = 1;
            else                       idx = idx + 5;
            break;
        case 'E': 
            if(idx == books.size()) exit = true;
            else if(idx != 0)       books[idx - 1]->preview();
            break;        
    } 
    return;
}

void Library::coutMainPage(){
    system("clear");
    // display "Title"
    for(int i = 0; i < 100; i++) cout << "=";
    cout << endl;
    cout << endl;
    for(int i = 0; i < 33; i++) cout << " ";
    cout << "Welcome to NYCU library system !!!";
    for(int i = 0; i < 33; i++) cout << " ";
    cout << endl;
    cout << endl;   
    
    // display "Search"
    if(idx == 0) cout << WHITE_B;
    cout << " Search";
    for(int i = 0; i < 93; i++) cout << " ";
    cout << NONE;
    cout << endl;
    cout << endl;


    // display "Books"
    cout << " Books" << endl;
    int bookNum = 0;
    while(bookNum < books.size()){
        coutBookIcon(bookNum);
        bookNum += 5;
    }

    // display "Quit"
    if(idx == books.size()) cout << WHITE_B;
    cout << " Quit";
    for(int i = 0; i < 95; i++) cout << " ";
    cout << NONE;
    cout << endl;
    cout << endl;

    // display guide
    cout << " Guide" << endl;
    cout << "  > move up    : [↑]" << endl;
    cout << "  > move down  : [↓]" << endl;
    cout << "  > move left  : [←]" << endl;
    cout << "  > move right : [→]" << endl;
    cout << "  > confirm    : [↲]" << endl;
    cout << endl;
    for(int i = 0; i < 100; i++) cout << "=";
    cout << endl;
    


    return;
}

void Library::coutBookIcon(int bookNum){
    for(int j = 0; j < 7; j++){
        cout << "     ";
        for(int i = 0; i < 5; i++){
            // char* title [36];
            string title = "";
            if(bookNum + i < books.size() - 1) {
                title = books[bookNum + i]->getTitle();

                if(bookNum + i == idx - 1) cout << WHITE_B;
                switch(j){
                    case 0: case 4: cout << "###############" << NONE; break;
                    case 1: case 3: cout << "#             #" << NONE; break;
                    case 2: cout << "#" << setw(7) << title[0] << "      #" << NONE; break;
                    case 5: 
                        for(uint k = 0; k < 15; k++) 
                            if(k < title.size()) cout << title[k];
                            else                 cout << " ";

                        cout << NONE; break;
                    case 6: 
                        for(uint k = 0; k < 15; k++) 
                            if(15 + k < title.size()) cout << title[15 + k];
                            else                      cout << " ";
                        cout << NONE; break;
                }
            }
            cout << "    ";
        }
        cout << endl;
    }
    cout << endl;
    return;
}

bool Library::getExit(){
    return exit;
}

int Library::getIdx(){
    return idx;
}