#pragma once

#include <MathLib/math.h>

namespace MeshEditor
{
    class BlockStorage;

    class IAction
    {
    public:
        virtual ~IAction() = default;

        virtual void Execute(BlockStorage * bs) = 0;
        virtual void Undo(BlockStorage * bs) = 0;
    };

    class AddBlockAction final : public IAction
    {
    public:
        AddBlockAction(const MathLib::Vector3I & pos);
        virtual void Execute(BlockStorage * bs) override;
        virtual void Undo(BlockStorage * bs) override;

    private:
        bool exec = false;
        MathLib::Vector3I pos;
    };

    class RemoveBlockAction final : public IAction
    {
    public:
        RemoveBlockAction(const MathLib::Vector3I & pos);
        virtual void Execute(BlockStorage * bs) override;
        virtual void Undo(BlockStorage * bs) override;

    private:
        bool exec = false;
        MathLib::Vector3I pos;
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
