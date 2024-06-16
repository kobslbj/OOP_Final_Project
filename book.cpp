#include "book.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <thread>
#include <chrono>
#include <cstdlib> 
#include <cstdio>  
#include <unistd.h> 
//===================================//
//          BOOK DEFINITION          //
//===================================//
Book::Book(string filename, string title, string author, string category){
    this->filename = filename;
    this->title    = title;
    this->author   = author;  
    this->category = category;
    page_vec.clear();
    page_vec.reserve(10);
}

string Book::getFilename(){
    return filename;
}   

string Book::getTitle(){
    return title;
}

string Book::getAuthor(){
    return author;
}

string Book::getCategory(){
    return category;
}

void Book::readContent(){
    int WIDTH = PAGE_W;
    int HEIGHT = PAGE_H;
    
    string path = "./TXT/";
    string dir = path + filename;
    fstream fin;

    string str;

    fin.open(dir, ios::in);
    cout << "readContent ..." << endl;


    // Book Category
    while(1){
        getline(fin, str);
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
        getline(fin, str);
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
        getline(fin, str);
        const auto find = str.find("Author:");
        if(find != string::npos){
            str.erase(find, 7);
            str.erase(str.length()-1, 1);
            while(str[0] == ' ')  str.erase(0, 1);
            author = str;
            break;
        }
    }
            
    // Book content 
    int page_cnt = 0;
    int line_cnt = 0;
    char** page_cont = new char*[HEIGHT];
    bool get_new_page = true;
    bool get_new_line = true;
    Page* new_page_ptr;
    while(!fin.eof() || (fin.eof() && !get_new_line)){
        // Construct new page
        if(get_new_page){
            cout << "Get new page ..." << endl;
            new_page_ptr = new Page (page_cnt, PAGE_W, PAGE_H);
            page_cont = new char*[HEIGHT];
            for(int i=0; i<HEIGHT; i++){
                *(page_cont+i) = new char[WIDTH];
                page_cont[i][0] = '\0';
            }
            new_page_ptr->setPageCont(page_cont);
            page_vec.push_back(new_page_ptr);
            get_new_page = false;
            line_cnt = 0;
        }

        if(get_new_line){
            getline(fin, str);
            get_new_line = false;
        }

        const auto find = str.find("\r");

        if(find != string::npos){
            str.erase(find, 1);
        }

        if(str.length() > WIDTH-1){
            for(int i=0; i<WIDTH-1; i++){
                page_cont[line_cnt][i] = str[i];
            }
            page_cont[line_cnt][WIDTH-1] = '\0';
            str.erase(0, WIDTH-1);
        }
        else{
            // To prevent putting endline at the beginning of the page
            if(line_cnt==0 && str.length()==0){
                get_new_line = true;
                continue;
            }

            int i;
            for(i=0; i<str.length(); i++){
                page_cont[line_cnt][i] = str[i];
            }
            if(i < WIDTH){
                page_cont[line_cnt][i] = '\0';
            }
            get_new_line = true;
        }
        
        if(line_cnt == HEIGHT-1){
            get_new_page = true;
        }
        else{
            line_cnt++;
        }
    }
}

void Book::preview(){
    int cur_page = 0;
    string input;

    readContent();

    while (true) {
        system("clear");

        cout << title << "\n\n";

        for (int i = 0; i < PAGE_W; i++) {
            cout << "=";
        }
        cout << "\n\n";

        page_vec[cur_page]->showPageCont();

        cout << "\n" << string(PAGE_W / 2 - 4, ' ') << "Page " << cur_page << "\n\n";

        for (int i = 0; i < PAGE_W; i++) {
            cout << "=";
        }
        cout << "\n";

        cout << "########################\n";
        cout << "# Next Page: [N]       #\n";
        cout << "# Previous Page: [P]   #\n";
        cout << "# Jump to Page: [J]    #\n";
        cout << "# Back to Menu : [M]   #\n";
        cout << "########################\n";

        cout << "Enter command or page number: ";
        getline(cin, input);

        if (input.empty()) {
            continue;
        }

        char command = toupper(input[0]);

        switch (command) {
            case 'N':
                if (cur_page < page_vec.size() - 1) cur_page++;
                break;
            case 'P':
                if (cur_page > 0) cur_page--;
                break;
            case 'J':
                if (isdigit(input[1])) {
                    int pageNum = stoi(input.substr(1));
                    if (pageNum >= 0 && pageNum < page_vec.size()) {
                        goToPage(pageNum);
                        cur_page = pageNum;
                    } else {
                        cout << "Invalid page number.\n";
                        cin.get();
                    }
                }
                break;
            case 'M':
                for (auto& p : page_vec) {
                    delete p;
                }
                page_vec.clear();
                return;  // Exit the preview function properly
            default:
                cout << "Invalid command.\n";
                cin.get();
                break;
        }
    }
}

void Book::goToPage(int pageNum) {
    if (pageNum < 0 || pageNum >= page_vec.size()) {
        cout << "Invalid page number." << endl;
        return;
    }
    system("clear");
    cout << "Jumping to page: " << pageNum << endl;
    page_vec[pageNum]->showPageCont(); 
}
char Book::getKey(){
    char key;

    while(true){
        system("stty raw");
        key = getchar();
        system("stty cooked");

        if (key == '\x1b') {
            if (getchar() == '\x5b') {
                switch(getchar()) {
                    case 'C': return 'N'; // next page      (esc + [ + C)
                    case 'D': return 'P'; // previous page  (esc + [ + D)
                    case 'F': return 'M'; // previous page  (esc + [ + D)
                }
            }
        }
        // else if (key == '\x23') return 'M'; // return to main page (end)
    }
}


//===================================//
//        TXTBOOK DEFINITION         //
//===================================//

TxtBook::TxtBook(string filename, string title, string author, string category) : Book::Book(filename, title, author, category) {};

//===================================//
//        FIGBOOK DEFINITION         //
//===================================//
FigBook::FigBook(string filename, string title, string author, string category) : Book::Book(filename, title, author, category), figureLoaded(false){};

void FigBook:: loadFigure(int page, int start, int end) {
    if (page < page_vec.size() && start >= 0 && end < page_vec[page]->getHeight() && start <= end) {
        figure.page = page;
        figure.startLine = start;
        figure.endLine = end;
        figure.lines = vector<string>(page_vec[page]->getContent() + start, page_vec[page]->getContent() + end + 1);
        figureLoaded = true;
    }
}

void FigBook::moveFigure(char direction) {
    if (!figureLoaded) {
        cout << "No figure loaded." << endl;
        return;
    }
    switch (direction) {
        case 'w': // Move up
            if (figure.startLine > 0) {
                figure.startLine--;
                figure.endLine--;
            } else {
                cout << "This is the top of the page, you cannot move up." << endl;
            }
            break;
        case 's': // Move down
            if (figure.endLine < page_vec[figure.page]->getHeight() - 1) {
                figure.startLine++;
                figure.endLine++;
            } else {
                cout << "This is the bottom of the page, you cannot move down." << endl;
            }
            break;
    }
    updatePageWithFigure();
}

void FigBook::updatePageWithFigure() {
    // Clear the original position of the figure
    for (int i = figure.startLine; i <= figure.endLine; i++) {
        memset(page_vec[figure.page]->getContent()[i], ' ', page_vec[figure.page]->getWidth());
    }
    // Place the figure in the new position
    for (int i = 0; i < figure.lines.size(); i++) {
        strncpy(page_vec[figure.page]->getContent()[figure.startLine + i], figure.lines[i].c_str(), figure.lines[i].size());
    }
}

void FigBook::readContent() {
    int WIDTH = PAGE_W;
        int HEIGHT = PAGE_H;

        string path = "./TXT/";
        string dir = path + filename;
        fstream fin;

        fin.open(dir, ios::in);
        if (!fin.is_open()) {
            cerr << "Failed to open file: " << dir << endl;
            return;
        }

        string str;
        int page_cnt = 0;
        int line_cnt = 0;
        bool get_new_page = true;
        Page* new_page_ptr = nullptr;
        char** page_cont = nullptr;

        while (getline(fin, str)) {
            if (str.find(".fig") != string::npos) {
                int fig_h;
                char** fig_cont = get_figure(fin, &fig_h);
                adjustTextAroundFigure(page_cont, fig_cont, line_cnt, fig_h, WIDTH);
                continue;
            }

            if (get_new_page) {
                new_page_ptr = new Page(page_cnt, WIDTH, HEIGHT);
                page_cont = new char*[HEIGHT];
                for (int i = 0; i < HEIGHT; ++i) {
                    page_cont[i] = new char[WIDTH + 1];
                    memset(page_cont[i], ' ', WIDTH);
                    page_cont[i][WIDTH] = '\0';
                }
                new_page_ptr->setPageCont(page_cont);
                page_vec.push_back(new_page_ptr);
                page_cnt++;
                get_new_page = false;
                line_cnt = 0;
            }

            // Replace carriage return
            str.erase(remove(str.begin(), str.end(), '\r'), str.end());

            if (line_cnt < HEIGHT) {
                strncpy(page_cont[line_cnt], str.c_str(), WIDTH);
                page_cont[line_cnt][WIDTH] = '\0';
                line_cnt++;
            } else {
                get_new_page = true;
            }
        }

        if (new_page_ptr && line_cnt < HEIGHT) {
            for (int i = line_cnt; i < HEIGHT; ++i) {
                delete[] page_cont[i];
            }
        }

        fin.close();
    }

char** FigBook::get_figure(fstream& fin, int* fig_h) {
    int line_cnt = 0;
        string str;
        char** fig_ptr = new char*[PAGE_H];
        for (int i = 0; i < PAGE_H; i++) {
            fig_ptr[i] = new char[PAGE_W + 1];
            memset(fig_ptr[i], ' ', PAGE_W);
            fig_ptr[i][PAGE_W] = '\0';
        }

        while (getline(fin, str) && str.find(".figend") == string::npos) {
            if (line_cnt < PAGE_H) {
                strncpy(fig_ptr[line_cnt], str.c_str(), PAGE_W);
                fig_ptr[line_cnt][PAGE_W] = '\0';
            }
            line_cnt++;
        }
        *fig_h = line_cnt;
        return fig_ptr;
    }
void FigBook::adjustTextAroundFigure(char** page_cont, char** fig_cont, int& line_cnt, int fig_h, int WIDTH) {
    for (int i = 0; i < fig_h; i++) {
        if (line_cnt < PAGE_H) {
            // 清理行以避免文本重疊
            memset(page_cont[line_cnt], ' ', WIDTH);
            // 插入圖片
            strncpy(page_cont[line_cnt], fig_cont[i], WIDTH);
            page_cont[line_cnt][WIDTH] = '\0';  // 確保字符串結束符
            line_cnt++;
        }
    }
}

//===================================//
//        FormulaBook DEFINITION     //
//===================================//
FormulaBook::FormulaBook(string filename, string title, string author, string category)
    : Book(filename, title, author, category) {}

void FormulaBook::readContent() {
    // 先調用基類的 readContent 方法
    Book::readContent();
    
    // 然後處理每一頁中的算術公式
    for (auto& page : page_vec) {
        char** pageContent = page->getContent();
        for (int i = 0; i < PAGE_H; i++) {
            string line(pageContent[i]);
            size_t pos = line.find("Answer:");
            if (pos != string::npos) {
                size_t eq_pos = line.find("=");
                if (eq_pos != string::npos && eq_pos + 1 < line.length() && line[eq_pos + 1] == '?') {
                    string expression = line.substr(pos + 7, eq_pos - (pos + 7));
                    expression.erase(remove_if(expression.begin(), expression.end(), ::isspace), expression.end());
                    try {
                        double result = evaluateExpression(expression);
                        ostringstream oss;
                        oss << fixed << setprecision(2) << result;
                        line = line.substr(0, eq_pos + 1) + oss.str();
                        // 更新頁面內容
                        strcpy(pageContent[i], line.c_str());
                    } catch (const invalid_argument& e) {
                        cerr << "Invalid expression: " << expression << endl;
                    }
                }
            }
        }
    }
}

double FormulaBook::evaluateExpression(const string& expression) {
    std::istringstream iss(expression);
    std::vector<double> values;
    std::vector<char> ops;
    double val;
    char op;

    while (iss >> val) {
        values.push_back(val);
        if (iss >> op) {
            ops.push_back(op);
        } else {
            break;
        }
    }

    // Handle multiplication and division first
    for (size_t i = 0; i < ops.size(); i++) {
        if (ops[i] == '*' || ops[i] == '/') {
            double left = values[i];
            double right = values[i + 1];
            double result = 0.0;

            if (ops[i] == '*') {
                result = left * right;
            } else {
                if (right == 0) throw std::invalid_argument("Division by zero");
                result = left / right;
            }

            values[i] = result;
            values.erase(values.begin() + i + 1);
            ops.erase(ops.begin() + i);
            i--; // Adjust index to re-evaluate this position
        }
    }

    // Handle addition and subtraction
    while (!ops.empty()) {
        char curr_op = ops.front();
        ops.erase(ops.begin());
        double left = values.front();
        values.erase(values.begin());
        double right = values.front();
        values.erase(values.begin());

        double result = 0.0;
        if (curr_op == '+') {
            result = left + right;
        } else if (curr_op == '-') {
            result = left - right;
        }

        values.insert(values.begin(), result);
    }

    return values.front();
}


//===================================//
//        AnimatedBook DEFINITION    //
//===================================//

AnimatedBook::AnimatedBook(string filename, string title, string author, string category)
    : Book(filename, title, author, category) {}

void AnimatedBook::readContent() {
    Book::readContent();
    playAnimation();
}

void AnimatedBook::playAnimation() {
    for (size_t i = 0; i < page_vec.size(); i++) {
        system("clear");
        cout << "Animating Book: " << title << "\n\n";
        page_vec[i]->showPageCont();
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Adjust the delay as needed
    }
}


//===================================//
//       MorseCodeBook DEFINITION   //
//===================================//

MorseCodeBook::MorseCodeBook(string filename, string title, string author, string category)
    : Book(filename, title, author, category) {
    initializeMorseMap();
}

void MorseCodeBook::initializeMorseMap() {
    morseMap[".-"] = 'A';
    morseMap["-..."] = 'B';
    morseMap["-.-."] = 'C';
    morseMap["-.."] = 'D';
    morseMap["."] = 'E';
    morseMap["..-."] = 'F';
    morseMap["--."] = 'G';
    morseMap["...."] = 'H';
    morseMap[".."] = 'I';
    morseMap[".---"] = 'J';
    morseMap["-.-"] = 'K';
    morseMap[".-.."] = 'L';
    morseMap["--"] = 'M';
    morseMap["-."] = 'N';
    morseMap["---"] = 'O';
    morseMap[".--."] = 'P';
    morseMap["--.-"] = 'Q';
    morseMap[".-."] = 'R';
    morseMap["..."] = 'S';
    morseMap["-"] = 'T';
    morseMap["..-"] = 'U';
    morseMap["...-"] = 'V';
    morseMap[".--"] = 'W';
    morseMap["-..-"] = 'X';
    morseMap["-.--"] = 'Y';
    morseMap["--.."] = 'Z';
    morseMap["-----"] = '0';
    morseMap[".----"] = '1';
    morseMap["..---"] = '2';
    morseMap["...--"] = '3';
    morseMap["....-"] = '4';
    morseMap["....."] = '5';
    morseMap["-...."] = '6';
    morseMap["--..."] = '7';
    morseMap["---.."] = '8';
    morseMap["----."] = '9';
    morseMap[".-.-.-"] = '.';
    morseMap["--..--"] = ',';
    morseMap["..--.."] = '?';
}

void MorseCodeBook::readContent() {
    // 先調用基類的 readContent 方法
    Book::readContent();

    for (auto& page : page_vec) {
        char** pageContent = page->getContent();
        for (int i = 0; i < PAGE_H; i++) {
            std::string decodedContent = decodeMorse(pageContent[i]);
            strcpy(pageContent[i], decodedContent.c_str());
        }
    }
}

string MorseCodeBook::decodeMorse(const string& line) {
    std::stringstream ss(line);
    std::string decodedContent, morseCode;
    while (ss >> morseCode) {
        if (morseCode == "/") {
            decodedContent += ' ';
        } else {
            auto it = morseMap.find(morseCode);
            if (it != morseMap.end()) {
                decodedContent += it->second;
            } else {
                decodedContent += '?';
            }
        }
    }
    return decodedContent;
}