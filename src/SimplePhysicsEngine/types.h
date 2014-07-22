#pragma once

#include "../MathLib/math.h"
#include "../Utilities/cRefcounted.h"

using MathLib::Vector3D;

struct PhysObject : public Refcounted
{
	PhysObject(Vector3D _pos = Vector3D(0.0f, 0.0f, 0.0f), 
		Vector3D _vel = Vector3D(0.0f, 0.0f, 0.0f))
		: Position(_pos), Velocities(_vel), toDelete(false){};
	PhysObject(const PhysObject & ref);

	Vector3D Position;
	Vector3D Velocities;
	bool toDelete;
};

class pPhysObject
{
public:
	pPhysObject(PhysObject * obj = nullptr)
		: ptr(obj) 
	{
		if(obj)
			obj->addRef();
	}

	pPhysObject(const pPhysObject & ref)
	{
		if(ref.ptr)
		{
			this->ptr = ref.ptr;
			ptr->addRef();
		}
	}

	~pPhysObject()
	{
		if(this->ptr)
			ptr->release();
	}

	PhysObject * GetPtr()
	{
		if(this->ptr)
			return ptr;
		else
			return nullptr;
	}

	pPhysObject & operator = (const pPhysObject & ref)
	{
		if(this->ptr)
			this->ptr->release();
		if(ref.ptr)
		{
			this->ptr = ref.ptr;
			this->ptr->addRef();
		}
		return *this;
	}

private:
	PhysObject * ptr;
};

struct CollisionInfo	//now just for entities
{
	CollisionInfo(pPhysObject _obj) : obj(_obj) {};
	pPhysObject obj;
};