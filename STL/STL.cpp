#include<iostream>
#include<vector>
#include<list>
#include<map>
#include<unordered_map>
#include"BinarySearchTree.h"
using std::cout, std::vector, std::list, std::map, std::unordered_map;

#pragma region FuncPointer
#if 0
void Print()
{
	cout << "hello\n";
}

int Add(int a, int b)
{
	return a + b;
}

using addFuncPtrType = int(*)(int a, int b);

int DosomeThing(int a, int b, addFuncPtrType func)
{
	return func(a, b);
}

int main()
{
	//-----

	using voidFuncType = void(); // 함수 타입

	voidFuncType* ptr = &Print; // Print로도 가능. 근데 이건 C 레거시래

	ptr(); // 함수 포인터() 를 해서 그 함수를 호출할 수 있다.

	//-----

	using addFuncType = int(int, int); // 여러가지 시그니처도 가능하다.

	addFuncType* addPtr = &Add;

	cout << addPtr(1, 2) << '\n';

	//-----

	//void(*FuncPtrType)(); // 이렇게 함수 포인터 변수 자체 선언이 가능한데, 이건 C 레거시. 좀 복잡. 잘 안 씀.

	using voidFuncPtrType = void(*)(); // 함수 타입이 아니고 함수 '포인터'의 타입. 함수 이름 들어갈 곳에 *가 들어간다.

	voidFuncPtrType ptr2 = &Print;

	ptr2();

	//-----

	DosomeThing(10, 20, &Add); // 동작 자체를 인자로 넘김.
	DosomeThing(10, 20, [](int a, int b) { return a - b; });
}

///------------------------------------------------

/// 
/// 콜백 함수. 어떤 행동을 했을때 역으로 함수를 호출해주는 함수. (난 이게 아직도 뭔 개소린지 모르곘다)
/// 
/// ex 1) UI의 버튼을 눌렀을때 어떤 기능이 호출되어야 할 것이다.
/// 그런데 버튼이라는 기능을 담당하는 게임 쪽에서 어떤 함수가 호출되어야 할지, 알아서 알 수가 없음.
/// 그래서 우리가 어떤 동작이(함수가) 실행되어야할지, 인자로 넘겨주어야한다는 것.
/// 
/// ex 2)
/// 온라인 게임을 만들때, 클라가 서버에 입장 시 어떤 행동을 해야하는데,
/// 그걸 서버 입장에서는 알 수가 없으니까. 서버는 동작을 설정할 수 있게 끔 만들어주고
/// 클라에서 그 동작을 매핑하면 서버를 그걸 해주면 되는 것
/// 
/// ex 3)
/// 특정 키를 누르면 어떤 일이 일어나야 하는데. 이건 하드하게 연결되지 않고 플레이어마다 다른 키로 커스텀을 할 수 있어야하잖아.
/// 이러면 따로 함수 포인터로 매핑을 해주는 식으로 해줘야 하는 것임.
/// 어떤 키를 누르면 &Fire을 매핑, &Move를 매핑... 이런 식으로
/// 
/// ex 4)
/// 다른 온갖 정보를 소유하고 있는 Item이라는 클래스가 있다고 해보자.
/// 거기에 레어리티, 아이템ID, 소유자ID 등이 있음.
/// 인벤토리 상에서 특정 아이템이 있는 지 없는지 찾아야한다거나. 희귀도 별로 정렬을 해줘야한다거나...
/// 많은 상황이 있을 것임.
/// 그러면 레어리티마다, 아이템마다, 소유자ID마다 Find해주는 함수를 일일히 다 만들 것인가?
/// 관리가 너무 힘들다.
/// 
/// 그런데 보통 for문 돌고 순회하고 찾고 예외 던지는 건 똑같다. 
/// 그러면 '조건이 맞는지만 판별' 해주면 되는 거 아닌가?
/// 

class Item
{
public:
	int _itemId = 0;
	int _rarity = 0;
	int _ownerId = 0;
};

using ItemSelectorType = bool(*)(Item* item);

Item* FindItem(Item items[], int itemCount, ItemSelectorType selector)
{
	for (int i = 0; i < itemCount; ++i)
	{
		Item* item = &items[i];
		if (selector(item)) // item을 bool 함수로 보내어 검사
			return item;
	}

	return nullptr;
}

bool IsRare(Item* item)
{
	return item->_rarity == 1;
}

/// 
/// 멤버 함수 포인터
/// 

class Test
{
public:
	void PrintTest() {}
};

int main()
{
	Item items[10];
	items[3]._rarity = 1;

	cout << "레어인가요? :" << std::boolalpha << FindItem(items, 10, IsRare)->_rarity << '\n';

	//-----

	//
	// 멤버 함수 포인터
	// 
	// using ClassFuncPtrType = void(*)();
	// ClassFuncPtrType func = &Test::PrintTest; 
	// 어?? 이거 될 거 같은데 왜 안 돼죠?? 타입이 맞지 않다고 하는데 타입 맞고, 테스트 클래스의 것이란 것도 명시해줬잖아요!
	//
	// 멤버 함수는 정적/전역 함수와 다르다. 그냥 함수 호출 규약이 다르다.
	// 함수를 호출할때 전달되는 인자의 순서, 함수가 종료될때 stack을 누가 정리할지 등을 정하는 것이다.
	// 
	// 일반 함수는 cdecl, 멤버함수는 thiscall이라는 규약을 사용한다.
	// 
	// PrintTest는 Test 객체를 대상으로 실행하는 Test 객체의 종속적인 함수다.
	// 그래서 실제로는, 함수를 호출할 때 'Test 객체의 주소까지 같이 넘긴다.'
	// 첫 번째 매개변수로 자기 자신의 주소를 넘긴다.
	// 
	// 그러므로 '아니, 함수 타입 맨 앞에 나 자신이 없잖아!! < 이게 되는 거임.
	// 
	// 아니 그러면 파라미터 타입에 Test*를 맨 앞에 넣어줘야 해요? (이거 되나?)
	// 
	// 멤버 포인터 함수가 따로 있다.
	// 

	using MemFuncPtrType = void(Test::*)(); // * 앞에 Class:: 를 써주면 된다.

	MemFuncPtrType memFunc = &Test::PrintTest;

	Test t;
	(t.*memFunc)(); 

	// 아니 이게 문법이 무슨... 이런걸 쓰나요?
	// 서버에서 쓸 수 있습니다. 내가 쓰는 함수는 정해졌는데, 그걸 지연시켜서 내가 원하는 타이밍에 쓰고 싶을때.
	// 앞에 주문이 너무 많아서 지금 들어온 주문을 처리할 수 없을 때, 일단 지연시키고 차례가 오면 처리하는 방식으로.
	// 온라인에서는 유저가 몇 천명이고, 서버는 하나다. 이 모든 것을 한 번에 처리해줄 수 없으므로 이런 것이 필수다.
	// => 이게 바로 '비동기 처리'의 예시이다.
	// 물론, 일반 함수 포인터도 이렇게 활용할 수 있긴 하다.

	// 그런데, 함수 포인터만 사용하면 문제가 하나 있다.
	// 나는 몇 번 유저를 사용할래, 어느 좌표로 이동할래. 는 담아줄 수 없다.
	// 당연하다. 함수 포인터가 담고 있는 건 '어떤 정보가 아니라, 그저 동작'일 뿐이니까.

	using FuncPtrType = int(*)(int, int);
	FuncPtrType func = Add;

	int x = 20;
	int y = 30;
	func(x, y);

	// 예시로 위 같은 경우, 함수 포인터는 저 x,y의 정보를 담아줄 수 없다는 것임.
	// 사실 문제는 아니고, 그냥 당연한 태생적 한계임.

	// 아니 거기까지 하고 싶다고? 어떻게 해결할 건데?
	// => 함수 객체. 함수자. functor을 이용한다.
}
#endif
#pragma endregion

#pragma region Functor
#if 0
///------------------------------------------------

///
/// 함수 객체 : Functor
/// 

class Functor
{
public:
	void operator() ()
	{
		cout << _value << '\n';
	}

	// 이렇게 operator 을 오버로딩하여, 여러 시그니처를 케어해줄 수 있다!
	// 함수 포인터는 시그니처마다 타입마다 일일히 지정해줘야 했지만, 이건 그렇게 하지 않아도 된다. 

	void operator() (int n)
	{
		_value += n;
		cout << _value << '\n';
	}

public:
	int _value = 0;
};

// 어? 근데 그러면 함수 객체를 넘겨준다는 건 어떻게 파라미터로 지정해요?

struct AddStruct
{
public:
	int operator() (int a, int b)
	{
		return a + b;
	}
};

// 뭐긴 뭐야 템플릿이지
// 밑과 같이 되어있으면 내가 T가 뭔지는 모르겠고 관심도 없는데.
// 아무튼 func(a, b); 라는 시도가 int로 return 되면 돼 라는 의미가 된다.
// Functor은 Template와 굉~장히 궁합이 잘 맞는다. 기억해두자! 

template<typename T>
int DoSomethingTemplate(int a, int b, T func)
{
	return func(a, b);
}

/// 
/// 그럼 우리가 썼던 greater은 어떻게 굴러간 건가? 이것도 똑같다.
/// 
/// 참고로 Functor은 struct가 국룰이다. 어차피 외부에서 남한테 쓰이는 게 목적이라...
/// public으로 되는 게 편하다.
/// 
/// priority queue에서 뭐 거창한 걸 원하는 게 아니다. predicate를 받아준 후,
/// 그냥 그걸로 대소 비교를 해보고 잘 되면 그냥 정상 동작하는 것이다.
/// 
/// predicate는 별게 아니라 'bool를 return하는 모든 함수/객체'를 총칭하는 것 뿐이다.
/// 그냥 람다, 함수 포인터, 함수 객체 아무거나 넣어도 상관 없다.
/// 
/// Functor은 진짜로 웬만하면 함수 포인터 상위 호환이다. 다들 Functor을 쓰니 호환성이 좋기도 하고.
/// 

template<typename T>
struct Greater
{
	bool operator () (const T& left, const T& right)
	{
		return left > right;
	}
};

/// <summary>
/// 이게 클라의 요청이라고 해보자.
/// </summary>

class Job
{
public:
	virtual ~Job() {} // 이건 그냥 외워 시발! 최상위 객체면 그냥 만들어!
	virtual void operator() () = 0;
};

class MoveJob : public Job
{
public:
	MoveJob(int x, int y) : x(x), y(y) {}
	
	void operator() ()
	{
		cout << "player move\n";
	}

public:
	int x;
	int y;
};

class AttackJob : public Job
{
public:
	void operator() ()
	{
		cout << "player attack\n";
	}
};

int main()
{
	Functor func;
	func();
	func(10);

	AddStruct addFunc;

	DoSomethingTemplate(100, 200, addFunc); // 타입 추론 해줌.

	Job* movejob = new MoveJob(10, 20); // 데이터와 행동까지 저장!

	Job* attackJob = new AttackJob();

	//... 좀 뒤에, 내 차례가 왔을때 비동기로 처리해줄 수 있다.
	// 이런 클라의 일들을 queue에 넣고, FIFO로 공평하게 처리해주는 것이다.
	// 이걸 연결 리스트로 해도 장점이 있다. 만약 보스가 1분 후 필살기 장전을 했는데, 보스가 죽었으면 task에서 빼주는 것이다.
	// 큐로 해도 안 될 것은 아닌데. 그러면 bool 상태 값을 추가해주고 false면 안해준다던가 해도 된다.

	// 얼마나 task가 많느냐, 그리고 얼마나 빨리 처리해야하느냐에 따라 취사선택하면 될 것이다.

	(*movejob)(); // job -> operator() () 로도 호출 가능. operator () 가 함수 이름이니까.
	(*attackJob)();

	// 이러한 일을 일일이 객체로 만들기 귀찮다면, 그냥 람다로 처리해도 된다. 어차피 람다도 Functor니까.
	// 하지만 객체로 만들면 위처럼 job들을 최상위 객체 (인터페이스)로 모아 손쉽게 다룰 수 있다는 장점이 있다.
}
#endif
#pragma endregion

#pragma region std::vector
/// 
/// vector erase 시 주의점을 본다.
/// 
#if 0
int main()
{
	vector<int> v{ 1,2,3,4,5 };
#if 0
	// 이거 어떻게 될까? 좆된다.
	// 문제가 여러가지 있다. 1. it는 지워졌는데, it가 가리키는 곳은 그대로이고, erase시 벡터는 당겨져있다.
	// 여기서 ++it하면 '당겨진 원소'는 검사하지 않고 그 뒤로 바로 가는 것이다.
	// 2. 이게 핵심 문제인데, erase를 하면 그 iterator와 뒤는 '무효된 iterator' 처리한다.
	// 이것 때문에 ++it 할때 '어! 너 무효화된 it인데 왜 동작하려고 해!' 하고 크래시가 난다.

	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
	{
		if (*it % 2 == 0)
		{
			v.erase(it);
		}
	}
#endif

#if 0
	// erase는 반환할때 'erase 후, erase 되었던 것들 바로 뒤에 있는 원소의 iterator'을 반환한다.
	// 그것을 한번 이용해보자.

	// 이건 괜찮을까? 아니, 괜찮지 않다. 2를 지우고 3을 가리키고 있는데, 3을 가리킨 후 '아무것도 안 했다.'
	// 그냥 3을 넘어가 4로 가버린 것이다. 물론 동작 자체는 잘 되지만, '모든 원소를 검사한다'는 실패한 것.

	// 그리고, 가장 문제는 '마지막 원소를 지웠을때'이다. 이때 erase가 반환하는 건 v.end()이고, 
	// 이 v.end()에 ++를 해버리면 v.end()를 넘어가버려서 크래시를 내버린다. (C++은 iterator의 조회는 [begin, end] 까지만 허용한다.)

	for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
	{
		if (*it % 2 == 0)
		{
			it = v.erase(it);
		}
	}
#endif

	// 핵심은, for의 증가문 ++it를 지워야한다.
	// erase를 하지 못했을때만 증가하도록, if-else를 짜준다.

	for (vector<int>::iterator it = v.begin(); it != v.end();)
	{
		if (*it % 2 == 0)
		{
			it = v.erase(it);
		}
		else
		{
			++it;
		}
	}

	// 사실 이런 실시간 스캔을 피해주는 게 좋다. 버그 확률이 높아져서... 괜히 remove-erase가 있는 게 아니다.
}
#endif

#pragma endregion

#pragma region BinarySearch
#if 0
void BinarySearch(const vector<int>& vec, int searchNum)
{
	// 강의와 차별점 : 이거 굳이 왜 막아주냐 하면,
	// size_t - int는 size_t(unsigned int)라서 end가 -1인데 언더플로우되어 매우 큰 숫자가 되어버린다. 크래시가 남

	if (vec.empty())
	{
		return;
	}

	int start = 0;
	int mid;
	int end = vec.size() - 1; 

	bool found = false;

	while (start <= end) // start == end 일때도 허용해줘야한다. 값이 하나 남았을 때임.
	{
		// 뭔가 -1을 해줘야할 거 같지만, -1을 (start + end)에서든 최종값에서든 붙이면 안 된다.
		// (start + end)에 -1을 하면, {0번, 1번, 2번} 일때 1/2가 되어 중간이 1인데 0을 가리킨다.
		// 최종값에 -1을 붙이면, 크기가 하나일때, 그 위치를 0번이라 가정하면 -1번을 가리키게 된다.
		// 그냥 순순히 써라. -1을 붙여서 괜히 중간 앞을 취해줄 이유가 크게 없다. 연산만 늘어난다.

		// 그냥 잘 모르겠으면 '굉장히 적을때'를 생각해봐라.

		// 강의와 차별점 : 이렇게 하면 (start + end) / 2보다 오버플로우 위험이 줄어든다.
		// 물론 연산이 한번 더 들어가긴 한다만. C++20 이후로는 std::midpoint를 써주는 게 낫다.

		mid = start + (end - start) / 2;

		if (vec[mid] > searchNum)
		{
			end = mid - 1;
		}
		else if (vec[mid] < searchNum)
		{
			start = mid + 1;
		}
		else
		{
			found = true;
			break;
		}
	}

	if (found)
	{
		cout << "found " << searchNum << " at " << mid;
	}
	else
	{
		cout << "can't found " << searchNum;
	}
}

int main()
{
	vector<int> numbers = {1,8,15,23,32,44,56,64,81,91};
	BinarySearch(numbers, 44);
}
#endif
#pragma endregion

#pragma region BinarySearchTree
#if 0
int main()
{
	BinarySearchTree bst;
	bst.Insert(20);
	bst.Insert(30);
	bst.Insert(10);

	bst.Insert(25);
	bst.Insert(26);
	bst.Insert(40);
	bst.Insert(50);

	bst.Delete(26);

	bst.Print();
}
#endif
#pragma endregion

#pragma region Map
#if 0

class Player
{
public:
	Player() : _id(0) {}
	Player(int id) : _id(id) {}


	int _id = 0;
};

/// 
/// 만약 이런 상황에서 특정 ID에 해당하는 Player을 찾으라고 한다면?
/// O(N)임. 이진 탐색도 정렬이 필요함. 
/// 거기에, Player는 중간 삽입 삭제도 될 수 있음.
/// 
/// 아니 그럼 어쩌라고요, 그 구린 list라도 쓰자고요?
/// -> Map을 쓴다.
/// 

/// 
/// Map은 RBT 기반의 자료구조이다. 균형도 맞고, 이진 탐색 트리라 어느 대소 관계도 확실.
/// 
int main()
{
	vector<Player*> v;
	v.push_back(new Player(100));
	v.push_back(new Player(200));
	v.push_back(new Player(300));
	v.push_back(new Player(400));
	v.push_back(new Player(500));

	// (key, value) 값을 정해줘야 함, 당연히 value는 보관일 뿐 서칭에는 아무 도움도 되지 않음
	// 애초에 그 서칭에 도움되는 역할을 하라고 있는 게 key임.

	map<int, Player*> m;

	// 추가
	
	std::pair<int, Player*> p(v[0]->_id, v[0]); // 별 거 없음. 그냥 2가지를 담는 컨테이너임. first, second를 접근 가능.
	// 당연히 값은 둘 다 넣어줘야 함.
	m.insert(p); 
	m.insert(std::pair(v[1]->_id, v[1])); // 임시 객체로. std::make_pair와는 std::ref를 다루른 데 차이가 있다.
	// C++17 부터는 클래스 템플릿도 타입 추론이 되어서, 코드 작성 귀찮음의 차이는 이제는 없다.
	m.insert(std::make_pair(v[2]->_id, v[2])); // make_pair로도 임시 객체를 만들 수 있다.

	m[v[3]->_id] = v[3]; // 이런 식으로 추가해도 되는데, insert와는 차이가 있음. 없던 값도 만들고, 있던 값이면 덮어 씌운다.

	// []는 새로 만들기, 덮어 씌우기 둘 다 되지만, m[key] 가 없으면 기본값을 생성해줘야해서 기본 생성자가 있어야 하고, 
	// insert는 새로 추가만 되고 덮어 씌우지를 못한다.
	// insert_or_assign은 key가 있으면 넣은 value로 덮고, 없다면 그 value를 가진 key와의 pair을 생성한다.
	// []와 동작은 똑같은데, 내가 의도한 대로 동작해서 안전함.

	m.insert_or_assign(v[3]->_id, v[3]); // emplace와 파라미터가 같지만 내부에서 만들어 주지는 않는다.

	m.emplace(v[4]->_id, 400); // vector의 emplace_back과 같다. 재료만 주면 내부에서 pair을 만들고 넣는 거임.
	// 이렇게 하면 400이 Player(400)이 되고, 그걸 또 pair로 만들어 넣어준다.

	// 만약 인자가 여러개라면, std::piecewise_construct를 인자로 넣어주거나 try_emplace를 사용하자.

	// 벡터는 기본적으로 탐색을 O(N) 혹은 정렬된 상태에서 이진 탐색으로밖에 못한다.
	// 하지만! Map은 RBT 기반. 애초에 순회를 해줄 필요가 없다. 그냥 찾아!

	// 찾기
	// 찾기의 시간 복잡도는 당연히 O(logN). 높이가 무조건 logN이 보장됨. RBT니까. 한번 내려갈때마다 후보가 반이 털림.
	// 벡터 O(N)보다 좋다!
	
	// pair 찾기

	auto it = m.find(100); // 해당 노드를 가리키는 map<key, value>::iterator을 반환한다.
	
	// value 찾기. 사실 pair이나 value나 찾는 데는 key가 필요해서. 이미 key를 알아서 보통 이걸 많이 쓴다.

	Player* p = m[100];

	// 근데 조심해야될 게 있다. 만약 없는 key 값을 찾으려 했다면? value가 없다고 에러가 뜰까?
	// 크래시가 안 나고, m[100]에 접근하는 순간 value의 기본 값을 그 key 값에 넣어버린다.
	// 즉, m[key] 문법 자체가 '있으면 찾아오고, 없으면 기본값 만들어서 줘' 이다.

	// C++ STL은 그런데, UE TMap이란 게 있는데, 얘는 없는 key를 넣으면 크래시가 난다! 쉣!
	// 다른 것도 있는데, STL에서 empty()는 비었는지 안 비었는지 확인하는 건데, UE에서는 clear()이다. 이런 쉣!!

	// 만약 값이 없으면 m.end()를 반환함.

	if (it != m.end())
	{
		// 물론, 노드는 std::pair을 들고 있다.
		int key = it->first; // key 
		Player* value = it->second; // value
	}
	
	// 삭제
	// vector, list, deque와는 다르게, iterator로만 erase가 되는 건 아니다.
	// map의 erase는 key로도 가능하다!

	m.erase(200);

	// 순...회?

	// 전에 BST에서 구현을 해봤지만, Next를 찾는 게 꽤나 걸리는 작업이다.
	// 오른쪽 자식에서 쭉 왼쪽으로 가거나, 없다면 오른쪽 부모를 찾을때까지 올라가야한다.
	// 다음으로 갈때마다 이 Next를 해야하기에, Map은 찾기는 아주 좋지만, 순회를 하려면 벡터가 훨씬 낫다.
	// 그리고 벡터와 다르게, 메모리에 일렬로 붙어 있는 것도 아니고 떨어져 있는 것도 그렇다.

	for (auto it = m.begin(); it != m.end(); ++it)
	{
		int key = it->first;
		Player* p = it->second;
	}

	// 물론, 하나의 데이터를 하나의 자료구조로만 다뤄야하는 것은 아니다.
	// 같은 데이터를 하나는 map으로, 하나는 벡터로 다루어 찾기, 순회용으로 둬도 된다.
	// 그만큼의 메모리는 지불해야겠지만.
}

#endif
#pragma endregion

#pragma UnorderedMap(HashMap)

int main()
{
	unordered_map<int, int> um;

	// 추가 O(1)
	// 사용법은 map과 거의 같다

	um.insert(std::make_pair(10, 100));
	um[20] = 200;

	// 찾기 O(1)

	auto findIt = um.find(20);
	if (findIt != um.end())
	{
		cout << "Find it\n";
	}

	// 삭제 O(1)

	um.erase(10);
	um.erase(findIt);

	// 순회 O(N) 일일이 해쉬 함수 취해서 찾아갈 필요는 없음.
	// 어? 찾아가려면 key에 해쉬 함수 % 버킷 수 취해서 찾아가야하잖아요.
	// 맞는데, 외부에서 key를 통해 접근할때. unordered_map의 begin()은 실제 데이터를 가지는 노드의 첫 메모리 주소를 줌.
	// 그래서 그냥 그거 기반으로 next 노드로 가는 식임. 다른 거 없음
	// 
	// 해쉬 맵의 노드는 1. 같은 버킷 안에 있을 때 서로 이어져 있고
	// 2. 순회 용으로, 서로 다른 노드끼리도 이어져 있음.

	for (auto it = um.begin(); it != um.end(); ++it)
	{
		int key = it->first;
		int value = it->second;
	}

	// 근데 bucket이라는 게 있는데, 이건 뭐에요?
	// hash 테이블의 인덱스 = key를 hash 함수를 취한 후 % 버킷 수 한 것.
}

#pragma endregion