#ifndef BOOK_SYSTEM
#define BOOK_SYSTEM
#include "LogSystem.h"
#include "AccSystem.h"


struct ISBN_ {
    char s[22];
    ISBN_();
    ISBN_(const std::string &str);
    inline bool operator<(const ISBN_ &y) const;
    inline bool operator==(const ISBN_ &y) const;
    inline bool operator<=(const ISBN_ &y) const;
};

struct myString {
    char s[62];
    myString();
    myString(const std::string &str);
    inline bool operator<(const myString &y) const;
    inline bool operator==(const myString &y) const;
    inline bool operator<=(const myString &y) const;
};

struct Book_ {
    ISBN_ Isbn;
    myString Name;
    myString Author;
    myString Keyword;
    int Num = 0;
    double Price = 0, tot = 0;
    Book_();
    Book_(const std::string &tmpIsbn);
    Book_(const std::string &tmpIsbn, const std::string &tmpName,
          const std::string &tmpAuthor, const std::string &tmpKwd, const double &tmpPrice);
    void Show();
    void Import(int tmpNum);
    void Buy(int tmpNum);
    void ModifyISBN(const std::string &tmpIsbn);
    void ModifyName(const std::string &tmpName);
    void ModifyAuthor(const std::string &tmpAuthor);
    void ModifyKeyword(const std::string &tmpKeyword);
    void ModifyPrice(const double &tmpPrice);
};

class BookSystem {
    private:
    int n; // 0-based
    UnrolledLinkedList<ISBN_> ISBNIndex;
    UnrolledLinkedList<myString> NameIndex;
    UnrolledLinkedList<myString> AuthorIndex;
    UnrolledLinkedList<myString> KwdIndex;
    std::fstream _file;
    const std::string _filename = "book";
    
    public:
    BookSystem();
    ~BookSystem();
    void Show(const Typ &typ, const std::string &str);
    void Buy(const std::string &isbn, const int &num);
    void Select(const std::string &isbn);
    void Modify(const std::string &status);
    void Import(const int &num, const double &tot);
    bool InternalCheckIsbn(const std::string &s);
};

extern BookSystem book_system;

#endif // BOOK_SYSTEM
