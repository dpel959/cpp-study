#include <iostream>
#include <vector>
using std::vector, std::cout;

class Node
{
public:
    Node(const char* data) : data(data) {}
public:
    const char* data;
    vector<Node*> children;
};

Node* CreateTree()
{
    Node* root = new Node("R1 개발실");
    {
        Node* node = new Node("디자인");
        root->children.push_back(node);
        {
            Node* leaf = new Node("전투");
            node->children.push_back(leaf);
        }
        {
            Node* leaf = new Node("경제");
            node->children.push_back(leaf);
        }
        {
            Node* leaf = new Node("스토리");
            node->children.push_back(leaf);
        }
    }
    {
        Node* node = new Node("프로그래밍");
        root->children.push_back(node);
        {
            Node* leaf = new Node("클라");
            node->children.push_back(leaf);
        }
        {
            Node* leaf = new Node("서버");
            node->children.push_back(leaf);
        }
        {
            Node* leaf = new Node("엔진");
            node->children.push_back(leaf);
        }
    }
    {
        Node* node = new Node("아트");
        root->children.push_back(node);
        {
            Node* leaf = new Node("배경");
            node->children.push_back(leaf);
        }
        {
            Node* leaf = new Node("캐릭터");
            node->children.push_back(leaf);
        }
    }

    return root;
}

void PrintTree(Node* root, int depth = 0)
{
    for (int i = 0; i < depth; ++i)
    {
        cout << '-';
    }

    cout << root->data << '\n';

    int size = root->children.size();

    for (int i = 0; i < size; ++i)
    {
        PrintTree(root->children[i], depth + 1);
    }
}

int GetHeight(Node* root)
{
    int ret = 1;

    int size = root->children.size();

    for (int i = 0; i < size; ++i)
    {
        int h = GetHeight(root->children[i]) + 1;
        ret = std::max(ret, h);
    }

    return ret;
}

int main()
{
    Node* root = CreateTree();
    PrintTree(root);
    cout << GetHeight(root) << '\n';
}