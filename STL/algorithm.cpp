#include<iostream>
#include<vector>
#include<list>
#include<queue>
#include<map>
#include<unordered_map>
#include<algorithm>
#include<string>
using std::cout, std::vector, std::list, std::map, std::unordered_map, std::priority_queue;

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

#pragma region Sort
#if 0
/// 
/// BubbleSort 한번 돌때마다 무조~건 제일 큰게 맨 뒤로감. 그걸 n - 1 번만큼 반복하는 거임.
/// 그래서 O(N^2)임. 아주 최악의 Sort
/// 

void BubbleSort(vector<int>& v)
{
	int end = v.size() - 1;

	for (int i = 0; i < end; ++i)
	{
		for (int j = 0; j < end - i; ++j)
		{
			if (v[j] > v[j + 1])
			{
				std::swap(v[j], v[j + 1]);
			}
		}
	}
}

/// 
/// SelectionSort. BubbleSort의 반대임. 가장 작은 애들을 앞으로 끌고 오는 거임.
/// 근데 알고리즘이 좀 다르긴 한게, 버블소트가 앞뒤만 비교해서 재귀적으로 돌았다면, 
/// 이건 하나가 최솟값을 잡고, 나머지는 쭉 순회하며 최솟값을 찾고 넣는, 투 포인터 방식.
/// 그래도 결국 원소 하나하나씩 잡고 정렬하는 것이라서, O(N^2)인건 똑같음.
/// 

void SelectionSort(vector<int>& v)
{
	int end = v.size();

	for (int i = 0; i < end - 1; ++i)
	{
		int bestidx = i;
		for (int j = i + 1; j < end; ++j)
		{
			if (v[bestidx] > v[j])
			{
				bestidx = j;
			}
		}
		
		if (bestidx != i)
		{
			std::swap(v[i], v[bestidx]);
		}
	}
}

/// 
/// 힙 소트 = 우선순위 큐 힙 트리 말하는 거 맞다.
/// 아니, 우선순위 큐는 부모가 자식보다 큰 것만 보장되지 BST가 아니잖아요
/// 맞아. 그래서 힙 트리 완성한 다음에 그 윗부분만 다시 쏙쏙 빼먹어
/// 
/// 아무튼 아주 좋은 방법이다. 힙 트리 자체가 높이 logN을 무조건 보장하기 때문에...
/// 
/// 그런데, 왜 HeapSort가 최고가 되지 못했을까?
/// 1. 캐시 효율이 좋지 않다. 보통 캐싱을 하면 공간적 지역성을 고려해 그 주변의 메모리들을 가져오기 마련인데,
/// 우선순위 큐는 자식을 이동할 때 2*i+1, 2*i+2, 부모로 이동할때 (i - 1)/2 를 한다.
/// 그래. '바로 옆'을 방문하지 않아 캐싱의 수혜를 띄엄띄엄 받을 수밖에 없다.
/// 
/// 2. NlogN이긴 한데, 그 NlogN의 수가 좀 많다.
/// 삽입을 할때 NlogN, 그리고 삭제를 할때는.. 2NlogN이다.
/// 왜냐면, 삭제를 하면 맨 뒤의 값을 맨 위로 올리고, 그것을 '양 옆'으로 비교하기 때문이다.
/// 그래서, 결국 3NlogN이 걸리는데, 퀵 소트 같은 경우는 원소를 하나 만날때마다 '피벗 보다 작은가?'라는 한번만 비교한다.
/// 퀵 소트의 평균 비교 횟수는 1.39NlogN이다.
/// 
/// 즉, 힙 소트는 무조건 NlogN, 그리고 메모리는 O(1)을 보장하는 아주 좋은 알고리즘이다.
/// (우리가 구현한 것과 다르게, 원래는 기존 벡터를 힙 트리로 만든다. 원래 힙 트리는 벡터로 구현 가능하니까.
/// 이때, 벡터를 힙 트리로 만드는데, 벡터는 다 구성되어있고, 트리가 확정되어있다. 
/// 그러므로 맨 밑의 부모 노드들에서부터 '양 쪽 자식 중 누가 나보다 큰 지 보고, 큰 쪽을 올리고 내가 내려간다.'는 방식을 사용.
/// 이를 기존 힙 트리 생성의 Top-down과는 다른 bottom-up이라고 한다. 이건 O(NlogN)이 아닌 O(N)이다.
/// )
/// 하지만, 평균적으로 NlogN 중에서는 빠르지 않다는 것이다. 즉 저점은 좋은데 고점은 낮은 알고리즘이다.
/// 
/// 3. Unstable Sort이다. 이건 비단 heapSort만 그런 건 아니고, 사실 힙, 퀵 다 똑같다만.
/// Merge는 leftIdx를 우선적으로 넣어주므로, StableSort이다.
/// 

// 밑이 날먹 힙 소트
#if 0
void HeapSort(vector<int>& v)
{
	priority_queue<int, vector<int>, std::greater<int>> pq;

	// 힙 트리는 삽입은 O(logN)이고 빼는 건 O(1)이다.
	// 그럼 이것의 시간 복잡도는?
	// 삽입이 logN, 그리고 그걸 v의 원소 만큼 반복하니 삽입은 O(NlogN)이다.

	for (int num : v)
	{
		pq.push(num);
	}

	v.clear();

	while (pq.empty() == false)
	{
		v.push_back(pq.top()); // O(1)
		pq.pop(); // 삭제 시 O(logN). 이때도 pq의 원소 수가 N이니 O(NlogN).
	}

	// 그래서 정확히는 O(NlogN) 작업을 2번 한다.
}
#endif

// 강의와 차별점 : 벡터를 힙으로 만드는 Heapify 구현

void Heapify(vector<int>& v, int parentIdx, int size)
{
	int maxIdx = parentIdx;
	int leftChildIdx = parentIdx * 2 + 1;
	int rightChildIdx = parentIdx * 2 + 2;

	if (leftChildIdx < size && v[leftChildIdx] > v[maxIdx]) {
		maxIdx = leftChildIdx;
	}

	if (rightChildIdx < size && v[rightChildIdx] > v[maxIdx])
	{
		maxIdx = rightChildIdx;
	}

	if (maxIdx != parentIdx)
	{
		std::swap(v[parentIdx], v[maxIdx]);

		Heapify(v, maxIdx, size); 
		// 그 밑 서브트리로 parent가 내려갔을떄, parent가 밑의 자식들보다 또 크지 않을 확률도 있음.
		// 그걸 케어해야함. maxIdx에는 이제 내려간 parent가 있는 자리이니, 거기서 해줌
	}
}

void HeapSort(vector<int>& v)
{
	int vectorSize = v.size();

	// 맨 뒤 부모부터 시작 해서 맨 위 부모까지 Heapify
	for (int parentIdx = (vectorSize - 2) / 2; parentIdx >= 0; --parentIdx)
	{
		Heapify(v, parentIdx, vectorSize);
	}

	// 힙 트리 만들었으면, 이제 맨 앞이 최댓값인 상황을 반복하며 뒤로 밀어주기
	for (int i = vectorSize - 1; i > 0; --i)
	{
		// 이 부분 때문에 unstable이다. 처음 있던 최댓값이 맨 뒤로 간다.
		std::swap(v[0], v[i]);
		Heapify(v, 0, i);
	}
}

/// 
/// MergeSort 
/// 
/// Divide and Conquer 방법을 사용한다. 이건 꽤 많이 쓰니 이 방식 자체를 알아두면 좋다.
/// - Divide 분할 / - Conquer 정복 / - Combine 결합
/// 말은 멋진데, 그냥 큰 문제를 작은 문제로 나누고 작은 문제를 각자 해결 한 후 다시 취지에 맞게 합치겠다는거다.
/// 왜냐면, 그냥 작은 문제가 더 쉬운 방법으로 풀 수 있으니까..
/// 그리고, 분할한 문제는 다른 분할한 문제와 독립적이다.
/// 그러므로 병렬적으로 다른 놈이 문제를 풀고 결과 값을 줘도 OK 라는 것
/// 

/// [3][K][7][2][J][4][8][9] < 8
/// [3][K][7][2] [J][4][8][9] < 4 * 2
/// [3][K] [7][2] [J][4] [8][9] < 2 * 4
/// [3] [K] [7] [2] [J] [4] [8] [9] < 1 * 8
/// 
/// 그리고 이제 인접한 것들 끼리 정렬해주면 된단 것이다.
/// [3] [K] [7] [2] [J] [4] [8] [9] << 하나씩 비교해주면 됨.
/// [3][K] [2][7] [4][J] [8][9] << 여기서부터, 각 배열마다 하나씩 맨 앞을 보고 더 작은 값을 먼저 앞에 넣어주면 된다.
/// [2][3][7][K] [4][8][9][J]
/// [2][3][4][7][8][9][J][K]
/// 

/// 
/// 근데 실제로 벡터를 나누진 않음. 어차피 같은 크기인데 인덱스만 나누면 됐지
/// 진짜로 벡터 나누고 합치고 하는 건 비효율적이니까.
/// 

/// 
/// 강의와 차별점 : 근데 MergeResult 굴릴때마다 vector 생성하는 게 좀 아깝지 않은가?
/// 그냥 N짜리 벡터 하나 만들고 계속 굴리면 될텐데.
/// Q. 어, 그럼 MergeResult에 static 벡터 하나 만들면 어때요?
/// A. 1. 그거 멀티 스레드로 다른 스레드에서 동시 접속하면?
/// 2. MergeSort를 안 해도 쓸데 없이 메모리에 계속 남아있는데?
///  
/// Q. 아니 그러면 어쩌라고요
/// A. MergeSort단부터 N크기의 vector을 만들어 끼워 넣어준다.
/// 
/// Q. 아니, MergeSort는 재귀함수잖아요? 그거마다 N크기 벡터 만들면 오히려 낭비죠?
/// A. 그래서 재귀 부분을 따로 나눠줄거다.
/// 
/// Q. 아...ㅅ1팔...
/// 

// 이게 원본

#if 0
void MergeResult(vector<int>& v, int left, int mid, int right)
{
	// 위에서 보는 작게 나눈 것들에서 뭐가 더 작은 값인지 보고 하나하나 넣는 거임
	// 어차피 left ~ right까지 서로 원래 일렬로 이어져 있던 것들.
	// 1~2와 3~4 번을 합치지 막 1~2 번과 7~8번을 merge 한다던가 그러지는 않음.

	int leftIdx = left;
	int rightIdx = mid + 1;

	// 강의와 차별점 : reserve를 먼저 해준다.
	vector<int> temp;
	temp.reserve(mid - left + 1 + right - mid);

	while (leftIdx <= mid && rightIdx <= right)
	{
		if (v[leftIdx] <= v[rightIdx])
		{
			temp.push_back(v[leftIdx]);
			++leftIdx;
		}
		else
		{
			temp.push_back(v[rightIdx]);
			++rightIdx;
		}
	}

	// 이건 무조건 거쳐야한다. 위에서 왼쪽이나 오른쪽이나 둘 중 하나는 먼저 끝내서 나온 거니까.
	
	if (leftIdx > mid)
	{
		while (rightIdx <= right)
		{
			temp.push_back(v[rightIdx]);
			++rightIdx;
		}
	}
	else
	{
		while (leftIdx <= mid)
		{
			temp.push_back(v[leftIdx]);
			++leftIdx;
		}
	}

	std::copy(temp.begin(), temp.end(), v.begin() + left);
}

void MergeSort(vector<int>& v, int left, int right)
{
	// 헉 더 이상 분할이 안 돼 -> return;. 분할을 멈춤.
	// 사실 left가 right를 넘어갈 일이 없긴 한데, 하나만 남으면 left == right가 되긴 함
	if (left >= right)
	{
		return;
	}

	// 이얍 일단 분할을 존나 해

	// 잘 보면 홀수 개 일때는 우측이 하나 덜 가져간다
	// 짝수 개일때는 mid가 .5라서 잘림. 좌측이 하나 덜 가져가게 됨. 그래서 갯수가 같아짐

	int mid = (left + right) / 2;

	MergeSort(v, left, mid); 
	MergeSort(v, mid + 1, right); 

	MergeResult(v, left, mid, right);
	
	// 아니 이게 뭡니까? -> MergeSort로 left, mid, right를 다 잘게 분할함.
	// 그러면 원소가 하나가 된 순간 return. 그러면 하나 된 것들은 이제 빠져 나올테니, 
	// 하나 하나 된 것 부터 합치는 것이다.
	// 잘 보면 왼쪽 끝 부터 쭉 파고들고 거기서부터 합치고,
	// 다음 MergeSort(v, mid + 1, right); 가 실행되고, 그 뒤 또 하나가 되면 MergeResult를 하고... 이걸 반복하는 식이다.
}
#endif

void MergeResult(vector<int>& v, vector<int>& temp, int left, int mid, int right)
{
	int leftIdx = left;
	int rightIdx = mid + 1;
	int curIdx = left;

	while (leftIdx <= mid && rightIdx <= right)
	{
		if (v[leftIdx] <= v[rightIdx])
		{
			temp[curIdx] = v[leftIdx];
			++leftIdx;
		}
		else
		{
			temp[curIdx] = v[rightIdx];
			++rightIdx;
		}
		++curIdx;
	}

	while (leftIdx <= mid)
	{
		temp[curIdx] = v[leftIdx];
		++leftIdx;
		++curIdx;
	}

	while (rightIdx <= right)
	{
		temp[curIdx] = v[rightIdx];
		++rightIdx;
		++curIdx;
	}

	std::copy(temp.begin() + left, temp.begin() + curIdx, v.begin() + left);
}

void MergeSortInternal(vector<int>& v, vector<int>& temp, int left, int right)
{
	if (left >= right)
	{
		return;
	}

	int mid = (left + right) / 2;

	MergeSortInternal(v, temp, left, mid);
	MergeSortInternal(v, temp, mid + 1, right);

	MergeResult(v, temp, left, mid, right);
}

void MergeSort(vector<int>& v)
{
	if (v.empty())
	{
		return;
	}

	vector<int>temp(v.size());

	MergeSortInternal(v, temp, 0, v.size() - 1);
}

/// 
/// 그래서, 이 MergeSort 놈의 시간 복잡도는 어떻게 분석할까.
/// 나누는 거? logN. 반씩 나누니까.
/// 그 후 넣는 건 두 벡터를 보고 비교하여, 원소 갯수만큼 정직하게 해야한다.
/// 근데 이게 원래 봤던 것도 또 봐야함. 내가 1, 2번에 대해 정렬했더라도 1~2, 3~4번에 대해서는 어떨지 모르니까.
/// 1 + 1, 2 + 2, 4 + 4, ... N/2 + N/2 만큼 봐야 하는 것이다.
/// 그런데, 그 전체를 보면, 사실 1 + 1을 할때도, 2 + 2를 할때도. 사실 비교하면서 벡터에 넣는 수 자체는 똑같다.
/// 내가 2 + 2의 비교를 하며 temp 벡터에 넣고 있을 때, 다른 2 + 2의 비교도 여러 개 굴러가고 있거든.
/// 그런데 N/2 + N/2의 비교와 벡터 넣기? 이거는 마지막 단 한번 밖에 일어나지 않는다.
/// 합칠 때가 1 + 1에서 N/2 + N/2까지 가야하는데. 이게 점점 보는 수가 2배씩 늘어나므로, 
/// 1 + 1 에서 N/2 + N/2까지 가는 데 속도는 2배씩 늘어날 것이다. 방금 말했던 1 + 1, 2 + 2, 4 + 4, ... N/2 + N/2 와 같이.
/// 그러므로, 총 '합치는 층'의 갯수는 logN 개이다.
/// 그리고, 이 합치는 행위 자체는 한 층마다 언제나 데이터 수 N개만큼 일어난다.
/// 그러므로, 복잡도는 O(NlogN)이다.
/// 

/// 
/// QuickSort. 평균 O(NlogN). 메모리 사용 최선의 경우 O(logN) 최악의 경우 O(N). (재귀 호출의 스택 사용.)
/// '평균적으로는' 가장 빠르다. 하지만 MergeSort, HeapSort와 다르게 언제나 O(NlogN)을 보장하지 않는다. 최악의 경우 일렬로 줄지어져 O(N^2).
/// 
/// 알고리즘 적으로는, MergeSort는 '나누고 정렬한다'였지만, QuickSort는 '정렬하면서 나눈다'로 보면 된다.
/// 그래서, MergeSort의 다 나누고 -> 합치는 2 단계가 아닌 바로 1단계만 거치면 되어서 굉장히 빠른것.
/// 단, unstable sort라는 것과 언제나 NlogN을 보장하지는 않는다.
/// 

///
/// 강의와 차별점 : pivot == 일때 통과시키면 안 됨. 이때 멈춰야 함.
/// 왜냐하면, 같은 중복 데이터를 건너 뛰면 중복 데이터가 많은 배열에서 O(N^2)이 되어버림.
/// 예시를 들어보자. 같은 값만 있는 리스트라고 가정했을 때. low가 쭉~ 넘어가서 right 뒤 까지 넘어가버린다.
/// 그러면 high는 맨 끝 right 그대로이다. pivot은 맨 끝에 위치한다.
/// 즉, 그러면 같은 값만 있는 것은 언제가 pivot이 맨 끝이 되어, 일렬 리스트 같이 되어버린다.
/// 
/// 하지만, 같은 값에 멈추면? low도, high도 처음에서 멈추고, swap한다. (이건 쓸모 없는 과정이 맞다.)
/// 그리고 하나하나 차근히 나아가며, 딱 중앙에서 엇갈려 중앙의 high가 pivot이 된다.
/// 쓸모 없는 교환의 손실보다, 중앙에 잡아서 NlogN이 되는 이득이 크다.
/// 

int Partition(vector<int>& v, int left, int right)
{
	int pivot = v[left];
	int low = left + 1;
	int high = right;

	// 이 과정을 거치면 'pivot보다 낮고 / 높은 것'으로 정렬된다.
	// 그러므로, 나뉜 것 안의 관계는 모르겠지만, 아무튼 낮고 / 높은 것으로 나뉘었고. 그 사이 pivot이 존재한다는 것.
	while (low <= high)
	{
		while (low <= right && pivot > v[low]) // pivot == v[low] 허용하면 절대 안 돼!
		{
			++low;
		}

		while (high > left && pivot < v[high]) // high는 left보다 클때만. left는 pivot이기 떄문에. left - 1로 넘어가면 마지막 swap때 크래시가 난다.
		{
			--high;
		}

		// 낮은 게 앞으로, 높은 게 뒤로 와야 함. 하지만 이는 low가 뒤에 있을때만 유효하다.
		// low가 high보다 크다는 건, low가 pivot보다 높은 값을 못 찾았거나, high가 pivot보다 작은 값을 엇갈릴때까지 못 찾았다는 뜻이다.
		// 그리고, 그 '엇갈리는 지점'은 우리가 찾던 pivot이다.

		// 강의와 차별점 : 여기서 ++low, --high 안 해주면 '같은 값일때 계속 루프를 돈다.' 이건 내가 위 조건을 pivot과 같은 값일때도 멈추게 수정해서 여기도 고쳐야 함.
		// 그리고 else때 break;도 해주어야 함. 왜냐면, 이건 low == high일때는 실행되지 않아서.. 만약 '둘 다 pivot과 같은 값' + 'low == high'인 상황이면, 여기서 무한루프가 난다.  
		// 맨 위 while 조건문 low < high로 바꾸는 건 또 안됨. 왜냐하면 pivot 이외에 나머지 하나가 있을때 그 나머지 하나를 검사하지 않아버리게 됨.
		// 그러면 만약 맨 위에 있는 애가 pivot보다 작으면, 걔를 검사하지 않아서 앞으로 옮겨주지를 못하게 됨.
		if (low < high)
		{
			std::swap(v[low], v[high]);
			++low;
			--high;
		}
		else
		{
			break;
		}
	}
	
	std::swap(v[left], v[high]); // 이때 high는 앞 뒤 구분이 모두 끝난 후의 위치.

	return high;
}

void QuickSort(vector<int>& v, int left, int right)
{
	if (left >= right)
	{
		return;
	}

	int pivot = Partition(v, left, right);

	QuickSort(v, left, pivot - 1); // MergeSort와 다르게, pivot을 넣으면 안 된다. 이미 pivot은 정렬이 된 상태기 때문에.
	QuickSort(v, pivot + 1, right);
}

int main()
{
	vector<int> v{ 1,5,3,4,2,2,3 };
	//BubbleSort(v);
	//SelectionSort(v);
	//HeapSort(v);
	MergeSort(v);
	//QuickSort(v, 0, v.size() - 1);

	// 이렇게 해도 인라이닝을 해줘서 괜찮다. 그리고 오히려 STL의 함수들은 SIMD 유닛에 최적화되어 굴러가도록 되어있어서, 오히려 쓰면 좋다. 
	// 물론 이 경우는 cout을 쓰는 I/O 같은 경우이므로 불가능 하지만, 간단한 분기문이나 사칙연산 정도라면 SIMD 유닛에서 처리가 가능하다
	std::for_each(v.begin(), v.end(), [] (int elem) {cout << elem << ' '; });
	cout << '\n';
}
#endif
#pragma endregion


#pragma region DP
#if 0
// 조합 문제
int cache[50][50];

int combination(int n, int r)
{
	// 1. 기저사례!
	if (r == 0 || n == r)
	{
		return 1;
	}

	// 2. 캐시!
	int& ret = cache[n][r]; // 이래야 바꾸면 바로 cache로 들어감

	if (ret != -1)
	{
		return ret;
	}
	// 3. 적용!
	// 이 3단계만 잘 거치면 된다. DP는.
	return ret = combination(n - 1, r - 1) + combination(n - 1, r);
}

int main()
{
	::memset(cache, -1, sizeof(cache));

	cout << combination(45, 6);
}

#endif


// 만약 +1, +2, +3 만 할 수 있다고 하자.
// 그러면 +9 가 될 수 있는 경우의 수는?

const int N = 9;
int cache[100];

// [num] 부터 시작해서, [+N]까지 가는 경우의 수
// 그러면, 내가 어떤 수가 되었을때 과거는 중요하지 않다는 점에서 착안

int Enchant(int num)
{
	// 기저사례

	// N 넘으면 실패! 더해주면 안됨
	if (num > N)
	{
		return 0;
	}

	// 1 리턴! 네가 온 길은 잘 온 거야! (경우의 수 +1 추가)
	if (num == N)
	{
		return 1;
	}

	// 캐시

	int& ret = cache[num];
	if (ret != -1)
	{
		return ret;
	}

	// 적용

	return ret = Enchant(num + 1) + Enchant(num + 2) + Enchant(num + 3);
}

int main()
{
	::memset(cache, -1, sizeof(cache));

	cout << Enchant(0);
}

#pragma endregion