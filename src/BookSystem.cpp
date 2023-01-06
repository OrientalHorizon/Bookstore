#include "BookSystem.h"
extern AccSystem acc_system;
extern FinLog fin_log;

ISBN_::ISBN_() {
    s[0] = '\0';
}
ISBN_::ISBN_(const std::string &str) {
    strcpy(s, str.c_str());
}
inline bool ISBN_::operator==(const ISBN_ &y) const {
    return !strcmp(s, y.s);
}
inline bool ISBN_::operator<(const ISBN_ &y) const {
    return strcmp(s, y.s) < 0;
}
inline bool ISBN_::operator<=(const ISBN_ &y) const {
    return ((*this) == y) || ((*this) < y);
}
std::ostream& operator<<(std::ostream &x, const ISBN_ &y) {
    x << y.s;
    return x;
}

myString::myString() {
    s[0] = '\0';
}
myString::myString(const std::string &str) {
    strcpy(s, str.c_str());
}
inline bool myString::operator<(const myString &y) const {
    return (strcmp(s, y.s) < 0);
}
inline bool myString::operator==(const myString &y) const {
    return (!strcmp(s, y.s));
}
inline bool myString::operator<=(const myString &y) const {
    return ((*this) == y) || ((*this) < y);
}
std::ostream& operator<<(std::ostream &x, const myString &y) {
    x << y.s;
    return x;
}

Book_::Book_() = default;
Book_::Book_(const std::string &tmpIsbn) : Isbn(tmpIsbn), Name(), Author(), Keyword() {}
Book_::Book_(const std::string &tmpIsbn, const std::string &tmpName,
             const std::string &tmpAuthor, const std::string &tmpKwd,
             const double &tmpPrice) : Isbn(tmpIsbn), Name(tmpName), Author(tmpAuthor),
             Keyword(tmpKwd), Price(tmpPrice) {}
void Book_::Show() {
    std::cout << std::fixed << std::setprecision(2) << Isbn << "\t" << Name << "\t" << Author << "\t" << Keyword << "\t" << Price << "\t" << Num << std::endl;
}
void Book_::Import(int tmpNum) {
    if (tmpNum <= 0) throw error();
    Num += tmpNum;

}
void Book_::Buy(int tmpNum) {
    if (!tmpNum) {
        throw error("Invalid buy");
    }
    if (tmpNum > Num) {
        throw error("No enough books");
    }
    Num -= tmpNum;
    std::cout << std::fixed << std::setprecision(2) << Price * tmpNum << std::endl;
    fin_log.Add(Price * tmpNum, 0.0);
}
void Book_::ModifyISBN(const std::string &tmpIsbn) {
    if (!strcmp(Isbn.s, tmpIsbn.c_str())) {
        // 一样的 ISBN
        throw error("The same ISBN");
    }
    strcpy(Isbn.s, tmpIsbn.c_str());
}
void Book_::ModifyName(const std::string &tmpName) {
    strcpy(Name.s, tmpName.c_str());
}
void Book_::ModifyAuthor(const std::string &tmpAuthor) {
    strcpy(Author.s, tmpAuthor.c_str());
}
void Book_::ModifyPrice(const double &tmpPrice) {
    Price = tmpPrice;
}
void Book_::ModifyKeyword(const std::string &tmpKeyword) {
    strcpy(Keyword.s, tmpKeyword.c_str());
}
inline bool book_cmp(const Book_ &p, const Book_ &q) {
    return p.Isbn < q.Isbn;
}

BookSystem::BookSystem() : ISBNIndex("isbn-index"), NameIndex("name-index"), AuthorIndex("author-index"), KwdIndex("keyword-index") {
    _file.open(_filename);
    if (!_file) {
        // 创建一个文件
        _file.open(_filename, std::fstream::out);
        _file.close();
        _file.open(_filename);
        n = -1;
        _file.seekp(0);
        _file.write(reinterpret_cast<char *>(&n), sizeof(n));
        _file.seekp(4);
        static Book_ tmp;
        _file.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
    }
    else {
        _file.seekg(0);
        _file.read(reinterpret_cast<char *>(&n), sizeof(n));
    }
}
BookSystem::~BookSystem() {
    _file.seekp(0);
    _file.write(reinterpret_cast<char *>(&n), sizeof(n));
    _file.close();
}

void BookSystem::Show(const Typ &typ, const std::string &str) {
    if (!acc_system.isUser()) {
        throw error("No user logged in");
    }
    static std::vector<int> vec;
    vec.clear();
    static std::vector<Book_> vec1;
    vec1.clear();
    // ISBNIndex.findall(vec);
    Book_ cur_book;
    switch (typ) {
    case All: {
        ISBNIndex.findall(vec);
        for (unsigned i = 0; i < vec.size(); ++i) {
            _file.seekg(4 + vec[i] * sizeof(Book_));
            _file.read(reinterpret_cast<char *>(&cur_book), sizeof(Book_));
            vec1.push_back(cur_book);
        }
    }
    break;

    case ISBN: {
        ISBN_ tmp_isbn(str);
        ISBNIndex.find(tmp_isbn, vec);
        for (unsigned i = 0; i < vec.size(); ++i) {
            _file.seekg(4 + vec[i] * sizeof(Book_));
            _file.read(reinterpret_cast<char *>(&cur_book), sizeof(Book_));
            vec1.push_back(cur_book);
        }
    }
    break;

    case Name: {
        myString mys(str);
        NameIndex.find(str, vec);
        for (unsigned i = 0; i < vec.size(); ++i) {
            _file.seekg(4 + vec[i] * sizeof(Book_));
            _file.read(reinterpret_cast<char *>(&cur_book), sizeof(Book_));
            vec1.push_back(cur_book);
        }
        std::sort(vec1.begin(), vec1.end(), book_cmp);
    }
    break;

    case Author: {
        myString mys(str);
        AuthorIndex.find(str, vec);
        for (unsigned i = 0; i < vec.size(); ++i) {
            _file.seekg(4 + vec[i] * sizeof(Book_));
            _file.read(reinterpret_cast<char *>(&cur_book), sizeof(Book_));
            vec1.push_back(cur_book);
        }
        std::sort(vec1.begin(), vec1.end(), book_cmp);
    }
    break;

    case Keyword: {
        myString mys(str);
        KwdIndex.find(str, vec);
        for (unsigned i = 0; i < vec.size(); ++i) {
            _file.seekg(4 + vec[i] * sizeof(Book_));
            _file.read(reinterpret_cast<char *>(&cur_book), sizeof(Book_));
            vec1.push_back(cur_book);
        }
        std::sort(vec1.begin(), vec1.end(), book_cmp);
    }
    break;

    default:
    throw error();
    break;
    }
    if (vec1.empty()) {
        std::cout << std::endl;
    }
    else {
        for (unsigned i = 0; i < vec1.size(); ++i) {
            vec1[i].Show();
        }
    }
}
void BookSystem::Buy(const std::string &isbn, const int &num) {
    if (!acc_system.isUser()) {
        throw error("No user logged in");
    }
    ISBN_ tmp_isbn(isbn);
    std::vector<int> vec;
    ISBNIndex.find(tmp_isbn, vec);
    if (vec.empty()) {
        throw error("Book not found");
    }
    static Book_ cur_book;
    _file.seekg(4 + vec[0] * sizeof(Book_));
    _file.read(reinterpret_cast<char *>(&cur_book), sizeof(cur_book));
    cur_book.Buy(num);
    _file.seekp(4 + vec[0] * sizeof(Book_));
    _file.write(reinterpret_cast<char *>(&cur_book), sizeof(cur_book));
}
void BookSystem::Select(const std::string &isbn) {
    static Acc_ cur_user;
    int tmp_int = acc_system.curUser(cur_user);
    if (tmp_int == -1 || cur_user.Privilege < 3) {
        throw error();
    }
    std::vector<int> vec;
    ISBN_ tmp_isbn(isbn);
    ISBNIndex.find(tmp_isbn, vec);
    if (vec.empty()) {
        // 创建一本只有 ISBN 的书
        Book_ tmp_book(isbn);
        ++n;
        _file.seekp(4 + n * sizeof(Book_));
        _file.write(reinterpret_cast<char *>(&tmp_book), sizeof(Book_));
        ISBNIndex.insert(Element<ISBN_>(tmp_isbn, n));
        acc_system.book_stk.pop();
        acc_system.book_stk.push(n);
    }
    else {
        acc_system.book_stk.pop();
        acc_system.book_stk.push(vec[0]);
    }
}
void BookSystem::Import(const int &num, const double &tot) {
    static Acc_ cur_user;
    int tmp_int = acc_system.curUser(cur_user);
    if (tmp_int == -1 || cur_user.Privilege < 3) {
        throw error();
    }
    // 如果没有选中的书
    if (acc_system.book_stk.empty() || acc_system.book_stk.top() == -1) {
        throw error();
    }
    if (num == 0 || tot == 0.0) {
        throw error();
    }
    static Book_ tmp_book;
    int book_id = acc_system.book_stk.top();
    _file.seekg(4 + book_id * sizeof(Book_));
    _file.read(reinterpret_cast<char *>(&tmp_book), sizeof(tmp_book));
    tmp_book.Num += num, tmp_book.tot += tot;
    _file.seekp(4 + book_id * sizeof(Book_));
    _file.write(reinterpret_cast<char *>(&tmp_book), sizeof(tmp_book));
    fin_log.Add(0, tot);
}
bool BookSystem::InternalCheckIsbn(const std::string &s) {
    if (acc_system.book_stk.empty() || acc_system.book_stk.top() == -1) {
        return false;
    }
    static Book_ tmp_book;
    int book_id = acc_system.book_stk.top();
    _file.seekg(4 + book_id * sizeof(Book_));
    _file.read(reinterpret_cast<char *>(&tmp_book), sizeof(tmp_book));
    ISBN_ tmp(s);
    // std::cout << tmp_book.Isbn << " " << tmp << std::endl;
    if (tmp_book.Isbn == tmp) {
        return false;
    }
    static std::vector<int> vec;
    vec.clear();
    ISBNIndex.find(tmp, vec);
    if (!vec.empty()) return false;
    return true;
}
void BookSystem::Modify(const std::string &status) {
    // Modify 规则：由 main 把字符串拆开，这里接受单个的
    // -ISBN=... -keyword="" 等等
    static Acc_ cur_user;
    int tmp_int = acc_system.curUser(cur_user);
    if (tmp_int == -1 || cur_user.Privilege < 3) {
        throw error();
    }
    // 如果没有选中的书
    if (acc_system.book_stk.empty() || acc_system.book_stk.top() == -1) {
        throw error();
    }
    static Book_ tmp_book;
    int book_id = acc_system.book_stk.top();
    _file.seekg(4 + book_id * sizeof(Book_));
    _file.read(reinterpret_cast<char *>(&tmp_book), sizeof(tmp_book));
    switch (status[1]) {
    case 'I': {
        // ISBN
        if (status.substr(0, 6) != "-ISBN=") {
            throw error("Invalid argument");
        }
        std::string isbn = status.substr(6);
        ISBN_ tmp_isbn(isbn);
        ISBNIndex.del(Element<ISBN_>(tmp_book.Isbn, book_id));
        tmp_book.ModifyISBN(isbn);
        ISBNIndex.insert(Element<ISBN_>(tmp_isbn, book_id));
    }
    break;

    case 'n': {
        // name
        if (status.substr(0, 6) != "-name=") {
            throw error("Invalid argument");
        }
        std::string name = status.substr(7);
        name.pop_back();
        myString mystr(name);
        NameIndex.del(Element<myString>(tmp_book.Name, book_id));
        tmp_book.ModifyName(name);
        #ifdef DEBUG
        std::cout << tmp_book.Name << std::endl;
        #endif
        NameIndex.insert(Element<myString>(mystr, book_id));
    }
    break;

    case 'a': {
        // author
        if (status.substr(0, 8) != "-author=") {
            throw error("Invalid argument");
        }
        std::string author = status.substr(9);
        author.pop_back();
        myString mystr(author);
        AuthorIndex.del(Element<myString>(tmp_book.Author, book_id));
        tmp_book.ModifyAuthor(author);
        AuthorIndex.insert(Element<myString>(mystr, book_id));
    }
    break;

    case 'p': {
        if (status.substr(0, 7) != "-price=") {
            throw error("Invalid argument");
        }
        std::string str_price = status.substr(7);
        try {
            double price = std::stod(str_price);
            tmp_book.ModifyPrice(price);
        }
        catch (std::exception &ex) {
            throw error("double exception");
        }
    }
    break;

    case 'k': {
        // 它也是用 | 分隔的多个 keyword
        std::set<std::string> set_string;
        if (status.substr(0, 9) != "-keyword=") {
            throw error("Invalid argument");
        }
        std::string new_keywords = status.substr(10);
        new_keywords.pop_back();
        // 接着，我们挨个把原来的 keywords 拿出来删掉
        myString old_keyword = tmp_book.Keyword;
        std::string str_keyword = old_keyword.s;
        std::string keyword;
        for (unsigned i = 0; i < str_keyword.size(); ++i) {
            if (str_keyword[i] == '|') {
                // 现在 keyword 是一个关键词
                KwdIndex.del(Element<myString>(myString(keyword), book_id));
                keyword.clear();
            }
            else {
                keyword.push_back(str_keyword[i]);
            }
        }
        if (keyword.size()) {
            KwdIndex.del(Element<myString>(myString(keyword), book_id));
        }
        // 现在，我们要插入新的 keywords
        if (new_keywords.empty()) {
            throw error();
        }
        if (new_keywords[0] == '|' || new_keywords[new_keywords.size() - 1] == '|')
            throw error("keyword string error");
        keyword.clear();
        for (unsigned i = 0; i < new_keywords.size(); ++i) {
            if (new_keywords[i] == '|') {
                if (keyword.size() == 0 || set_string.find(keyword) != set_string.end()) {
                    throw error();
                }
                set_string.insert(keyword);
                KwdIndex.insert(Element<myString>(myString(keyword), book_id));
                keyword.clear();
            }
            else {
                keyword.push_back(new_keywords[i]);
            }
        }
        if (keyword.size() == 0 || set_string.find(keyword) != set_string.end()) {
            throw error();
        }
        set_string.clear();
        KwdIndex.insert(Element<myString>(myString(keyword), book_id));
        // 现在，我们调整这本书的 keyword
        tmp_book.ModifyKeyword(new_keywords);
    }
    break;

    default:
    throw error("Error argument type");
    break;
    }

    _file.seekp(4 + book_id * sizeof(Book_));
    _file.write(reinterpret_cast<char *>(&tmp_book), sizeof(tmp_book));
}
