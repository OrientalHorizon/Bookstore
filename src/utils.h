#ifndef UTILS
#define UTILS
#include <bits/stdc++.h>

enum Typ {
    All = 0, ISBN = 1, Name = 2, Author = 3, Keyword = 4, Price = 5
};

class error {
    private:
    std::string message;
    
    public:
    error() = default;
    error(const std::string &msg_) {
        message = msg_;
        #ifdef DEBUG
            std::cout << "Error: " << message << std::endl;
        #endif
    }
};

bool CheckVisible(const std::string &s);
bool CheckUserStr(const std::string &s);
bool CheckUsername(const std::string &s);
bool CheckBookStr(const std::string &s);
bool CheckIsbn(const std::string &s);
bool CheckInt(const std::string &s);
bool CheckDouble(const std::string &s);

#endif // UTILS
