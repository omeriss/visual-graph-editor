#include "UnionFind.h"

UnionFind::UnionFind(int len)
{
    per = new int[len];
    srank = new int[len];
    for (int i = 0; i < len; i++) {
        per[i] = i;
        srank[i] = 0;
    }
}

UnionFind::~UnionFind()
{
    delete per;
    delete srank;
}

int UnionFind::Sfind(int x)
{
    if (x == per[x])
        return x;
    return per[x] = Sfind(per[x]);
}

void UnionFind::Sunion(int x, int y)
{
    x = Sfind(x);
    y = Sfind(y);
    if (x != y) {
        if (srank[x] < srank[y]) {
            per[x] = y;
            if (srank[x] == srank[y])
                srank[y]++;
            return;
        }
        per[y] = x;
        if (srank[x] == srank[y])
            srank[x]++;
    }
}
