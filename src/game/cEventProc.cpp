#include "cEventProc.h"

void cGameEventHandler::Initialize( cPlayer * _p, cWorld * _c, EntityManager * _em, ItemManager * _im )
{
	this->pPlayer = _p;
	this->pWorld = _c;
	this->pEM = _em;
	this->pIM = _im;
}

void cGameEventHandler::onBlockRemoved( BlockType bt, Vector3I Pos )
{
	if(bt == ID_GRASS)
		pEM->CreateItemEntity(Vector3D(Pos.x + 0.5f, Pos.y + 0.5f, Pos.z + 0.5f), Vector3D(0.0f, 0.0f, 2.5f), pIM->GetBlockItem(ID_DIRT));
	else
		pEM->CreateItemEntity(Vector3D(Pos.x + 0.5f, Pos.y + 0.5f, Pos.z + 0.5f), Vector3D(0.0f, 0.0f, 2.5f), pIM->GetBlockItem(bt));
}

void cGameEventHandler::onBlockAdded(BlockType /*bt*/)
{
}

void cGameEventHandler::onPlayerMoves()
{
	static int PlayerCX = 0;
	static int PlayerCY = 0;
	static int prePlayerCX = PlayerCX;
	static int prePlayerCY = PlayerCY;

	prePlayerCX = PlayerCX;
	prePlayerCY = PlayerCY;

	PlayerCX = (int)floor(pPlayer->GetPosition().x / (int)pWorld->GetDataStorage()->GetChunkWidth());
	PlayerCY = (int)floor(pPlayer->GetPosition().y / (int)pWorld->GetDataStorage()->GetChunkWidth());

	if(PlayerCX != prePlayerCX)
	{
		if(PlayerCX - prePlayerCX < 0)
			pWorld->ShiftChunkX(-1);
		if(PlayerCX - prePlayerCX > 0)
			pWorld->ShiftChunkX(1);
	}

	if(PlayerCY != prePlayerCY)
	{
		if(PlayerCY - prePlayerCY < 0)
			pWorld->ShiftChunkY(-1);
		if(PlayerCY - prePlayerCY > 0)
			pWorld->ShiftChunkY(1);
	}
}

bool cGameEventHandler::onPlayerPicksItem( Item * item )
{
	return pPlayer->GetInventoryPtr()->AddItem(item);
}

void cGameEventHandler::onPlayerDropsItem( Item * item )
{
	if(!pPlayer->GetInventoryPtr()->RemoveItem(item))
	{
		MainLog.Error("Dropping item that doesn't exist");
		return;
	}

	auto pos = pPlayer->GetPosition();
	pos.z += 10.0f;
	pEM->CreateItemEntity(pos, Vector3D(0.0, 0.0, 3.0), item);
}
