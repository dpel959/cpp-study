#include "List.h"
#include <iostream>

int main()
{
    List<int> list;

    Node<int>* removeNode = list.AddAtHead(2); // 이렇게 주소를 저장해야 중간 삽입/삭제 O(1)이 의미가 있음
    Node<int>* insertnode = list.AddAtHead(1); 
    list.AddAtTail(3);
    list.PrintAllNodes();

    list.Remove(removeNode);
    list.PrintAllNodes();

    list.Insert<InsertPos::Back>(insertnode, 2);
    list.PrintAllNodes();

    list.Insert<InsertPos::Front>(insertnode, 0);
    list.PrintAllNodes();
}