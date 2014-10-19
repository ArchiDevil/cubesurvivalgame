#include "world.h"

#include <sstream>

#define waterLevel 70

cWorld::cWorld()
{
}

cWorld::~cWorld()
{
	int size = ChunksStorage->GetChunksPerSide();
	size *= size;
	auto chunks = ChunksStorage->GetChunksArray();

	for (int i = 0; i < size; i++)
		if (chunks[i].GetStatus() > CS_EMPTY)
			Streamer->Save(chunks[i].GetWorldX(), chunks[i].GetWorldY());
}

void cWorld::Initialize(unsigned int ChunksPerSide, int CentralChunkX, int CentralChunkY, const std::string & worldName)
{
	if (ChunksPerSide % 2 == 0)
		ChunksPerSide++;		//необходимо для нечетного числа в чанках

	Generator.reset(new cWorldGenerator);
	ChunksStorage.reset(new cChunksStorage);
	TypesStorage.reset(new typesStorage);
	DataStorage.reset(new WorldStorage(ChunksPerSide * DataStorage->GetChunkWidth()));
	Streamer.reset(new cChunkStreamer(DataStorage.get()));
	Tesselator.reset(new WorldTesselator(DataStorage.get()));
	Tesselator->SetCriticalSection(&critSect);

	SetWorldName(worldName);

	std::wstring BlockPath = L"resources/gamedata/blocks/";
	TypesStorage->loadTypes(utils::filesystem::CollectFileNames(BlockPath, L"blk"));
	ChunksStorage->Initialize(ChunksPerSide, CentralChunkX, CentralChunkY, DataStorage->GetChunkWidth(), TypesStorage.get());
	FillQueue(CentralChunkX, CentralChunkY, ChunksPerSide);


	std::ostringstream str;
	str << "saves/worlds/" << worldName << "/" << worldName << ".lihm";
	Generator->LoadHeightMap(str.str());
	Generator->GetNoise()->SetFrequency(0.001);
	Generator->GetNoise()->SetOctaves(8);
}

void cWorld::Unload()
{
	this->loadingQueue.clear();
}

void cWorld::ClearChunkData(int WorldX, int WorldY)
{
	unsigned int ChunkW = DataStorage->GetChunkWidth();

	for (unsigned int i = 0; i < ChunkW; ++i)
		for (unsigned int j = 0; j < ChunkW; ++j)
			for (unsigned int k = 0; k < DataStorage->GetColumnsCount(); ++k)
				DataStorage->SetColumn(WorldX * ChunkW + i, WorldY * ChunkW + j, k, BlockColumn(BT_Empty, 0));
}

void cWorld::GenerateChunk(int WorldX, int WorldY)
{
	unsigned int ChunkWidth = DataStorage->GetChunkWidth();

	int FromX = WorldX * ChunkWidth;
	int ToX = WorldX * ChunkWidth + ChunkWidth;

	int FromY = WorldY * ChunkWidth;
	int ToY = WorldY * ChunkWidth + ChunkWidth;

	for (int i = FromX; i < ToX; i++)
	{
		for (int j = FromY; j < ToY; j++)
		{
			Generator->GetNoise()->SetFrequency(0.007);
			Generator->GetNoise()->SetOctaves(5);

			double noise = Generator->GetNoise()->SimplexNoise(i, j) * 10.0f;
			double distance = MathLib::distance(Vector2F(), Vector2F(i, j)) / 400.0;
			int result = (noise + 90) * (1.0 - distance);
			unsigned int kMax = result < 3 ? 3 : result; //+ GetInterpolatedHeight(i, j);
			//unsigned int kMax = 90;

			DataStorage->SetColumn(i, j, 0, BlockColumn(BlockTypes::BT_Stone, kMax));
			if (kMax < waterLevel)
			{
				DataStorage->SetColumn(i, j, 1, BlockColumn(BlockTypes::BT_Water, waterLevel - kMax + 1));
			}
			else if (kMax >= waterLevel && kMax <= waterLevel + 3)
			{
				DataStorage->SetColumn(i, j, 0, BlockColumn(BlockTypes::BT_Stone, waterLevel));
				DataStorage->SetColumn(i, j, 1, BlockColumn(BlockTypes::BT_Sand, kMax - waterLevel + 2));
			}
			else
			{
				DataStorage->SetColumn(i, j, 1, BlockColumn(BlockTypes::BT_Dirt, 1));
				DataStorage->SetColumn(i, j, 2, BlockColumn(BlockTypes::BT_Grass, 1));
			}
		}
	}
}

void cWorld::ShiftChunkX(int ShiftingValue)
{
	//updatingSection.lock();

	//int ccy = ChunksStorage->GetCenterY();
	//int width = (ChunksStorage->GetChunksPerSide() - 1) / 2;
	//int xUnload = 0;
	//int xLoad = 0;

	//if (ShiftingValue > 0)
	//{
	//	xUnload = ChunksStorage->GetCenterX() - width;
	//	xLoad = ChunksStorage->GetCenterX() + width + 1;
	//}
	//else
	//{
	//	xUnload = ChunksStorage->GetCenterX() + width;
	//	xLoad = ChunksStorage->GetCenterX() - width - 1;
	//}

	//for (int i = ccy - width; i <= ccy + width; i++)
	//{
	//	WorldChunk * ChunkPtr = ChunksStorage->GetChunkPtr(xUnload, i);

	//	if (ChunkPtr->GetStatus() > CS_EMPTY)
	//		Streamer->Save(ChunkPtr->GetWorldX(), ChunkPtr->GetWorldY());

	//	ChunkPtr->SetWorldX(xLoad);
	//	ChunkPtr->SetStatus(CS_EMPTY);
	//	ChunkPtr->Hide();

	//	loadingQueue.push_back(LoadQuery(xLoad, i, Load));
	//	loadingQueue.push_back(LoadQuery(xLoad, i, Tesselate));
	//}

	//ChunksStorage->SetCenterX(ChunksStorage->GetCenterX() + ShiftingValue);

	//updatingSection.unlock();
}

void cWorld::ShiftChunkY(int ShiftingValue)
{
	//updatingSection.lock();

	//int ccx = ChunksStorage->GetCenterX();
	//int width = (ChunksStorage->GetChunksPerSide() - 1) / 2;
	//int yUnload = 0;
	//int yLoad = 0;

	//if (ShiftingValue > 0)
	//{
	//	yUnload = ChunksStorage->GetCenterY() - width;
	//	yLoad = ChunksStorage->GetCenterY() + width + 1;
	//}
	//else
	//{
	//	yUnload = ChunksStorage->GetCenterY() + width;
	//	yLoad = ChunksStorage->GetCenterY() - width - 1;
	//}

	//for (int i = ccx - width; i <= ccx + width; i++)
	//{
	//	WorldChunk * ChunkPtr = ChunksStorage->GetChunkPtr(i, yUnload);

	//	if (ChunkPtr->GetStatus() > CS_EMPTY)
	//		Streamer->Save(ChunkPtr->GetWorldX(), ChunkPtr->GetWorldY());

	//	ChunkPtr->SetWorldY(yLoad);
	//	ChunkPtr->SetStatus(CS_EMPTY);
	//	ChunkPtr->Hide();
	//	loadingQueue.push_back(LoadQuery(i, yLoad, Load));
	//	loadingQueue.push_back(LoadQuery(i, yLoad, Tesselate));
	//}

	//ChunksStorage->SetCenterY(ChunksStorage->GetCenterY() + ShiftingValue);

	//updatingSection.unlock();
}

void cWorld::ProcessLoading()
{
	updatingSection.lock();

	for (auto iter = loadingQueue.begin(); iter != loadingQueue.end(); ++iter)
	{
		if (!ChunksStorage->IsExist((*iter).x, (*iter).y))
		{
			iter = loadingQueue.erase(iter);
			continue;
		}

		int status = ProcessChunk((*iter).x, (*iter).y, (*iter).action);

		if (status == LOADED)
		{
			iter = loadingQueue.erase(iter);
			break;
		}
		else if (status == ALREADYLOADED)
		{
			iter = loadingQueue.erase(iter);
		}
	}

	updatingSection.unlock();
}

cChunksStorage * cWorld::GetChunksStorage()
{
	return ChunksStorage.get();
}

WorldStorage * cWorld::GetDataStorage()
{
	return DataStorage.get();
}

//bool cWorld::PlaceBlock( int x, int y, int z, BlockType type )
//{
//	updatingSection.lock();
//
//	if(HaveSolidsNear(x, y, z))	//we can place block only if have solids near it
//	{
//		WorldStorage->GetBlock(x, y, z)->TypeID = type; //HACK: TEMP
//		WorldStorage->GetBlock(x, y, z)->LightValue = 0;
//
//		double ChunkX = x / (int)GetDataStorage()->GetChunkWidth();
//		double ChunkY = y / (int)GetDataStorage()->GetChunkWidth();
//		ChunkX = floor(ChunkX);
//		ChunkY = floor(ChunkY);
//		int iX = (int)ChunkX;
//		int iY = (int)ChunkY;
//
//		UpdateChunk(iX - 1, iY - 1);
//		UpdateChunk(iX + 1, iY + 1);
//		UpdateChunk(iX + 1, iY - 1);
//		UpdateChunk(iX - 1, iY + 1);
//
//		UpdateChunk(iX - 1, iY);
//		UpdateChunk(iX + 1, iY);
//		UpdateChunk(iX, iY - 1);
//		UpdateChunk(iX, iY + 1);
//
//		UpdateChunk(iX, iY);
//
//		updatingSection.unlock();
//
//		return true;
//	}
//
//	updatingSection.unlock();
//
//	return false;
//}
//
//bool cWorld::RemoveBlock( int x, int y, int z )
//{
//	updatingSection.lock();
//
//	if(WorldStorage->GetBlock(x, y, z)->TypeID != ID_AIR &&
//		WorldStorage->GetBlock(x, y, z)->TypeID != ID_WATER && 
//		WorldStorage->GetBlock(x, y, z)->TypeID != ID_WROOT)
//	{
//		GetDataStorage()->GetBlock(x, y, z)->TypeID = ID_AIR;
//		GetDataStorage()->GetBlock(x, y, z)->LightValue = 0;
//
//		double ChunkX = x / (int)GetDataStorage()->GetChunkWidth();
//		double ChunkY = y / (int)GetDataStorage()->GetChunkWidth();
//		ChunkX = floor(ChunkX);
//		ChunkY = floor(ChunkY);
//
//		int iX = (int)ChunkX;
//		int iY = (int)ChunkY;
//
//		UpdateChunk(iX - 1, iY - 1);
//		UpdateChunk(iX + 1, iY + 1);
//		UpdateChunk(iX + 1, iY - 1);
//		UpdateChunk(iX - 1, iY + 1);
//
//		UpdateChunk(iX - 1, iY);
//		UpdateChunk(iX + 1, iY);
//		UpdateChunk(iX, iY - 1);
//		UpdateChunk(iX, iY + 1);
//
//		UpdateChunk(iX, iY);
//
//		updatingSection.unlock();
//
//		return true;
//	}
//
//	updatingSection.unlock();
//
//	return false;
//}

void cWorld::UpdateChunk(int WorldX, int WorldY)
{
	ChunksStorage->GetChunkPtr(WorldX, WorldY)->SetStatus(CS_FILLED);
	loadingQueue.push_front(LoadQuery(WorldX, WorldY, Tesselate));
}

int cWorld::ProcessChunk(int WorldX, int WorldY, Action action)
{
	WorldChunk * ChunkPtr = ChunksStorage->GetChunkPtr(WorldX, WorldY);

	if (action == Load)
	{
		if (ChunkPtr->GetStatus() > CS_EMPTY)
			return ALREADYLOADED;

		ClearChunkData(WorldX, WorldY);

		if (Streamer->Check(WorldX, WorldY))
			Streamer->Load(WorldX, WorldY);
		else
			GenerateChunk(WorldX, WorldY);

		ChunkPtr->SetStatus(CS_FILLED);
		return LOADED;
	}

	if (action == Tesselate)
	{
		if (ChunkPtr->GetStatus() > CS_FILLED)
			return ALREADYLOADED;
		if (ChunksStorage->IsBorder(WorldX, WorldY))
			return NOTLOADED;
		if (!ChunksStorage->HaveRightNeighbors(WorldX, WorldY, CS_FILLED))
			return NOTLOADED;

		if (Tesselator->TesselateChunk(WorldX, WorldY, ChunkPtr->GetLandNode()))
		{
			//Tesselator->BuildWaterMesh(WorldX, WorldY);
			ChunkPtr->SetStatus(CS_TESSELATED);
			ChunkPtr->Show();
			return LOADED;
		}
		else
		{
			return NOTLOADED;
		}
	}

	return NOTLOADED;
}

float cWorld::GetInterpolatedHeight(int x, int y)
{
	unsigned int ChunkWidth = DataStorage->GetChunkWidth();

	int LeftChunkX = floor((float)(x - (int)ChunkWidth / 2) / (int)ChunkWidth);
	int RightChunkX = LeftChunkX + 1;
	int TopChunkY = floor((float)(y - (int)ChunkWidth / 2) / (int)ChunkWidth);
	int BottomChunkY = TopChunkY + 1;

	int left = LeftChunkX * ChunkWidth + (int)ChunkWidth / 2;
	int top = TopChunkY * ChunkWidth + (int)ChunkWidth / 2;

	float k = (float)(x - left) / (int)ChunkWidth;
	float v1 = MathLib::LinearInterpolation(Generator->GetHeight(LeftChunkX, TopChunkY),
		Generator->GetHeight(RightChunkX, TopChunkY), k);
	float v2 = MathLib::LinearInterpolation(Generator->GetHeight(LeftChunkX, BottomChunkY),
		Generator->GetHeight(RightChunkX, BottomChunkY), k);
	k = (float)(y - top) / (int)ChunkWidth;
	float v3 = MathLib::LinearInterpolation(v1, v2, k);

	return v3;
}

void cWorld::FillQueue(int centralX, int centralY, int cps)
{
	int width2 = (cps - 1) / 2;

	loadingQueue.push_back(LoadQuery(centralX, centralY, Load));
	loadingQueue.push_back(LoadQuery(centralX, centralY, Tesselate));

	for (int i = 0; i <= width2; i++)	//for all
	{
		int startX = centralX - i;
		int startY = centralY - i;
		int endX = centralX + i;
		int endY = centralY + i;

		for (int index = startX; index < endX; index++)
		{
			loadingQueue.push_back(LoadQuery(index, startY, Load));
			loadingQueue.push_back(LoadQuery(index, startY, Tesselate));
		}

		for (int index = startY; index < endY; index++)
		{
			loadingQueue.push_back(LoadQuery(endX, index, Load));
			loadingQueue.push_back(LoadQuery(endX, index, Tesselate));
		}

		for (int index = endX; index > startX; index--)
		{
			loadingQueue.push_back(LoadQuery(index, endY, Load));
			loadingQueue.push_back(LoadQuery(index, endY, Tesselate));
		}

		for (int index = endY; index > startY; index--)
		{
			loadingQueue.push_back(LoadQuery(startX, index, Load));
			loadingQueue.push_back(LoadQuery(startX, index, Tesselate));
		}
	}
}

typesStorage * cWorld::GetTypesStorage()
{
	return TypesStorage.get();
}

void cWorld::SetWorldName(const std::string & worldName)
{
	this->worldName = worldName;
}

std::string cWorld::GetWorldName() const
{
	return worldName;
}
