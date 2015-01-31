#pragma once

#include <MathLib/math.h>

#include "BlockWorkspace.h"

namespace MeshEditor
{
    class BlockWorkspace;

    class IAction
    {
    public:
        virtual ~IAction() = default;

        virtual void Execute(BlockWorkspace * ws) = 0;
        virtual void Undo(BlockWorkspace * ws) = 0;
    };

    class AddBlockAction final : public IAction
    {
    public:
        AddBlockAction(const MathLib::Vector3I & pos);
        virtual void Execute(BlockWorkspace * ws) override;
        virtual void Undo(BlockWorkspace * ws) override;

    private:
        bool exec = false;
        MathLib::Vector3I pos;
    };

    class RemoveBlockAction final : public IAction
    {
    public:
        RemoveBlockAction(const MathLib::Vector3I & pos);
        virtual void Execute(BlockWorkspace * ws) override;
        virtual void Undo(BlockWorkspace * ws) override;

    private:
        bool exec = false;
        MathLib::Vector3I pos;
    };

    class SetBlockColorAction final : public IAction
    {
    public:
        SetBlockColorAction(const MathLib::Vector3I & pos, const MathLib::Vector3F & newColor);
        virtual void Execute(BlockWorkspace * ws) override;
        virtual void Undo(BlockWorkspace * ws) override;

    private:
        MathLib::Vector3I pos;
        MathLib::Vector3F newColor;
        MathLib::Vector3F oldColor;
    };
}
