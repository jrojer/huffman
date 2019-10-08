#include "cmd_args.h"

#include <iostream>

namespace huffman
{
Parameters ParseCmdArgs(int argc, char** argv)
{
    Parameters result = {};
    result.error      = true;
    if (!(argc == 4 || argc == 5))
    {
        return result;
    }
    int         i   = 1;
    std::string key = argv[i];
    if (argc == 5)
    {
        if (key == "-v")
        {
            result.verbose = true;
            ++i;
        }
        else
        {
            return result;
        }
    }
    key = argv[i];
    if (key == "-c")
    {
        result.mode = Parameters::kEncode;
    }
    else if (key == "-d")
    {
        result.mode = Parameters::kDecode;
    }
    else
    {
        return result;
    }
    result.input_filename  = argv[++i];
    result.output_filename = argv[++i];
    result.error           = false;
    return result;
}

void Test1(const Parameters& params)
{
    if (params.verbose)
        std::cout << "verbose" << std::endl;
    if (params.mode == Parameters::kEncode)
        std::cout << "encode" << std::endl;
    if (params.mode == Parameters::kDecode)
        std::cout << "decode" << std::endl;

    std::cout << "input " << params.input_filename << std::endl;
    std::cout << "output " << params.output_filename << std::endl;
}
}  // namespace huffman
