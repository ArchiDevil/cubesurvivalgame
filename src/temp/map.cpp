#include <iostream>
#include <time.h>
#include <fstream>
#include <math.h>

using namespace std;

const short sSize = 65; //���������� ������� ������ + 1, ��-�� ����, ��� �������� ����� 
short sKoff = 20; //�����������, ������� ��������� ����� � ��������� �����, +- ��������� �������� � ������������
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
	//������� ��� ������ � ��
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
	sCenter = (sSize - 1) / pow (2, level + 1); //����� ������ �������� ��������� ����� 1�� ����, ������� ���� ������� ��� 1� ����� ���������
	for (int i = 1; i <= pow (2, level); i++) //��� ����� ��������� Diamond (���������� ������)
	{
		for (int j = 1; j <= pow (2, level); j++)
		{
			 
			sTempCenter [0] = sCenter + sCenter * 2 *(i-1);
			sTempCenter [1] = sCenter + sCenter * 2 *(j-1);//����� �� i,j ������ �������� ��������
			
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


	for (int i = 1; i <= pow (2, level); i++) //��� ����� ��������� Diamond (������, ���������� �� 45 ��������)
	{
		for (int j = 1; j <= pow (2, level); j++)
		{
			sTempCenter [0] = sCenter + sCenter * 2 *(i-1);
			sTempCenter [1] = sCenter + sCenter * 2 *(j-1);//����� �� i,j ������ �������� ��������

			//������� �����
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
			//������ �����
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
			//����� �����
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
			//������ �����
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
	int level = 0; //������� ��������
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
		//�������� ���� �����
		map[i][0] = 0;
		map[i][sSize-1] = 0;
		map[0][i] = 0;
		map[sSize-1][i] = 0;
	}

	
	*/
	for (int i = 0; i < sSize ; i++)  //����� ����� ���� ������������� ������ � ���������� ���������� ������ ��� �����
		for (int j = 0; j < sSize ; j++)
			if(map[i][j] < sWaterLevel)
				map[i][j] = 0; 
			else
				map[i][j] -= sWaterLevel;
	//"��������" ��� �������� ���� 20
	
	for (int i = 1; i < sSize-1 ; i++)
		for (int j = 1; j < sSize-1 ; j++)
			newMap[i][j] = (map[i-1][j] + map[i+1][j] + map[i][j-1] + map[i][j+1]) / 4; //������ ���������� ����
 
	//Print(map, "test1.txt");
	Print(newMap, "test2.txt");
	
	return 0;
}