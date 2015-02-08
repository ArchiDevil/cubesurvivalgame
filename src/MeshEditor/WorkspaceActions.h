#pragma once

#include "BlockWorkspace.h"

#include <MathLib/math.h>

#include <memory>

namespace MeshEditor
{
    class BlockStorage;
    struct Brush;

    class IAction
    {
    public:
        virtual ~IAction() = default;

        virtual void Execute(BlockStorage * bs) = 0;
        virtual void Undo(BlockStorage * bs) = 0;
    };

    class AddBrushAction final : public IAction
    {
    public:
        AddBrushAction(const Brush & brush);
        virtual void Execute(BlockStorage * bs) override;
        virtual void Undo(BlockStorage * bs) override;

    private:
        std::unique_ptr<Block[]> affectedBlocks = nullptr;
        const Brush brush;
    };

    class RemoveBrushAction final : public IAction
    {
    public:
        RemoveBrushAction(const Brush & brush);
        virtual void Execute(BlockStorage * bs) override;
        virtual void Undo(BlockStorage * bs) override;

    private:
        std::unique_ptr<Block[]> affectedBlocks = nullptr;
        const Brush brush;
    };

    class SetBlockColorAction final : public IAction
    {
    public:
        SetBlockColorAction(const MathLib::Vector3I & pos, const MathLib::Vector3F & newColor);
        virtual void Execute(BlockStorage * bs) override;
        virtual void Undo(BlockStorage * bs) override;

    private:
        MathLib::Vector3I pos;
        MathLib::Vector3F newColor;
        MathLib::Vector3F oldColor;
    };

    class ResizeAction final : public IAction
    {
    public:
        ResizeAction(const MathLib::Vector3I& posDelta, const MathLib::Vector3I& negDelta);
        virtual void Execute(BlockStorage * bs) override;
        virtual void Undo(BlockStorage * bs) override;

    private:
        MathLib::Vector3I posDelta;
        MathLib::Vector3I negDelta;
    };
}
