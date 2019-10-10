#include "huffman.h"
#include <algorithm>
#include <array>
#include <cstring>

namespace huffman
{
const size_t kByteLen      = 8;
const size_t kAlphabetSize = 256;

using histogram_t = std::array<uint32_t, kAlphabetSize>;

histogram_t MakeHistogram(const std::vector<uint8_t>& items)
{
    histogram_t hist = {};
    for (auto x : items)
    {
        ++hist[x];
    }
    return hist;
}

struct Node
{
    uint8_t  value;
    uint32_t weight;
    Node*    left;
    Node*    right;
    bool     visited;
};

Node* MakeTree(const histogram_t& hist)
{
    std::vector<Node*> heap;
    for (size_t i = 0; i < hist.size(); ++i)
    {
        uint8_t  value  = static_cast<uint8_t>(i);
        uint32_t weight = hist[i];
        if (weight > 0)
            heap.push_back(new Node{ value, weight, nullptr, nullptr, false });
    }
    const auto cmp = [](Node* a, Node* b) { return a->weight > b->weight; };
    std::make_heap(heap.begin(), heap.end(), cmp);
    while (heap.size() > 1)
    {
        Node* n1 = heap.front();
        std::pop_heap(heap.begin(), heap.end(), cmp);
        heap.pop_back();
        Node* n2 = heap.front();
        std::pop_heap(heap.begin(), heap.end(), cmp);
        heap.pop_back();
        heap.push_back(new Node{ 0, n1->weight + n2->weight, n1, n2, false });
        std::push_heap(heap.begin(), heap.end(), cmp);
    }
    Node* root = heap.front();
    return root;
}

std::array<std::vector<int>, kAlphabetSize> GetCodeTable(Node* root)
{
    std::array<std::vector<int>, kAlphabetSize> res = {};
    std::vector<Node*>                          stack(1, root);
    std::vector<int>                            code;
    while (!stack.empty())
    {
        Node* node = stack.back();
        stack.pop_back();
        node->visited = true;
        if (node->left && node->right)
        {
            if (!node->left->visited)
            {
                stack.push_back(node);
                stack.push_back(node->left);
                code.push_back(0);
            }
            else if (!node->right->visited)
            {
                stack.push_back(node);
                stack.push_back(node->right);
                code.push_back(1);
            }
            else
            {
                if (!code.empty())
                    code.pop_back();
            }
        }
        else
        {
            res[node->value] = code;
            if (!code.empty())
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
        if (node != nullptr)
        {
            stack.push_back(node->left);
            stack.push_back(node->right);
            delete node;
        }
    }
}

int GetBit(const uint8_t* src, size_t pos)
{
    size_t byte_pos = pos / kByteLen;
    size_t bit_pos  = pos % kByteLen;
    return (src[byte_pos] >> bit_pos) & 1;
}

void SetBit(uint8_t* dst, size_t pos, int bit)
{
    size_t byte_pos = pos / kByteLen;
    size_t bit_pos  = pos % kByteLen;
    dst[byte_pos] &= ~(1 << bit_pos);
    dst[byte_pos] ^= (bit << bit_pos);
}

histogram_t ReadHistogram(const uint8_t* src)
{
    histogram_t hist;
    for (size_t i = 0; i < hist.size(); ++i)
    {
        uint32_t weight;
        memcpy(&weight, src, sizeof(weight));
        hist[i] = weight;
        src += sizeof(weight);
    }
    return hist;
}

void WriteHistogram(uint8_t* dst, const histogram_t& hist)
{
    for (uint32_t x : hist)
    {
        memcpy(dst, &x, sizeof(x));
        dst += sizeof(x);
    }
}

struct MetaObject
{
    histogram_t         hist;
    uint64_t            bitlen;
    static const size_t hist_size = kAlphabetSize * sizeof(uint32_t);
    static const size_t size      = hist_size + sizeof(bitlen);
};

MetaObject ReadMetaObject(const uint8_t* src)
{
    auto     hist = ReadHistogram(src);
    uint64_t bitlen;
    memcpy(&bitlen, src + MetaObject::hist_size, sizeof(bitlen));
    return { hist, bitlen };
}

void WriteMetaObject(uint8_t* dst, const MetaObject& obj)
{
    WriteHistogram(dst, obj.hist);
    memcpy(dst + MetaObject::hist_size, &obj.bitlen, sizeof(obj.bitlen));
}

ProcessResult Encode(const std::vector<uint8_t>& chunk)
{
    const auto hist       = MakeHistogram(chunk);
    const auto tree       = MakeTree(hist);
    const auto code_table = GetCodeTable(tree);

    std::vector<uint8_t> result(MetaObject::size + chunk.size());

    uint64_t result_bitlen = 0;

    uint8_t* dst = result.data() + MetaObject::size;
    for (uint8_t x : chunk)
    {
        for (int bit : code_table[x])
        {
            SetBit(dst, result_bitlen, bit);
            ++result_bitlen;
        }
    }
    DeleteTree(tree);
    const size_t compressed_size = (result_bitlen + kByteLen - 1) / kByteLen;
    result.resize(MetaObject::size + compressed_size);
    WriteMetaObject(result.data(), { hist, result_bitlen });
    return { chunk.size(), compressed_size, MetaObject::size, result };
}

ProcessResult Decode(const std::vector<uint8_t>& chunk)
{
    auto meta = ReadMetaObject(chunk.data());
    auto root = MakeTree(meta.hist);

    const uint8_t* src = chunk.data() + MetaObject::size;

    Node* node = root;

    std::vector<uint8_t> result;
    for (size_t i = 0; i < meta.bitlen; ++i)
    {
        int bit = GetBit(src, i);
        if (node->left && node->right)
        {
            node = bit == 0 ? node->left : node->right;
            if (!(node->left && node->right))
            {
                result.push_back(node->value);
                node = root;
            }
        }
    }
    DeleteTree(root);
    return { chunk.size() - MetaObject::size, result.size(), MetaObject::size, result };
}

struct CodeItem
{
    size_t      value;
    std::string code;
};

void PrintCodeTable(const std::vector<uint8_t>& chunk, std::ostream& out)
{
    const auto hist       = MakeHistogram(chunk);
    const auto tree       = MakeTree(hist);
    const auto code_table = GetCodeTable(tree);

    std::vector<CodeItem> table;
    for (size_t i = 0; i < kAlphabetSize; ++i)
    {
        if (code_table[i].empty())
            continue;

        std::string s;
        for (int x : code_table[i]) s += x == 0 ? "0" : "1";
        table.push_back({ i, s });
    }

    std::sort(table.begin(), table.end(), [](const auto& a, const auto& b) { return a.code < b.code; });

    for (const auto& item : table)
    {
        out << item.code << " " << item.value << std::endl;
    }
}

}  // namespace huffman
