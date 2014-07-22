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
	Vector3F position;		// местоположение
	Vector3F texcoord;		// UVW текстурные координаты
	Vector3F normal;		// нормали
	float color;			// цвет и солнечный свет
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

	int GetWorldX() const;				//возвращает позицию по X чанка
	int GetWorldY() const;				//возвращает позицию по Y чанка
	void SetWorldX(int _x);				//устанавливает позицию по X чанку
	void SetWorldY(int _y);				//устанавливает позицию по Y чанку

	ChunkStatus GetStatus() const;		//возвращает статус чанка
	void SetStatus(ChunkStatus val);	//устанавливает статус чанку
	
	unsigned int GetPointer() const;	//возвращает указатель в массиве блоков

	void Show();						//показывает чанк :3
	void Hide();						//прячет чанк :3
	bool CanBeDrawn() const;			//можно ли рисовать этот чанк?

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
