#include "BookSystem.h"
#include "utils.h"
#include "AccSystem.h"
#include <cmath>


signed main() {
    std::string line;
    // 广泛采用 stringstream 从而解决行内数据不好处理的问题
    // 我们考虑让前一个函数读好下一个命令的开头
    acc_system.Init();
    while (getline(std::cin, line)) {
        static std::stringstream ss;
        ss.clear();
        ss << line;
        std::string buf;
        ss >> buf;
        if (buf == "su") {
            // 登录
            buf = ss.str();
            bool flag = false;
            std::string cur_id;
            std::string cur_pwd;
            for (unsigned i = 0; i < buf.size(); ++i) {
                if (buf[i] == ' ') {
                    flag = true;
                    cur_id = buf.substr(0, i);
                    cur_pwd = buf.substr(i + 1);
                    break;
                }
            }
            if (!flag) {
                // 只有一个参数
                cur_id = buf;
                acc_system.Su(cur_id);
            }
            else {
                acc_system.Su(cur_id, cur_pwd);
            }
        }
        else if (buf == "useradd") {
            std::string userid, pwd, name;
            int pri;
            ss >> userid >> pwd >> pri >> name;
            acc_system.Useradd(userid, name, pwd, pri);
        }
        else if (buf == "logout") {
            acc_system.Logout();
        }
        else if (buf == "register") {
            std::string userid, pwd, name;
            ss >> userid >> pwd >> name;
            acc_system.Register(userid, name, pwd);
        }
        else if (buf == "exit" || buf == "quit") {
            break;
        }
        else if (buf == "passwd") {
            std::string userid, pwd1, pwd2;
            ss >> userid >> pwd1;
            if (ss >> pwd2) {
                acc_system.Passwd(userid, pwd1, pwd2);
            }
            else {
                // 只有一个参数
                acc_system.Passwd(userid, pwd1);
            }
        }
        else if (buf == "delete") {
            std::string userid;
            acc_system.Delete(userid);
        }
        else if (buf == "show") {
            // 可以是 show 也可以是 show finance
            // 先 getchar, 判断有没有附加参数（空格）
            // 要考虑再读进来东西就要影响后面的 buf 了
            // todo:
            // show finance
            std::string aug;
            if (ss >> aug) {
                std::string argument;
                switch (aug[1]) {
                case 'I': {
                    // ISBN
                    argument = aug.substr(6);
                    book_system.Show(ISBN, argument);
                }
                break;

                case 'n': {
                    argument = aug.substr(7);
                    argument.pop_back();
                    book_system.Show(Name, argument);
                }
                break;
                
                case 'a': {
                    argument = aug.substr(9);
                    argument.pop_back();
                    book_system.Show(Author, argument);
                }
                break;
                
                case 'k': {
                    argument = aug.substr(10);
                    argument.pop_back();
                    book_system.Show(Keyword, argument);
                }
                break;

                default:
                throw error();
                break;
                }
            }
            else {
                // show 光棍
                book_system.Show(Any, "Fuck bookstore");
            }
        }
        else if (buf == "buy") {
            // 固定两个参数
            // 购买数量可以不是正整数（是些奇怪的东西）
            std::string isbn;
            double num;
            ss >> isbn;
            if (ss >> num) {
                if (num <= 0.0 || num != std::floor(num)) {
                    throw error();
                }
                book_system.Buy(isbn, (int)num);
            }
            else {
                throw error();
            }
        }
        else if (buf == "select") {
            // 固定一个参数
            std::string isbn;
            ss >> isbn;
            book_system.Select(isbn);
        }
        else if (buf == "modify") {
            // 事大了，还要判重复附加参数 / 空附加参数
            // todo: 重复附加参数
            // 不行，现在就写
            std::set<Typ> set_type;
            std::string aug;
            if (ss >> aug) {
            
            do {
                
            } while (ss >> aug);
            
            }
            else {
                // 空附加参数
                throw error();
            }
        }
        else if (buf == "import") {
            // 固定两个参数
            double a, b;
            if (!(ss >> a)) {
                throw error();
            }
            if (a != std::floor(a)) {
                throw error();
            }
            if (!(ss >> b)) {
                throw error();
            }
            book_system.Import((int)a, b);
        }
    }
    return 0;
}