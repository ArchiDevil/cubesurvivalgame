#pragma once

#include <memory>
#include <map>
#include <vector>

#include "../../IProgram.h"

#include "D3D10Texture.h"
#include "D3D10ShaderProgram.h"
#include "D3D10VertexDeclaration.h"

namespace ShiftEngine
{
    struct D3D10BufferDescription
    {
        std::string Name = "";
        bool isDirty = true;
        uint32_t Size = 0;

        uint32_t StartSlotVertex = UINT_MAX;
        uint32_t StartSlotPixel = UINT_MAX;
        uint32_t StartSlotGeometry = UINT_MAX;

        ID3D10Buffer * cbFromShader = nullptr;
        uint8_t * bufferData = nullptr;

        bool vertex = false;
        bool pixel = false;
        bool geometry = false;
    };

    struct D3D10VarDesc
    {
        std::string Name = "";
        uint32_t Offset = 0;
        uint32_t Size = 0;
        uint32_t BufferIndex = 0;
    };

    struct D3D10ResDesc
    {
        std::string Name = "";
        uint32_t BindPointVertex = UINT_MAX;
        uint32_t BindPointPixel = UINT_MAX;
        uint32_t BindPointGeometry = UINT_MAX;
        ITexturePtr View = nullptr;
        bool isDirty = true;

        bool vertex = false;
        bool pixel = false;
        bool geometry = false;
    };

    class D3D10Program : public IProgram
    {
        friend class D3D10ContextManager;
    public:
        D3D10Program(D3D10ShaderPtr & vertexShader, D3D10ShaderPtr & pixelShader, ID3D10Device * _pDevice);
        ~D3D10Program();

        virtual bool SetMatrixConstantByName(const char * nameInShader, const float * mat) override;
        virtual bool SetScalarConstantByName(const char * nameInShader, const float * scalar) override;
        virtual bool SetVectorConstantByName(const char * nameInShader, const float * vec) override;
        virtual bool SetArrayConstantByName(const char * nameInShader, const void * data) override;
        virtual bool SetTextureByName(const char * textureName, const ITexturePtr & texture) override;

        virtual void SetMatrixConstantByIndex(unsigned int index, const float * mat) override;
        virtual void SetScalarConstantByIndex(unsigned int index, const float * mat) override;
        virtual void SetVectorConstantByIndex(unsigned int index, const float * mat) override;
        virtual void SetArrayConstantByIndex(unsigned int index, const void * data) override;
        virtual bool SetTextureByIndex(unsigned int index, const ITexturePtr & texture) override;

        virtual bool GetVariableIndex(const char * name, unsigned int * index) override;
        virtual bool GetResourceIndex(const char * name, unsigned int * index) override;

        virtual void Apply(bool setShader) override;

    private:
        void ParseInfo();

        void SetUniformByIndex(unsigned int index, const void * data);

        //mapped buffers
        std::vector<D3D10BufferDescription> constantBuffers;
        std::vector<D3D10VarDesc> variables;

        //samplers only
        std::vector<D3D10ResDesc> resources;

        //shaders to bound
        D3D10ShaderPtr vertexShader = nullptr;
        D3D10ShaderPtr pixelShader = nullptr;
        D3D10ShaderPtr geometryShader = nullptr;

        ID3D10Device * pDevice = nullptr;
    };

    typedef std::shared_ptr<D3D10Program> D3D10ProgramPtr;

}   //end of namespace ShiftEngine
