#include "BinarySearchTree.h"
#include <iostream>
#include <windows.h>
using std::cout;

void SetCursorPosition(int x, int y)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	::SetConsoleCursorPosition(output, pos);
}

/// 
/// 파라미터에 Node는 필요하다. 재귀를 하기 위해서.
/// 물론 while로 하면 필요 없음.
/// 

void BinarySearchTree::Print(Node* node, int x, int y)
{
	if (node == nullptr)
	{
		return;
	}

	SetCursorPosition(x, y);

	cout << node->key;

	Print(node->left, x - (5 / (y + 1)), y + 1);
	Print(node->right, x + (5 / (y + 1)), y + 1); 
	// 재귀. 전위, 중위, 후위 순회냐에 따라 이 위치가 바뀔 수 있음.
	// 수식은 그냥 윈도우 커서용
}

// 꼭 재귀로 안 해도 된다만, 보통 Tree가 재귀가 편해용. SubTree가 구조를 똑같이 따라가서.

Node* BinarySearchTree::Search(Node* node, int key)
{
	// 못 찾았거나, 찾았거나, 아무튼 할 일이 끝난 건 똑같다.
	if (node == nullptr || key == node->key)
	{
		return node;
	}

	if (key > node->key)
	{
		return Search(node->right, key);
	}
	else
	{
		return Search(node->left, key);
	}

#if 0

	Node* current = _root;

	while (current != nullptr)
	{
		if (key > current->key)
		{
			current = current->right;
		}
		else if (key < current->key)
		{
			current = current->left;
		}
		else
		{
			break;
		}
	}

	return current;
#endif
}

/// 
/// 이건 while이 좀 더 나음.
/// 처음 체크만 해주면 되는 node == nullptr을 재귀로 하면 계속 체크해야해서.
/// 얘는 특정 값을 찾는 게 목표가 아니라 최소를 찾는 거라, 자기 자신만 보면 자기가 nullptr이 될때까지 들어갈 수밖에 없음.
/// 그러면 이제 노드가 무조건 nullptr이 되는데. 이러면 최솟값 node를 찾아줄 수가 없는거임.
/// 그래서 Search와는 다르게 node 자체를 보는 것이 아니라 node->left를 보는 것이 필요한데, 그러면 node와 node->left를 계속 동시에 봐야한다.
/// 사실 매번 볼 것은 node -> left 밖에 없는데. 그래서 이번엔 재귀의 편의성을 포기할 만 하다는 것.
/// 
/// 또, 보통 한 줄기로만 쭉 파고드는 건 while, 전체를 뒤지는 건 재귀가 더 코드를 짜기가 편하다고한다.
/// 전체를 뒤지려면 보통 stack이나 queue의 도움을 받아야하니까.
/// <- 이거 좀 중요하니까 이 감각을 잊지 말자.
/// 
/// (그리고 재귀는 RVO 혜택을 받을... 수가 있나? 만약 된다고 치면 다음 함수의 첫 번째 파라미터가 전 스택의 로컬 변수를 가리키고... 이게 반복될텐데?)
/// 

Node* BinarySearchTree::Min(Node* node)
{
	Node* current = node; // RVO가 동작하도록 여기서부터.
	// node == nullptr에서 node나 nullptr을 리턴하면 뭘 리턴해야하지?? 하고 컴파일러가 RVO를 포기한다.

	if (current == nullptr)
	{
		return current;
	}

	while (current->left != nullptr)
	{
		current = current->left;
	}

	return current;
}

Node* BinarySearchTree::Max(Node* node)
{
	Node* current = node;

	if (current == nullptr)
	{
		return current;
	}

	while (current->right != nullptr)
	{
		current = current->right;
	}

	return current;
}

/// 
/// Node 바로 다음의 값을 찾아라!
/// 이것도 특정 없이 끝까지 파고드는 것이니 자기 자신이 끝까지 파고 들수는 없다. 
/// 그리고 처음에 딱 한번 오른쪽에 자식이 있는지 없는지 판별하는 것도 필요하다.while 써주자.
/// 

Node* BinarySearchTree::Next(Node* node)
{
	Node* current = node;

	if (current == nullptr)
	{
		return current;
	}

	if (current->right == nullptr)
	{
		Node* parent = current->parent;

		// 왜 parent->right면 계속하느냐? = 반대로, 내가 '왼쪽 자식이 될때 끝내면 되어서'이다.
		// 지금 이 작업은 계속 부모를 타고 올라가는 것이다. 내가 왼쪽 자식이 되는 순간, 오른쪽 부모를 가지게 되는 것이고,
		// 그 순간, '그 부모의 왼쪽에 있는 전체 서브트리는 그 부모보다 작다'라는 것이 성립한다.
		// 그러므로 내가 왼쪽 자식이 되는 부모를 찾아 떠나는 것이다.
		while (parent != nullptr && current == parent->right)
		{
			current = parent;
			parent = parent->parent;
		}

		return parent;
	}
	else
	{
		return Min(current->right);
	}
}

void BinarySearchTree::Insert(int key)
{
	Node* newNode = new Node();

	newNode->key = key;

	if (_root == nullptr)
	{
		_root = newNode;
		return;
	}

	// 이런 코드를 생각을 해봤었는데, 그냥 parent 하나 더 이용하는 게 분기문을 훨씬 줄일 수 있어
	// 그쪽이 나은 것 같다.
	
	//Node* current = _root;

	//while (true)
	//{
	//	if (current->key >= key)
	//	{
	//		if (current->left == nullptr)
	//		{
	//			current->left = newNode;
	//		}
	//		else
	//		{
	//			current = current->left;
	//		}
	//	}
	//	else
	//	{
	//		if (current->right == nullptr)
	//		{
	//			current->right = newNode;
	//		}
	//		else
	//		{
	//			current = current->right;
	//		}
	//	}
	//}

	// 위 코드와는 메모리(parent 하나 더) + parent의 매 루프 대입과 if문 수의 트레이드 오프라 할 수 있다.

	Node* current = _root; // current가 실제 돌아주는 애
	Node* parent = nullptr;

	while (current)
	{
		parent = current; // 부모를 저장해준다. current가 nullptr이 되면 멈춰서 부모를 저장해 줄 게 필요
		if (key > current->key) // current는 그 위치에서 key보고 '얘가 니보다 커? 작아?' 하고 물어보는 역할
		{
			current = current->right;
		}
		else
		{
			current = current->left;
		}
	}

	// 그리고 '어디서 왔는지'는 parent가 저장해주지 않기에. 그걸 다시 판별한다.
	// 동작만 보면 위 코드보다 굳이 이걸 한번 더 하는 것이지만, while문에서 도는 if문을 크게 줄일 수 있다.
	// 아니, 분기 예측 있잖아요! 라고 하는데, 이진 트리가 진짜 '지그재그'가 잘 일어날 놈이라서. 지그재그가 분기 예측에 최악이다.

	if (parent->key < key)
	{
		parent->right = newNode;
	}
	else
	{
		parent->left = newNode;
	}

	newNode->parent = parent;

	// (근데 같으면 어디 있어야 하지?)
}

void BinarySearchTree::Delete(int key)
{
	Node* node = Search(_root, key);
	Delete(node);
}

/// 
/// 자식이 몇 개 있는지에 따라 처리가 달라짐.
/// 

void BinarySearchTree::Delete(Node* node)
{
	if (node == nullptr)
	{
		return;
	}

	if (node->left == nullptr)
	{
		// 여기서 양쪽이 비어도, node->right에 nullptr이 들어가서 괜찮음.
		// 그리고 여기서 if를 넣어서 Replace 처리를 안 해준다고 해도, 결국 분기 예측 문제가 다시 생김.
		Replace(node, node->right);
	}
	else if (node->right == nullptr)
	{
		Replace(node, node->left);
	}
	else
	{
		Node* next = Next(node);
		node->key = next->key;
		Delete(next);
	}
}

/// 
/// 삭제용. 정확히는 교체가 아니고 v노드를 u자리에 넣음.
/// 

void BinarySearchTree::Replace(Node* u, Node* v)
{
	// 이거, '그냥 u키를 v키 값으로 복사' 하면 안 되는게, 그러면 v 노드의 자식 관계가 안 이어짐
	if (u->parent == nullptr)
	{
		_root = v;
	}
	else if(u == u->parent->left)
	{
		u->parent->left = v;
	}
	else
	{
		u->parent->right = v;
	}

	// 왜 v == nullptr인걸 마지막에 검사? 처음부터 넣으면 되잖아
	// -> v에 nullptr을 넣고 청소용으로 쓸 때도 있어서. 그래서 마지막에 처리
	if (v != nullptr)
	{
		v->parent = u->parent;
	}

	delete u;
}