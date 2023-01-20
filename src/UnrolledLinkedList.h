#ifndef UNROLLED_LINKED_LIST
#define UNROLLED_LINKED_LIST
#include <bits/stdc++.h>

template<class keyType>
struct Element {
    keyType str;
    int val;
    Element();
    Element(const keyType &t, const int &v);
};

template<class keyType>
struct Block {
    Element<keyType> mini, maxi;
    int nxt = -1; // 它的下一位是第几个块，-1 表示最后一位
    int size = 0;
    Element<keyType> ele[2001];
};

template<class keyType>
class UnrolledLinkedList {
private:
    const int maxsize = 800, minsize = 400;
    int nowsize = -1;
    // 为每个块预留 801 * sizeof(Element) 的文件空间
    std::fstream _file;
    std::string _filename;
    void rdall(int pos, Block<keyType> &blk);
    void wtall(int pos, const Block<keyType> &blk);
    void rdfirst(int pos, Block<keyType> &blk);
    void split(int pos, Block<keyType> &blk);
    void internal_insert(int pos, const Element<keyType> &ele);
    void combine(int pos, Block<keyType> &blk);
    void internal_delete(int pos, const Element<keyType> &ele);

public:
    UnrolledLinkedList();
    UnrolledLinkedList(std::string name);
    ~UnrolledLinkedList();

    void insert(const Element<keyType> &ele);
    void del(const Element<keyType> &ele);
    void find(const keyType &key, std::vector<int> &vec);
    void findall(std::vector<int> &ret);
};

template<class keyType>
Element<keyType>::Element() = default;
template<class keyType>
Element<keyType>::Element(const keyType &t, const int &v) {
    str = t;
    val = v;
}

template<class keyType>
bool operator==(const Element<keyType> &a, const Element<keyType> &b) {
    if (!(a.str == b.str)) {
        return false;
    }
    return a.val == b.val;
}
template<class keyType>
bool operator<(const Element<keyType> &a, const Element<keyType> &b) {
    if (!(a.str == b.str)) {
        return (a.str < b.str);
    }
    return a.val < b.val;
}
template<class keyType>
bool operator>(const Element<keyType> &a, const Element<keyType> &b) {
    if (!(a.str == b.str)) {
        return !(a.str < b.str);
    }
    return a.val > b.val;
}
template<class keyType>
bool operator<=(const Element<keyType> &a, const Element<keyType> &b) {
    return !(a > b);
}
template<class keyType>
bool operator>=(const Element<keyType> &a, const Element<keyType> &b) {
    return !(a < b);
}

template<class keyType>
void UnrolledLinkedList<keyType>::rdall(int pos, Block<keyType> &blk) {
    if (pos < 0) exit(-1);
    _file.seekg(pos * sizeof(Block<keyType>) + sizeof(int));
    _file.read(reinterpret_cast<char *>(&blk), sizeof(blk));
}

template<class keyType>
void UnrolledLinkedList<keyType>::wtall(int pos, const Block<keyType> &blk) {
    if (pos < 0) exit(-1);
    _file.seekp(pos * sizeof(Block<keyType>) + sizeof(int));
    _file.write(reinterpret_cast<const char *>(&blk), sizeof(blk));
}

template<class keyType>
void UnrolledLinkedList<keyType>::rdfirst(int pos, Block<keyType> &blk) {
    if (pos < 0) exit(-1);
    _file.seekg(pos * sizeof(Block<keyType>) + sizeof(int));
    _file.read(reinterpret_cast<char *>(&(blk.mini)), sizeof(blk.mini));
    _file.read(reinterpret_cast<char *>(&(blk.maxi)), sizeof(blk.maxi));
    _file.read(reinterpret_cast<char *>(&(blk.nxt)), sizeof(blk.nxt));
    _file.read(reinterpret_cast<char *>(&(blk.size)), sizeof(blk.size));
}

template<class keyType>
void UnrolledLinkedList<keyType>::split(int pos, Block<keyType> &blk) { // 裂块
    ++nowsize;
    static Block<keyType> cur;
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

template<class keyType>
void UnrolledLinkedList<keyType>::internal_insert(int pos, const Element<keyType> &ele) {
    // 确定添加到哪个块之后的加入
    static Block<keyType> cur;
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

template<class keyType>
void UnrolledLinkedList<keyType>::combine(int pos, Block<keyType> &blk) { // 并块
    int nx1 = blk.nxt;
    if (nx1 == -1) {
        wtall(pos, blk);
        return;
    }
    static Block<keyType> nx;
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

template<class keyType>
void UnrolledLinkedList<keyType>::internal_delete(int pos, const Element<keyType> &ele) { // 确定在哪个块的删除
    static Block<keyType> cur;
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

template<class keyType>
UnrolledLinkedList<keyType>::UnrolledLinkedList() = default;
template<class keyType>
UnrolledLinkedList<keyType>::UnrolledLinkedList(std::string name) {
    _filename = name;
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
        static Block<keyType> tmp;
        _file.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
    }
    else {
        _file.seekg(0);
        _file.read(reinterpret_cast<char *>(&nowsize), sizeof(nowsize));
    }
}

template<class keyType>
UnrolledLinkedList<keyType>::~UnrolledLinkedList() {
    _file.seekp(0);
    _file.write(reinterpret_cast<char *>(&nowsize), sizeof(nowsize));
    _file.close();
}

template<class keyType>
void UnrolledLinkedList<keyType>::insert(const Element<keyType> &ele) {
    static Block<keyType> cur; rdfirst(0, cur);
    // 默认添加到后侧的块，第一个除外
    if (cur.size == 0 || ele < cur.mini) {
        // 表是空的，或者当前元素过小
        // 插进第一个块中
        internal_insert(0, ele);
        return;
    }
    static Block<keyType> nx; int i = 0;
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

template<class keyType>
void UnrolledLinkedList<keyType>::del(const Element<keyType> &ele) {
    static Block<keyType> cur; int i = 0;
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

template<class keyType>
void UnrolledLinkedList<keyType>::find(const keyType &key, std::vector<int> &vec) {
    static Block<keyType> cur; int i = 0;
    bool find = false;
    vec.clear();
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
                    vec.push_back(cur.ele[j].val);
                }
            }
        }
        else if (key < cur.mini.str) {
            break;
        }
        i = cur.nxt;
    }
}

template<class keyType>
void UnrolledLinkedList<keyType>::findall(std::vector<int> &ret) {
    ret.clear();
    int i = 0; Block<keyType> blk;
    while (i != -1) {
        rdall(i, blk);
        for (int i = 0; i < blk.size; ++i) {
            ret.push_back(blk.ele[i].val);
        }
        i = blk.nxt;
    }
}

#endif // UNROLLED_LINKED_LIST
