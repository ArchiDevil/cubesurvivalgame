#include "cXConverter.h"

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

bool cXConverter::Convert(const std::string & in, const std::string & out)
{
    std::ifstream input;
    input.open(in.c_str());
    if (input.fail())
        return false;

    std::string bufStr;
    std::stringstream Reader;
    unsigned int DelimiterPosition;

    while (input >> bufStr)
        if (bufStr == "Mesh")
            break;  //we found beginning of mesh description

    getline(input, bufStr);
    getline(input, bufStr, ';');
    Reader << bufStr;
    unsigned int verticesCount; //count of vertices
    Reader >> verticesCount;
    Reader.clear();
    bufStr.clear();

    bool haveNormals = false;
    bool haveTextureCoords = false;

    std::vector<Vertex> Mesh(verticesCount);
    unsigned int VertexNum = 0;

    while (getline(input, bufStr))
    {
        if (bufStr.size() == 0)
            continue;
        else if (bufStr[0] != '-' && bufStr[0] != '\t' && bufStr[0] != ' ' && !::isdigit(bufStr[0]))
            continue;

        for (int index = 0; index < 3; index++)
        {
            DelimiterPosition = bufStr.find(';');
            std::string curKey = bufStr.substr(0, DelimiterPosition);
            bufStr = bufStr.substr(DelimiterPosition + 1, bufStr.size());

            Reader << curKey;
            Reader >> Mesh[VertexNum].Pos.el[index];
            Reader.clear();
        }

        VertexNum++;

        if (bufStr.size() != 0 && bufStr[bufStr.size() - 1] == ';') //last element is ';'
            break;
    }


    getline(input, bufStr);
    getline(input, bufStr, ';');

    Reader << bufStr;
    int indicesCount;   //count of indices
    Reader >> indicesCount;
    Reader.clear();
    bufStr.clear();

    const int indicesInString = 3;
    std::vector<unsigned long> Ind(indicesCount * indicesInString);

    VertexNum = 0;

    while (getline(input, bufStr))
    {
        if (bufStr.size() == 0)
            continue;
        else if (bufStr[0] != '-' && bufStr[0] != '\t' && bufStr[0] != ' ' && !::isdigit(bufStr[0]))
            continue;

        DelimiterPosition = bufStr.find(';');
        bufStr = bufStr.substr(DelimiterPosition + 1, bufStr.size());

        for (int index = 0; index < 3; index++)
        {
            if (index != 2)
                DelimiterPosition = bufStr.find(',');
            else
                DelimiterPosition = bufStr.find(';');

            std::string curKey = bufStr.substr(0, DelimiterPosition);
            bufStr = bufStr.substr(DelimiterPosition + 1, bufStr.size());

            Reader << curKey;
            Reader >> Ind[VertexNum * indicesInString + index];
            Reader.clear();
        }

        VertexNum++;

        if (bufStr.size() != 0 && bufStr[bufStr.size() - 1] == ';') //last element is ';'
            break;
    }

    while (getline(input, bufStr))
    {
        if (bufStr.find("MeshNormals") != std::string::npos)
        {
            haveNormals = true;
            break;
        }
    }

    getline(input, bufStr);

    if (haveNormals)
    {
        VertexNum = 0;

        while (getline(input, bufStr))
        {
            if (bufStr.size() == 0)
                continue;
            else if (bufStr[0] != '-' && bufStr[0] != '\t' && bufStr[0] != ' ' && !::isdigit(bufStr[0]))
                continue;

            for (int index = 0; index < 3; index++)
            {
                DelimiterPosition = bufStr.find(';');
                std::string curKey = bufStr.substr(0, DelimiterPosition);
                bufStr = bufStr.substr(DelimiterPosition + 1, bufStr.size());

                Reader << curKey;
                Reader >> Mesh[VertexNum].Normal.el[index];
                Reader.clear();
            }

            VertexNum++;

            if (bufStr.size() != 0 && bufStr[bufStr.size() - 1] == ';') //last element is ';'
                break;
        }
    }

    while (getline(input, bufStr))
    {
        if (bufStr.find("MeshTextureCoords") != std::string::npos)
        {
            haveTextureCoords = true;
            break;
        }
    }

    getline(input, bufStr);

    if (haveTextureCoords)
    {
        VertexNum = 0;

        while (getline(input, bufStr))
        {
            if (bufStr.size() == 0)
                continue;
            else if (bufStr[0] != '-' && bufStr[0] != '\t' && bufStr[0] != ' ' && !::isdigit(bufStr[0]))
                continue;

            for (int index = 0; index < 2; index++)
            {
                DelimiterPosition = bufStr.find(';');
                std::string curKey = bufStr.substr(0, DelimiterPosition);
                bufStr = bufStr.substr(DelimiterPosition + 1, bufStr.size());

                Reader << curKey;
                Reader >> Mesh[VertexNum].TexCoord.el[index];
                Reader.clear();
            }

            VertexNum++;

            if (bufStr.size() != 0 && bufStr[bufStr.size() - 1] == ';') //last element is ';'
                break;
        }
    }

    MeshLIMHeader header;
    header.version = LIM_HEADER_VERSION;
    header.hasNormals = haveNormals;
    header.hasTexCoords = haveTextureCoords;
    header.hasColors = false;
    header.verticesCount = verticesCount;
    header.indicesCount = indicesCount * indicesInString;
    return LIMSaver::Save(out, Mesh.data(), Ind.data(), header);
}
