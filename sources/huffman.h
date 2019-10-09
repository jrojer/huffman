#pragma once
#include <algorithm>
#include <array>
#include <vector>

namespace huffman
{
struct HistItem
{
    uint8_t value;
    size_t  weight;
};

bool operator<(const HistItem& lhs, const HistItem& rhs)
{
    return lhs.weight > rhs.weight;
}

std::vector<HistItem> MakeHistogram(const std::vector<uint8_t>& items)
{
    const int alphabet_size = 256;

    std::array<size_t, alphabet_size> hist = {};
    for (auto x : items)
    {
        ++hist[x];
    }
    std::vector<HistItem> res;
    for (int i = 0; i < alphabet_size; ++i)
    {
        if (hist[i] > 0)
            res.push_back({ static_cast<uint8_t>(i), hist[i] });
    }
    return res;
}

struct Node
{
    uint8_t value;
    size_t  weight;
    Node*   left;
    Node*   right;
    bool    visited;
};

Node* MakeTree(const std::vector<HistItem>& hist)
{
    std::vector<Node*> heap;
    for (auto x : hist)
    {
        heap.push_back(new Node{ x.value, x.weight, nullptr, nullptr,false });
    }
    const auto cmp = [](Node* a, Node* b) { return a->weight > b->weight; };
    std::make_heap(heap.begin(), heap.end(), cmp);
    while (heap.size() > 1)
    {
        Node* n1 = heap.front();
        std::pop_heap(heap.begin(), heap.end(), cmp);
        Node* n2 = heap.front();
        std::pop_heap(heap.begin(), heap.end(), cmp);
        heap.push_back(new Node{ 0, n1->weight + n2->weight, n1, n2,false });
        std::push_heap(heap.begin(), heap.end(), cmp);
    }
    Node* root = heap.front();
    return root;
}

std::array<std::vector<int>, 256> GetCodeTable(Node* root)
{
    std::array<std::vector<int>, 256> res = {};
    std::vector<Node*>          stack(1, root);
    std::vector<int>            code;
    while (!stack.empty())
    {
        Node* node = stack.back();
        node->visited = true;
        if (node->left && node->right)
        {
            if(!node->left->visited)
            {
                stack.push_back(node);
                stack.push_back(node->left);
                code.push_back(0);
            }
            else if(!node->right->visited)
            {
                stack.push_back(node);
                stack.push_back(node->right);
                code.push_back(1);
            }
            else
            {
                stack.pop_back();
                if(!code.empty())
                    code.pop_back();
            }
        }
        else
        {
            res[node->value] = code;
            stack.pop_back();
            if(!code.empty())
                code.pop_back();
        }
    }
    return res;
}

void DeleteTree(Node* root)
{
    std::vector<Node*> stack(1, root);
    while (!stack.empty())
    {
        Node* node = stack.back();
        stack.pop_back();
        if(node != nullptr)
        {
            stack.push_back(node->left);
            stack.push_back(node->right);
            delete node;
        }
    }
}

std::vector<uint8_t> Encode(const std::vector<uint8_t>& chunk)
{
    const auto hist       = MakeHistogram(chunk);
    const auto tree       = MakeTree(hist);
    const auto code_table = GetCodeTable(tree);
    // process chunk with code table
    // ...
    DeleteTree(tree);
    return {};
}
std::vector<uint8_t> Decode(const std::vector<uint8_t>& chunk)
{
    // for
    // get bit, traversing tree

    return {};
}
}  // namespace huffman
