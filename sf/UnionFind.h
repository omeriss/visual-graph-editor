#pragma once
class UnionFind
{
public:
    UnionFind(int len);
    ~UnionFind();
    int Sfind(int x);
    void Sunion(int x, int y);

private:
    int* per, * srank;
};

