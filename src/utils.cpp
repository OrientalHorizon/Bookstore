#include "utils.h"

bool CheckVisible(const std::string &s) {
    int t = s.size();
    for (int i = 0; i < t; ++i) {
        if (!isgraph(s[i])) {
            return false;
        }
    }
    return true;
}
bool CheckUserStr(const std::string &s) {
    int t = s.size();
    if (t > 30 || t == 0) return false;
    for (int i = 0; i < t; ++i) {
        if (!isdigit(s[i]) && !isalpha(s[i]) && (s[i] != '_')) {
            return false;
        }
    }
    return true;
}
bool CheckUsername(const std::string &s) {
    int t = s.size();
    if (t > 30 || t == 0) return false;
    for (int i = 0; i < t; ++i) {
        if (!isgraph(s[i])) {
            return false;
        }
    }
    return true;
}
bool CheckBookStr(const std::string &s) {
    int t = s.size();
    if (t > 60 || t == 0) return false;
    for (int i = 0; i < t; ++i) {
        if (!isgraph(s[i]) || s[i] == '\"') {
            return false;
        }
    }
    return true;
}
bool CheckIsbn(const std::string &s) {
    int t = s.size();
    if (t > 20 || t == 0) return false;
    for (int i = 0; i < t; ++i) {
        if (!isgraph(s[i])) {
            return false;
        }
    }
    return true;
}
bool CheckInt(const std::string &s) {
    int t = s.size();
    if (t > 11 || t == 0) return false;
    for (int i = 0; i < t; ++i) {
        if (!isdigit(s[i])) {
            return false;
        }
    }
    long long tt = stoll(s);
    if (tt > 2147483647ll) return false;
    return true;
}
bool CheckDouble(const std::string &s) {
    int t = s.size();
    if (t > 13 || t == 0) return false;
    try {
        double db = std::stod(s);
    }
    catch (std::exception) {
        return false;
    }
    if (s[0] == '.' || s[t - 1] == '.') {
        throw error("decimal error");
    }
    for (int i = 0; i < t; ++i) {
        if (!isdigit(s[i]) && s[i] != '.') {
            throw error("decimal error");
        }
    }

    return true;
}