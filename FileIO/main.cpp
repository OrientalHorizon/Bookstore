#include <bits/stdc++.h>
// Modified on 2022/12/17

class myString {
private:
    char s[68];
public:
    myString() = default;
    myString(const char* t) {
        int len = strlen(t);
        if (len > 64) exit(-1);
        for (int i = 0; i < len; ++i) {
            s[i] = t[i];
        }
        s[len] = '\0';
    }
    ~myString() = default;
    friend bool operator<(const myString &x, const myString &y);
    friend bool operator==(const myString &x, const myString &y);
    friend std::ostream& operator<<(std::ostream &x, const myString &y);
};
bool operator<(const myString &x, const myString &y) {
    return (strcmp(x.s, y.s) < 0);
}
bool operator==(const myString &x, const myString &y) {
    return (!strcmp(x.s, y.s));
}
bool operator<=(const myString &x, const myString &y) {
    return (x < y) || (x == y);
}
std::ostream& operator<<(std::ostream &x, const myString &y) {
    x << y.s;
    return x;
}

struct Element {
    myString str;
    int val;
    Element() = default;
    Element(const myString &t, const int &v) {
        str = t;
        val = v;
    }
};
bool operator==(const Element &a, const Element &b) {
    if (!(a.str == b.str)) {
        return false;
    }
    return a.val == b.val;
}
bool operator<(const Element &a, const Element &b) {
    if (!(a.str == b.str)) {
        return (a.str < b.str);
    }
    return a.val < b.val;
}
bool operator>(const Element &a, const Element &b) {
    if (!(a.str == b.str)) {
        return !(a.str < b.str);
    }
    return a.val > b.val;
}
bool operator<=(const Element &a, const Element &b) {
    return !(a > b);
}
bool operator>=(const Element &a, const Element &b) {
    return !(a < b);
}

struct Block {
    Element mini, maxi;
    int nxt = -1; // 它的下一位是第几个块，-1 表示最后一位
    int size = 0;
    Element ele[2001];
};
class UnrolledLinkedList {
private:
    const int maxsize = 2000, minsize = 1000;
    int nowsize = -1;
    // 为每个块预留 2001 * sizeof(Element) 的文件空间
    // 然后不停做操作就可以了，每次可以把一个块读进内存
    std::fstream _file;
    const std::string _filename = "ull";
    void rdall(int pos, Block &blk) {
        if (pos < 0) exit(-1);
        _file.seekg(pos * sizeof(Block) + sizeof(int));
        _file.read(reinterpret_cast<char *>(&blk), sizeof(blk));
    }
    void wtall(int pos, const Block &blk) {
        if (pos < 0) exit(-1);
        _file.seekp(pos * sizeof(Block) + sizeof(int));
        _file.write(reinterpret_cast<const char *>(&blk), sizeof(blk));
    }
    void rdfirst(int pos, Block &blk) {
        if (pos < 0) exit(-1);
        _file.seekg(pos * sizeof(Block) + sizeof(int));
        _file.read(reinterpret_cast<char *>(&(blk.mini)), sizeof(blk.mini));
        _file.read(reinterpret_cast<char *>(&(blk.maxi)), sizeof(blk.maxi));
        _file.read(reinterpret_cast<char *>(&(blk.nxt)), sizeof(blk.nxt));
        _file.read(reinterpret_cast<char *>(&(blk.size)), sizeof(blk.size));
    }
    void split(int pos, Block &blk) { // 裂块
        ++nowsize;
        static Block cur;
        for (int i = minsize; i < blk.size; ++i) {
            cur.ele[i - minsize] = blk.ele[i];
        }
        cur.size = minsize + 1;
        cur.nxt = blk.nxt;
        blk.nxt = nowsize;
        blk.size = minsize;
        blk.mini = blk.ele[0];
        blk.maxi = blk.ele[blk.size - 1];
        cur.mini = cur.ele[0];
        cur.maxi = cur.ele[cur.size - 1];
        wtall(pos, blk);
        wtall(nowsize, cur);
    }
    void internal_insert(int pos, const Element &ele) { // 确定添加到哪个块之后的加入
        static Block cur;
        if (nowsize == -1) {
            // 块状链表是空的
            ++nowsize;
            cur.size = 1;
            cur.mini = cur.maxi = ele;
            cur.ele[0] = ele;
            wtall(nowsize, cur);
            return;
        }
        rdall(pos, cur);
        int tmp = -1;
        for (int i = 0; i < cur.size; ++i) {
            if (ele < cur.ele[i]) {
                tmp = i;
                break;
            }
            else if (ele == cur.ele[i]) {
                return;
            }
        }
        if (tmp == -1) {
            // 插入最后面
            ++cur.size;
            cur.ele[cur.size - 1] = ele;
        }
        else {
            // 插入中间
            ++cur.size;
            for (int i = cur.size - 1; i > tmp; --i) {
                cur.ele[i] = cur.ele[i - 1];
            }
            cur.ele[tmp] = ele;
        }
        if (cur.size > maxsize) {
            split(pos, cur);
        }
        else {
            cur.mini = cur.ele[0];
            cur.maxi = cur.ele[cur.size - 1];
            wtall(pos, cur);
        }
    }
    void combine(int pos, Block &blk) { // 并块
        int nx1 = blk.nxt;
        if (nx1 == -1) {
            wtall(pos, blk);
            return;
        }
        static Block nx;
        rdall(nx1, nx);
        if (blk.size + nx.size <= maxsize) {
            // 并块 todo
            for (int i = blk.size; i < blk.size + nx.size; ++i) {
                blk.ele[i] = nx.ele[i - blk.size];
            }
            blk.size += nx.size;
            blk.nxt = nx.nxt;
            blk.mini = blk.ele[0];
            blk.maxi = blk.ele[blk.size - 1];
        }
        wtall(pos, blk);
    }
    void internal_delete(int pos, const Element &ele) { // 确定在哪个块的删除
        static Block cur;
        rdall(pos, cur);
        for (int i = 0; i < cur.size; ++i) {
            if (cur.ele[i] > ele) break;
            if (cur.ele[i] == ele) {
                // 找到对应元素，删了
                for (int j = i + 1; j < cur.size; ++j) {
                    cur.ele[j - 1] = cur.ele[j];
                }
                --cur.size;
                if (!cur.size) {
                    wtall(pos, cur);
                    break;
                }
                cur.mini = cur.ele[0];
                cur.maxi = cur.ele[cur.size - 1];
                if (cur.size < minsize) {
                    // 并块
                    combine(pos, cur);
                }
                else {
                    wtall(pos, cur);
                }
                break;
            }
        }
    }

public:
    UnrolledLinkedList() {
        _file.open(_filename);
        if (!_file) {
            // 文件不存在，创建新文件
            _file.open(_filename, std::fstream::out);
            _file.close();
            _file.open(_filename);
            nowsize = -1;
            _file.seekp(0);
            _file.write(reinterpret_cast<char *>(&nowsize), sizeof(nowsize));
            _file.seekp(4);
            static Block tmp;
            _file.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
        }
        else {
            _file.seekg(0);
            _file.read(reinterpret_cast<char *>(&nowsize), sizeof(nowsize));
        }
    }
    ~UnrolledLinkedList() {
        _file.seekp(0);
        _file.write(reinterpret_cast<char *>(&nowsize), sizeof(nowsize));
        _file.close();
    }

    void insert(const Element &ele) {
        static Block cur; rdfirst(0, cur);
        // 默认添加到后侧的块，第一个除外
        if (cur.size == 0 || ele < cur.mini) {
            // 表是空的，或者当前元素过小
            // 插进第一个块中
            internal_insert(0, ele);
            return;
        }
        static Block nx; int i = 0;
        while (true) {
            if (cur.nxt == -1) {
                break;
            }
            rdfirst(cur.nxt, nx);
            if (ele >= cur.mini && ele <= nx.mini) {
                // 插入这里
                if (ele == nx.mini) return;
                internal_insert(i, ele);
                return;
            }
            i = cur.nxt;
            cur = nx;
        }
        // 还没插入好，插入最后一个块
        internal_insert(i, ele);
    }
    void del(const Element &ele) {
        static Block cur; int i = 0;
        while (i != -1) {
            rdfirst(i, cur);
            if (cur.size == 0) {
                i = cur.nxt;
                continue;
            }
            if (cur.mini <= ele && cur.maxi >= ele) {
                internal_delete(i, ele);
                break;
            }
            else if (ele < cur.mini) {
                break;
                // 没找到
            }
            i = cur.nxt;
        }
    }
    void find(const myString &key) {
        static Block cur; int i = 0;
        bool find = false;
        while (i != -1) {
            rdfirst(i, cur);
            if (cur.size == 0) {
                i = cur.nxt;
                continue;
            }
            if ((cur.mini.str <= key) && (key <= cur.maxi.str)) {
                // 读进来
                rdall(i, cur);
                for (int j = 0; j < cur.size; ++j) {
                    if (cur.ele[j].str == key) {
                        find = true;
                        std::cout << cur.ele[j].val << " ";
                    }
                }
            }
            else if (key < cur.mini.str) {
                break;
            }
            i = cur.nxt;
        }
        if (!find) {
            std::cout << "null\n";
        }
        else {
            std::cout << "\n";
        }
    }
    // void findall() {
    //     int i = 0; Block blk;
    //     std::cout << "duck";
    //     while (i != -1) {
    //         rdall(i, blk);
    //         for (int i = 0; i < blk.size; ++i) {
    //             std::cout << blk.ele[i].str << " " << blk.ele[i].val << std::endl;
    //         }
    //         i = blk.nxt;
    //         std::cout << std::endl;
    //     }
    // } 调试用
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr); std::cout.tie(nullptr);
    UnrolledLinkedList ull;
    int T; scanf("%d", &T);
    for (; T; --T) {
        char opt[68]; int val;
        scanf("%s", opt);
        if (!strcmp(opt, "insert")) {
            scanf("%s", opt);
            scanf("%d", &val);
            myString tmp(opt);
            Element t(tmp, val);
            ull.insert(t);
            // ull.findall();
        }
        else if (!strcmp(opt, "delete")) {
            scanf("%s", opt);
            scanf("%d", &val);
            myString tmp(opt);
            Element t(tmp, val);
            ull.del(t);
        }
        else if (!strcmp(opt, "find")) {
            scanf("%s", opt);
            myString tmp(opt);
            ull.find(tmp);
            // ull.findall();
        }
    }
    return 0;
}