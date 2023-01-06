#ifndef UNROLLED_LINKED_LIST
#define UNROLLED_LINKED_LIST
#include "utils.h"

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

#endif // UNROLLED_LINKED_LIST
