#include "cEventProc.h"

#include "player/cInventory.h"
#include "game.h"

void cGameEventHandler::onBlockRemoved( BlockType bt, Vector3I Pos )
{
	//if(bt == ID_GRASS)
	//	pEM->CreateItemEntity(Vector3D(Pos.x + 0.5f, Pos.y + 0.5f, Pos.z + 0.5f), Vector3D(0.0f, 0.0f, 2.5f), pIM->GetBlockItem(ID_DIRT));
	//else
	//	pEM->CreateItemEntity(Vector3D(Pos.x + 0.5f, Pos.y + 0.5f, Pos.z + 0.5f), Vector3D(0.0f, 0.0f, 2.5f), pIM->GetBlockItem(bt));
}

void cGameEventHandler::onBlockAdded(BlockType /*bt*/)
{
}

void cGameEventHandler::onPlayerMoves()
{
	//static int PlayerCX = 0;
	//static int PlayerCY = 0;
	//static int prePlayerCX = PlayerCX;
	//static int prePlayerCY = PlayerCY;

	//prePlayerCX = PlayerCX;
	//prePlayerCY = PlayerCY;

	//PlayerCX = (int)floor(pPlayer->GetPosition().x / (int)pWorld->GetDataStorage()->GetChunkWidth());
	//PlayerCY = (int)floor(pPlayer->GetPosition().y / (int)pWorld->GetDataStorage()->GetChunkWidth());

	//if(PlayerCX != prePlayerCX)
	//{
	//	if(PlayerCX - prePlayerCX < 0)
	//		pWorld->ShiftChunkX(-1);
	//	if(PlayerCX - prePlayerCX > 0)
	//		pWorld->ShiftChunkX(1);
	//}

	//if(PlayerCY != prePlayerCY)
	//{
	//	if(PlayerCY - prePlayerCY < 0)
	//		pWorld->ShiftChunkY(-1);
	//	if(PlayerCY - prePlayerCY > 0)
	//		pWorld->ShiftChunkY(1);
	//}
}

bool cGameEventHandler::onPlayerPicksItem( uint64_t itemId )
{
	return LostIsland::GetGamePtr()->Player->GetInventoryPtr()->AddItem(itemId);
}

void cGameEventHandler::onPlayerDropsItem( uint64_t itemId )
{
	if (!LostIsland::GetGamePtr()->Player->GetInventoryPtr()->RemoveItem(itemId))
	{
		MainLog.Error("Dropping item that doesn't exist");
		return;
	}

	auto pos = LostIsland::GetGamePtr()->Player->GetPosition();
	pos.z += 10.0f;
	LostIsland::GetGamePtr()->EntityMgr->CreateItemEntity(pos, Vector3D(0.0, 0.0, 3.0), itemId);
}
