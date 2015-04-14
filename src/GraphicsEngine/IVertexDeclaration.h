#pragma once

#include <memory>

namespace ShiftEngine
{
    class IVertexDeclaration
    {
    public:
        virtual ~IVertexDeclaration() {}
        virtual void Bind() = 0;
    };

    typedef std::shared_ptr<IVertexDeclaration> IVertexDeclarationPtr;
}
