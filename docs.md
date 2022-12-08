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

全局函数：

```cpp
int StringtoInt(std::string str); // 将字符串转化为正整数，如果转化不成功返回 -1
double StringtoDouble(std::string str); // 将字符串转化为精确到百分位的正实数，不符合要求返回 -1
```

书店类：

```cpp
class Bookstore {
    private:
    BookSystem book_system;
    AccSystem acc_system;
    FinLogSystem fin_log;
    LogSystem log_system;
    
    public:
    Bookstore(); // 构造函数
    void Init();
    void Su(const std::string &ID, const std::string &pwd); // 登录账户
    void Logout(); // 注销
    void Register(const std::string &ID, const std::string &pwd, const std::string &name); // 注册
    void Passwd(const std::string &ID, const std::string &cur_pwd, const std::string &pwd); // 修改密码
    void Useradd(const std::string &ID, const std::string &pwd, const std::string &pri, const std::string &name); // 创建账户
    void Delete(const std::string &ID); // 删除账户
    
    void Show(const std::string &idx); // 检索图书
    void Buy(const std::string &isbn, const std::string &num); // 购买图书，其中 num 应当可以转为正整数
    void Select(const std::string &isbn); // 选定图书
    void Modify(const std::string &status); // 调整信息
    void Import(const std::string &num, const std::string &tot); // 进货，其中 num 应当可以转为正整数，tot 应该可以转换为 double
    
    void AddFinLog();
    void AddLog();
    void ShowFinLog(const int &tot); // 财务日志
    void ShowLog(); // 日志
};
```

账户系统类：

```cpp
struct UserID_ {
    char s[31];
    UserID_();
    UserID_(const std::string &s);
    inline bool operator==(const UserID_ &y);
    inline bool operator<(const UserID_ &y);
};
struct Acc_ {
    UserID_ UserID;
    char UserName[31], Password[31];
    int Privilege; // {1, 3, 7}
    Acc_();
    Acc_(const std::string tmpID, const std::string tmpName,
         const std::string tmpPwd, const std::string tmpPri);
    void ChangePwd(std::string curPwd, std::string tmpPwd);
};
class AccSystem {
    private:
    int n, firstAddress, addrStep;
    ULList<UserID_, int> AccList;
    void InternalAdd(const Acc_ &tmpAcc); // 将信息存入文件系统中，并非注册用户
    bool InternalExist(UserID_ tmpID);
    void InternalFind(UserID_ tmpID);
    void InternalDel(Acc_ tmpAcc);
    
    public:
    AccSystem();
    
    void Su(const std::string &ID, const std::string &pwd); // 登录账户
    void Register(const std::string &ID, const std::string &pwd, const std::string &name);
    void Passwd(const std::string &ID, const std::string &cur_pwd, const std::string &pwd);
    void Useradd(const std::string &ID, const std::string &pwd, const std::string &pri, const std::string &name);
    void Delete(const std::string &ID);
    Acc_ Find(const std::string &ID);
    Acc_ Find(const UserID_ &ID);
    void ChangePwd(std::string curPwd, std::string tmpPwd);
};
```

图书系统类：

```cpp
struct ISBN_ {
    char[21] ISBN;
    ISBN_(const std::string &tmpISBN);
    ISBN_ &operator=(const ISBN_ &y);
    inline bool operator<(const ISBN_ &y);
}
struct Name_ {
    char[61] Name;
    Name_(const std::string &tmpName);
    ISBN_ &operator=(const Name_ &y);
    inline bool operator<(const Name_ &y);
}
struct Author_ {
    char[61] Author;
    Author_(const std::string &tmpAuthor);
    ISBN_ &operator=(const Author_ &y);
    inline bool operator<(const Author_ &y);
}
struct Keyword_ {
    char[61] Keyword;
    Keyword_(const std::string &tmpKeyword);
    ISBN_ &operator=(const Keyword_ &y);
    inline bool operator<(const Keyword_ &y);
}

struct Book_ {
    ISBN_ Isbn;
    Name_ Name;
    Author_ Author;
    int KWNum;
    Keyword_[KWnum] Keyword;
    int Num;
    double Price;
    Book();
    Book(const std::string &tmpIsbn);
    void Show();
    void Import(int tmpNum);
    void Buy(int tmpNum);
    void ModifyISBN(const std::string &tmpIsbn);
    void ModifyName(const std::string &tmpName);
    void ModifyAuthor(const std::string &tmpAuthor);
    void ModifyKeyword(const vector<std::string> &tmpKeyword);
    void ModifyPrice(const double &tmpPrice);
}

class BookSystem {
   	private:
    int n, firstAddress, addrStep;
    ULList<ISBN_, int> ISBNIndex;
    ULList<Name_, int> NameIndex;
    ULList<Author_, int> AuthorIndex;
    ULList<Keyword_, int> KeywordIndex;
    
    public:
    BookSystem();
    void Show(const std::string &idx);
    void Buy(const std::string &isbn, const std::string &num);
    void Select(const std::string &isbn);
    void Modify(const std::string &status);
    void Import(const std::string &num, const std::string &tot);
}
```

日志类：

```cpp
struct FinLog {
    char Command[21];
    Book_ Book;
    int Num;
    double Cost;
    
    FinLog();
    FinLog(const std::string &tmpCmd, const Book &tmpBook, const int &tmpNum, const double &tmpCost);
}

class FinLogSystem {
    private:
    int n, firstAddress, addrStep;
    
    public:
    FinLogSystem();
    void addFinLog(const std::string &tmpCmd, const Book &tmpBook, const int &tmpNum, const double &tmpCost);
    void ShowFinLog(int cnt);
}

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

