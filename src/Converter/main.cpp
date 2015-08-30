#include <iostream>
#include <fstream>
#include <string>

#include "cXConverter.h"
#include "cObjConverter.h"
#include "cBlkConverter.h"

#include <Utilities/ut.h>

int main(int argc, char* argv[])
{
    if (argc != 2 && argc != 3)
    {
        std::cout << "Usage: Converter.exe <input_file> <output_file>" << std::endl;
        std::cout << "Usage: Converter.exe <input_file>" << std::endl;
        return 1;
    }

    std::string open = argv[1];
    std::string save = "";
    auto ext = utils::ExtractExtension(utils::Widen(open));

    if (argc == 2)
    {
        save = std::string(open).erase(open.size() - ext.size()) + "lim";
        std::cout << "Saving to default " << save << std::endl;
    }
    else
    {
        save = argv[2];
    }

    std::unique_ptr<IConverter> converter = nullptr;

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

    std::ifstream input;
    input.open(open.c_str());

    if (input.fail() || !input.is_open())
    {
        std::cout << "Unable to open file: " << open << std::endl;
        return 1;
    }

    std::ofstream output;
    output.open(save.c_str(), std::ios_base::binary);

    if (output.fail() || !output.is_open())
    {
        std::cout << "Unable to open file: " << save << std::endl;
        return 1;
    }

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    MeshLIMHeader header;
    if (!converter->Convert(input, vertices, indices, header))
    {
        std::cout << "Unable to convert" << std::endl;
        return 1;
    }

    std::cout << "Converted: " << vertices.size() << " vertices, " << indices.size() << " indices" << std::endl;
    if (!LIMSaver::Save(output, vertices.data(), indices.data(), header))
    {
        std::cout << "Unable to save file " << save << std::endl;
        return 1;
    }
    return 0;
}
