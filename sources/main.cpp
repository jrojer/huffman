#include <iostream>
#include <fstream>
#include <string>
#include "huffman.h"

using namespace huffman;

struct Parameters
{
    enum Mode { kEncode, kDecode };
    Mode        mode;
    bool        verbose;
    std::string input_filename;
    std::string output_filename;
    bool        error;
};

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

std::vector<uint8_t> ReadFile(const std::string& filename)
{
    const size_t max_filesize = 5 * 1024 * 1024;
    std::ifstream input( filename, std::ios::binary);
    if(std::ios::failbit)
    {
        throw std::invalid_argument("cannot open file " + filename);
    }
    if(input.tellg() > max_filesize)
    {
        throw std::invalid_argument("input file is too large: " + filename);
    }
    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(input), {});
    return buffer;
}

void WriteFile(const std::vector<uint8_t>& file, const std::string& filename)
{
    std::ofstream output( filename, std::ios::binary);
    if(std::ios::failbit)
    {
        throw std::invalid_argument("cannot open file " + filename);
    }
    std::copy(file.cbegin(),file.cend(), std::ostreambuf_iterator<char>(output));
}

int main(int argc, char **argv)
{
    auto params = ParseCmdArgs(argc, argv);

    if(params.error)
    {
        std::cout << "Usage: huffman [-v] (-c|-d) input_file output_file" << std::endl;
        return 0;
    }
    
    Test1(params);

    auto file = ReadFile(params.input_filename);

    if(params.verbose)
    {
        PrintCodeTable(file,std::cout);
    }

    if(params.kEncode)
    {

    }
    else
    {

    }

    return 0;
}
