#ifndef UTILS
#define UTILS
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
enum Typ {
    Any = 0, ISBN = 1, Name = 2, Author = 3, Keyword = 4, Price = 5
    // Price 是单价
};
class error {
    private:
    std::string message;
    
    public:
    error() = default;
    error(const std::string &msg_) {
        message = msg_;
    }
};

#endif // UTILS