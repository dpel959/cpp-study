#include <iostream>
#include <vector>
#include <utility>
using std::vector, std::cout, std::swap, std::less, std::greater;

///
/// Tree
/// 

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

/// 
/// PriorityQueue
/// 

template <typename T, typename Predicate = less<T>>
class PriorityQueue
{
public:
    void push(const T& data)
    {
        // 일단 힙 구조 맞추기
        _heap.push_back(data);

        // 도장깨기 시작
        int curIdx = static_cast<int>(_heap.size()) - 1;

        // 처음엔 curIdx에서 값 < parentIdx에서 값로 만들었는데
        // 같은 코드를 반복해 쓰게 되어서, 좋지 않게 되어 수정
        while (curIdx > 0) 
        {
            int parentIdx = (curIdx - 1) / 2;

            if (_predicate(_heap[parentIdx], _heap[curIdx]))
            {
                swap(_heap[curIdx], _heap[parentIdx]);
            }
            else
            {
                break;
            }

            curIdx = parentIdx;
        }
    }

    void pop()
    {
        if (empty())
        {
            return;
        }

        _heap[0] = _heap.back();
        _heap.pop_back();
        int curIdx = 0;
        int leftIdx = curIdx * 2 + 1;
        int rightIdx;

        int heapSize = _heap.size();

        while (leftIdx < heapSize)
        {
            int nextIdx = curIdx;
            
            if (_predicate(_heap[nextIdx], _heap[leftIdx]))
            {
                nextIdx = leftIdx;
            }

            rightIdx = curIdx * 2 + 2;

            if (rightIdx < heapSize && _predicate(_heap[nextIdx], _heap[rightIdx]))
            {
                nextIdx = rightIdx;
            }

            if (nextIdx == curIdx)
            {
                break;
            }

            swap(_heap[curIdx], _heap[nextIdx]);

            curIdx = nextIdx;
            leftIdx = curIdx * 2 + 1;
        }
    }

    const T& top() const
    {
        return _heap[0];
    }

    bool empty() const // 값 반환이라 반환 타입에 const 안 붙임
    {
        return _heap.empty();
    }

private:
    vector<T> _heap;
    Predicate _predicate;
};

int main()
{
    PriorityQueue<int, greater<int>> pq;
    
    pq.push(10);
    pq.push(40);
    pq.push(30);
    pq.push(50);
    pq.push(20);

    int value = pq.top();
    pq.pop();
}