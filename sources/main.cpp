#include <iostream>
#include"cmd_args.h"

using namespace huffman;

int main(int argc, char **argv)
{
    auto params = ParseCmdArgs(argc, argv);

    if(params.error)
    {
        std::cout << "Usage: huffman [-v] (-c|-d) input_file output_file" << std::endl;
        return 0;
    }
    Test1(params);

    return 0;
}
