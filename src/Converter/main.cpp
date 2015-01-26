#include <iostream>
#include <fstream>
#include <string>

#include "cXConverter.h"
#include "cObjConverter.h"
#include "cBlkConverter.h"

#include <Utilities/ut.h>

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: Converter.exe <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string open = argv[1];
    std::string save = argv[2];

    std::unique_ptr<IConverter> converter = nullptr;

    auto ext = utils::ExtractExtension(utils::StrToWStr(open));
    if (ext == L"x")
    {
        converter.reset(new cXConverter());
    }
    else if (ext == L"obj")
    {
        converter.reset(new cObjConverter());
    }
    else if (ext == L"block" || ext == L"blk")
    {
        converter.reset(new cBlkConverter());
    }
    else
    {
        std::cout << "Unknown extension. Unable to convert." << std::endl;
        return 1;
    }

    if (!converter->Convert(open, save))
    {
        std::cout << "Unable to convert" << std::endl;
        return 1;
    }

    return 0;
}
