#pragma once
#include <cstddef>  // size_t
#include <cstdint>  // uint
#include <iostream>
#include <vector>

namespace huffman
{
struct ProcessResult
{
    size_t               raw_size;
    size_t               processed_size;
    size_t               meta_size;
    std::vector<uint8_t> buffer;
};

ProcessResult Encode(const std::vector<uint8_t>& chunk);
ProcessResult Decode(const std::vector<uint8_t>& chunk);
void          PrintCodeTable(const std::vector<uint8_t>& chunk, std::ostream& out);

}  // namespace huffman
