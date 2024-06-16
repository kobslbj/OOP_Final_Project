#ifndef _USER_H_
#define _USER_H_

#include <string>
#include <vector>

struct BorrowRecord {
    std::string bookID;
    std::string borrowDate;
    std::string returnDate;
};

class User {
public:
    std::string userID;
    std::string name;
    std::vector<BorrowRecord> borrowingHistory;

    void borrowBook(std::string bookID, std::string date);
    void returnBook(std::string bookID, std::string date);
};

#endif
