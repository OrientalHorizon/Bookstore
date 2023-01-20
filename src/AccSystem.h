#ifndef ACC_SYSTEM
#define ACC_SYSTEM
#include "UnrolledLinkedList.h"
#include "utils.h"

struct UserID_ {
    char s[31];
    UserID_();
    UserID_(const std::string &str);
    inline bool operator==(const UserID_ &y) const;
    inline bool operator<(const UserID_ &y) const;
    inline bool operator<=(const UserID_ &y) const;
};

struct Acc_ {
    int Login_num = 0; // 登录几次
    UserID_ UserID;
    char UserName[31], Password[31];
    int Privilege; // {1, 3, 7}
    Acc_();
    Acc_(const std::string &tmpID, const std::string &tmpName,
         const std::string &tmpPwd, const int &tmpPri);
    void ChangePwd(const std::string &curPwd, const std::string &tmpPwd);
    void ChangePwd(const std::string &tmpPwd);
};

class AccSystem {
    private:
    int n; // 0-based
    UnrolledLinkedList<UserID_> IDIndex;
    std::fstream _file;
    const std::string _filename = "account";
    Acc_ cur_user;
    bool InternalExist(const UserID_ &tmpID);
    void InternalDel(Acc_ &tmpAcc, const int &pos);
    
    public:
    std::stack<int> stk; // 登录栈，记录的是各账户在 account 文件中位置
    std::stack<int> book_stk; // 书栈
    AccSystem();
    ~AccSystem();
    int InternalFind(const UserID_ &tmpID, Acc_ &cur);
    void Su(const std::string &ID, const std::string &pwd); // 登录账户
    void Su(const std::string &ID);
    void Logout();
    void Register(const std::string &ID, const std::string &name, const std::string &pwd);
    void Passwd(const std::string &ID, const std::string &cur_pwd, const std::string &pwd);
    void Passwd(const std::string &ID, const std::string &pwd);
    void Useradd(const std::string &ID, const std::string &name, const std::string &pwd, const int &pri);
    void Init();
    void Delete(const std::string &ID);
    int curUser(Acc_ &x);
    bool isUser();
};

extern AccSystem acc_system;

#endif // ACC_SYSTEM
