#pragma once
#include <string>

namespace huffman
{
struct Parameters
{
    enum Mode { kEncode, kDecode };
    Mode        mode;
    bool        verbose;
    std::string input_filename;
    std::string output_filename;
    bool        error;
};

Parameters ParseCmdArgs(int argc, char** argv);
void Test1(const Parameters& params);
}  // namespace huffman
