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

        if (type == 'T') {
            TxtBook* tb = new TxtBook(filename, title, author, category);
            books.push_back(tb);
        } else if (type == 'F') {
            FigBook* fb = new FigBook(filename, title, author, category);
            books.push_back(fb);
        } else if (type == 'A') {  
            AnimatedBook* ab = new AnimatedBook(filename, title, author, category);
            books.push_back(ab);
        } else if (type == 'M') { 
            MorseCodeBook* mb = new MorseCodeBook(filename, title, author, category);
            books.push_back(mb);
        } else if (type == 'B') {  
            FormulaBook* fb = new FormulaBook(filename, title, author, category);
            books.push_back(fb);
        }
    }
    inList.close();
}

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

void Library::operation(char opCode) {
    int maxIdx = books.size() + 3; // 最大的 idx 值，包括書籍、Quit、Add User、Borrow Book、Return Book
    switch (opCode) {
        case 'L': 
            if (idx > 1 && idx <= books.size()) idx--; 
            break;
        case 'R': 
            if (idx >= 0 && idx < books.size()) idx++; 
            break;
        case 'U': 
            if (idx > 0) idx--; // 簡單地減少 idx，如果在最頂端將循環到底部
            else idx = maxIdx; // 從頂部循環到底部
            break;
        case 'D': 
            if (idx < maxIdx) idx++; // 簡單地增加 idx，如果在底部將循環到頂部
            else idx = 0; // 從底部循環到頂部
            break;
        case 'E':  
            if (idx == books.size()) {
                exit = true;
            } else if (idx == 0) {
                searchBook();
            } else if (idx > 0 && idx < books.size()) {
                books[idx - 1]->preview();
            } else if (idx == books.size() + 1) {
                addUser();
            } else if (idx == books.size() + 2) {
                borrowBookInteraction();
            } else if (idx == books.size() + 3) {
                returnBookInteraction();
            }
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
    // display "Total books number"
    cout << " Total books number: " << books.size()-1 << endl;

    // display "Quit"
    if(idx == books.size()) cout << WHITE_B;
    cout << " Quit";
    if(idx == books.size()) cout << NONE; // 確保退出反白後立即關閉
    cout << endl;
    cout << endl;

        // display "Add user"
    if(idx == books.size() + 1) cout << WHITE_B;
    cout << " Add user";
    for(int i = 0; i < 95; i++) cout << " ";
    if(idx == books.size() + 1) cout << NONE; // 確保添加用戶反白後立即關閉
    cout << endl;
    cout << endl;

    // display "Borrow Book"
    if (idx == books.size() + 2) cout << WHITE_B;
    cout << " Borrow Book";
    if (idx == books.size() + 2) cout << NONE;
    cout << endl;
    cout << endl;

    // display "Return Book"
    if (idx == books.size() + 3) cout << WHITE_B;
    cout << " Return Book";
    if (idx == books.size() + 3) cout << NONE;
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
void Library::searchBook() {
    cout << "********** Search book **********\n";
    cout << "Index by ...\n";
    cout << "Press 1 to choose filename\n";
    cout << "Press 2 to choose title\n";
    cout << "Press 3 to choose author\n";
    cout << "Press 4 to choose category\n";
    cout << "Press 5 to Exit\n";
    cout << "Enter search type: ";
    
    int type;
    cin >> type;

    if (type == 5) {
        cout << "Exiting search..." << endl;
        return;
    }

    string searchTerm;
    switch (type) {
        case 1:
            cout << "Please enter the filename you want to search: ";
            break;
        case 2:
            cout << "Please enter the title you want to search: ";
            break;
        case 3:
            cout << "Please enter the author you want to search: ";
            break;
        case 4:
            cout << "Please enter the category you want to search: ";
            break;
        default:
            cout << "Invalid search type. Please try again." << endl;
            return;
    }

    cin >> searchTerm;

    cout << "You entered: '" << searchTerm << "'" << endl;

    transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::tolower);
    cout << "Searching for: '" << searchTerm << "'\n";

    vector<Book*> foundBooks;
    for (auto& book : books) {
        string toSearch;
        switch (type) {
            case 1: toSearch = book->getFilename(); break;
            case 2: toSearch = book->getTitle(); break;
            case 3: toSearch = book->getAuthor(); break;
            case 4: toSearch = book->getCategory(); break;
        }
        transform(toSearch.begin(), toSearch.end(), toSearch.begin(), ::tolower);
        if (toSearch.find(searchTerm) != string::npos) {
            foundBooks.push_back(book);
        }
    }

    if (foundBooks.empty()) {
        cout << "No books found." << endl;
    } else {
        cout << "These are books related to your search ...\n";
        int num = 1;
        for (auto& book : foundBooks) {
            cout << num++ << ". " << book->getTitle() << " by " << book->getAuthor() << endl;
        }
    }

    string tmp;
    cout << "Press e to continue...";
    cin >> tmp;
    if (tmp == "e") {
        cout << "Continuing..." << endl;
    }
}

void Library::addUser() {
    string userID, name;
    cout << "Enter user ID: ";
    cin >> userID;
    cout << "Enter name: ";
    cin >> name;
    users.push_back(new User{userID, name});
    cout<< "User added successfully!" << endl;
    cout << "Current Users:\n";
    for (const auto& user : users) {
        cout << "User ID: " << user->userID << ", Name: " << user->name << "\n";
    }
    cout << endl;
    string tmp;
    cout << "Press e to continue...";
    cin >> tmp;
    if (tmp == "e") {
        cout << "Continuing..." << endl;
    }
}

bool Library::borrowBook(const string& userID, const string& bookID, const string& date) {
    // Find user and book, add to borrowing history
    for (auto& user : users) {
        if (user->userID == userID) {
            user->borrowBook(bookID, date);
            bookPopularity[bookID]++;  // Increase book popularity
            return true; // 借书成功
        }
    }
    return false; // 没有找到用户，借书失败
}

void Library::returnBook(const string& userID, const string& bookID, const string& date) {
    for (auto& user : users) {
        if (user->userID == userID) {
            user->returnBook(bookID, date);
            // Optional: Decrease popularity
            break;
        }
    }
}

vector<string> Library::getMostPopularBooks() {
    vector<pair<string, int>> sortedBooks(bookPopularity.begin(), bookPopularity.end());
    sort(sortedBooks.begin(), sortedBooks.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    vector<string> result;
    for (const auto& book : sortedBooks) {
        result.push_back(book.first);
    }
    return result;
}

void Library::borrowBookInteraction() {
    cout << "Enter user ID: ";
    string userID;
    cin >> userID;

    // 检查用户是否存在
    auto it = find_if(users.begin(), users.end(), [&userID](const User* user) {
        return user->userID == userID;
    });

    if (it == users.end()) {
        // 如果用户不存在
        cout << "User ID not found. Would you like to add a new user? (yes/no): ";
        string response;
        cin >> response;
        if (response == "yes" || response == "Yes") {
            // 如果用户同意添加新用户
            addUser();
            // 再次提示输入用户ID，因为可能用户不想借书了或者希望使用另一个ID
            cout << "Enter user ID for borrowing the book: ";
            cin >> userID;
        } else {
            // 如果用户不想添加，退出交互
            cout << "Operation cancelled." << endl;
            return;
        }
    }

    cout << "Enter book ID: ";
    string bookID;
    cin >> bookID;

    // 执行借书操作
    bool borrowed = borrowBook(userID, bookID, "2024-01-01");  // Use current date or input
    if (borrowed) {
        cout << "Book borrowed successfully!\n";
    } else {
        cout << "Book borrowing failed. Please check book ID and availability.\n";
    }
    
    string tmp;
    cout << "Press e to continue...";
    cin >> tmp;
    if (tmp == "e") {
        cout << "Continuing..." << endl;
    }
}

void Library::returnBookInteraction() {
    cout << "Enter user ID: ";
    string userID;
    cin >> userID;

    cout << "Enter book ID: ";
    string bookID;
    cin >> bookID;

    returnBook(userID, bookID, "2024-01-01"); // Use return date or input
    cout << "Book returned successfully!\n";
    string tmp;
    cout << "Press e to continue...";
    cin >> tmp;
    if (tmp == "e") {
        cout << "Continuing..." << endl;
    }
}
