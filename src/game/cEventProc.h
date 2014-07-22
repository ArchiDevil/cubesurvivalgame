#pragma once

#include "world/world.h"
#include "player/player.h"
#include "Items/cItemManager.h"
#include "EntityManager.h"

using MathLib::Vector3I;
using MathLib::Vector3D;

class cPlayer;
class cWorld;
class cEntityManager;
class cItemManager;

class cGameEventHandler
{
public:
	void Initialize(cPlayer * _p, cWorld * _c, cEntityManager * _em, cItemManager * _im);

	//world events
	void onBlockRemoved(BlockType bt, Vector3I Pos);	//���������� ��� �������� �����
	void onBlockAdded(BlockType bt);					//���������� ��� ���������� �����

	//player events
	void onPlayerMoves();								//���������� ��� �������� ������
	void onPlayerAttack();								//��� ����� �������
	bool onPlayerPicksItem(cItem * item);				//����� ����� �������
	void onPlayerDropsItem(cItem * item);				//����� ������� �����
	
private:
	//use some more

	cPlayer * pPlayer;
	cWorld * pWorld;
	cEntityManager * pEM;
	cItemManager * pIM;
};