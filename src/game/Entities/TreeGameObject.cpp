#include "TreeGameObject.h"

TreeGameObject::TreeGameObject(ShiftEngine::MeshNode * mesh)
	: StaticGameObject(mesh)
{
}

TreeGameObject::~TreeGameObject()
{
}

std::unique_ptr<IEntityAction> TreeGameObject::GetInteraction()
{
	throw std::exception("The method or operation is not implemented.");
}
