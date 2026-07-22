#include "pch.h"
#include "List.h"

// *************************
// 굳이 prevNode를 쓰는 것은, 노드 이동 후 전 삭제 -> 이동 후가 nullptr일 수 있음. 현재 노드 삭제 후 이동 -> 삭제했는데 next 참조를 어떻게 함
// 이런 문제가 있어서 저장하고 이동만 한 후 삭제하는 것.
// *************************

// *************************
// 명시적 템플릿 선언이라는 번거로움이 존재하지만 템플릿 프로그래밍을 cpp를 나눠서 하는 이유는,
// 헤더가 바뀌었을때 재컴파일이 일어나기 때문임. 물론 웬만하면 헤더에 다 적는 경우가 많지만, 게임 엔진 라이브러리 같은 경우는 그렇지 않은 경우도 있기에.
// 물론, 애초에 유연성을 위해 헤더에 몰아 쓰는 경우가 더 많다. 혹은 ipp로 나눠서 헤더 맨 뒤에 include 하던가.
// *************************

template <typename T>
List<T>::~List()
{
	Node<T>* iterator = _head;
	while (iterator != nullptr)
	{
		Node<T>* prevNode = iterator;
		iterator = iterator->next;
		delete prevNode; 
	}
}

template <typename T>
void List<T>::PrintAllNodes()
{
	Node<T>* iterator = _head;
	while (iterator != nullptr)
	{
		cout << iterator->data << ' ';
		iterator = iterator->next;
	}
	cout << '\n';
}

template <typename T>
Node<T>* List<T>::GetNodebyIdx(int idx)
{
	if (idx >= _count || idx < 0)
		return nullptr;

	Node<T>* iterator = _head;
	for (int i = 0; i < idx; ++i)
	{
		iterator = iterator->next;
	}

	return iterator;
}

template <typename T>
Node<T>* List<T>::AddAtHead(const T& data)
{
	Node<T>* newNode = new Node<T>(data);

	if (_head != nullptr)
	{
		newNode->next = _head;
		_head->prev = newNode;
	}
	else
	{
		_tail = newNode;
	}

	_head = newNode;
	++_count;

	return newNode;
}

template <typename T>
Node<T>* List<T>::AddAtTail(const T& data)
{
	Node<T>* newNode = new Node<T>(data);

	if (_tail != nullptr)
	{
		_tail->next = newNode;
		newNode->prev = _tail;
	}
	else
	{
		_head = newNode;
	}
	_tail = newNode;
	++_count;

	return newNode;
}

template <typename T>
template <InsertPos Pos>
Node<T>* List<T>::Insert(Node<T>* paramNode, const T& data)
{
	if (paramNode == nullptr)
		return nullptr;

	Node<T>* newNode = new Node<T>(data);

	if(Pos == InsertPos::Front)
	{
		Node<T>* prevNode = paramNode->prev;

		if (prevNode != nullptr)
		{
			prevNode->next = newNode;
		}
		else
		{
			_head = newNode;
		}
		newNode->prev = prevNode;
		newNode->next = paramNode;
		paramNode->prev = newNode;
	}
	else
	{
		Node<T>* nextNode = paramNode->next;

		if (nextNode != nullptr)
		{
			nextNode->prev = newNode;
		}
		else
		{
			_tail = newNode;
		}
		newNode->next = nextNode;
		newNode->prev = paramNode;
		paramNode->next = newNode;
	}
	++_count;

	return newNode;
}

template <typename T>
bool List<T>::Remove(Node<T>* paramNode)
{
	if (paramNode == nullptr)
		return false;

	Node<T>* prevNode = paramNode->prev;
	Node<T>* nextNode = paramNode->next;

	if (prevNode != nullptr)
	{
		prevNode->next = nextNode;
	}
	else
	{
		_head = nextNode;
	}
	if (nextNode != nullptr)
	{
		nextNode->prev = prevNode;
	}
	else
	{
		_tail = prevNode;
	}

	delete paramNode;

	--_count;

	return true;
}

template class List<int>;
template Node<int>* List<int>::Insert<InsertPos::Front>(Node<int>*, const int&);
template Node<int>* List<int>::Insert<InsertPos::Back>(Node<int>*, const int&);