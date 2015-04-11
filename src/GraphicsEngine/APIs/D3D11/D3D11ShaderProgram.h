#pragma once

#include <D3D11.h>
#include <D3Dcompiler.h>
#include <memory>

#include <Utilities/logger.hpp>

struct ID3D11ShaderReflection;

namespace ShiftEngine
{
    enum class D3D11ShaderType
    {
        ST_Vertex,
        ST_Pixel,
        ST_Geometry,
        ST_Compute,
        ST_Hull,
        ST_Domain
    };

    class D3D11Shader
    {
        friend class D3D11Program;

    public:
        D3D11Shader(ID3D11DeviceChild * _shader, D3D11ShaderType _type, ID3D11ShaderReflection * _refl)
            : shader(_shader)
            , type(_type)
            , reflection(_refl)
        {
        }

        ~D3D11Shader()
        {
            if (shader) 
                shader->Release();
            if (reflection) 
                reflection->Release();
        }

        void BindShader(ID3D11DeviceContext * dev)
        {
            switch (type)
            {
            case ShiftEngine::D3D11ShaderType::ST_Vertex:
            {
                ID3D11VertexShader * query = nullptr;
                if (SUCCEEDED(shader->QueryInterface(__uuidof(ID3D11VertexShader), (void**)&query)))
                {
                    dev->VSSetShader(query, nullptr, 0);
                    query->Release();
                }
                else
                {
                    LOG_ERROR("Unable to bind vertex shader!");
                    return;
                }
            }
            break;
            case ShiftEngine::D3D11ShaderType::ST_Pixel:
            {
                ID3D11PixelShader * query = nullptr;
                if (SUCCEEDED(shader->QueryInterface(__uuidof(ID3D11PixelShader), (void**)&query)))
                {
                    dev->PSSetShader(query, nullptr, 0);
                    query->Release();
                }
                else
                {
                    LOG_ERROR("Unable to bind pixel shader!");
                    return;
                }
            }
            break;
            case ShiftEngine::D3D11ShaderType::ST_Geometry:
                LOG_ERROR("Unable to bind geometry shader: operation is not implemented yet");
                break;
            default:
                break;
            }
        }

    private:
        ID3D11DeviceChild * shader;
        D3D11ShaderType type;
        ID3D11ShaderReflection * reflection; //reflection of this shader
    };

    typedef std::shared_ptr<D3D11Shader> D3D11ShaderPtr;

}   //end of namespace ShiftEngine
