#include "cWorldGenerator.h"

#include <Utilities/ut.h>

cWorldGenerator::cWorldGenerator()
{
	for (int i = 0; i < WorldSize ; i++)
	{
		for (int j = 0; j < WorldSize ; j++)
		{
			heightMap[i][j] = 0;
		}
	}
}

double length(double x, double y)
{
	return sqrt(pow(x - 1.0, 2.0)
		+ pow(y - 1.0, 2.0));
}

void cWorldGenerator::GenerateHeightMap( int size )
{
	int mapSize = size;
	double r = 0.0;
	noise.SetOctaves(15);
	noise.SetFrequency(1.0 + (double)r / 10);

	for (double i = 0.0; i < 2.0 ; i += (double)1 / mapSize)
	{
		for (double j = 0.0; j < 2.0 ; j += (double)1 / mapSize)
		{
			double value = noise.SimplexNoise(i, j);

			value += 1.0;
			value /= 2.0;

			double length2 = length(i, j) > 1.0 ? 1.0 : length(i, j)*length(i, j);
			double curMult = 0.05 + 0.95*length2;
			value *= (1 - curMult*curMult);

			int cellX = (int)(i * mapSize / 2);
			int cellY = (int)(j * mapSize / 2);
			heightMap[(int)cellX][(int)cellY] = value * 100.0;
		}
	}
}

void cWorldGenerator::LoadHeightMap( const std::string & filename )
{
	std::ifstream in;
	in.open(filename, std::ios_base::binary);

	if(in.fail())
	{
		//GenerateHeightMap(WorldSize);
		//SaveHeightMap(filename);
		return;
	}
	else
	{
		for (int i = 0; i < WorldSize ; i++)
		{
			for (int j = 0; j < WorldSize; j++)
			{
				in.read( reinterpret_cast<char*>(&(heightMap[i][j])), sizeof(int) );
			}
		}
	}
}

void cWorldGenerator::SaveHeightMap( const std::string & filename )
{
	utils::filesystem::CreateDir(L"saves/tempWorld");

	std::ofstream out;
	out.open(filename, std::ios_base::binary);
	if(out.fail())
	{
		return;
	}
	else
	{
		for (int i = 0; i < WorldSize ; i++)
		{
			for (int j = 0; j < WorldSize ; j++)
			{
				out.write( reinterpret_cast<char*>(&(heightMap[i][j])), sizeof(int) );
			}
		}
	}
}

int cWorldGenerator::GetHeight( int ChunkX, int ChunkY )
{
	if(ChunkX < WorldSize / 2 && ChunkX > - WorldSize / 2 &&
		ChunkY < WorldSize / 2 && ChunkY > - WorldSize / 2)
	{
		return heightMap[ChunkX + WorldSize / 2][ChunkY + WorldSize / 2];
	}
	else
	{
		return 0;
	}
}

cNoise * cWorldGenerator::GetNoise()
{
	return &noise;
}
