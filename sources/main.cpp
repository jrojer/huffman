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
    const int max_filesize = 5 * 1024 * 1024;
    std::ifstream input( filename, std::ios::binary);
    if(input.fail())
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
    if(output.fail())
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
    
    //Test1(params);

    std::vector<uint8_t> file;
    try
    {
        file = ReadFile(params.input_filename);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 0;
    }

    ProcessResult output;
    if(params.mode == Parameters::kEncode)
    {
        output = Encode(file);
    }
    else
    {
        output = Decode(file);
    }

    std::cout << output.raw_size << std::endl;
    std::cout << output.processed_size << std::endl;
    std::cout << output.meta_size << std::endl;

    if(params.verbose)
    {
        if(params.mode == Parameters::kEncode)
        {
            PrintCodeTable(file,std::cout);
        }
        else
        {
            PrintCodeTable(output.buffer,std::cout);
        }
    }

    try
    {
        WriteFile(output.buffer,params.output_filename);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 0;
    }

    return 0;
}
