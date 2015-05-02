#include "cXConverter.h"

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

bool cXConverter::Convert(std::ifstream & in, std::vector<Vertex> & vertices, std::vector<uint32_t> & indices, MeshLIMHeader & header)
{
    std::string bufStr;
    std::stringstream Reader;
    unsigned int DelimiterPosition;

    while (in >> bufStr)
    {
        if (bufStr == "Mesh")
            break;  //we found beginning of mesh description
    }

    getline(in, bufStr);
    getline(in, bufStr, ';');
    Reader << bufStr;
    unsigned int verticesCount; //count of vertices
    Reader >> verticesCount;
    Reader.clear();
    bufStr.clear();

    bool haveNormals = false;
    bool haveTextureCoords = false;

    vertices.resize(verticesCount);
    unsigned int VertexNum = 0;

    while (getline(in, bufStr))
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
            Reader >> vertices[VertexNum].Pos.el[index];
            Reader.clear();
        }

        VertexNum++;

        if (bufStr.size() != 0 && bufStr[bufStr.size() - 1] == ';') //last element is ';'
            break;
    }


    getline(in, bufStr);
    getline(in, bufStr, ';');

    Reader << bufStr;
    int indicesCount;   //count of indices
    Reader >> indicesCount;
    Reader.clear();
    bufStr.clear();

    const int indicesInString = 3;
    indices.resize(indicesCount * indicesInString, 0);

    VertexNum = 0;

    while (getline(in, bufStr))
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
            Reader >> indices[VertexNum * indicesInString + index];
            Reader.clear();
        }

        VertexNum++;

        if (bufStr.size() != 0 && bufStr[bufStr.size() - 1] == ';') //last element is ';'
            break;
    }

    while (getline(in, bufStr))
    {
        if (bufStr.find("MeshNormals") != std::string::npos)
        {
            haveNormals = true;
            break;
        }
    }

    getline(in, bufStr);

    if (haveNormals)
    {
        VertexNum = 0;

        while (getline(in, bufStr))
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
                Reader >> vertices[VertexNum].Normal.el[index];
                Reader.clear();
            }

            VertexNum++;

            if (bufStr.size() != 0 && bufStr[bufStr.size() - 1] == ';') //last element is ';'
                break;
        }
    }

    while (getline(in, bufStr))
    {
        if (bufStr.find("MeshTextureCoords") != std::string::npos)
        {
            haveTextureCoords = true;
            break;
        }
    }

    getline(in, bufStr);

    if (haveTextureCoords)
    {
        VertexNum = 0;

        while (getline(in, bufStr))
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
                Reader >> vertices[VertexNum].TexCoord.el[index];
                Reader.clear();
            }

            VertexNum++;

            if (bufStr.size() != 0 && bufStr[bufStr.size() - 1] == ';') //last element is ';'
                break;
        }
    }

    header.hasNormals = haveNormals;
    header.hasTexCoords = haveTextureCoords;
    header.hasColors = false;
    header.verticesCount = verticesCount;
    header.indicesCount = indicesCount * indicesInString;

    return true;
}
