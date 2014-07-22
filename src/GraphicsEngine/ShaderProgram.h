#pragma once

#include <D3D10.h>
#include <memory>

#include "../Utilities/logger.h"

struct ID3D11ShaderReflection;

namespace ShiftEngine
{
	enum class ShaderType
	{
		ST_Vertex,
		ST_Pixel,
		ST_Geometry,
	};

	class D3D10Shader
	{
	friend class D3D10Program;
		//now DX10 only
		//extending to new functionality can be achieved with inheritance
		//from interface which should be extracted from this class
	public:
		D3D10Shader(ID3D10DeviceChild * _shader, ShaderType _type, ID3D10ShaderReflection * _refl)
			: shader(_shader), type(_type), reflection(_refl)
		{}

		~D3D10Shader() 
		{
			if(shader) shader->Release();
			if(reflection) reflection->Release();
		}

		void BindShader(ID3D10Device * dev)
		{
			switch (type)
			{
			case ShiftEngine::ShaderType::ST_Vertex:
				{
					ID3D10VertexShader * query = nullptr;
					if(SUCCEEDED(shader->QueryInterface(__uuidof(ID3D10VertexShader), (void**)&query)))
					{
						dev->VSSetShader(query);
						query->Release();
					}
					else
					{
						MainLog.Error("Unable to bind vertex shader!");
						return;
					}
				}
				break;
			case ShiftEngine::ShaderType::ST_Pixel:
				{
					ID3D10PixelShader * query = nullptr;
					if(SUCCEEDED(shader->QueryInterface(__uuidof(ID3D10PixelShader), (void**)&query)))
					{
						dev->PSSetShader(query);
						query->Release();
					}
					else
					{
						MainLog.Error("Unable to bind pixel shader!");
						return;
					}
				}
				break;
			case ShiftEngine::ShaderType::ST_Geometry:
				MainLog.Error("Unable to bind geometry shader: operation is not implemented yet");
				break;
			default:
				break;
			}
		}

	private:
		ID3D10DeviceChild * shader;
		ShaderType type;
		ID3D10ShaderReflection * reflection; //reflection of this shader
	};

	typedef std::shared_ptr<D3D10Shader> D3D10ShaderPtr;

}	//end of namespace ShiftEngine