#include <iostream>
#include <time.h>
#include <fstream>
#include <vector>
#include <math.h>
#include <sstream>

using namespace std;

struct Elem
{
	Elem(int x, int y) : x(x), y(y){};
	int x, y;
};

const int mapSize = 1000;
const int regions = 150;
unsigned char map[mapSize][mapSize] = {0};
vector<Elem> vecPos;
vector<Elem> centralPoints;

void Print (string filename)
{
	ofstream fout (filename, ios_base::binary);
	for (int i = 0; i < mapSize; i++)
	{
		for (int j = 0; j < mapSize; j++)
		{
			fout << map [i][j];
		}
	}
	fout.close();
}

inline float Distance(int x1, int y1, int x2, int y2)
{
	return (float)(x1 - x2)*(x1 - x2) + (float)(y1 - y2)*(y1 - y2);
}

void BuildVoronoyDiagram()
{
	for (int i = 0; i < mapSize ; i++)
	{
		for (int j = 0; j < mapSize ; j++)
		{
			int elemNum = 0;
			float dist = 10000;
			for (int k = 0; k < vecPos.size() ; k++)
			{
				float tempDist = Distance(i, j, vecPos[k].x, vecPos[k].y);
				if (tempDist < dist*dist)
				{
					dist = sqrt(tempDist);
					elemNum = k;
				}
			}
			map[i][j] = elemNum;
		}
	}
}

void ProcessLloydRelaxation()
{
	vecPos.clear();
	for (int i = 0; i < regions ; i++)
	{
		vecPos.push_back(centralPoints[i]);
	}
}

void FindCentralPoints()
{
	centralPoints.clear();

	for (int i = 0; i < regions ; i++)
	{
		int xmin = mapSize, xmax = 0, ymin = mapSize, ymax = 0;
		for (int x = 0; x < mapSize ; x++)
		{
			for (int y = 0; y < mapSize ; y++)
			{
				if(map[x][y] == i)
				{
					if(x < xmin)
						xmin = x;
					if(x > xmax)
						xmax = x;
					if(y < ymin)
						ymin = y;
					if(y > ymax)
						ymax = y;
				}
			}
			
		}
		Elem point((xmin + xmax) / 2, (ymin + ymax) / 2);
		centralPoints.push_back(point);
	}
}

int main()
{
	srand(time(0));
	
	for (int i = 0; i < regions ; i++)
		vecPos.push_back(Elem(rand()%mapSize, rand()%mapSize));

	//first iteration - setting random points
	//second - finding regions
	//third - lloyd relaxation
	//go to second

	cout << "Started main processing..." << endl;

	int LloydIterations = 6;

	for (int i = 0; i < LloydIterations ; i++)
	{
		BuildVoronoyDiagram();
		cout << "Diagramm " << i << " builded." << endl;
		FindCentralPoints();
		cout << "Central points (iteration " << i << ") found." << endl;
		ProcessLloydRelaxation();
		cout << "Lloyd relaxation (" << i << " iteration) processed." << endl;

		std::ostringstream str;
		str << "out" << i << ".raw";
		Print(str.str());

		cout << "Iteration " << i << " completed." << endl << "---------------------" << endl;
	}

	BuildVoronoyDiagram();
	Print("final.raw");
	
	return 0;
}