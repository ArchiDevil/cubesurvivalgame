#pragma once

#include <fstream>

#include "misc.h"

class LIMSaver
{
public:
    static bool Save(std::ofstream & output, const Vertex * verticesArray,
        const uint32_t * indicesArray, MeshLIMHeader header)
    {
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
