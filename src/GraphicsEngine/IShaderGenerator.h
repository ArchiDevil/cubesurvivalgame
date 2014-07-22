#pragma once

#include <vector>

#include "VertexTypes.h"
#include "MaterialInfo.h"

namespace ShiftEngine
{
	class IShaderGenerator
	{
	public:
		virtual ~IShaderGenerator() {}
		virtual std::string CreateShaderCode(const VertexSemantic & verticesInfo, const MaterialInfo & flags) = 0;
	};
}