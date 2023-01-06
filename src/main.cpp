#include "BookSystem.h"

AccSystem acc_system;
FinLog fin_log;
BookSystem book_system;

signed main() {
    std::string line;
    Acc_ tmpAcc;
    UserID_ tmpID("root");
    int tmp_int = acc_system.InternalFind(tmpID, tmpAcc);
    if (tmp_int == -1) {
        acc_system.Init();
    }
    std::string commands[32];
    while (getline(std::cin, line)) {
        // 一起把指令读进来
        try {
        int cmd_cnt = 0;
        std::stringstream ss;
        ss.clear();
        ss << line;
        std::string buf;
        while (ss >> buf) {
            commands[++cmd_cnt] = buf;
            if (cmd_cnt > 10) throw error();
        }
        if (cmd_cnt == 0) {
            continue;
        }
        if (commands[1] == "exit") {
            if (cmd_cnt > 1) throw error();
            break;
        }
        else if (commands[1] == "quit") {
            if (cmd_cnt > 1) throw error();
            break;
        }
        else if (commands[1] == "su") {
            // 登录
            if (cmd_cnt > 3 || cmd_cnt == 1) throw error();
            if (!CheckUserStr(commands[2])) {
                throw error("Invalid userstr");
            }
            if (cmd_cnt == 2) {
                acc_system.Su(commands[2]);
            }
            else {
                if (!CheckUserStr(commands[3])) {
                    throw error("Invalid userstr");
                }
                acc_system.Su(commands[2], commands[3]);
            }
        }
        else if (commands[1] == "logout") {
            if (cmd_cnt > 1) {
                throw error("Too many args");
            }
            acc_system.Logout();
        }
        else if (commands[1] == "register") {
            if (cmd_cnt != 4) {
                throw error();
            }
            for (int i = 2; i <= 3; ++i) {
                if (!CheckUserStr(commands[i])) {
                    throw error();
                }
            }
            if (!CheckUsername(commands[4])) {
                throw error();
            }
            acc_system.Register(commands[2], commands[4], commands[3]);
        }
        else if (commands[1] == "passwd") {
            if (cmd_cnt == 4) {
                for (int i = 2; i <= 4; ++i)
                    if (!CheckUserStr(commands[i])) {
                        throw error();
                    }
                acc_system.Passwd(commands[2], commands[3], commands[4]);
            }
            else if (cmd_cnt == 3) {
                for (int i = 2; i <= 3; ++i)
                    if (!CheckUserStr(commands[i])) {
                        throw error();
                    }
                acc_system.Passwd(commands[2], commands[3]);
            }
            else throw error("Invalid arg num");
        }
        else if (commands[1] == "useradd") {
            if (cmd_cnt != 5) throw error("Invalid arg num");
            for (int i = 2; i <= 3; ++i)
                if (!CheckUserStr(commands[i])) {
                    throw error();
                }
            if (!CheckInt(commands[4])) {
                throw error();
            }
            if (!CheckUsername(commands[5])) {
                throw error();
            }

            int pri = std::stoi(commands[4]);
            if (pri != 1 && pri != 3) {
                throw error("Wrong priv");
            }
            acc_system.Useradd(commands[2], commands[5], commands[3], pri);
        }
        else if (commands[1] == "delete") {
            if (cmd_cnt != 2) throw error("Invalid arg num");
            if (!CheckUserStr(commands[2])) {
                throw error();
            }
            acc_system.Delete(commands[2]);
        }
        else if (commands[1] == "show") {
            if (cmd_cnt == 1) {
                // show all
                book_system.Show(All, "Fuck bookstore");
            }
            else {
                if (commands[2] == "finance") {
                    if (cmd_cnt >= 4) throw error();
                    if (cmd_cnt == 2) {
                        fin_log.Show();
                    }
                    else {
                        CheckInt(commands[3]);
                        int nn = std::stoi(commands[3]);
                        if (nn == 0) {
                            std::cout << std::endl;
                        }
                        else {
                            fin_log.Show(nn);
                        }
                    }
                }
                else {
                    if (cmd_cnt >= 3) {
                        throw error();
                    }
                    if (commands[2].size() >= 6 && commands[2].substr(0, 6) == "-ISBN=") {
                        std::string str = commands[2].substr(6);
                        if (!CheckIsbn(str)) {
                            throw error();
                        }
                        book_system.Show(ISBN, str);
                    }
                    else if (commands[2].size() >= 7 && commands[2].substr(0, 7) == "-name=\"") {
                        std::string str = commands[2].substr(7);
                        if (str.size() == 0 || str[str.size() - 1] != '\"') {
                            throw error("fanyinhao");
                        }
                        str.pop_back();
                        if (!CheckBookStr(str)) {
                            throw error();
                        }
                        book_system.Show(Name, str);
                    }
                    else if (commands[2].size() >= 9 && commands[2].substr(0, 9) == "-author=\"") {
                        std::string str = commands[2].substr(9);
                        if (str.size() == 0 || str[str.size() - 1] != '\"') {
                            throw error("fanyinhao");
                        }
                        str.pop_back();
                        if (!CheckBookStr(str)) {
                            throw error();
                        }
                        book_system.Show(Author, str);
                    }
                    else if (commands[2].size() >= 10 && commands[2].substr(0, 10) == "-keyword=\"") {
                        std::string str = commands[2].substr(10);
                        if (str.size() == 0 || str[str.size() - 1] != '\"') {
                            throw error("fanyinhao");
                        }
                        str.pop_back();
                        if (!CheckBookStr(str)) {
                            throw error();
                        }
                        if (str.find('|') != std::string::npos) {
                            throw error();
                        }
                        book_system.Show(Keyword, str);
                    }
                    else {
                        throw error("Invalid opt");
                    }
                }
            }
        }
        else if (commands[1] == "buy") {
            if (cmd_cnt != 3) {
                throw error("Invalid arg num");
            }
            if (!CheckIsbn(commands[2]) || !CheckInt(commands[3])) {
                throw error();
            }
            int num = std::stoi(commands[3]);
            book_system.Buy(commands[2], num);
        }
        else if (commands[1] == "select") {
            if (cmd_cnt != 2) {
                throw error("Invalid arg num");
            }
            if (!CheckIsbn(commands[2])) {
                throw error();
            }
            book_system.Select(commands[2]);
        }
        else if (commands[1] == "modify") {
            // 好日子还在后头呢！
            if (cmd_cnt == 1) {
                throw error();
            }
            std::set<Typ> set_typ;
            std::set<std::string> set_str;
            for (int i = 2; i <= cmd_cnt; ++i) {
                if (commands[i].size() >= 6 && commands[i].substr(0, 6) == "-ISBN=") {
                    if (set_typ.find(ISBN) != set_typ.end()) {
                        throw error("Repeated arg");
                    }
                    set_typ.insert(ISBN);
                    std::string str = commands[i].substr(6);
                    if (!CheckIsbn(str)) {
                        throw error();
                    }
                    if (!book_system.InternalCheckIsbn(str)) {
                        throw error("repeated isbn");
                    }
                }
                else if (commands[i].size() >= 7 && commands[i].substr(0, 7) == "-name=\"") {
                    if (set_typ.find(Name) != set_typ.end()) {
                        throw error("Repeated arg");
                    }
                    set_typ.insert(Name);
                    std::string str = commands[i].substr(7);
                    if (str.size() == 0 || str[str.size() - 1] != '\"') {
                        throw error("fanyinhao");
                    }
                    str.pop_back();
                    if (!CheckBookStr(str)) {
                        throw error();
                    }
                }
                else if (commands[i].size() >= 9 && commands[i].substr(0, 9) == "-author=\"") {
                    if (set_typ.find(Author) != set_typ.end()) {
                        throw error("Repeated arg");
                    }
                    set_typ.insert(Author);
                    std::string str = commands[i].substr(9);
                    if (str.size() == 0 || str[str.size() - 1] != '\"') {
                        throw error("fanyinhao");
                    }
                    str.pop_back();
                    if (!CheckBookStr(str)) {
                        throw error();
                    }
                }
                else if (commands[i].size() >= 10 && commands[i].substr(0, 10) == "-keyword=\"") {
                    if (set_typ.find(Keyword) != set_typ.end()) {
                        throw error("Repeated arg");
                    }
                    set_typ.insert(Keyword);
                    std::string str = commands[i].substr(10);
                    if (str.size() == 0 || str[str.size() - 1] != '\"') {
                        throw error("fanyinhao");
                    }
                    str.pop_back();
                    if (!CheckBookStr(str)) {
                        throw error();
                    }
                    int siz = str.size();
                    // 判断 '|'
                    std::string tmp;
                    if (str[0] == '|' || str[siz - 1] == '|') {
                        throw error("|");
                    }
                    for (int i = 0; i < siz; ++i) {
                        if (str[i] == '|') {
                            if (tmp.size() == 0) {
                                throw error("Empty kwd");
                            }
                            if (set_str.find(tmp) != set_str.end()) {
                                throw error("Repeated kwd");
                            }
                            set_str.insert(tmp);
                            tmp.clear();
                        }
                        else {
                            tmp.push_back(str[i]);
                        }
                    }
                    if (tmp.size() == 0) {
                        throw error("Empty kwd");
                    }
                    if (set_str.find(tmp) != set_str.end()) {
                        throw error("Repeated kwd");
                    }
                    set_str.clear();
                    tmp.clear();
                }
                else if (commands[i].size() >= 7 && commands[i].substr(0, 7) == "-price=") {
                    if (set_typ.find(Price) != set_typ.end()) {
                        throw error("Repeated arg");
                    }
                    set_typ.insert(Price);
                    std::string str = commands[i].substr(7);
                    if (!CheckDouble(str)) {
                        throw error();
                    }
                }
                else {
                    throw error("Invalid opt");
                }
            }
            for (int i = 2; i <= cmd_cnt; ++i) {
                book_system.Modify(commands[i]);
            }
        }
        else if (commands[1] == "import") {
            if (cmd_cnt != 3) {
                throw error("Invalid arg num");
            }
            if (!CheckInt(commands[2]) || !CheckDouble(commands[3])) {
                throw error();
            }
            int num = std::stoi(commands[2]);
            double tot = std::stod(commands[3]);
            book_system.Import(num, tot);
        }
        else {
            throw error("Invalid arg type");
        }

        }
        catch(error) {
            std::cout << "Invalid" << std::endl;
        }
        catch(std::exception) {
            std::cout << "Invalid" << std::endl;
        }
    }
    return 0;
}
