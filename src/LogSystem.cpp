#include "LogSystem.h"

FinLog fin_log;

opt_::opt_() = default;
opt_::opt_(double p, double q) {
    inc = p, outc = q;
}

FinLog::FinLog() {
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
        static opt_ tmp;
        _file.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
    }
    else {
        _file.seekg(0);
        _file.read(reinterpret_cast<char *>(&n), sizeof(n));
    }
}

FinLog::~FinLog() {
    _file.seekp(0);
    _file.write(reinterpret_cast <char *> (&n), sizeof(n));
    _file.close();
}

void FinLog::Add(double in, double out) {
    ++n;
    _file.seekp(4 + n * sizeof(opt_));
    opt_ tmp_op(in, out);
    _file.write(reinterpret_cast<char *>(&tmp_op), sizeof(tmp_op));
}
void FinLog::Show() {
    // 统计所有的收入支出
    static Acc_ cur_user;
    int tmp_int = acc_system.curUser(cur_user);
    if (tmp_int == -1 || cur_user.Privilege < 7) {
        throw error();
    }
    double totin = 0, totout = 0;
    for (int i = 0; i <= n; ++i) {
        _file.seekg(4 + i * sizeof(opt_));
        static opt_ tmp_op;
        _file.read(reinterpret_cast<char *>(&tmp_op), sizeof(tmp_op));
        totin += tmp_op.inc;
        totout += tmp_op.outc;
    }
    std::cout << std::fixed << std::setprecision(2) << "+ " << totin << " - " << totout << std::endl;
}
void FinLog::Show(int num) {
    static Acc_ cur_user;
    int tmp_int = acc_system.curUser(cur_user);
    if (tmp_int == -1 || cur_user.Privilege < 7) {
        throw error();
    }
    if (n + 1 < num) {
        throw error("Not enough logs");
    }
    double totin = 0, totout = 0;
    for (int j = 0; j < num; ++j) {
        int i = n - j;
        _file.seekg(4 + i * sizeof(opt_));
        static opt_ tmp_op;
        _file.read(reinterpret_cast<char *>(&tmp_op), sizeof(tmp_op));
        totin += tmp_op.inc;
        totout += tmp_op.outc;
    }
    std::cout << std::fixed << std::setprecision(2) << "+ " << totin << " - " << totout << std::endl;
}
