#include "player.h"

const float BlockRadius = 4.0f;	//насколько далеко можно выделять блок

cPlayer::cPlayer() : 
	pWorldStorage(nullptr), fSpeed(3.0f), PlayerHeight(1.6f) 
{
	Inventory = new cInventory;
}

cPlayer::~cPlayer() 
{
	if(Inventory)
		delete Inventory;
}

void cPlayer::Initialize( cWorldStorage * Land )
{
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

void cPlayer::LevelUp()
{
	Stats.sLevel++;
}

int cPlayer::GetHunger() const
{
	return Stats.iHunger;
}

int cPlayer::GetHealth() const
{
	return Stats.iHealth;
}

int cPlayer::GetFastFatique() const
{
	return Stats.iFastFatique;
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

int cPlayer::GetLongFatique() const
{
	return Stats.iLongFatique;
}

cInventory * cPlayer::GetInventoryPtr()
{
	return Inventory;
}

void cPlayer::SetVelocities( const Vector3D & velocities )
{
	PlayerBox.GetPtr()->Velocities = velocities;
}

void cPlayer::SetHunger( int hungerLevel )
{
	Stats.iHunger = hungerLevel;
}

void cPlayer::SetHealth( int healthLevel )
{
	Stats.iHealth = healthLevel;
}

void cPlayer::SetLongFatique( int fatiqueLevel )
{
	Stats.iLongFatique = fatiqueLevel;
}

void cPlayer::SetFastFatique( int fatiqueLevel )
{
	Stats.iFastFatique = fatiqueLevel;
}
