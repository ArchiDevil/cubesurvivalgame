#pragma once

#include "world/world.h"
#include "player/player.h"
#include "Items/ItemManager.h"
#include "EntityManager.h"

using MathLib::Vector3I;
using MathLib::Vector3D;

class cPlayer;
class cWorld;
class cEntityManager;
class ItemManager;

class cGameEventHandler
{
public:
	void Initialize(cPlayer * _p, cWorld * _c, cEntityManager * _em, ItemManager * _im);

	//world events
	void onBlockRemoved(BlockType bt, Vector3I Pos);	//���������� ��� �������� �����
	void onBlockAdded(BlockType bt);					//���������� ��� ���������� �����

	//player events
	void onPlayerMoves();								//���������� ��� �������� ������
	void onPlayerAttack();								//��� ����� �������
	bool onPlayerPicksItem(Item * item);				//����� ����� �������
	void onPlayerDropsItem(Item * item);				//����� ������� �����
	
private:
	//use some more

	cPlayer * pPlayer;
	cWorld * pWorld;
	cEntityManager * pEM;
	ItemManager * pIM;
};