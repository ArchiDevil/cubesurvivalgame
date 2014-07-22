#pragma once

#include "../../GraphicsEngine/SceneGraph/MeshNode.h"
#include "../../MathLib/math.h"

enum ChunkStatus
{
	//chunk is empty
	CS_EMPTY,
	
	//chunk filled with data
	CS_FILLED,
	
	//upside-down light computed
	CS_LIGHTDOWN,
	
	//all light in chunk computed
	CS_LIGHTED,
	
	//chunk tesselated and can be drawn
	CS_TESSELATED
};

struct LandVertex 
{
	Vector3F position;		// ��������������
	Vector3F texcoord;		// UVW ���������� ����������
	Vector3F normal;		// �������
	float color;			// ���� � ��������� ����
};

struct WaterVertex
{
	WaterVertex(Vector3F _pos, Vector3F _normal) 
		: pos(_pos), normal(_normal)
	{}

	Vector3F pos;
	Vector3F normal;
};

struct texPos
{
	texPos()
	: animV(0.0f)
	{
	}
	
	texPos(Vector3F p, Vector2F t, float av) 
	: pos(p), tc(t), animV(av) 
	{
	}
	
	Vector3F pos;
	Vector2F tc;
	float animV;
};

class WorldChunk
{
public:
	WorldChunk();

	void Initialize(ShiftEngine::MeshNode * _landNode, ShiftEngine::MeshNode * _waterNode, unsigned int chunkWidth, unsigned int chunkHeight);

	int GetWorldX() const;				//���������� ������� �� X �����
	int GetWorldY() const;				//���������� ������� �� Y �����
	void SetWorldX(int _x);				//������������� ������� �� X �����
	void SetWorldY(int _y);				//������������� ������� �� Y �����

	ChunkStatus GetStatus() const;		//���������� ������ �����
	void SetStatus(ChunkStatus val);	//������������� ������ �����
	
	unsigned int GetPointer() const;	//���������� ��������� � ������� ������

	void Show();						//���������� ���� :3
	void Hide();						//������ ���� :3
	bool CanBeDrawn() const;			//����� �� �������� ���� ����?

	ShiftEngine::MeshNode *		GetLandNode();
	ShiftEngine::MeshNode *		GetWaterNode();

private:
	WorldChunk(const WorldChunk & ref);

	int WorldX, WorldY;					//world position
	unsigned int Pointer;				//pointer to WorldX * 16, WorldY * 16, 0
	unsigned int chunkWidth, chunkHeight;
	ChunkStatus Status;

	ShiftEngine::MeshNode * landNode;	//ONLY LANDSCAPE FOR NOW (it would be updated in future to batch)!
	ShiftEngine::MeshNode * waterNode;
};
