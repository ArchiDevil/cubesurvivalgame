#pragma once

#include "IProgram.h"
#include "Material.h"
#include "VertexTypes.h"

namespace ShiftEngine
{
    class IShaderManager
    {
    public:
        virtual IProgramPtr CreateProgramFromFile(const std::wstring & fileName) = 0;
        virtual IProgramPtr CreateProgramFromSource(const std::string & source) = 0;
        virtual IProgramPtr CreateProgramFromMaterialFlags(const MaterialInfo & mtlInfo, const VertexSemantic & verticesInfo) = 0;
    };
}
