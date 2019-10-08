#pragma once
#include <algorithm>
#include <array>
#include <map>
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

std::vector<HistItem> MakeHistogram(std::vector<uint8_t> items)
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
            res.push_back({ i, hist[i] });
    }
    return res;
}

struct Node
{
    uint8_t value;
    size_t  weight;
    Node*   left;
    Node*   right;
};


std::vector<uint8_t> Encode(const std::vector<uint8_t>& chunk)
{
    auto hist = MakeHistogram(chunk);
    std::vector<Node*> heap;
    for(auto x: hist)
    {
        heap.push_back(new Node{x.value,x.weight,nullptr,nullptr});
    }
    const auto cmp = [](Node* a, Node* b){return a->weight > b->weight;};
    std::make_heap(heap.begin(), heap.end(),cmp);
    while (heap.size() > 1)
    {
        Node* n1 = heap.front();
        std::pop_heap(heap.begin(), heap.end(),cmp);
        Node* n2 = heap.front();
        std::pop_heap(heap.begin(), heap.end(),cmp);
        heap.push_back(new Node{0,n1->weight+n2->weight,n1,n2});
        std::push_heap(heap.begin(), heap.end(),cmp);
    }
    Node* root = heap.front();
    std::vector<Node*> stack(1,root);
    std::vector<int> code;
    while(!stack.empty())
    {
        Node* node = stack.back();
        stack.pop_back();
        if(node->left && node->right)
        {

        }
    }
    for(auto x: heap)
    {
        delete x;
    }
}
std::vector<uint8_t> Decode(const std::vector<uint8_t>& chunk)
{}
}  // namespace huffman
