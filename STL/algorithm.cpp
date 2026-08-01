#include<iostream>
#include<vector>
#include<list>
#include<map>
#include<unordered_map>
#include<algorithm>
using std::cout, std::vector, std::list, std::map, std::unordered_map;

#pragma region StdAlgorithms
#if 0
int main()
{
	vector<int> v{1,2,3,4,5};

	// iterator std::find(begin(), end(), 찾는 수). 못 찾으면 end() 반환함
	// 별 다를 것 없음. 그냥 처음부터 끝까지 순차 순회. O(N)
	// 그냥 딱 눈에 봐서 뭘 하는 지 아는 게 장점임.

	{
		auto it = std::find(v.begin(), v.end(), 3);

		if (it != v.end())
		{
			cout << "와 찾음\n";
		}
		else
		{
			cout << "못 찾음...\n";
		}
	}


	// iterator std::find_if(begin(), end(), Predicate)
	// find랑 똑같은데, Predicate로 조건 봐 줌.
	// Predicate는 별 거 없다. 그냥 bool 반환하는 거. 함수 포인터, Functor, 람다... 마음대로

	{
		auto it = std::find_if(v.begin(), v.end(), [](int elem) {return elem % 11 == 0; });

		if (it != v.end())
		{
			cout << "와 찾음\n";
		}
		else
		{
			cout << "못 찾음...\n";
		}
	}

	// int std::count(begin(), end(), 찾는 수)
	// int std::count_if(begin(), end(), Predicate)
	// find랑 똑같고 순차 순회 O(N)인데 그냥 센다는 것만 달라.
	// 근데 find는 일단 찾으면 바로 그만두고 얘는 계속 센다는 차이는 있음.

	{
		cout << "3 갯수: " << std::count(v.begin(), v.end(), 3) << '\n';
	}
	
	struct IsOdd
	{
		bool operator() (int elem) {
			return elem % 2 != 0;
		}
	};

	// Predicate 자리에 ()로 operator()를 호출하여 넣어줘야한다. 아니면 그냥 객체 타입 불러주는 거라서...
	{
		cout << "홀수 갯수: " << std::count_if(v.begin(), v.end(), IsOdd()) << '\n';
	}

	// all_of = 모든 데이터가 Predicate를 만족하는가?
	// any_of = 데이터 중 하나라도 Predicate를 만족하는가?
	// none_of = 모든 데이터가 Predicate를 만족하지 않는가?

	{
		cout << "모든 원소가 홀수인가? " << std::boolalpha << all_of(v.begin(), v.end(), IsOdd()) << '\n';
		cout << "원소 중 홀수가 하나라도 있는가? " << std::boolalpha << any_of(v.begin(), v.end(), IsOdd()) << '\n';
		cout << "모든 원소가 홀수가 아닌가? " << std::boolalpha << none_of(v.begin(), v.end(), IsOdd()) << '\n';
	}

	// 원소 전체 대상으로 무언가를 할때. for_each
	// void 형태인 Predicate를 받아, 그것을 실행한다. bool return은 아니라서, Unary Function (단항 함수)이다. 
	// 그리고 주의점이, 자기 자신 원소를 받아와서 고치는 거라서 & 붙여줘야한다.

	{
		std::for_each(v.begin(), v.end(), [](int& elem) { elem = elem * 3; });
	}

	// std::transform 도 '범위 내 모든 원소를 기반으로 작업을' 하는 건 같지만, 그건 다른 컨테이너를 대상으로 할 수도 있다.
	// std::transform(v.begin(), v.end(), std::back_inserter(dest), [](int a){ return a * 2; }); 이런 식으로 다른 컨테이너에 옮겨 쓸 수도 있지만,
	// std::transform(v1.begin(), v1.end(), v2.begin(), std::back_inserter(dest), [](int a, int b){ return a + b; });
	// 위와 같은 식의 2개의 컨테이너에 대해 연산을 하는 Binary Function도 가능하다.
	// 단, v2의 크기는 v1의 크기보다 커야함. (3번째 파라미터에 넣는 건 시작점이라 간주함.) 
	// + dest 컨테이너는 충분히 크지 않으면 뒤에 꽂아넣어주는 std::back_inserter을 사용할 것.
	// (그냥 capacity 확인해주고 부족하면 넓히고 v.end() 반환해주는 놈이라 보면 된다.)

	// remove, remove_if 정확히는 동작이, 정한 값, 혹은 Predicate를 만족하는 값들을 만족하지 않는 뒤의 값들로 덮어 씌워버리는 것.
	// 즉, 앞의 지울 값들을 '뒤의 지우지 않을 값들(유효할 값들)'로 덮어 씌우는 것이다.
	// 그렇다면, 자연히 덮어 씌운 뒤는 전혀 유효하지 않은 것이 될 것이다. 하지만 C++는 이를 지워주지 않는다.
	// remove, remove_if는 그저 다 덮어 씌운 바로 뒤의 iterator을 반환한다.
	// 그러니, remove를 갈기고 난 다음 컨테이너는 사실상 쓸모 없는 데이터들이 뒤에 박혀있는 상태라는 것이다.

	// 1 4 3 5 8 2 -> 홀수 remove ->  4 8 2 5(5 가리키는 iterator을 반환) 8 2

	// 그러므로, 컨테이너 자체의 erase 함수를 병행해주어야한다. 이는 remove-erase 패턴으로 유명하다.
	// remove가 돌려준 '유효한 데이터만 있는 바로 뒤 iterator' 부터 '컨테이너의 끝'까지 지우는 것이다.

	v.erase(std::remove_if(v.begin(), v.end(), IsOdd()), v.end());

	std::for_each(v.begin(), v.end(), [](int elem) {cout << elem << ' '; });

	cout << '\n';

	// 근데 위를 왜 쓰는 것이냐? 간편한 것도 그런데, erase 순회하면서 하면 erase가 반환한 iterator을 반환받아야하고, 아니면 ++를 해주는 수작업을 해줘야했다.
	// 번거롭고, 안전성도 떨어지고, 무엇보다 삭제할때마다 O(N)의 작업을 반복해야했다.
	// 하지만, remove는 투 포인터 동작으로 O(N)을 한번 슥 하고, 그 뒤 뒤의 모든 것을 지워주면 되므로, O(N)의 작업 2번으로 모든 삭제가 끝난다.

	// std::unique도 비슷하게 동작해서, unique-erase를 해주기도 한다.
	// std::unique는 remove와 비슷하게 투 포인터로 동작하는 '인접한 중복된 것을 없애주는' 함수라서, 
	// std::unique를 사용하기 전, sort로 정렬이 필요하다.
}
#endif
#pragma endregion