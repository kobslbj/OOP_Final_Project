#include "user.h"
#include <algorithm>

// Function to borrow a book
void User::borrowBook(std::string bookID, std::string date) {
    BorrowRecord newRecord;
    newRecord.bookID = bookID;
    newRecord.borrowDate = date;
    newRecord.returnDate = "";  // Initially empty since the book is not yet returned
    borrowingHistory.push_back(newRecord);
}

// Function to return a book
void User::returnBook(std::string bookID, std::string date) {
    auto it = std::find_if(borrowingHistory.begin(), borrowingHistory.end(),
                           [&bookID](const BorrowRecord& record) {
                               return record.bookID == bookID && record.returnDate.empty();
                           });
    
    if (it != borrowingHistory.end()) {
        it->returnDate = date;  // Set the return date for the book
    }
}
