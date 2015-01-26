#pragma once

#include <fstream>

#include "misc.h"

class LIMSaver
{
public:
    static bool Save(const std::string & fileName, const Vertex * verticesArray,
        const unsigned long * indicesArray, MeshLIMHeader header)
    {
        std::ofstream output;
        output.open(fileName.c_str(), std::ios_base::binary);

        if (output.fail())
            return false;
        output.write(reinterpret_cast<const char*>(&header), sizeof(MeshLIMHeader));

        for (unsigned int i = 0; i < header.verticesCount; i++)
            output.write(reinterpret_cast<const char*>(&(verticesArray[i].Pos)), 3 * sizeof(float));

        if (header.hasNormals)
            for (unsigned int i = 0; i < header.verticesCount; i++)
                output.write(reinterpret_cast<const char*>(&(verticesArray[i].Normal)), 3 * sizeof(float));

        if (header.hasTexCoords)
            for (unsigned int i = 0; i < header.verticesCount; i++)
                output.write(reinterpret_cast<const char*>(&(verticesArray[i].TexCoord)), 2 * sizeof(float));

        if (header.hasColors)
            for (unsigned int i = 0; i < header.verticesCount; ++i)
                output.write(reinterpret_cast<const char*>(&(verticesArray[i].Color)), 3 * sizeof(float));

        output.write(reinterpret_cast<const char*>(indicesArray), sizeof(unsigned long) * header.indicesCount);
        output.close();

        return true;
    }
};

