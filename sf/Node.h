#pragma once
class Node
{
public:
    Node(int _val, Node* _next);
    int val;
    Node* next;
};