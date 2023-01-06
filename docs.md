# Bookstore 开发文档

Written by OrientalHorizon/马逸飞

这是上海交通大学 ACM 班的第五次 lab：书店管理系统。

### 程序功能概述

我们需要实现一个使用命令行交互的书店管理系统，为不同类型的账户（店主、员工、顾客、访客）提供购买书籍、查找书籍、编辑相关信息、图书进货、查看日志等功能。

#### 1. 交互方式

基础任务仅需实现命令行交互。具体的交互命令与规则参考 [作业发布仓库](https://github.com/ACMClassCourse-2022/Bookstore-2022/blob/master/requirements.md#交互方式)。

#### 2. 账户相关

对于一个账户，需要存储、可能会更改（只允许更改部分）的信息主要有：ID、用户名、权限、密码等。

可以实现的操作：注册账户（游客及以上）、创建账户（员工及以上）、登录、修改密码、退出登录（注销）、销号。

#### 3. 图书相关

图书所含的信息有：ISBN (Internal Strange Book Number)、书名、作者、关键词、购买数量、单价。

（细节：图书单价可能改变，后面的单价会覆盖前面的。）

可以实现的操作：检索图书（顾客及以上）、购买图书、选择一本书更改信息、进货。

#### 4. 日志相关

需要实现财务系统查询（收入支出），和生成完整日志两个操作。

#### 5. 错误处理

[作业发布仓库](https://github.com/ACMClassCourse-2022/Bookstore-2022/blob/master/requirements.md) 中几乎每条指令均可能出现不合法操作，我们需要严格判定合法操作，对不合法操作输出 `INVALID`。

### 主体逻辑

在 `main` 函数中逐条读入指令，按顺序操作。

在读入指令之后，先识别指令类型。对于不需查找书籍、账户信息的指令语法错误（例如附加参数为空或不合法）应当立刻抛出并交由错误处理函数处理。

对语法无误的指令，交由对应函数（程序）进行操作，操作过程中判定其他类型的错误。

每一个类型的处理函数都应当对特定的类进行操作，读入硬盘中相应文件添加 / 修改信息，并记录到日志中。

`main` 函数中应当实现一个登录栈，并且将当前账户的信息读入到内存中方便存取。

### 各程序介绍

`Bookstore.h`：头文件，对书店类（整体的一个大类，内含账户类、图书类、日志类等）的变量作定义，并给出接口。

`main.cpp`：处理指令，分配给各个函数作具体处理。以及对登录账户的控制，包括使用一个栈存储登录账户的嵌套情况。

- 内部应有处理指令（主要涉及字符串处理）的函数。还要实现抛出语法错误。

账户操作程序：不涉及图书的操作应当在此处完成。本程序要实现账户类，维护各个账户的信息，从硬盘中存取保存相应信息的文件。具体地，要实现账户的查找、信息变更与删除。

图书操作程序：涉及图书的操作应当在此处完成。本程序要实现图书类，维护每一本图书的信息，从硬盘中存取保存相应信息的文件，并将这些变更记录在 `log` 和 / 或 `financial-log` 中。具体地，应当高效地实现图书的创建、查找、信息变更。

日志操作程序：按照要求处理并输出财务日志和全部日志。读取 `log` 和 / 或 `financial-log` 并输出到 Terminal 中。

错误处理：对每种语法错误与非法操作报错，输出到命令行中。

### 各文件介绍

在根目录下应当有使用说明 `manual.txt`。

（Note: 本栏以下内容为暂定，不排除文件结构和功能有大的变动。）

账户信息存储在 `account` 文件中；通过 ID 到账户具体信息的索引（映射）信息存储在 `account-index` 中。

图书信息存储在 `book` 文件中；通过 ISBN、书名、作者、关键词到图书具体信息的索引（映射）信息分别存储在 `isbn-index`, `name-index`, `author-index`, `keyword-index` 中。

日志分为两类：财务日志 `financial-log` 和全部日志 `total-log`。

### 实现算法

文件存储部分：每一个账户和每一本书的信息都顺序存储在相应的主文件中；块状链表用于将相应信息索引到主文件中的位置。

### 实现细节

- 内部存储应当全部使用 C 风格字符串，以保证信息所占空间固定。处理操作时可以使用 `std::string`。

### 类成员与接口

全局函数，用于检验输入的合法性：

```cpp
bool CheckVisible(const std::string &s);
bool CheckUserStr(const std::string &s); // 用户名之外的字符串
bool CheckUsername(const std::string &s);
bool CheckBookStr(const std::string &s); // ISBN 之外的字符串
bool CheckIsbn(const std::string &s);
bool CheckInt(const std::string &s);
bool CheckDouble(const std::string &s);
```

账户系统类：

```cpp
struct UserID_ { // 用于表示 ID 的结构体
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
    std::stack<int> book_stk; // 书栈，与登录栈保持同步
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
```

图书系统类：

```cpp
struct ISBN_ {
    char s[22];
    ISBN_();
    ISBN_(const std::string &str);
    inline bool operator<(const ISBN_ &y) const;
    inline bool operator==(const ISBN_ &y) const;
    inline bool operator<=(const ISBN_ &y) const;
};

struct myString { // 用于表示除 ISBN 外其他字符串
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
```

日志类：

```cpp
struct opt_ {
    double inc, outc;
    opt_();
    opt_(double income, double outcome);
};

class FinLog {
    private:
    int n = -1;
    std::fstream _file;
    const std::string _filename = "finlog";

    public:
    FinLog();
    ~FinLog();
    void Show(); // 统计全部收入支出
    void Show(int num); // 统计后 num 项收入支出
    void Add(double in, double out); // 记入
};

struct Log {
    char Command[121];
    Acc_ Account;
    
    Log();
    Log(const std::string &tmpCmd, const Acc_ &tmpAcc);
}
class LogSystem {
    private:
    int n, firstAddress, addrStep;
    
    public:
    LogSystem();
    void addLog(const std::string &tmpCmd, const Book &tmpBook, const int &tmpNum, const double &tmpCost);
    void ShowLog();
}
```

