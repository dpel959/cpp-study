#pragma once

// *************************
// 템플릿 프로그래밍으로, h와 cpp를 나누어 작성.
// 
// Insert, Remove 의 파라미터를 Node<T>*로 한 건, 인덱스로 노드를 찾으면 O(1)으로 할 수 없기에 List를 만든 의미가 없기 때문
// *************************

template <typename T>
class Node
{
public:
	Node(const T& data) : data(data), prev(nullptr), next(nullptr) {}

public:
	T data;
	Node<T>* prev;
	Node<T>* next;
};

enum class InsertPos
{
	Front,
	Back
};

template <typename T>
class List
{
public:

	~List();

	void PrintAllNodes();
	Node<T>* GetNodebyIdx(int idx);
	Node<T>* AddAtHead(const T& data);
	Node<T>* AddAtTail(const T& data);
	template<InsertPos Pos>
	Node<T>* Insert(Node<T>* paramNode, const T& data);
	bool Remove(Node<T>* paramNode);
private:
	Node<T>* _head = nullptr;
	Node<T>* _tail = nullptr;
	int _count = 0;
};