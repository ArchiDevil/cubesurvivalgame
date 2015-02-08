#pragma once

#include <MathLib/math.h>

namespace MeshEditor
{
    struct Block
    {
        MathLib::Vector3F color = {};
        bool exist = false;
    };

    struct Brush
    {
        enum class BrushType
        {
            BT_BlockArray,
            BT_Sphere
        };

        Brush(MathLib::Vector3F color = { 1.0f, 1.0f, 1.0f })
            : color(color)
        {
        }

        BrushType type = BrushType::BT_BlockArray;
        MathLib::Vector3F color;
        MathLib::Vector3I startPos = {};
        MathLib::vec3<size_t> size = {};
    };
}
