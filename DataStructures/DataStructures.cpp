#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <utility>

using std::vector, std::cout, std::swap, std::less, std::greater, std::stack, std::queue;

/// ********************************
/// Tree
/// ********************************

//class Node
//{
//public:
//    Node(const char* data) : data(data) {}
//public:
//    const char* data;
//    vector<Node*> children;
//};
//
//Node* CreateTree()
//{
//    Node* root = new Node("R1 개발실");
//    {
//        Node* node = new Node("디자인");
//        root->children.push_back(node);
//        {
//            Node* leaf = new Node("전투");
//            node->children.push_back(leaf);
//        }
//        {
//            Node* leaf = new Node("경제");
//            node->children.push_back(leaf);
//        }
//        {
//            Node* leaf = new Node("스토리");
//            node->children.push_back(leaf);
//        }
//    }
//    {
//        Node* node = new Node("프로그래밍");
//        root->children.push_back(node);
//        {
//            Node* leaf = new Node("클라");
//            node->children.push_back(leaf);
//        }
//        {
//            Node* leaf = new Node("서버");
//            node->children.push_back(leaf);
//        }
//        {
//            Node* leaf = new Node("엔진");
//            node->children.push_back(leaf);
//        }
//    }
//    {
//        Node* node = new Node("아트");
//        root->children.push_back(node);
//        {
//            Node* leaf = new Node("배경");
//            node->children.push_back(leaf);
//        }
//        {
//            Node* leaf = new Node("캐릭터");
//            node->children.push_back(leaf);
//        }
//    }
//
//    return root;
//}
//
//void PrintTree(Node* root, int depth = 0)
//{
//    for (int i = 0; i < depth; ++i)
//    {
//        cout << '-';
//    }
//
//    cout << root->data << '\n';
//
//    int size = root->children.size();
//
//    for (int i = 0; i < size; ++i)
//    {
//        PrintTree(root->children[i], depth + 1);
//    }
//}
//
//int GetHeight(Node* root)
//{
//    int ret = 1;
//
//    int size = root->children.size();
//
//    for (int i = 0; i < size; ++i)
//    {
//        int h = GetHeight(root->children[i]) + 1;
//        ret = std::max(ret, h);
//    }
//
//    return ret;
//}

/// ********************************
/// PriorityQueue
/// ********************************

//template <typename T, typename Predicate = less<T>>
//class PriorityQueue
//{
//public:
//    void push(const T& data)
//    {
//        // 일단 힙 구조 맞추기
//        _heap.push_back(data);
//
//        // 도장깨기 시작
//        int curIdx = static_cast<int>(_heap.size()) - 1;
//
//        // 처음엔 curIdx에서 값 < parentIdx에서 값로 만들었는데
//        // 같은 코드를 반복해 쓰게 되어서, 좋지 않게 되어 수정
//        while (curIdx > 0) 
//        {
//            int parentIdx = (curIdx - 1) / 2;
//
//            if (_predicate(_heap[parentIdx], _heap[curIdx]))
//            {
//                swap(_heap[curIdx], _heap[parentIdx]);
//            }
//            else
//            {
//                break;
//            }
//
//            curIdx = parentIdx;
//        }
//    }
//
//    void pop()
//    {
//        if (empty())
//        {
//            return;
//        }
//
//        _heap[0] = _heap.back();
//        _heap.pop_back();
//        int curIdx = 0;
//        int leftIdx = curIdx * 2 + 1;
//        int rightIdx;
//
//        int heapSize = _heap.size();
//
//        while (leftIdx < heapSize)
//        {
//            int nextIdx = curIdx;
//            
//            if (_predicate(_heap[nextIdx], _heap[leftIdx]))
//            {
//                nextIdx = leftIdx;
//            }
//
//            rightIdx = curIdx * 2 + 2;
//
//            if (rightIdx < heapSize && _predicate(_heap[nextIdx], _heap[rightIdx]))
//            {
//                nextIdx = rightIdx;
//            }
//
//            if (nextIdx == curIdx)
//            {
//                break;
//            }
//
//            swap(_heap[curIdx], _heap[nextIdx]);
//
//            curIdx = nextIdx;
//            leftIdx = curIdx * 2 + 1;
//        }
//    }
//
//    const T& top() const
//    {
//        return _heap[0];
//    }
//
//    bool empty() const // 값 반환이라 반환 타입에 const 안 붙임
//    {
//        return _heap.empty();
//    }
//
//private:
//    vector<T> _heap;
//    Predicate _predicate;
//};


/// ********************************
/// Graph
/// ********************************

//void CreateGraph_1()
//{
//	struct Vertex
//	{
//		// int data;
//		vector<Vertex*> edges;
//	};
//
//	vector<Vertex> v(6);
//
//	v[0].edges.push_back(&v[1]);
//	v[0].edges.push_back(&v[3]);
//
//	v[1].edges.push_back(&v[0]);
//	v[1].edges.push_back(&v[2]);
//	v[1].edges.push_back(&v[3]);
//
//	v[3].edges.push_back(&v[4]);
//
//	v[5].edges.push_back(&v[4]);
//
//	// 0번이 3번과 이어져 있나요?
//
//	bool connected = false;
//
//	int size = v[0].edges.size();
//
//	for (int i = 0; i < size; ++i)
//	{
//		if (v[0].edges[i] == &v[3])
//		{
//			connected = true;
//			break;
//		}
//	}
//}

/// 
/// 위 같이 짜는 게 가장 직관적이겠지만, 저렇게 짜지는 않음.
/// vertex를 직접 가리키는 게 벡터가 크기가 넘어서 재할당되면 댕글링 포인터가 되어서 그런 것도 있는데.
/// 그래프는 root에서 타고 내려갈 수 있는데, 이건 그것도 안되고. (모든 정점이 평등)
/// 그래프는 connection 관계가 중요한데 그게 바로 파악되는 것도 아니고. 그래프의 핵심 장점이 사라짐.
/// 

///
/// 이번에는 '인접 리스트' 라는 방식을 써보자.
/// 자료구조 리스트 말하는 거 아니다. 인접한 것을 리스트로 저장해 관리한다는 뜻.
///

//void CreateGraph_2()
//{
//	struct Vertex
//	{
//		int data;
//	};
//
//	vector<Vertex> v(6);
//
//	vector<vector<int>> adjacent(6); // 각 정점마다 인접한 것의 목록을 저장
//
//	adjacent[0] = { 1,3 }; // 얘는 벡터다. 사실 그래서 위에서 { {1, 3}, { 0, 2, 3 } ... } 형태로 초기화해도 됐다
//	adjacent[1] = { 0, 2, 3 };
//	adjacent[3] = { 4 };
//	adjacent[5] = { 4 };
//
//	//0번이 3번을 가리키고 있나요? 확인
//
//	bool connected = false;
//
//	int size = adjacent[0].size();
//
//	for (int i = 0; i < size; ++i)
//	{
//		if (adjacent[0][i] == 3)
//		{
//			connected = true;
//			break;
//		}
//	}
//}

/// 
/// 그래프마다 장단점이 갈린다.
/// 
/// 지하철 노선도는 서로 드문 드문 연결이 되어있겠지만, (희소 그래프, 인접 리스트 유리)
/// SNS 친구같은 경우는 엄청나게 연결이 많을 것이다. 즉 간선이 많다. 서로 뺵빽하다. (밀집 그래프, 인접 행렬 유리)
/// 
/// 만약 SNS 친구 같은 경우, 인접 리스트 방식의 경우 저 모든 걸 각 유저마다 다 벡터 저장해주어야한다는 건데.
/// 그렇게 따지면 꽤나 비효율적이다. 누구랑 연결되어 있는지 찾을 건데 일일히 O(N)으로 순회할 건가?
/// 그렇다면 차라리 메모리를 아예 많이 써버리고 편하게 하는 건 어떤가.
/// 

/// 
/// 인접 행렬 방식.
/// 말 그대로 인접을 행렬으로 관리한다.
/// 
/// 리스트랑 뭐가 다른데요? -> 리스트는 '이어져 있는 것만' 관리함.
/// 그리고 리스트 형식이라서, 이어져 있는지 아닌지 보려면 전부 순회해야 함.
/// 즉 O(1)이 아니라 O(N)임.
/// 
/// 이건 '이어져 있지 않은 것도' 0으로 관리함.
/// 그러면 '인접 룩업 테이블' 비슷하게 활용할 수 있는 것임.
/// 조회가 O(1)임.
/// 

//void CreateGraph_3()
//{
//	struct Vertex
//	{
//		int data;
//	};
//
//	vector<Vertex> v(6);
//
//	vector<vector<bool>> adjacent(6, vector<bool>(6, false));
//	// bool은 false로 자동 초기화해줘서 이렇게 할 필요는 없다만. 안전상.
//
//	// 대입은 좀 불편하다.
//
//	adjacent[0][1] = true;
//	adjacent[0][3] = true;
//	adjacent[1][0] = true;
//	adjacent[1][2] = true;
//	adjacent[1][3] = true;
//	adjacent[3][4] = true;
//	adjacent[5][4] = true;
//
//	// 0이 3을 가리키고있나요? 바로 대답 가능.
//
//	bool connected = adjacent[0][3];
//
//	// 만약 여기서 가중치를 넣어주고 싶다면?
//	// 그냥 bool 대신 int를 넣으면 된다. -1이면 안 이어져 있는 거고, 그 외는 이어져 있는 것. 보통은 이어져있지 않은 것을 아주 큰 값을 넣는다. 나중에 다익스트라 등 최단 길찾기를 할때 가중치가 적은데 허수 값이면 방해가 된다.
//	// 참고로 자기 자신은 그냥 끊긴 걸로 한다. 괜히 이어져 있는 것을 찾을 때 방해가 될 수 있다.
//
//	vector<vector<int>> adjacent2 =
//	{
//		{ -1, 15, -1, 35, -1, -1},
//		{15, -1, 5, 10, -1, -1},
//		{-1,5,-1,-1,-1,-1},
//		{35, 10, -1, -1, 5, -1},
//		{-1,-1,-1,5,-1,5},
//		{-1,-1,-1,-1,5,-1}
//	};
//}

///
/// 아무튼, 그래프의 핵심은 데이터와 인접 여부를 따로 저장한다는 것이다.
/// 결국 인접이 가장 중요하니까.
/// 

/// 
/// 그런데, 이거 순회는 어떻게 할까? Tree 처럼 재귀로? 그게 생각보다 쉽지 않다는 걸 알 수 있다.
/// Tree는 한 방향으로 계층구조로 내려가는 형식이다. 그래서 재귀로 해결할 수 있었다만.
/// Graph는 누가 확실히 누구 밑에 있는 것도 아니고, 양방향으로 서로를 가리킬 수도 있다.
/// 잘못하면 사이클이 일어나서, 재귀를 빙빙 돌수도 있는 것이다.
/// 
/// 그래서, 이 순회 방식으로 사용하는 것이 DFS, BFS.
/// 

/// 
/// DFS (depth first search)
/// 아무튼 깊게 들어가면 되는 거 아님? 재귀함수 박아!
/// 아니 Tree 같이 쓰면 안 된다면서요. 
/// -> 맞음. Tree 같이 쓰면 안 됨. 이건 양방향이 되니까. 그 부분은 케어해야 함.
/// 내가 지금까지 방문한 애들을 추적을 해서, 거기는 가지 않게 한다.
/// 

// Vertex는 진짜 데이터 담는 역할만.

struct Vertex
{
	
};

vector<Vertex> vertices;
vector<vector<int>> adjacent;

// 내가 방문한 곳을 추척해야 함!
vector<bool> visited;

void CreateGraph()
{
	vertices.resize(6);

	// 인접 리스트
	//adjacent = vector<vector<int>>(6);
	//adjacent[0] = { 1,3 };
	//adjacent[1] = { 0, 2, 3 };
	//adjacent[3] = { 4 };
	//adjacent[5] = { 4 };

	// 인접 행렬
	adjacent = vector<vector<int>>
	{
		{0,1,0,1,0,0},
		{1,0,1,1,0,0},
		{0,0,0,0,0,0},
		{0,0,0,0,1,0},
		{0,0,0,0,0,0},
		{0,0,0,0,1,0}
	};
}

/// 
/// DFS 재귀함수버전
///

void DFS(int here)
{
	visited[here] = true;

	cout << "Visited : " << here << '\n';

	// 인접 리스트 버전. adjacent 자기 리스트 안에 있는 것만 체크하면 됨
	//for (int i = 0; i < adjacent[here].size(); ++i)
	//{
	//	int there = adjacent[here][i];
	//	if (visited[there] == false)
	//	{
	//		DFS(there);
	//	}
	//}

	// 인접 행렬 버전. 뭐가 이어진지 봐야함

	int size = adjacent.size();

	for (int i = 0; i < size; ++i)
	{
		if (adjacent[here][i] == 1 && visited[i] == false)
		{
			DFS(i);
		}
	}
	
	// 공통적으로 adjacent[내 위치][가는 위치] 이다.
}

///
/// 인접 리스트의 DFS 시간 복잡도 O(V+E)
/// 
/// V는 정점의 총 갯수, E는 간선의 총 갯수이다.
/// 왜 VE가 아닌 V+E일까.
/// 모든 V가 같은 E만큼의 간선을 갖고 있는 것이 아니다.
/// V에서 E만큼 for문을 돌지 않는다.
/// 
/// 인접 행렬의 DFS 시간 복잡도 O(V^2)
/// 
/// V개의 정점을 한 번씩 방문하고, 거기서 각 정점마다 V번씩 for문을 돈다.
/// visited로 정점은 한 번씩만 들르기는 하지만, for문은 정직하다. 무조건 돌아아야한다.
/// 그러므로 시간 복잡도는 O(V^2)이다.
///
/// 그렇다고 인접 리스트가 무조건 더 빠를까? 그건 아니다. 그건 +의 함정이다.
/// 만약 모든 정점이 연결되어있다고 하면, E는 V^2이다.
/// 즉 O(V^2 + V) = O(V^2)이다.
/// 
/// 즉, 그냥 상황따라 다르다는 것이다. 
/// 희소 그래프는 인접 리스트, 밀집 그래프는 인접 행렬이 조회에 더 우수하다.
/// 물론 비슷하다면 조회가 O(1)로 되는 인접 행렬이 더 우수할 것이다.
/// 트레이드 오프는 언제나 있다. 정답은 없다! 절대로!
/// 
/// 근데 그렇다고 DFS가 특히 느린거냐면, 그렇지도 않다. 사실 BFS도 똑같다.
/// 
/// DFS의 문제는 다음과 같다.
/// 1. 재귀함수 특유의 스택 오버플로우 문제. (대충 N이 10만 이상이면 터진다)
/// 2. BFS가 아님으로 인한 최단 경로 서치할때의 백트래킹 문제. (엉뚱한 길을 끝까지 들어갔다가 나옴)
/// 

/// 
/// 만약 고립된 정점까지 모두 스캔하고 싶다면? 그때는 모든 정점에 대해 DFS를 for문으로 한번씩 돌려주면 됨.
/// 그냥 DFS안의 인접 했을때만 가는 조건 없애서 한번씩 들러줘도 되는데... 이건 사실 DFS의 정체성을 잃어버림.
/// 그냥 시작점부터 순차적으로 들르는 게 끝임. 가짜 순회지. 그래서 안 쓰는 것
/// 

void DfsAll()
{
	int size = adjacent.size();
	
	for (int i = 0; i < size; ++i)
	{
		if (visited[i] == false)
		{
			DFS(i);
		}
	}
}

/// 
/// 근데 재귀함수로 안 하고 싶다면 어떻게 해야할까? stack으로 구현하자.
/// 재귀함수 = stack이다. 이건 진짜 기억해두면 좋음. 
/// 재귀는 이어진 정점들의 앞부터 들르는데, 스택은 뒤부터 들름.
/// 

void StackDfs(int here)
{
	stack<int> dfsStack;
	dfsStack.push(here);
	visited[here] = true;

	// 인접 리스트 버전

	//while (dfsStack.empty() == false)
	//{
	//	int here = dfsStack.top();

	//	cout << "Visited : " << here << '\n';

	//	dfsStack.pop();

	//	int size = adjacent[here].size();

	//	for (int i = 0; i < size; ++i)
	//	{
	//		int there = adjacent[here][i];
	//		if (visited[there] == false)
	//		{
	//			dfsStack.push(there);
	//			visited[there] = true;
	//		}
	//	}
	//}

	// 인접 행렬 버전

	int size = adjacent.size();

	while (dfsStack.empty() == false)
	{
		here = dfsStack.top();

		cout << "Visited : " << here << '\n';

		dfsStack.pop();

		for (int i = 0; i < size; ++i)
		{
			
			if (adjacent[here][i] == 1 && visited[i] == false)
			{
				dfsStack.push(i);
				visited[i] = true;
				// 여기서 visited[i] = true를 안해주고 pop 할때 해주면, 
				// 자기랑 같이 들어온 애들이 stack에 들어왔는데 아직 false 상태라, 걔들을 다시 넣어버리는 문제가 발생.
			}
		}
	}
}

///
/// BFS (Breadth First Search) 너비 우선 서치
/// 먼저 발견한 놈에게 먼저 접근한다. = Queue를 사용한다.
/// visited를 사용하지 않는다. discovered를 사용한다.
/// 
/// BFS는 여러가지를 좀 기록해주자! 얘는 누가 발견했는지, (최단 거리 경로 추적)
/// start로부터 얼만큼 떨어져 있는지 정보 수집이 좀 중요해서. (최단 거리)
/// 

vector<bool> discovered;

void BFS(int here)
{
	vector<int> parent(6, -1); // -1이면 아무도 발견 못한 것. (고립)

	vector<int> dist(6, -1); // -1이면 아무도 발견 못한 것. (고립)

	queue<int> q;
	q.push(here);
	discovered[here] = true; // stack 형식 DFS와 유사하죠? 예약과 visited/discovered는 같이 움직임.

	parent[here] = here; // 시작점은 자기가 자신을 발견한 것으로 처리
	dist[here] = 0; // 시작점과의 거리는 0. (자기 자신)

	//// 인접 리스트 방식

	//while (q.empty() == false)
	//{
	//	here = q.front();

	//	cout << "Visited : " << here << '\n';

	//	q.pop();

	//	int size = adjacent[here].size();

	//	for (int i = 0; i < size; ++i)
	//	{
	//		int there = adjacent[here][i];
	//		if (discovered[there] == false)
	//		{
	//			q.push(there);
	//			discovered[there] = true;
	//		}
	//	}
	//}

	// 인접 행렬 방식

	int size = adjacent.size();

	// 같은 깊이 애들을 동시에 돌려줄 필요가 없는게, 어차피 큐에서 FIFO로 알아서 먼저 나올거라서.

	while (q.empty() == false)
	{
		here = q.front();
		
		cout << "Visited : " << here << '\n';

		q.pop();

		for (int i = 0; i < size; ++i)
		{		
			if(adjacent[here][i] == 1 && discovered[i] == false)
			{
				q.push(i);
				discovered[i] = true;

				parent[i] = here;
				dist[i] = dist[here] + 1;
			}
		}
	}
}

void BfsAll()
{
	int size = adjacent.size();

	for (int i = 0; i < size; ++i)
	{
		if (discovered[i] == false)
		{
			BFS(i);
		}
	}
}


/// 
/// BFS의 시간 복잡도는... DFS와 같다.
/// 인접 리스트 시의 조회 -> V마다 E개의 만큼 반복문을 도는 것은 변하지 않는다. 방문은 안 하더라도.
/// 
/// 인접 행렬 시의 조회 -> 이것도 V마다 V개의 for문을 도는 것도 변하지 않는다.
/// 
/// 그냥 이유가 단순하다. 어차피 그만큼 도는 건 똑같다. 그런데 queue면 FIFO로 탐색하고 stack은 LIFO로 탐색할 뿐이다.
/// 그게 끝이다.
/// 

int main()
{
	CreateGraph();

	cout << "DFS-----\n";
	visited = vector<bool>(6, false);
	//StackDfs(0);
	//DFS(0);
	DfsAll();

	cout << "BFS-----\n";
	discovered = vector<bool>(6, false);
	//BFS(0);
	BfsAll();
}

///
/// DFS : 그럼 여기서 3번은 0번에 의해 들러지는 걸까? 1번에 의해 들러지는 걸까? 하면 1번이다.
/// 0->1->2->1->3->4->3->1->0 으로 가고, 0이 3으로 가려했더니 visited가 찍혀있어서 못 간다.
/// 보통 재귀문을 저렇게 for문으로 돌면 자식의 for문이 전부 실행되고 return 된다.
/// 물론 구조에 따라 다르겠다만. 재귀 자체를 for문으로 돌면 그렇다.
/// 
/// BFS : 0 pop -> (1, 3) -> 1 pop -> (3, 2) -> 3 pop -> (2, 4) -> 2 pop -> (4) -> 4 pop
/// 그냥 먼저 넣어진 놈이 자기와 가장 가까운 애들을 다 넣고 가는 거다.
/// 
/// 큐에 넣어지는 건 무조건. 무조건 거리 순서이다. 나중에 최단 거리가 되는 애가 나오는 경우 같은 게 있을 수가 없다.
/// [1칸, 1칸 .. 2칸, 2칸 ..., 3칸, 3칸...]
/// 그냥 FIFO라서 무조건이다. 1칸 떨어진 애들이 2칸 떨어진 애들을 넣고, 2칸 떨어진 애들이 3칸 떨어진 애들을 넣고.
/// 무조건 가까운 애들부터 처리되므로.
/// 