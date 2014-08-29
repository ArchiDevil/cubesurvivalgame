#include "world.h"

#include <sstream>

#define waterLevel 70

cWorld::cWorld()
{
	Generator		= new cWorldGenerator;
	WorldStorage	= new cWorldStorage;
	ChunksStorage	= new cChunksStorage;
	Streamer		= new cChunkStreamer(WorldStorage);
	Tesselator		= new cTesselator;
		Tesselator->SetCriticalSection(&critSect);
	TypesStorage	= new typesStorage;
}

cWorld::~cWorld()
{
	int size = ChunksStorage->GetChunksPerSide();
	size *= size;
	auto chunks = ChunksStorage->GetChunksArray();

	for (int i = 0; i < size ; i++)
		if(chunks[i].GetStatus() > CS_EMPTY)
			Streamer->Save(chunks[i].GetWorldX(), chunks[i].GetWorldY());

	delete Generator;
	delete WorldStorage;
	delete ChunksStorage;
	delete Streamer;
	delete Tesselator;
	delete TypesStorage;
}

void cWorld::Initialize( unsigned int ChunksPerSide, int CentralChunkX, int CentralChunkY, cEnvironmentManager * envMgr, const std::wstring & worldName )
{
	if(ChunksPerSide % 2 == 0)
		ChunksPerSide++;		//необходимо для нечетного числа в чанках

	SetWorldName(worldName);

	std::wstring BlockPath = L"resources/gamedata/blocks/";
	TypesStorage->loadTypes(utils::filesystem::CollectFileNames(BlockPath, L"blk"));

	ChunksStorage->Initialize(ChunksPerSide, CentralChunkX, CentralChunkY, WorldStorage->GetChunkWidth(), WorldStorage->GetChunkHeight(), TypesStorage);

	unsigned int size = ChunksPerSide * WorldStorage->GetChunkWidth() * ChunksPerSide
		* WorldStorage->GetChunkWidth() * WorldStorage->GetChunkHeight();

	WorldStorage->ArrayWidth = ChunksPerSide * WorldStorage->GetChunkWidth();
	WorldStorage->WorldData = new Block[size];
	WorldStorage->Size = size;
	memset(WorldStorage->WorldData, 0, size * sizeof(Block));

	FillQueue(CentralChunkX, CentralChunkY, ChunksPerSide);

	std::wostringstream str;
	str << L"saves\\worlds\\" << worldName << L"\\" << worldName << L".lihm";
	Generator->LoadHeightMap(str.str());
	Generator->GetNoise()->SetFrequency(0.001);
	Generator->GetNoise()->SetOctaves(8);

	Tesselator->Initialize(WorldStorage, ChunksStorage, Generator);
//	Renderer->Initialize(ChunksStorage, TypesStorage, envMgr);
}

void cWorld::Unload()
{
	this->loadingQueue.clear();
}

void cWorld::ClearChunkData( int WorldX, int WorldY )
{
	unsigned int ChunkW = WorldStorage->GetChunkWidth();
	unsigned int ChunkH = WorldStorage->GetChunkHeight();

	for(unsigned int i = 0; i < ChunkW; i++)
	{
		for(unsigned int j = 0; j < ChunkW; j++)
		{
			unsigned int PTR = WorldStorage->GetIndex(WorldX * ChunkW + i, WorldY * ChunkW + j, 0);
			memset(WorldStorage->WorldData + PTR, 0, sizeof(Block) * ChunkH); //very fast
		}
	}
}

void cWorld::GenerateChunk( int WorldX, int WorldY )
{
	//Способы оптимизировать функцию:
	//1. Заполнять memcpy из некоторого буфера, который будет создаваться ДО циклов, следовательно циклов надо будет всего два 16x16, либо вообще один 256
	//2. Обращаться к массиву напрямую - не есть гуд, хотя функция inline и такое обращение в release сборке выполняется именно так
	//3. Находить указатель на нижний элемент колонны и в цикле пробегать до его верха - так не придется считать много <используется>

	unsigned int ChunkWidth = WorldStorage->GetChunkWidth();

	int FromX = WorldX * ChunkWidth;
	int ToX = WorldX * ChunkWidth + ChunkWidth;

	int FromY = WorldY * ChunkWidth;
	int ToY = WorldY * ChunkWidth + ChunkWidth;

	for(int i = FromX; i < ToX; i++)
	{
		for(int j = FromY; j < ToY; j++)
		{
			Generator->GetNoise()->SetFrequency(0.001);
			Generator->GetNoise()->SetOctaves(8);

			double noise = Generator->GetNoise()->SimplexNoise(i, j) * 40.0f;
			unsigned int kMax = noise + 70 + GetInterpolatedHeight(i, j);

 			unsigned int PTR = WorldStorage->GetIndex(i, j, 0);
 			WorldStorage->WorldData[PTR].TypeID = ID_WROOT;
 			WorldStorage->WorldData[PTR].LightValue = 0;

			for (unsigned int k = 1; k < kMax - 2 ; k++)
			{
				WorldStorage->WorldData[PTR + k].TypeID = ID_STONE;
			}

			if(kMax > waterLevel + 1)
			{
				WorldStorage->WorldData[PTR + kMax - 2].TypeID = ID_DIRT;
				WorldStorage->WorldData[PTR + kMax - 1].TypeID = ID_GRASS;
			}
			else if (kMax <= waterLevel + 1 && kMax > waterLevel - 1)
			{
				WorldStorage->WorldData[PTR + kMax - 2].TypeID = ID_SAND;
				WorldStorage->WorldData[PTR + kMax - 1].TypeID = ID_SAND;
			}
			else if (kMax <= waterLevel - 1)
			{
 				WorldStorage->WorldData[PTR + kMax - 2].TypeID = ID_STONE;
 				WorldStorage->WorldData[PTR + kMax - 1].TypeID = ID_STONE;

				for (unsigned int k = kMax; k < waterLevel ; k++)
				{
					WorldStorage->WorldData[PTR + k].TypeID = ID_WATER;
				}
			}
		}
	}
}

bool cWorld::HaveSolidsNear( int x, int y, int z )
{
	if(z == 0)
	{
		if(WorldStorage->GetBlock(x, y, z+1)->TypeID != ID_AIR ||
			WorldStorage->GetBlock(x+1, y, z)->TypeID != ID_AIR ||
			WorldStorage->GetBlock(x-1, y, z)->TypeID != ID_AIR ||
			WorldStorage->GetBlock(x, y+1, z)->TypeID != ID_AIR ||
			WorldStorage->GetBlock(x, y-1, z)->TypeID != ID_AIR)
			return true;
	}
	else if(z == WorldStorage->GetChunkHeight() - 1)
	{
		if(WorldStorage->GetBlock(x, y, z-1)->TypeID != ID_AIR ||
			WorldStorage->GetBlock(x+1, y, z)->TypeID != ID_AIR ||
			WorldStorage->GetBlock(x-1, y, z)->TypeID != ID_AIR ||
			WorldStorage->GetBlock(x, y+1, z)->TypeID != ID_AIR ||
			WorldStorage->GetBlock(x, y-1, z)->TypeID != ID_AIR)
			return true;
	}
	else
	{
		if(WorldStorage->GetBlock(x, y, z+1)->TypeID != ID_AIR ||
			WorldStorage->GetBlock(x, y, z-1)->TypeID != ID_AIR ||
			WorldStorage->GetBlock(x+1, y, z)->TypeID != ID_AIR ||
			WorldStorage->GetBlock(x-1, y, z)->TypeID != ID_AIR ||
			WorldStorage->GetBlock(x, y+1, z)->TypeID != ID_AIR ||
			WorldStorage->GetBlock(x, y-1, z)->TypeID != ID_AIR)
			return true;
	}
	return false;
}

void cWorld::ShiftChunkX( int ShiftingValue )
{
	updatingSection.lock();

	int ccy = ChunksStorage->GetCenterY();
	int width = (ChunksStorage->GetChunksPerSide() - 1) / 2;
	int xUnload = 0;
	int xLoad = 0;

	if(ShiftingValue > 0)
	{
		xUnload = ChunksStorage->GetCenterX() - width;
		xLoad = ChunksStorage->GetCenterX() + width + 1;
	}
	else
	{
		xUnload = ChunksStorage->GetCenterX() + width;
		xLoad = ChunksStorage->GetCenterX() - width - 1;
	}

	for (int i = ccy - width; i <= ccy + width; i++)
	{
		WorldChunk * ChunkPtr = ChunksStorage->GetChunkPtr(xUnload, i);

		if(ChunkPtr->GetStatus() > CS_EMPTY)
			Streamer->Save(ChunkPtr->GetWorldX(), ChunkPtr->GetWorldY());

		ChunkPtr->SetWorldX(xLoad);
		ChunkPtr->SetStatus(CS_EMPTY);
		ChunkPtr->Hide();

		loadingQueue.push_back(LoadQuery(xLoad, i, Load));
		loadingQueue.push_back(LoadQuery(xLoad, i, UpLight));
		loadingQueue.push_back(LoadQuery(xLoad, i, ScatterLight));
		loadingQueue.push_back(LoadQuery(xLoad, i, Tesselate));
	}

	ChunksStorage->SetCenterX(ChunksStorage->GetCenterX() + ShiftingValue);

	updatingSection.unlock();
}

void cWorld::ShiftChunkY( int ShiftingValue )
{
	updatingSection.lock();

	int ccx = ChunksStorage->GetCenterX();
	int width = (ChunksStorage->GetChunksPerSide() - 1) / 2;
	int yUnload = 0;
	int yLoad = 0;

	if(ShiftingValue > 0)
	{
		yUnload = ChunksStorage->GetCenterY() - width;
		yLoad = ChunksStorage->GetCenterY() + width + 1;
	}
	else
	{
		yUnload = ChunksStorage->GetCenterY() + width;
		yLoad = ChunksStorage->GetCenterY() - width - 1;
	}

	for (int i = ccx - width; i <= ccx + width; i++)
	{
		WorldChunk * ChunkPtr = ChunksStorage->GetChunkPtr(i, yUnload);

		if(ChunkPtr->GetStatus() > CS_EMPTY)
			Streamer->Save(ChunkPtr->GetWorldX(), ChunkPtr->GetWorldY());

		ChunkPtr->SetWorldY(yLoad);
		ChunkPtr->SetStatus(CS_EMPTY);
		ChunkPtr->Hide();
		loadingQueue.push_back(LoadQuery(i, yLoad, Load));
		loadingQueue.push_back(LoadQuery(i, yLoad, UpLight));
		loadingQueue.push_back(LoadQuery(i, yLoad, ScatterLight));
		loadingQueue.push_back(LoadQuery(i, yLoad, Tesselate));
	}

	ChunksStorage->SetCenterY(ChunksStorage->GetCenterY() + ShiftingValue);

	updatingSection.unlock();
}

void cWorld::ProcessLoading()
{
	updatingSection.lock();

	for (auto iter = loadingQueue.begin(); iter != loadingQueue.end(); ++iter)
	{
		if(!ChunksStorage->IsExist((*iter).x, (*iter).y))
		{
			iter = loadingQueue.erase(iter);
			continue;
		}

		int status = ProcessChunk((*iter).x, (*iter).y, (*iter).action);

		if(status == LOADED)
		{
			iter = loadingQueue.erase(iter);
			break;
		}
		else if(status == ALREADYLOADED)
		{
			iter = loadingQueue.erase(iter);
		}
	}

	updatingSection.unlock();
}

cChunksStorage * cWorld::GetChunksStorage()
{
	return ChunksStorage;
}

cWorldStorage * cWorld::GetDataStorage()
{
	return WorldStorage;
}

bool cWorld::PlaceBlock( int x, int y, int z, BlockType type )
{
	updatingSection.lock();

	if(HaveSolidsNear(x, y, z))	//we can place block only if have solids near it
	{
		WorldStorage->GetBlock(x, y, z)->TypeID = type; //HACK: TEMP
		WorldStorage->GetBlock(x, y, z)->LightValue = 0;

		double ChunkX = x / (int)GetDataStorage()->GetChunkWidth();
		double ChunkY = y / (int)GetDataStorage()->GetChunkWidth();
		ChunkX = floor(ChunkX);
		ChunkY = floor(ChunkY);
		int iX = (int)ChunkX;
		int iY = (int)ChunkY;

		UpdateChunk(iX - 1, iY - 1);
		UpdateChunk(iX + 1, iY + 1);
		UpdateChunk(iX + 1, iY - 1);
		UpdateChunk(iX - 1, iY + 1);

		UpdateChunk(iX - 1, iY);
		UpdateChunk(iX + 1, iY);
		UpdateChunk(iX, iY - 1);
		UpdateChunk(iX, iY + 1);

		UpdateChunk(iX, iY);

		updatingSection.unlock();

		return true;
	}

	updatingSection.unlock();

	return false;
}

bool cWorld::RemoveBlock( int x, int y, int z )
{
	updatingSection.lock();

	if(WorldStorage->GetBlock(x, y, z)->TypeID != ID_AIR &&
		WorldStorage->GetBlock(x, y, z)->TypeID != ID_WATER && 
		WorldStorage->GetBlock(x, y, z)->TypeID != ID_WROOT)
	{
		GetDataStorage()->GetBlock(x, y, z)->TypeID = ID_AIR;
		GetDataStorage()->GetBlock(x, y, z)->LightValue = 0;

		double ChunkX = x / (int)GetDataStorage()->GetChunkWidth();
		double ChunkY = y / (int)GetDataStorage()->GetChunkWidth();
		ChunkX = floor(ChunkX);
		ChunkY = floor(ChunkY);

		int iX = (int)ChunkX;
		int iY = (int)ChunkY;

		UpdateChunk(iX - 1, iY - 1);
		UpdateChunk(iX + 1, iY + 1);
		UpdateChunk(iX + 1, iY - 1);
		UpdateChunk(iX - 1, iY + 1);

		UpdateChunk(iX - 1, iY);
		UpdateChunk(iX + 1, iY);
		UpdateChunk(iX, iY - 1);
		UpdateChunk(iX, iY + 1);

		UpdateChunk(iX, iY);

		updatingSection.unlock();

		return true;
	}

	updatingSection.unlock();

	return false;
}

void cWorld::UpdateChunk( int WorldX, int WorldY )
{
	ChunksStorage->GetChunkPtr(WorldX, WorldY)->SetStatus(CS_FILLED);
	loadingQueue.push_front(LoadQuery(WorldX, WorldY, Tesselate));
	loadingQueue.push_front(LoadQuery(WorldX, WorldY, ScatterLight));
	loadingQueue.push_front(LoadQuery(WorldX, WorldY, UpLight));
}

int cWorld::ProcessChunk( int WorldX, int WorldY, Action action )
{
	WorldChunk * ChunkPtr = ChunksStorage->GetChunkPtr(WorldX, WorldY);

	if(action == Load)
	{
		if(ChunkPtr->GetStatus() > CS_EMPTY)
			return ALREADYLOADED;

		ClearChunkData(WorldX, WorldY);

		if(Streamer->Check(WorldX, WorldY))
			Streamer->Load(WorldX, WorldY);
		else
			GenerateChunk(WorldX, WorldY);

		ChunkPtr->SetStatus(CS_FILLED);
		return LOADED;
	}

	if(action == UpLight)
	{
		if(ChunkPtr->GetStatus() > CS_FILLED)
			return ALREADYLOADED;

		ComputeUpsideLight(WorldX, WorldY);
		ChunkPtr->SetStatus(CS_LIGHTDOWN);
		return LOADED;
	}

	if(action == ScatterLight)
	{
		if(ChunkPtr->GetStatus() > CS_LIGHTDOWN)
			return ALREADYLOADED;

		if(ChunksStorage->IsBorder(WorldX, WorldY))	return NOTLOADED;
		if(!ChunksStorage->HaveRightNeighbors(WorldX, WorldY, CS_LIGHTDOWN)) return NOTLOADED;
		ComputeScatteringLight(WorldX, WorldY);
		ChunkPtr->SetStatus(CS_LIGHTED);
		return LOADED;
	}

	if(action == Tesselate)
	{
		if(ChunkPtr->GetStatus() > CS_LIGHTED)
			return ALREADYLOADED;

		if(ChunksStorage->IsBorder(WorldX, WorldY))	return NOTLOADED;
		if(!ChunksStorage->HaveRightNeighbors(WorldX, WorldY, CS_LIGHTED)) return NOTLOADED;

		if(Tesselator->BuildChunkMesh(WorldX, WorldY))
		{
			Tesselator->BuildWaterMesh(WorldX, WorldY);
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

void cWorld::ComputeUpsideLight( int WorldX, int WorldY )
{
	const int startX = WorldX * WorldStorage->GetChunkWidth();
	const int startY = WorldY * WorldStorage->GetChunkWidth();
	const unsigned int startZ = WorldStorage->GetChunkHeight() - 1;

	for (int x = startX; x < startX + (int)WorldStorage->GetChunkWidth(); x++)
	{
		for (int y = startY; y < startY + (int)WorldStorage->GetChunkWidth(); y++)
		{
			int ptr = WorldStorage->GetIndex(x, y, 0);

			for (unsigned int z = 0; z < WorldStorage->GetChunkHeight(); z++)
				WorldStorage->WorldData[ptr + z].LightValue = 0;
		}
	}

	for (int x = startX; x < startX + (int)WorldStorage->GetChunkWidth(); x++)
	{
		for (int y = startY; y < startY + (int)WorldStorage->GetChunkWidth(); y++)
		{
			unsigned int z = startZ;
			int PTR = WorldStorage->GetIndex(x, y, z);

			while(WorldStorage->WorldData[PTR].TypeID == ID_AIR || WorldStorage->WorldData[PTR].TypeID == ID_WATER)
			{
				WorldStorage->WorldData[PTR].LightValue = MAX_SUN_LIGHT_INDEX;
				PTR--;
			}
			//int waterLight = MAX_SUN_LIGHT_INDEX - 1;
			//if(WorldStorage->WorldData[PTR].TypeID == ID_WATER)
			//{
			//	while(WorldStorage->WorldData[PTR].TypeID == ID_WATER && waterLight >= 0)
			//	{
			//		WorldStorage->WorldData[PTR].LightValue = waterLight;
			//		waterLight--;
			//		PTR--;
			//	}
			//}
		}
	}
}

void cWorld::ComputeScatteringLight( int WorldX, int WorldY )
{
	const int startX = WorldX * WorldStorage->GetChunkWidth();
	const int startY = WorldY * WorldStorage->GetChunkWidth();

	const unsigned int ArrayWidth = WorldStorage->ArrayWidth;
	const unsigned int ChunkHeight = WorldStorage->ChunkHeight;
	const unsigned int ChunkWidth = WorldStorage->GetChunkWidth();

	const int xShift = ChunkHeight * ArrayWidth;
	const int yShift = ChunkHeight;

	for (int x = startX - 1; x < startX + (int)ChunkWidth + 1; x++)
	{
		for (int y = startY - 1; y < startY + (int)ChunkWidth + 1; y++)
		{
			int PTR = WorldStorage->GetIndex(x, y, 0);
			for(int z = 0; z < ChunkHeight; z++)
			{
				Block * block = WorldStorage->GetBlock(PTR + z);

				if(block->LightValue > 0)
				{
					LightProp(PTR + z + xShift, block->LightValue - 1);					//X+1
					LightProp(PTR + z - xShift, block->LightValue - 1);					//X-1
					LightProp(PTR + z + yShift, block->LightValue - 1);					//Y+1
					LightProp(PTR + z - yShift, block->LightValue - 1);					//Y-1
				}
			}
		}
	}
}

void cWorld::LightProp( int PTR, unsigned char value )
{
	static const unsigned int ArrayWidth = WorldStorage->ArrayWidth;
	static const unsigned int ChunkHeight = WorldStorage->ChunkHeight;

	static const int xShift = ChunkHeight * ArrayWidth;
	static const int yShift = ChunkHeight;

	if(value <= 0)
		return;

	if(PTR > WorldStorage->Size)
		PTR = PTR % (int)WorldStorage->Size;

	if(PTR < 0)
		PTR += WorldStorage->Size;

	Block * curBlock = &WorldStorage->WorldData[PTR];

	if (curBlock->TypeID != ID_AIR && curBlock->TypeID != ID_WATER)
		return;

	if(curBlock->LightValue >= value)
		return;

	curBlock->LightValue = value;

	LightProp(PTR + xShift, value - 1);
	LightProp(PTR + yShift, value - 1);
	LightProp(PTR - xShift, value - 1);
	LightProp(PTR - yShift, value - 1);
}

float cWorld::GetInterpolatedHeight( int x, int y )
{
	unsigned int ChunkWidth = WorldStorage->GetChunkWidth();

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

void cWorld::FillQueue( int centralX, int centralY, int cps )
{
	int width2 = (cps - 1) / 2;

	loadingQueue.push_back(LoadQuery(centralX, centralY, Load));
	loadingQueue.push_back(LoadQuery(centralX, centralY, UpLight));
	loadingQueue.push_back(LoadQuery(centralX, centralY, ScatterLight));
	loadingQueue.push_back(LoadQuery(centralX, centralY, Tesselate));

	for (int i = 0; i <= width2 ; i++)	//for all
	{
		int startX = centralX - i;
		int startY = centralY - i;
		int endX = centralX + i;
		int endY = centralY + i;

		for (int index = startX; index < endX ; index++)
		{
			loadingQueue.push_back(LoadQuery(index, startY, Load));
			loadingQueue.push_back(LoadQuery(index, startY, UpLight));
			loadingQueue.push_back(LoadQuery(index, startY, ScatterLight));
			loadingQueue.push_back(LoadQuery(index, startY, Tesselate));
		}

		for (int index = startY; index < endY ; index++)
		{
			loadingQueue.push_back(LoadQuery(endX, index, Load));
			loadingQueue.push_back(LoadQuery(endX, index, UpLight));
			loadingQueue.push_back(LoadQuery(endX, index, ScatterLight));
			loadingQueue.push_back(LoadQuery(endX, index, Tesselate));
		}

		for (int index = endX; index > startX ; index--)
		{
			loadingQueue.push_back(LoadQuery(index, endY, Load));
			loadingQueue.push_back(LoadQuery(index, endY, UpLight));
			loadingQueue.push_back(LoadQuery(index, endY, ScatterLight));
			loadingQueue.push_back(LoadQuery(index, endY, Tesselate));
		}

		for (int index = endY; index > startY ; index--)
		{
			loadingQueue.push_back(LoadQuery(startX, index, Load));
			loadingQueue.push_back(LoadQuery(startX, index, UpLight));
			loadingQueue.push_back(LoadQuery(startX, index, ScatterLight));
			loadingQueue.push_back(LoadQuery(startX, index, Tesselate));
		}
	}
}

typesStorage * cWorld::GetTypesStorage()
{
	return TypesStorage;
}

void cWorld::SetWorldName( const std::wstring & worldName )
{
	this->worldName = worldName;
}

std::wstring cWorld::GetWorldName() const
{
	return worldName;
}