#pragma once
#include <cstddef>  // size_t
#include <cstdint>  // uint
#include <iostream>
#include <vector>
#include <array>

namespace huffman
{
const size_t kAlphabetSize = 256;
struct ProcessResult
{
    size_t               raw_size;
    size_t               processed_size;
    size_t               meta_size;
    std::vector<uint8_t> buffer;
    std::array<std::vector<int>, kAlphabetSize> code_table;
};

ProcessResult Encode(const std::vector<uint8_t>& chunk);
ProcessResult Decode(const std::vector<uint8_t>& chunk);
void          PrintCodeTable(const std::array<std::vector<int>, kAlphabetSize>& code_table, std::ostream& out);

}  // namespace huffman
