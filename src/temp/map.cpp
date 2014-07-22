#include <iostream>
#include <time.h>
#include <fstream>
#include <math.h>

using namespace std;

const short sSize = 65; //необходима степень двойки + 1, из-за того, что нечетное найти 
short sKoff = 20; //коэффицинет, который добавляет хаоса в генерацию карты, +- радномное значение к высчитанному
short sCenter;
short sTempCenter [2] = {0};
int seed = 14682318;
short sWaterLevel = 100;

void Print (unsigned short map[][sSize], string filename)
{
	ofstream fout (filename, ios_base::binary);
	for (int i = 0; i < sSize; i++)
	{
		for (int j = 0; j < sSize; j++)
		{
			fout << map [i][j] << "\t";
		}
	fout << endl;
	}
	fout.close();
	//изменил для вывода в ФШ
}

float randFromPair(int x, int y) 
{
    for (int i = 0; i < 80; i++) 
	{
        int xm7 = x % 7;
        int xm13 = x % 13;
        int xm1301081 = x % 1301081;
        int ym8461 = y % 8461;
        int ym105467 = y % 105467;
        int ym105943 = y % 105943;
        y = x + seed;
        x += (xm7 + xm13 + xm1301081 + ym8461 + ym105467 + ym105943);
    }
    return x / 1520972.0;
}

void Diamond (unsigned short (& map)[sSize][sSize], float level)
{
	//cout << level << endl;
	sCenter = (sSize - 1) / pow (2, level + 1); //таким пиздец способом вычисляем центр 1го куба, который надо считать для 1й части алгоритма
	for (int i = 1; i <= pow (2, level); i++) //это кусок алгоритма Diamond (нормальные кубики)
	{
		for (int j = 1; j <= pow (2, level); j++)
		{
			 
			sTempCenter [0] = sCenter + sCenter * 2 *(i-1);
			sTempCenter [1] = sCenter + sCenter * 2 *(j-1);//центр по i,j нашего текущего квадрата
			
			//cout << sCenter << endl;
			//cout << sTempCenter [0] << " " << sTempCenter [1] << endl;
			
			map [sTempCenter[0]][sTempCenter[1]] =  
			   (map [sTempCenter[0] - sCenter][sTempCenter[1] - sCenter] +
				map [sTempCenter[0] - sCenter][sTempCenter[1] + sCenter] +
				map [sTempCenter[0] + sCenter][sTempCenter[1] + sCenter] +
				map [sTempCenter[0] + sCenter][sTempCenter[1] - sCenter]) / 4;
			int t = rand()%sKoff;
			if (map [sTempCenter[0]][sTempCenter[1]] + (t - (sKoff /2)) >= 0)
				map [sTempCenter[0]][sTempCenter[1]] += (t - (sKoff /2));
			else 
				map [sTempCenter[0]][sTempCenter[1]] = 0;
		}
	}


	for (int i = 1; i <= pow (2, level); i++) //это кусок алгоритма Diamond (кубики, повернутые на 45 градусов)
	{
		for (int j = 1; j <= pow (2, level); j++)
		{
			sTempCenter [0] = sCenter + sCenter * 2 *(i-1);
			sTempCenter [1] = sCenter + sCenter * 2 *(j-1);//центр по i,j нашего текущего квадрата

			//верхний кусок
			if (sTempCenter[0] - 2*sCenter <= 0)
			{
				map [sTempCenter[0] - sCenter][sTempCenter[1]] =
					(map [sTempCenter[0] - sCenter][sTempCenter[1] + sCenter] +
					map [sTempCenter[0]][sTempCenter[1]] +
					map [sTempCenter[0] - sCenter][sTempCenter[1] - sCenter]) / 4;
				int t = rand()%sKoff;
				if (map [sTempCenter[0]][sTempCenter[1]] + (t - (sKoff /2)) >= 0)
					map [sTempCenter[0]][sTempCenter[1]] += (t - (sKoff /2));
				else 
					map [sTempCenter[0]][sTempCenter[1]] = 0;
			}
			else
			{
				map [sTempCenter[0] - sCenter][sTempCenter[1]] =
					(map [sTempCenter[0] - 2*sCenter][sTempCenter[1]] +
					map [sTempCenter[0] - sCenter][sTempCenter[1] + sCenter] +
					map [sTempCenter[0]][sTempCenter[1]] +
					map [sTempCenter[0] - sCenter][sTempCenter[1] - sCenter]) / 4;
				int t = rand()%sKoff;
				if (map [sTempCenter[0]][sTempCenter[1]] + (t - (sKoff /2)) >= 0)
					map [sTempCenter[0]][sTempCenter[1]] += (t - (sKoff /2));
				else 
					map [sTempCenter[0]][sTempCenter[1]] = 0;
			}
			//правый кусок
			if (sTempCenter[1] + 2 * sCenter >= sSize)
			{
				map [sTempCenter[0]][sTempCenter[1] + sCenter] =
					(map [sTempCenter[0] - sCenter][sTempCenter[1] + sCenter] +
					map [sTempCenter[0] + sCenter][sTempCenter[1] + sCenter] +
					map [sTempCenter[0]][sTempCenter[1]]) / 4 ;
				int t = rand()%sKoff;
				if (map [sTempCenter[0]][sTempCenter[1]] + (t - (sKoff /2)) >= 0)
					map [sTempCenter[0]][sTempCenter[1]] += (t - (sKoff /2));
				else 
					map [sTempCenter[0]][sTempCenter[1]] = 0;

			}
			else
			{
				map [sTempCenter[0]][sTempCenter[1] + sCenter] =
					(map [sTempCenter[0] - sCenter][sTempCenter[1] + sCenter] +
					map [sTempCenter[0]][sTempCenter[1] + 2 * sCenter] +
					map [sTempCenter[0] + sCenter][sTempCenter[1] + sCenter] +
					map [sTempCenter[0]][sTempCenter[1]]) / 4 ;
				int t = rand()%sKoff;
				if (map [sTempCenter[0]][sTempCenter[1]] + (t - (sKoff /2)) >= 0)
					map [sTempCenter[0]][sTempCenter[1]] += (t - (sKoff /2));
				else 
					map [sTempCenter[0]][sTempCenter[1]] = 0;

			}
			//левый кусок
			if (sTempCenter[0] + 2*sCenter >= sSize)
			{
				map [sTempCenter[0] + sCenter][sTempCenter[1]] =
					(map [sTempCenter[0]][sTempCenter[1]] +
					map [sTempCenter[0] + sCenter][sTempCenter[1] + sCenter] +
					map [sTempCenter[0] + sCenter][sTempCenter[1] - sCenter]) / 4 ;
				int t = rand()%sKoff;
				if (map [sTempCenter[0]][sTempCenter[1]] + (t - (sKoff /2)) >= 0)
					map [sTempCenter[0]][sTempCenter[1]] += (t - (sKoff /2));
				else 
					map [sTempCenter[0]][sTempCenter[1]] = 0;
			}
			else
			{
				map [sTempCenter[0] + sCenter][sTempCenter[1]] =
					(map [sTempCenter[0]][sTempCenter[1]] +
					map [sTempCenter[0] + sCenter][sTempCenter[1] + sCenter] +
					map [sTempCenter[0] + 2*sCenter][sTempCenter[1]] +
					map [sTempCenter[0] + sCenter][sTempCenter[1] - sCenter]) / 4 ;
				int t = rand()%sKoff;
				if (map [sTempCenter[0]][sTempCenter[1]] + (t - (sKoff /2)) >= 0)
					map [sTempCenter[0]][sTempCenter[1]] += (t - (sKoff /2));
				else 
					map [sTempCenter[0]][sTempCenter[1]] = 0;
			}
			//нижний кусок
			if (sTempCenter[1] - 2 * sCenter <= 0)
			{
				map [sTempCenter[0]][sTempCenter[1] - sCenter] =
					(map [sTempCenter[0] - sCenter][sTempCenter[1] - sCenter] +
					map [sTempCenter[0]][sTempCenter[1]] +
					map [sTempCenter[0] + sCenter][sTempCenter[1] - sCenter] ) / 4 ;
				int t = rand()%sKoff;
				if (map [sTempCenter[0]][sTempCenter[1]] + (t - (sKoff /2)) >= 0)
					map [sTempCenter[0]][sTempCenter[1]] += (t - (sKoff /2));
				else 
					map [sTempCenter[0]][sTempCenter[1]] = 0;
			}
			else
			{
				map [sTempCenter[0]][sTempCenter[1] - sCenter] =
					(map [sTempCenter[0] - sCenter][sTempCenter[1] - sCenter] +
					map [sTempCenter[0]][sTempCenter[1]] +
					map [sTempCenter[0] + sCenter][sTempCenter[1] - sCenter] +
					map [sTempCenter[0]][sTempCenter[1] - 2 * sCenter]) / 4 ;
				int t = rand()%sKoff;
				if (map [sTempCenter[0]][sTempCenter[1]] + (t - (sKoff /2)) >= 0)
					map [sTempCenter[0]][sTempCenter[1]] += (t - (sKoff /2));
				else 
					map [sTempCenter[0]][sTempCenter[1]] = 0;
			}
		}
	}
	
	if (sCenter == 1)
		return;
	Diamond (map, ++level);
	
}

int main()
{
	int level = 0; //уровень рекурсии
	srand(time(0));
	unsigned short map [sSize][sSize] = {0};
	unsigned short newMap[sSize][sSize] = {0};
//	map [0][0] = rand()%255;
	map [0][0] = rand()%255;
	map [0][sSize - 1] = rand()%255;
	map [sSize - 1][0] = rand()%255;
	map [sSize - 1][sSize - 1] = rand()%255;

	Diamond (map, level);
	/*
	for (int i = 0; i < sSize ; i++)
	{
		//зануляем края карты
		map[i][0] = 0;
		map[i][sSize-1] = 0;
		map[0][i] = 0;
		map[sSize-1][i] = 0;
	}

	
	*/
	for (int i = 0; i < sSize ; i++)  //залив водой ниже определенного уровня и считывание абсолютной высоты для теста
		for (int j = 0; j < sSize ; j++)
			if(map[i][j] < sWaterLevel)
				map[i][j] = 0; 
			else
				map[i][j] -= sWaterLevel;
	//"обрезаем" все значения ниже 20
	
	for (int i = 1; i < sSize-1 ; i++)
		for (int j = 1; j < sSize-1 ; j++)
			newMap[i][j] = (map[i-1][j] + map[i+1][j] + map[i][j-1] + map[i][j+1]) / 4; //делаем простейший блюр
 
	//Print(map, "test1.txt");
	Print(newMap, "test2.txt");
	
	return 0;
}