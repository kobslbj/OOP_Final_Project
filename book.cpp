#include "book.h"

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
    char op;


    readContent();

    while(1){
        system("clear");

        cout << title << endl << endl;

        for(int i=0; i<PAGE_W; i++){
            cout << "=";
        }
        cout << endl << endl;

        page_vec[cur_page]->showPageCont();

        cout << endl;
        for(int i=0; i<PAGE_W/2-4; i++){
            cout << " ";
        }
        cout << "Page " << cur_page << endl;

        cout << endl;
        for(int i=0; i<PAGE_W; i++){
            cout << "=";
        }
        cout << endl;
    
        cout << "########################" << endl;
        cout << "# Next     Page: [ → ] #" << endl;
        cout << "# Previous Page: [ ← ] #" << endl;
        cout << "# Back to Menu : [end] #" << endl;
        cout << "########################" << endl;

        op = getKey();
        if(op == 'N'){
            if(cur_page == page_vec.size()-1){
                continue;
            }
            cur_page++;
        }
        else if(op == 'P'){
            if(cur_page == 0){
                continue;
            }
            cur_page--;
        }
        else if(op == 'M'){
            for(auto& p: page_vec) delete p;
            page_vec.clear();
            // for(auto& p: page_vec) delete p;
            break;
        }  

    }

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
FigBook::FigBook(string filename, string title, string author, string category) : Book::Book(filename, title, author, category) {};

void FigBook::readContent(){
    int WIDTH = PAGE_W;
    int HEIGHT = PAGE_H;
    
    string path = "./TXT/";
    fstream fin;

    string str;

    fin.open(path + filename, ios::in);
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
    int fig_h;
    char** page_cont = new char*[HEIGHT];
    char** fig_cont;
    bool get_new_page = true;
    bool get_new_line = true;
    bool write_fig    = false;
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

        if(str.length() > WIDTH){
            for(int i=0; i<WIDTH-1; i++){
                page_cont[line_cnt][i] = str[i];
            }
            page_cont[line_cnt][WIDTH-1] = '\0';
            str.erase(0, WIDTH);
        }
        else{
            // To prevent putting endline at the beginning of the page
            if(line_cnt==0 && str.length()==0){
                get_new_line = true;
                continue;
            }


            if(str.find(".fig") != string::npos){
                cout << "Read Figure ..." << endl;
                fig_cont = get_figure(fin, &fig_h);
                
                if (line_cnt + fig_h >= HEIGHT){
                    get_new_page = true;
                    get_new_line = false;
                    write_fig = true;
                    str = "";
                    continue;
                }
                else if(fig_h == 0) {
                    get_new_line = true;
                    continue;
                }
                else{
                    write_fig = true;
                }
            }
            else{
                int i;
                for(i=0; i<str.length(); i++){
                    page_cont[line_cnt][i] = str[i];
                }
                if(i < WIDTH){
                    page_cont[line_cnt][i] = '\0';
                }
                get_new_line = true;
            }

            if(write_fig){
                cout << "Write Figure ..." << endl;
                for(int i=0; i<fig_h; i++){
                    for(int j=0; j<WIDTH; j++){
                        page_cont[line_cnt+i][j] = fig_cont[i][j];
                    }
                }
                line_cnt = line_cnt+fig_h-1;

                write_fig = false;
                get_new_line = true;
            }
        }
        
        if(line_cnt == HEIGHT-1){
            get_new_page = true;
        }
        else{
            line_cnt++;
        }
    }
}

char** FigBook::get_figure(fstream& fin, int* fig_h){
    int i;
    int line_cnt = 0;
    string str;
    char** fig_ptr = new char* [PAGE_H];

    for(int i=0; i<PAGE_H; i++){
        *(fig_ptr + i) = new char [PAGE_W];                 
    }

    while(1){
        getline(fin, str);
        if(str.find(".figend") != string::npos){
            *fig_h = line_cnt;
            break;
        }

        const auto find = str.find("\r");

        if(find != string::npos){
            str.erase(find, 1);
        }

        for(i=0; i<str.length(); i++){
            fig_ptr[line_cnt][i] = str[i];
        }
        fig_ptr[line_cnt][i] = '\0';
        line_cnt++;
    }
    return fig_ptr;
}