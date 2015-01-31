#include "WorkspaceActions.h"

MeshEditor::AddBlockAction::AddBlockAction(const MathLib::Vector3I & pos)
    : pos(pos)
{
}

void MeshEditor::AddBlockAction::Execute(BlockWorkspace * ws)
{
    if (exec)
        return;

    auto &block = ws->GetBlock(pos.x, pos.y, pos.z);
    if (block.exist)
        exec = false;

    block.exist = true;
    block.color = { 0.9f, 0.9f, 0.9f };
}

void MeshEditor::AddBlockAction::Undo(BlockWorkspace * ws)
{
    if (!exec)
        return;

    ws->GetBlock(pos.x, pos.y, pos.z).exist = false;
}

//////////////////////////////////////////////////////////////////////////

MeshEditor::RemoveBlockAction::RemoveBlockAction(const MathLib::Vector3I & pos)
    : pos(pos)
{
}

void MeshEditor::RemoveBlockAction::Execute(BlockWorkspace * ws)
{
    if (exec)
        return;

    auto &block = ws->GetBlock(pos.x, pos.y, pos.z);
    if (!block.exist)
        exec = false;

    block.exist = false;
    block.color = { 0.9f, 0.9f, 0.9f };
}

void MeshEditor::RemoveBlockAction::Undo(BlockWorkspace * ws)
{
    if (!exec)
        return;

    ws->GetBlock(pos.x, pos.y, pos.z).exist = true;
}

//////////////////////////////////////////////////////////////////////////

MeshEditor::SetBlockColorAction::SetBlockColorAction(const MathLib::Vector3I & pos, const MathLib::Vector3F & newColor)
    : pos(pos)
    , newColor(newColor)
    , oldColor()
{
}

void MeshEditor::SetBlockColorAction::Execute(BlockWorkspace * ws)
{
    Block &block = ws->GetBlock(pos.x, pos.y, pos.z);
    oldColor = block.color;
    block.color = newColor;
}

void MeshEditor::SetBlockColorAction::Undo(BlockWorkspace * ws)
{
    Block &block = ws->GetBlock(pos.x, pos.y, pos.z);
    block.color = oldColor;
}
