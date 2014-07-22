#pragma once

#include <fstream>

#include "misc.h"

class LIMSaver
{
public:
	bool Save(const std::wstring & fileName, const Vertex * verticesArray, 
		const unsigned long * indicesArray, MeshLIMHeader header) const
	{
		std::ofstream output;
#ifdef WIN32
		output.open(fileName.c_str(), std::ios_base::binary);
#else
		std::string str(fileName.begin(), fileName.end());
		output.open(str.c_str());
#endif
		if(output.fail())
			return false;
		output.write(reinterpret_cast<const char*>(&header), sizeof(MeshLIMHeader));

		for (unsigned int i = 0; i < header.verticesCount ; i++)
				output.write(reinterpret_cast<const char*>(&(verticesArray[i].Pos)), 3 * sizeof(float));

		if(header.hasNormals)
			for (unsigned int i = 0; i < header.verticesCount ; i++)
				output.write(reinterpret_cast<const char*>(&(verticesArray[i].Normal)), 3 * sizeof(float));

		if(header.hasTexCoords)
			for (unsigned int i = 0; i < header.verticesCount ; i++)
				output.write(reinterpret_cast<const char*>(&(verticesArray[i].TexCoord)), 2 * sizeof(float));

		output.write(reinterpret_cast<const char*>(indicesArray), sizeof(long) * header.indicesCount);

		output.close();
		return true;
	}
};

