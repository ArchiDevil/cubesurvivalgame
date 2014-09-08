#pragma once

#include "cNoise.h"

#include <cmath>
#include <fstream>
#include <string>

class cWorldGenerator
{
public:
	cWorldGenerator();

	void GenerateHeightMap(int size);
	void LoadHeightMap(const std::string & filename);
	void SaveHeightMap(const std::string & filename);
	int GetHeight(int ChunkX, int ChunkY);
	
	cNoise * GetNoise();

private:
	cNoise noise;

	static const int WorldSize = 1024;
	int	heightMap[WorldSize][WorldSize];

};