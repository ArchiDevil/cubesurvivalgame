#pragma once

#include <memory>
#include <map>
#include <vector>

#include "../../IProgram.h"

#include "D3D11Texture.h"
#include "D3D11ShaderProgram.h"
#include "D3D11VertexDeclaration.h"

using std::map;
using std::string;
using std::vector;

namespace ShiftEngine
{
    struct D3D11BufferDescription
    {
        string Name = "";
        bool isDirty = true;
        uint32_t Size = 0;
        uint32_t StartSlotVertex = UINT_MAX;
        uint32_t StartSlotPixel = UINT_MAX;
        ID3D11Buffer * cbFromShader = nullptr;
        uint8_t * bufferData = nullptr;
        bool vertex = false;
        bool pixel = false;
    };

    struct D3D11VarDesc
    {
        string Name = "";
        uint32_t Offset = 0;
        uint32_t Size = 0;
        uint32_t BufferIndex = 0;
    };

    struct D3D11ResDesc
    {
        string Name = "";
        uint32_t BindPointVertex = UINT_MAX;
        uint32_t BindPointPixel = UINT_MAX;
        ITexturePtr View = nullptr;
        bool isDirty = true;
        bool vertex = false;
        bool pixel = false;
    };

    class D3D11Program : public IProgram
    {
        friend class D3D11ContextManager;
    public:
        D3D11Program(D3D11ShaderPtr & _vertexShader, D3D11ShaderPtr & _pixelShader, ID3D11Device * _pDevice, ID3D11DeviceContext * pDeviceContext);
        ~D3D11Program();

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
        vector<D3D11BufferDescription> constantBuffers;
        vector<D3D11VarDesc> variables;

        //samplers only
        vector<D3D11ResDesc> resources;

        //shaders to bound
        D3D11ShaderPtr vertexShader = nullptr;
        D3D11ShaderPtr pixelShader = nullptr;

        ID3D11Device * pDevice = nullptr;
        ID3D11DeviceContext * pDeviceContext = nullptr;
    };

    typedef std::shared_ptr<D3D11Program> D3D11ProgramPtr;

}   //end of namespace ShiftEngine
