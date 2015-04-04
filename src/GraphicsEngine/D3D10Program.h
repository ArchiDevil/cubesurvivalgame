#pragma once

#include <memory>
#include <map>
#include <vector>

#include "IProgram.h"

#include "D3D10Texture.h"
#include "D3D10ShaderProgram.h"
#include "D3D10VertexDeclaration.h"

using std::map;
using std::string;
using std::vector;

namespace ShiftEngine
{
    struct bufferDescription
    {
        bufferDescription()
        {
        }

        string Name;
        bool isDirty = true;
        uint32_t Size = 0;
        uint32_t StartSlotVertex = UINT_MAX;
        uint32_t StartSlotPixel = UINT_MAX;
        ID3D10Buffer * cbFromShader = nullptr;
        uint8_t * bufferData = nullptr;
        bool vertex = false;
        bool pixel = false;
    };

    struct varDesc
    {
        string Name;
        uint32_t Offset;
        uint32_t Size;
        uint32_t BufferIndex;
    };

    struct resDesc
    {
        resDesc()
            : Name("")
            , BindPointVertex(UINT_MAX)
            , BindPointPixel(UINT_MAX)
            , View(nullptr)
            , isDirty(true)
            , pixel(false)
            , vertex(false)
        {}
        string Name = "";
        uint32_t BindPointVertex = UINT_MAX;
        uint32_t BindPointPixel = UINT_MAX;
        ID3D10ShaderResourceView * View = nullptr;
        bool isDirty = true;
        bool vertex = false;
        bool pixel = false;
    };

    class D3D10Program : public IProgram
    {
        friend class D3D10ContextManager;
    public:
        D3D10Program(D3D10ShaderPtr & _vertexShader, D3D10ShaderPtr & _pixelShader, ID3D10Device * _pDevice);
        ~D3D10Program();

        virtual bool SetMatrixConstantByName(const char * nameInShader, const float * mat) override;
        virtual bool SetScalarConstantByName(const char * nameInShader, const float * scalar) override;
        virtual bool SetVectorConstantByName(const char * nameInShader, const float * vec) override;
        virtual bool SetArrayConstantByName(const char * nameInShader, const void * data) override;
        virtual bool SetTextureByName(const char * textureName, const TexturePtr & texture) override;

        virtual void SetMatrixConstantByIndex(unsigned int index, const float * mat) override;
        virtual void SetScalarConstantByIndex(unsigned int index, const float * mat) override;
        virtual void SetVectorConstantByIndex(unsigned int index, const float * mat) override;
        virtual void SetArrayConstantByIndex(unsigned int index, const void * data) override;
        virtual bool SetTextureByIndex(unsigned int index, const TexturePtr & texture) override;

        virtual bool GetVariableIndex(const char * name, unsigned int * index) override;
        virtual bool GetResourceIndex(const char * name, unsigned int * index) override;

        virtual void Apply(bool setShader) override;

    private:
        void ParseInfo();

        void SetUniformByIndex(unsigned int index, const void * data);

        //mapped buffers
        vector<bufferDescription> constantBuffers;
        vector<varDesc> variables;

        //samplers only
        vector<resDesc> resources;

        //shaders to bound
        D3D10ShaderPtr vertexShader;
        D3D10ShaderPtr pixelShader;

        ID3D10Device * pDevice;
    };

    typedef std::shared_ptr<D3D10Program> D3D10ProgramPtr;

}   //end of namespace ShiftEngine
