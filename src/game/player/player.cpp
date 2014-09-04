#include "player.h"

#include "../Items/ItemManager.h"
#include "cInventory.h"

const float BlockRadius = 4.0f;	//насколько далеко можно выделять блок

cPlayer::cPlayer() 
	: pWorldStorage(nullptr)
	, fSpeed(3.0f)
	, PlayerHeight(1.6f) 
{
}

cPlayer::~cPlayer() 
{
}

void cPlayer::Initialize( cWorldStorage * Land, ItemManager * pItemManager )
{
	Inventory.reset(new cInventory(pItemManager));
	pWorldStorage = Land;
	PlayerBox = cSimplePhysicsEngine::GetInstance().GetPlayerBBox();
	SelectedBlock.Initialize();
}

void cPlayer::FindSelectedBlock( const Vector3D & LOOK )
{
	if(PlayerBox.GetPtr()->Position.z >= 0.0f)
	{
		Vector3D LookTemp;
		bool flag = false;

		for(float i = 0.1f; i < BlockRadius; i += 0.1f)
		{
			LookTemp = LOOK * i;

			double x = LookTemp.x + PlayerBox.GetPtr()->Position.x;
			double y = LookTemp.y + PlayerBox.GetPtr()->Position.y;
			double z = LookTemp.z + PlayerBox.GetPtr()->Position.z + PlayerHeight;

			if(pWorldStorage->GetBlock((int)floor(x), (int)floor(y), (int)floor(z))->TypeID != ID_AIR)
			{
				SelectedBlock.SetPositionSolid(Vector3D(floor((float)x) + 0.5f, floor((float)y) + 0.5f, floor((float)z)));
				flag = true;
				break;
			}
			else
			{
				continue;
			}
		}

		if(!flag)
			SelectedBlock.SetPositionSolid(Vector3D(-1.0f, -1.0f, -1.0f));

		flag = false;

		for(float i = 0.1f; i < BlockRadius; i += 0.1f)
		{
			LookTemp = LOOK * i;

			double x = LookTemp.x + PlayerBox.GetPtr()->Position.x;
			double y = LookTemp.y + PlayerBox.GetPtr()->Position.y;
			double z = LookTemp.z + PlayerBox.GetPtr()->Position.z + PlayerHeight;

			if(pWorldStorage->GetBlock((int)floor(x), (int)floor(y), (int)floor(z))->TypeID != ID_AIR)
			{
				LookTemp = LOOK * (i - 0.1f);

				x = LookTemp.x + PlayerBox.GetPtr()->Position.x;
				y = LookTemp.y + PlayerBox.GetPtr()->Position.y;
				z = LookTemp.z + PlayerBox.GetPtr()->Position.z + PlayerHeight;

				SelectedBlock.SetPositionFree(Vector3D(floor((float)x) + 0.5f, floor((float)y) + 0.5f, floor((float)z)));
				flag = true;
				break;
			}
			else
			{
				continue;
			}
		}

		if(!flag)
			SelectedBlock.SetPositionFree(Vector3D(-1.0f, -1.0f, -1.0f));
	}
	else
	{
		SelectedBlock.SetPositionFree(Vector3D(-1.0f, -1.0f, -1.0f));
		SelectedBlock.SetPositionSolid(Vector3D(-1.0f, -1.0f, -1.0f));
	}
}

float cPlayer::GetSpeed() const
{
	return fSpeed;
}

Vector3D cPlayer::GetPosition()
{
	return PlayerBox.GetPtr()->Position;
}

Vector3D * cPlayer::GetVelocitiesPtr()
{
	return &PlayerBox.GetPtr()->Velocities;
}

float cPlayer::GetHeight() const
{
	return PlayerHeight;
}

cSelectedBlock * cPlayer::GetSelectedBlockPtr()
{
	return &SelectedBlock;
}

void cPlayer::SetPosition( float x, float y, float z )
{
	PlayerBox.GetPtr()->Position = Vector3D(x, y, z);
}

void cPlayer::SetPosition( const Vector3D & vec )
{
	PlayerBox.GetPtr()->Position = vec;
}

void cPlayer::ResetVelocities()
{
	PlayerBox.GetPtr()->Velocities = Vector3D();
}

cInventory * cPlayer::GetInventoryPtr()
{
	return Inventory.get();
}

void cPlayer::SetVelocities( const Vector3D & velocities )
{
	PlayerBox.GetPtr()->Velocities = velocities;
}

unsigned cPlayer::GetHunger() const
{
	return Stats.hunger;
}

unsigned cPlayer::GetHealth() const
{
	return Stats.health;
}

unsigned cPlayer::GetTemperature() const
{
	return Stats.temperature;
}

void cPlayer::SetHunger(int hunger)
{
	Stats.hunger = hunger;
}

void cPlayer::SetHealth(int health)
{
	Stats.health = health;
}

void cPlayer::SetTemperature(int temperature)
{
	Stats.temperature = temperature;
}
