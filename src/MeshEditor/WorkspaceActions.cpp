#include "WorkspaceActions.h"

#include "Common.h"
#include "BlockStorage.h"

#include <cassert>

MeshEditor::AddBrushAction::AddBrushAction(const Brush & brush)
    : brush(brush)
{
}

void MeshEditor::AddBrushAction::Execute(BlockStorage * bs)
{
    if (affectedBlocks)
        return;

    affectedBlocks.reset(new Block[brush.size.x * brush.size.y * brush.size.z]);

    switch (brush.type)
    {
    case Brush::BrushType::BT_BlockArray:
        for (size_t x = 0; x < brush.size.x; ++x)
            for (size_t y = 0; y < brush.size.y; ++y)
                for (size_t z = 0; z < brush.size.z; ++z)
                    affectedBlocks[x * brush.size.y * brush.size.z + y * brush.size.z + z] = bs->GetBlock(brush.startPos.x + x, brush.startPos.y + y, (int)brush.startPos.z + z);

        for (size_t x = 0; x < brush.size.x; ++x)
        {
            for (size_t y = 0; y < brush.size.y; ++y)
            {
                for (size_t z = 0; z < brush.size.z; ++z)
                {
                    Block& b = bs->GetBlock(brush.startPos.x + x, brush.startPos.y + y, (int)brush.startPos.z + z);
                    if (!b.exist)
                    {
                        b.exist = true;
                        b.color = { 0.9f, 0.9f, 0.9f };
                    }
                }
            }
        }
        break;
    case Brush::BrushType::BT_Sphere:
        assert(false);
        break;
    default:
        assert(false);
        break;
    }
}

void MeshEditor::AddBrushAction::Undo(BlockStorage * bs)
{
    if (!affectedBlocks)
        return;

    switch (brush.type)
    {
    case Brush::BrushType::BT_BlockArray:
        for (size_t x = 0; x < brush.size.x; ++x)
        {
            for (size_t y = 0; y < brush.size.y; ++y)
            {
                for (size_t z = 0; z < brush.size.z; ++z)
                {
                    Block& b = bs->GetBlock(brush.startPos.x + x, brush.startPos.y + y, (int)brush.startPos.z + z);
                    b = affectedBlocks[x * brush.size.y * brush.size.z + y * brush.size.z + z];
                }
            }
        }
        break;
    case Brush::BrushType::BT_Sphere:
        assert(false);
        break;
    default:
        assert(false);
        break;
    }
}

//////////////////////////////////////////////////////////////////////////

MeshEditor::RemoveBrushAction::RemoveBrushAction(const Brush & brush)
    : brush(brush)
{
}

void MeshEditor::RemoveBrushAction::Execute(BlockStorage * bs)
{
    if (affectedBlocks)
        return;

    affectedBlocks.reset(new Block[brush.size.x * brush.size.y * brush.size.z]);

    switch (brush.type)
    {
    case Brush::BrushType::BT_BlockArray:
        for (size_t x = 0; x < brush.size.x; ++x)
            for (size_t y = 0; y < brush.size.y; ++y)
                for (size_t z = 0; z < brush.size.z; ++z)
                    affectedBlocks[x * brush.size.y * brush.size.z + y * brush.size.z + z] = bs->GetBlock(brush.startPos.x + x, brush.startPos.y + y, (int)brush.startPos.z + z);

        for (size_t x = 0; x < brush.size.x; ++x)
        {
            for (size_t y = 0; y < brush.size.y; ++y)
            {
                for (size_t z = 0; z < brush.size.z; ++z)
                {
                    Block& b = bs->GetBlock(brush.startPos.x + x, brush.startPos.y + y, (int)brush.startPos.z + z);
                    if (b.exist)
                        b.exist = false;
                }
            }
        }
        break;
    case Brush::BrushType::BT_Sphere:
        assert(false);
        break;
    default:
        assert(false);
        break;
    }
}

void MeshEditor::RemoveBrushAction::Undo(BlockStorage * bs)
{
    if (!affectedBlocks)
        return;

    switch (brush.type)
    {
    case Brush::BrushType::BT_BlockArray:
        for (size_t x = 0; x < brush.size.x; ++x)
        {
            for (size_t y = 0; y < brush.size.y; ++y)
            {
                for (size_t z = 0; z < brush.size.z; ++z)
                {
                    Block& b = bs->GetBlock(brush.startPos.x + x, brush.startPos.y + y, (int)brush.startPos.z + z);
                    b = affectedBlocks[x * brush.size.y * brush.size.z + y * brush.size.z + z];
                }
            }
        }
        break;
    case Brush::BrushType::BT_Sphere:
        assert(false);
        break;
    default:
        assert(false);
        break;
    }
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
