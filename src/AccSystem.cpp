#include "AccSystem.h"

UserID_::UserID_() = default;
UserID_::UserID_(const std::string &str) {
    strcpy(s, str.c_str());
}
inline bool UserID_::operator==(const UserID_ &y) const {
    return !strcmp(s, y.s);
}
inline bool UserID_::operator<(const UserID_ &y) const {
    return strcmp(s, y.s) < 0;
}
inline bool UserID_::operator<=(const UserID_ &y) const {
    return ((*this) < y) || ((*this) == y);
}
std::ostream& operator<<(std::ostream &x, const UserID_ &y) {
    x << y.s;
    return x;
}

Acc_::Acc_() = default;
Acc_::Acc_(const std::string &tmpID, const std::string &tmpName,
           const std::string &tmpPwd, const int &tmpPri) : UserID(tmpID) {
    strcpy(UserName, tmpName.c_str());
    strcpy(Password, tmpPwd.c_str());
    Privilege = tmpPri;
    Login_num = 0;
}
void Acc_::ChangePwd(const std::string &curPwd, const std::string &tmpPwd) {
    if ((std::string)Password != curPwd) throw error("Incorrect pwd");
    strcpy(Password, tmpPwd.c_str());
}
void Acc_::ChangePwd(const std::string &tmpPwd) {
    strcpy(Password, tmpPwd.c_str());
}

AccSystem::AccSystem() : IDIndex("account-index") {
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
        static Acc_ tmp;
        _file.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
    }
    else {
        _file.seekg(0);
        _file.read(reinterpret_cast<char *>(&n), sizeof(n));
    }
    while (!stk.empty()) {
        stk.pop();
    }
}
AccSystem::~AccSystem() {
    _file.seekp(0);
    _file.write(reinterpret_cast<char *>(&n), sizeof(n));
    _file.close();
    while (!stk.empty()) {
        stk.pop();
    }
}
int AccSystem::InternalFind(const UserID_ &tmpID, Acc_ &cur) {
    std::vector<int> vec;
    IDIndex.find(tmpID, vec);
    if (vec.empty()) return -1;
    _file.seekg(4 + vec[0] * sizeof(Acc_));
    _file.read(reinterpret_cast<char *>(&cur), sizeof(cur));
    // std::cout << cur.UserID << std::endl;
    return vec[0];
}
bool AccSystem::InternalExist(const UserID_ &tmpID) {
    std::vector<int> vec;
    IDIndex.find(tmpID, vec);
    // if (vec.size()) std::cout << vec[0] << std::endl;
    return !(vec.empty());
}
void AccSystem::Su(const std::string &ID, const std::string &pwd) {
    UserID_ cur_id(ID);
    static Acc_ cur;
    int tmp_int = InternalFind(cur_id, cur);
    if (tmp_int == -1) {
        // 没找到用户
        throw error("No user matched");
    }
    // std::cout << cur.UserID << "\t" << cur.UserName << "\t";
    if (strcmp(cur.Password, pwd.c_str())) {
        // 密码不同
        // std::cout << cur.Password << "\t" << pwd.c_str() << std::endl;
        // std::cout << "wrong pwd" << std::endl;
        throw error("Wrong Password");
    }
    // std::cout << "yes" << std::endl;
    stk.push(tmp_int);
    book_stk.push(-1);
    cur_user = cur;
    cur.Login_num++;
    _file.seekp(tmp_int * sizeof(Acc_) + 4);
    _file.write(reinterpret_cast<char *>(&cur.Login_num), sizeof(int));
}
void AccSystem::Su(const std::string &ID) {
    UserID_ cur_id(ID);
    static Acc_ cur;
    int tmp_int = InternalFind(cur_id, cur);
    if (tmp_int == -1) {
        // 没找到用户
        throw error("No user matched");
    }
    // 当前账户
    if (stk.empty()) throw error();
    if (cur_user.Privilege <= cur.Privilege) {
        // 权限不够
        throw error("No right to log in");
    }
    stk.push(tmp_int);
    book_stk.push(-1);
    cur_user = cur;
    _file.seekp(tmp_int * sizeof(Acc_) + 4);
    _file.write(reinterpret_cast<char *>(&cur.Login_num), sizeof(int));
}
void AccSystem::Logout() {
    if (stk.empty()) throw error("No user is logged in");
    int top = stk.top();
    _file.seekp(4 + top * sizeof(Acc_));
    --cur_user.Login_num;
    _file.write(reinterpret_cast<char *>(&cur_user.Login_num), sizeof(int));
    stk.pop();
    book_stk.pop();
    if (!stk.empty()) {
        top = stk.top();
        _file.seekg(4 + top * sizeof(Acc_));
        _file.read(reinterpret_cast<char *>(&cur_user), sizeof(cur_user));
    }
}
void AccSystem::Useradd(const std::string &ID, const std::string &name, const std::string &pwd, const int &pri) {
    if (stk.empty()) throw error("Not logged in");
    if (cur_user.Privilege < 3) throw error("No right to add user");
    if (pri >= cur_user.Privilege) throw error("No right to add user");
    UserID_ tmp_id(ID);
    // std::cout << tmp_id << std::endl;
    if (InternalExist(tmp_id)) {
        throw error("Repeated User ID");
    }
    Acc_ tmp_acc(ID, name, pwd, pri);
    ++n;
    _file.seekp(4 + n * sizeof(Acc_));
    _file.write(reinterpret_cast<char *>(&tmp_acc), sizeof(Acc_));
    // std::cout << tmp_id << std::endl;
    IDIndex.insert(Element<UserID_>(tmp_id, n));
}
void AccSystem::Register(const std::string &ID, const std::string &name, const std::string &pwd) {
    UserID_ tmp_id(ID);
    if (InternalExist(tmp_id)) {
        throw error("Repeated User ID");
    }
    int pri = 1;
    Acc_ tmp_acc(ID, name, pwd, pri);
    ++n;
    _file.seekp(4 + n * sizeof(Acc_));
    _file.write(reinterpret_cast<char *>(&tmp_acc), sizeof(Acc_));
    IDIndex.insert(Element<UserID_>(tmp_id, n));
}
void AccSystem::InternalDel(Acc_ &tmpAcc, const int &pos) {
    Element<UserID_> ele(tmpAcc.UserID, pos);
    IDIndex.del(ele);
}
void AccSystem::Delete(const std::string &ID) {
    if (stk.empty()) throw error("Not logged in");
    if (cur_user.Privilege < 7) throw error("No right to delete user");
    static Acc_ tmpAcc;
    UserID_ tmpID(ID);
    int tmp_int = InternalFind(tmpID, tmpAcc);
    // std::cout << tmp_int << std::endl;
    if (tmp_int == -1) throw error("No such user");
    if (tmpAcc.Login_num > 0) {
        // 正在登录
        throw error("This account is logged in");
    }
    InternalDel(tmpAcc, tmp_int);
}
void AccSystem::Passwd(const std::string &ID, const std::string &cur_pwd, const std::string &pwd) {
    if (stk.empty()) throw error("No user logged in");
    static Acc_ tmpAcc;
    UserID_ tmpID(ID);
    int tmp_int = InternalFind(tmpID, tmpAcc);
    if (tmp_int == -1) throw error("No such user");
    tmpAcc.ChangePwd(cur_pwd, pwd);
    // 写回去
    _file.seekp(4 + tmp_int * sizeof(Acc_));
    _file.write(reinterpret_cast<char *>(&tmpAcc), sizeof(Acc_));
}
void AccSystem::Passwd(const std::string &ID, const std::string &pwd) {
    if (stk.empty()) throw error("No user logged in");
    if (cur_user.Privilege < 7) throw error("Missing current password");
    static Acc_ tmpAcc;
    UserID_ tmpID(ID);
    int tmp_int = InternalFind(tmpID, tmpAcc);
    if (tmp_int == -1) throw error("No such user");
    tmpAcc.ChangePwd(pwd);
    // 写回去
    _file.seekp(4 + tmp_int * sizeof(Acc_));
    _file.write(reinterpret_cast<char *>(&tmpAcc), sizeof(Acc_));
}
int AccSystem::curUser(Acc_ &x) {
    if (stk.empty()) return -1;
    x = cur_user;
    return stk.top();
}
bool AccSystem::isUser() {
    return !(stk.empty());
}

void AccSystem::Init() {
    UserID_ tmp_id("root");
    Acc_ tmp_acc("root", "root", "sjtu", 7);
    ++n;
    _file.seekp(4 + n * sizeof(Acc_));
    _file.write(reinterpret_cast<char *>(&tmp_acc), sizeof(Acc_));
    // std::cout << tmp_id << std::endl;
    IDIndex.insert(Element<UserID_>(tmp_id, n));
}
