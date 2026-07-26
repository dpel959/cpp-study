#pragma once

// *************************
// 템플릿 프로그래밍으로, h와 cpp를 나누어 작성.
// 
// Insert, Remove 의 파라미터를 Node<T>*로 한 건, 인덱스로 노드를 찾으면 O(1)으로 할 수 없기에 List를 만든 의미가 없기 때문
// *************************

/// 
/// List의 Iterator은 조금 다르다.
/// vector은 그냥 그 데이터를 가리키면 됐지만 (어차피 +, -로 이동이 가능) List는 그렇지가 않다.
/// 그렇다면, Node 자체를 가져 이전, 다음 데이터의 주소를 알아 이동해야할 필요성이 있다.
/// 그러므로 T* _ptr이 아닌 Node<T>* +node를 가진다.
/// 

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

template <typename T>
class ListIterator
{
public:
	ListIterator() : _node(nullptr) {}
	ListIterator(Node<T>* node) : _node(node) {}

	ListIterator& operator++()
	{
		_node = _node->next;
		return *this;
	}

	ListIterator operator++(int)
	{
		ListIterator temp = *this;
		_node = _node->next;
		return temp;
	}

	ListIterator operator + (const int count)
	{
		ListIterator temp = *this;

		for (int i = 0; i < count; ++i)
		{
			temp._node = temp._node->next;
		}

		return temp;
	}

	bool operator == (const ListIterator& other) const
	{
		return _node == other._node;
	}

	bool operator != (const ListIterator& other) const
	{
		return _node != other._node;
	}

	T& operator * ()
	{
		return _node -> data; // 헷갈릴수도 있겠다만, 가리키는 값을 원하는거니까
	}

private:
	Node<T>* _node;
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

	using iterator = ListIterator<T>;

	iterator begin() { return _head; }
	iterator end() { return _tail->next; } // 이거 주의해야한다.

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