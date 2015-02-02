#include "WorkspaceActions.h"

#include "BlockStorage.h"

MeshEditor::AddBlockAction::AddBlockAction(const MathLib::Vector3I & pos)
    : pos(pos)
{
}

void MeshEditor::AddBlockAction::Execute(BlockStorage * bs)
{
    if (exec)
        return;

    auto &block = bs->GetBlock(pos.x, pos.y, pos.z);
    if (block.exist)
        exec = false;

    block.exist = true;
    block.color = { 0.9f, 0.9f, 0.9f };
    exec = true;
}

void MeshEditor::AddBlockAction::Undo(BlockStorage * bs)
{
    if (!exec)
        return;

    bs->GetBlock(pos.x, pos.y, pos.z).exist = false;
}

//////////////////////////////////////////////////////////////////////////

MeshEditor::RemoveBlockAction::RemoveBlockAction(const MathLib::Vector3I & pos)
    : pos(pos)
{
}

void MeshEditor::RemoveBlockAction::Execute(BlockStorage * bs)
{
    if (exec)
        return;

    auto &block = bs->GetBlock(pos.x, pos.y, pos.z);
    if (!block.exist)
        exec = false;

    block.exist = false;
    block.color = { 0.9f, 0.9f, 0.9f };
    exec = true;
}

void MeshEditor::RemoveBlockAction::Undo(BlockStorage * bs)
{
    if (!exec)
        return;

    bs->GetBlock(pos.x, pos.y, pos.z).exist = true;
}

//////////////////////////////////////////////////////////////////////////

MeshEditor::SetBlockColorAction::SetBlockColorAction(const MathLib::Vector3I & pos, const MathLib::Vector3F & newColor)
    : pos(pos)
    , newColor(newColor)
    , oldColor()
{
}

void MeshEditor::SetBlockColorAction::Execute(BlockStorage * bs)
{
    Block &block = bs->GetBlock(pos.x, pos.y, pos.z);
    oldColor = block.color;
    block.color = newColor;
}

void MeshEditor::SetBlockColorAction::Undo(BlockStorage * bs)
{
    Block &block = bs->GetBlock(pos.x, pos.y, pos.z);
    block.color = oldColor;
}

//////////////////////////////////////////////////////////////////////////

MeshEditor::ResizeAction::ResizeAction(const MathLib::Vector3I& posDelta, const MathLib::Vector3I& negDelta)
    : negDelta(negDelta)
    , posDelta(posDelta)
{
}

void MeshEditor::ResizeAction::Execute(BlockStorage * bs)
{
    bs->Resize(posDelta.x, posDelta.y, posDelta.z, negDelta.x, negDelta.y, negDelta.z);
}

void MeshEditor::ResizeAction::Undo(BlockStorage * bs)
{
    bs->Resize(-posDelta.x, -posDelta.y, -posDelta.z, -negDelta.x, -negDelta.y, -negDelta.z);
}
