#include "D3D11Program.h"

#include <cassert>
#include <D3Dcompiler.h>

ShiftEngine::D3D11Program::D3D11Program(D3D11ShaderPtr & vertexShader, D3D11ShaderPtr & pixelShader, ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
    : vertexShader(vertexShader)
    , pixelShader(pixelShader)
    , pDevice(pDevice)
    , pDeviceContext(pDeviceContext)
{
    ParseInfo();
}

ShiftEngine::D3D11Program::~D3D11Program()
{
}

void ShiftEngine::D3D11Program::ParseInfo()
{
    D3D11_SHADER_DESC desc;
    //VERTEX SHADER
    vertexShader->reflection->GetDesc(&desc);
    for (uint32_t i = 0; i < desc.ConstantBuffers; i++)
    {
        ID3D11ShaderReflectionConstantBuffer * cb;
        cb = vertexShader->reflection->GetConstantBufferByIndex(i);
        D3D11_SHADER_BUFFER_DESC buffDesc;
        cb->GetDesc(&buffDesc);

        bool flag = false;
        D3D11BufferDescription * pBuffDesc = nullptr;
        uint32_t buffIndex = 0;
        for (size_t k = 0; k < constantBuffers.size(); k++)
        {
            if (constantBuffers[k].Name == buffDesc.Name)
            {
                flag = true;
                pBuffDesc = &constantBuffers[k];
                buffIndex = k;
            }
        }

        if (!flag)
        {
            constantBuffers.push_back(D3D11BufferDescription());
            pBuffDesc = &constantBuffers[constantBuffers.size() - 1];
            buffIndex = constantBuffers.size() - 1;
        }

        pBuffDesc->isDirty = true;
        pBuffDesc->Name = buffDesc.Name;
        pBuffDesc->Size = buffDesc.Size;
        pBuffDesc->StartSlotVertex = i;
        if (pBuffDesc->bufferData == nullptr) pBuffDesc->bufferData = new uint8_t[buffDesc.Size];
        pBuffDesc->vertex = true;

        for (uint32_t j = 0; j < buffDesc.Variables; j++)
        {
            ID3D11ShaderReflectionVariable * pVar = nullptr;
            pVar = cb->GetVariableByIndex(j);
            D3D11_SHADER_VARIABLE_DESC variableDesc;
            pVar->GetDesc(&variableDesc);
            D3D11VarDesc variable;
            unsigned int curIndex = 0;
            if (!GetVariableIndex(variableDesc.Name, &curIndex))
            {
                variable.Name = variableDesc.Name;
                variable.Offset = variableDesc.StartOffset;
                variable.Size = variableDesc.Size;
                variable.BufferIndex = buffIndex;
                variables.push_back(variable);
                if (variableDesc.DefaultValue != nullptr)
                    memcpy(pBuffDesc->bufferData + variableDesc.StartOffset, variableDesc.DefaultValue, variableDesc.Size);
            }
        }
    }

    for (uint32_t i = 0; i < desc.BoundResources; i++)
    {
        D3D11_SHADER_INPUT_BIND_DESC bindDesc;
        vertexShader->reflection->GetResourceBindingDesc(i, &bindDesc);
        if (bindDesc.Type == D3D_SIT_TEXTURE)
        {
            //trying to find it
            bool found = false;
            for (auto & elem : resources)
            {
                if (elem.Name == bindDesc.Name)
                {
                    found = true;
                    elem.BindPointVertex = bindDesc.BindPoint;
                    elem.vertex = true;
                    elem.isDirty = true;
                }
            }

            if (!found)
            {
                D3D11ResDesc resource;
                resource.Name = bindDesc.Name;
                resource.BindPointVertex = bindDesc.BindPoint;
                resource.vertex = true;
                resources.push_back(resource);
            }
        }
    }

    //PIXEL SHADER
    pixelShader->reflection->GetDesc(&desc);
    for (uint32_t i = 0; i < desc.ConstantBuffers; i++)
    {
        ID3D11ShaderReflectionConstantBuffer * cb;
        cb = pixelShader->reflection->GetConstantBufferByIndex(i);
        D3D11_SHADER_BUFFER_DESC buffDesc;
        cb->GetDesc(&buffDesc);

        bool flag = false;
        D3D11BufferDescription * pBuffDesc = nullptr;
        uint32_t buffIndex = 0;
        for (size_t k = 0; k < constantBuffers.size(); k++)
        {
            if (constantBuffers[k].Name == buffDesc.Name)
            {
                flag = true;
                pBuffDesc = &constantBuffers[k];
                buffIndex = k;
            }
        }

        if (!flag)
        {
            constantBuffers.push_back(D3D11BufferDescription());
            pBuffDesc = &constantBuffers[constantBuffers.size() - 1];
            buffIndex = constantBuffers.size() - 1;
        }

        pBuffDesc->isDirty = true;
        pBuffDesc->Name = buffDesc.Name;
        pBuffDesc->Size = buffDesc.Size;
        pBuffDesc->StartSlotPixel = i;
        if (pBuffDesc->bufferData == nullptr) pBuffDesc->bufferData = new uint8_t[buffDesc.Size];
        pBuffDesc->pixel = true;

        for (uint32_t j = 0; j < buffDesc.Variables; j++)
        {
            ID3D11ShaderReflectionVariable * pVar = nullptr;
            pVar = cb->GetVariableByIndex(j);
            D3D11_SHADER_VARIABLE_DESC variableDesc;
            pVar->GetDesc(&variableDesc);
            D3D11VarDesc variable;
            unsigned int curIndex = 0;
            if (!GetVariableIndex(variableDesc.Name, &curIndex))
            {
                variable.Name = variableDesc.Name;
                variable.Offset = variableDesc.StartOffset;
                variable.Size = variableDesc.Size;
                variable.BufferIndex = buffIndex;
                variables.push_back(variable);
                if (variableDesc.DefaultValue != nullptr)
                    memcpy(pBuffDesc->bufferData + variableDesc.StartOffset, variableDesc.DefaultValue, variableDesc.Size);
            }
        }
    }

    for (uint32_t i = 0; i < desc.BoundResources; i++)
    {
        D3D11_SHADER_INPUT_BIND_DESC bindDesc;
        pixelShader->reflection->GetResourceBindingDesc(i, &bindDesc);
        if (bindDesc.Type == D3D_SIT_TEXTURE)
        {
            //trying to find it
            bool found = false;
            for (auto & elem : resources)
            {
                if (elem.Name == bindDesc.Name)
                {
                    found = true;
                    elem.BindPointPixel = bindDesc.BindPoint;
                    elem.isDirty = true;
                    elem.pixel = true;
                }
            }

            if (!found)
            {
                D3D11ResDesc resource;
                resource.Name = bindDesc.Name;
                resource.BindPointPixel = bindDesc.BindPoint;
                resource.pixel = true;
                resources.push_back(resource);
            }
        }
    }

    for (auto & elem : constantBuffers)
    {
        D3D11_BUFFER_DESC cbDesc;
        cbDesc.ByteWidth = elem.Size;
        cbDesc.Usage = D3D11_USAGE_DYNAMIC;
        cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbDesc.MiscFlags = 0;
        HRESULT hr = pDevice->CreateBuffer(&cbDesc, NULL, &elem.cbFromShader);
        if (FAILED(hr))
            LOG_FATAL_ERROR("Unable to bind constant buffer for shader");
    }
}

bool ShiftEngine::D3D11Program::SetMatrixConstantByName(const char * nameInShader, const float * mat)
{
    unsigned int index = 0;
    if (GetVariableIndex(nameInShader, &index))
    {
        SetMatrixConstantByIndex(index, mat);
        return true;
    }
    return false;
}

bool ShiftEngine::D3D11Program::SetScalarConstantByName(const char * nameInShader, const float * scalar)
{
    unsigned int index = 0;
    if (GetVariableIndex(nameInShader, &index))
    {
        SetScalarConstantByIndex(index, scalar);
        return true;
    }
    return false;
}

bool ShiftEngine::D3D11Program::SetVectorConstantByName(const char * nameInShader, const float * vec)
{
    unsigned int index = 0;
    if (GetVariableIndex(nameInShader, &index))
    {
        SetVectorConstantByIndex(index, vec);
        return true;
    }
    return false;
}

bool ShiftEngine::D3D11Program::SetArrayConstantByName(const char * nameInShader, const void * data)
{
    unsigned int index = 0;
    if (GetVariableIndex(nameInShader, &index))
    {
        SetArrayConstantByIndex(index, data);
        return true;
    }
    return false;
}

bool ShiftEngine::D3D11Program::SetTextureByName(const char * textureName, const ITexturePtr & texture)
{
    unsigned int index = 0;
    if (GetResourceIndex(textureName, &index))
    {
        SetTextureByIndex(index, texture);
        return true;
    }
    return false;
}

void ShiftEngine::D3D11Program::Apply(bool shaderChanged)
{
    if (shaderChanged)
    {
        vertexShader->BindShader(pDeviceContext);
        pixelShader->BindShader(pDeviceContext);
    }

    for (auto & elem : constantBuffers)
    {
        if (elem.isDirty || shaderChanged)
        {
            D3D11_MAPPED_SUBRESOURCE mapped;
            HRESULT hr = pDeviceContext->Map(elem.cbFromShader, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped);
            if (FAILED(hr))
                throw;

            memcpy(mapped.pData, elem.bufferData, elem.Size);
            pDeviceContext->Unmap(elem.cbFromShader, NULL);

            elem.isDirty = false;
            if (elem.vertex && shaderChanged)
                pDeviceContext->VSSetConstantBuffers(elem.StartSlotVertex, 1, &elem.cbFromShader);
            if (elem.pixel && shaderChanged)
                pDeviceContext->PSSetConstantBuffers(elem.StartSlotPixel, 1, &elem.cbFromShader);
        }
    }

    for (auto & elem : resources)
    {
        if (elem.isDirty || shaderChanged)
        {
            if (elem.vertex)
                elem.View->Bind(elem.BindPointVertex, BindingPoint::Vertex);
            if (elem.pixel)
                elem.View->Bind(elem.BindPointPixel, BindingPoint::Pixel);
            elem.isDirty = false;
        }
    }
}

bool ShiftEngine::D3D11Program::GetVariableIndex(const char * name, unsigned int * index)
{
    if (index != nullptr)
    {
        for (size_t i = 0; i < variables.size(); i++)
        {
            if (variables[i].Name == name)
            {
                *index = i;
                return true;
            }
        }
        *index = (unsigned)-1; //more than any index
        return false;
    }
    else
    {
        return false;
    }
}

bool ShiftEngine::D3D11Program::GetResourceIndex(const char * name, unsigned int * index)
{
    if (index != nullptr)
    {
        for (size_t i = 0; i < resources.size(); i++)
        {
            if (resources[i].Name == name)
            {
                *index = i;
                return true;
            }
        }
        *index = (unsigned)-1; //more than any index
        return false;
    }
    else
    {
        return false;
    }
}

void ShiftEngine::D3D11Program::SetMatrixConstantByIndex(unsigned int index, const float * mat)
{
    return SetUniformByIndex(index, mat);
}

void ShiftEngine::D3D11Program::SetScalarConstantByIndex(unsigned int index, const float * scalar)
{
    return SetUniformByIndex(index, scalar);
}

void ShiftEngine::D3D11Program::SetVectorConstantByIndex(unsigned int index, const float * vec)
{
    return SetUniformByIndex(index, vec);
}

void ShiftEngine::D3D11Program::SetArrayConstantByIndex(unsigned int index, const void * data)
{
    return SetUniformByIndex(index, data);
}

void ShiftEngine::D3D11Program::SetUniformByIndex(unsigned int index, const void * data)
{
    assert(index < variables.size());
    const D3D11VarDesc & var = variables[index];
    D3D11BufferDescription & buffer = constantBuffers[var.BufferIndex];
    buffer.isDirty = true;
    memcpy(buffer.bufferData + var.Offset, data, var.Size);
}

bool ShiftEngine::D3D11Program::SetTextureByIndex(unsigned int index, const ITexturePtr & texture)
{
    if (!texture)
        return false;

    if (index >= resources.size())
        return false;
    D3D11ResDesc & var = resources[index];
    var.isDirty = true;
    var.View = texture;
    return true;
}
