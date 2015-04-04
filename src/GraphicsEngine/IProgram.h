#pragma once

#include <memory>

// remove and unify by apis
#include "D3D10Texture.h"

namespace ShiftEngine 
{
    class IProgram
    {
    public:
        virtual ~IProgram() {}

        virtual bool SetMatrixConstantByName(const char * nameInShader, const float * mat) = 0;
        virtual bool SetScalarConstantByName(const char * nameInShader, const float * scalar) = 0;
        virtual bool SetVectorConstantByName(const char * nameInShader, const float * vec) = 0;
        virtual bool SetArrayConstantByName(const char * nameInShader, const void * data) = 0;
        virtual bool SetTextureByName(const char * textureName, const TexturePtr & texture) = 0;

        virtual void SetMatrixConstantByIndex(unsigned int index, const float * mat) = 0;
        virtual void SetScalarConstantByIndex(unsigned int index, const float * mat) = 0;
        virtual void SetVectorConstantByIndex(unsigned int index, const float * mat) = 0;
        virtual void SetArrayConstantByIndex(unsigned int index, const void * data) = 0;
        virtual bool SetTextureByIndex(unsigned int index, const TexturePtr & texture) = 0;

        virtual bool GetVariableIndex(const char * name, unsigned int * index) = 0;
        virtual bool GetResourceIndex(const char * name, unsigned int * index) = 0;

        virtual void Apply(bool setShader) = 0;
    };

    typedef std::shared_ptr<IProgram> IProgramPtr;
}
