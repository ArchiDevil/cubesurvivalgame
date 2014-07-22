#include "ShaderGenerator.h"

#include <cassert>
#include <sstream>

#define ADD_LINE(x) stream << ##x << endl

using std::vector;
using std::string;
using std::ostringstream;
using std::endl;

ShiftEngine::D3D10ShaderGenerator::D3D10ShaderGenerator()
{
}

ShiftEngine::D3D10ShaderGenerator::~D3D10ShaderGenerator()
{
}

std::string ShiftEngine::D3D10ShaderGenerator::CreateShaderCode( const VertexSemantic & verticesInfo, const MaterialInfo & info )
{
	ostringstream stream;
	ADD_LINE("//generated with custom shader generator");
	CreateUniforms(stream, info);
	CreateSamplers(stream, info);
	CreateInput(stream, verticesInfo);
	CreateOutput(stream, verticesInfo, info);
	CreateVS(stream, verticesInfo, info);
	CreatePS(stream, verticesInfo, info);
	return std::move(stream.str());
}

void ShiftEngine::D3D10ShaderGenerator::CreateUniforms( ostringstream & stream, const MaterialInfo & info )
{
	vector<string> perObjectBuffer;
	vector<string> perFrameBuffer;
	vector<string> perLightBuffer;
	vector<string> perMaterialBuffer;

	perObjectBuffer.push_back("float4x4 matWorld;");

	perFrameBuffer.push_back("float4x4 matView;");
	perFrameBuffer.push_back("float4x4 matProjection;");

	if(info.GetFlags()->isTransparent)
		perMaterialBuffer.push_back("float opacity;");

	if(info.GetFlags()->lightingModel != MaterialLightingModel::None)
	{
		perFrameBuffer.push_back("float3 ambientColor;");
		perFrameBuffer.push_back("float3 eyePos;");

		perMaterialBuffer.push_back("float shininess;");
		perMaterialBuffer.push_back("float4 diffuseColor;");
		perMaterialBuffer.push_back("float4 specularColor;");
		perMaterialBuffer.push_back("float4 emissionColor;");

		ADD_LINE("struct Light {");
		ADD_LINE("float3 lightColor;");
		ADD_LINE("float lightType;");
		ADD_LINE("float3 lightDir;");
		ADD_LINE("float lightRadius;");
		ADD_LINE("float3 lightPos;");
		ADD_LINE("float packFloat;");
		ADD_LINE("};");

		perLightBuffer.push_back("Light lights[4];");
		perLightBuffer.push_back("float lightsCount;");
	}

	//generating code

	if(!perObjectBuffer.empty())
	{
		ADD_LINE("cbuffer perObject {");
		for(auto & elem : perObjectBuffer)
			ADD_LINE(elem);
		ADD_LINE("};");
	}

	if(!perFrameBuffer.empty())
	{
		ADD_LINE("cbuffer perFrame {");
		for(auto & elem : perFrameBuffer)
			ADD_LINE(elem);
		ADD_LINE("};");
	}

	if(!perLightBuffer.empty())
	{
		ADD_LINE("cbuffer perLight {");
		for(auto & elem : perLightBuffer)
			ADD_LINE(elem);
		ADD_LINE("};");
	}

	if(!perMaterialBuffer.empty())
	{
		ADD_LINE("cbuffer perMaterial {");
		for(auto & elem : perMaterialBuffer)
			ADD_LINE(elem);
		ADD_LINE("};");
	}
}

void ShiftEngine::D3D10ShaderGenerator::CreateSamplers( ostringstream & stream, const MaterialInfo & info )
{
	ADD_LINE("SamplerState SS;");

	if(info.diffuseMap.GetType() == TextureType::Texture2D)
		ADD_LINE("Texture2D diffuseMap;");
	else if (info.diffuseMap.GetType() == TextureType::Texture2DArray)
		ADD_LINE("Texture2DArray diffuseMap;");

	if(info.alphaMap.GetType() == TextureType::Texture2D)
		ADD_LINE("Texture2D alphaMap;");
	else if (info.alphaMap.GetType() == TextureType::Texture2DArray)
		ADD_LINE("Texture2DArray alphaMap;");
}

void ShiftEngine::D3D10ShaderGenerator::CreateInput( ostringstream & stream, const VertexSemantic & verticesInfo )
{
	ADD_LINE("struct VS_IN" << endl << "{");

	for(const VertexInfo & elem : verticesInfo.getInnerRepresentation())
	{
		stream << GetTypeBySemantic(elem.type, elem.count) << " " << GetNameBySemantic(elem) << " : " << GetSemanticBySemantic(elem.semantic) << ";" << endl;
	}

	ADD_LINE("};");
}

void ShiftEngine::D3D10ShaderGenerator::CreateOutput( ostringstream & stream, const VertexSemantic & verticesInfo, const MaterialInfo & info )
{
	ADD_LINE("struct VS_OUT" << endl << "{");
	ADD_LINE("float4 OutPos : SV_Position;");

	if(info.GetFlags()->lightingModel != MaterialLightingModel::None)
	{
		ADD_LINE("float3 Position : POSITION;");
		ADD_LINE("float3 Normal : NORMAL;");
	}

	if(info.diffuseMap.GetType() != TextureType::Unknown || 
		info.alphaMap.GetType() != TextureType::Unknown)
	{
		if(info.diffuseMap.GetType() == TextureType::Texture2DArray)
		{
			ADD_LINE("float3 Texcoord : TEXCOORD;");
		}
		else
		{
			ADD_LINE("float2 Texcoord : TEXCOORD;");
		}
	}

	if(info.GetFlags()->useVertexColors)
	{
		ADD_LINE("float Color : COLOR;");
	}

	ADD_LINE("};");
}

void ShiftEngine::D3D10ShaderGenerator::CreateVS( ostringstream & stream, const VertexSemantic & verticesInfo, const MaterialInfo & info )
{
	ADD_LINE("VS_OUT VS(VS_IN Input)" << endl << "{");
	ADD_LINE("VS_OUT Output;");
	ADD_LINE("Output.OutPos = mul(float4(Input.Position, 1.0f), matWorld);");

	if(info.GetFlags()->lightingModel != MaterialLightingModel::None)
		ADD_LINE("Output.Position = Output.OutPos.xyz;");

	ADD_LINE("Output.OutPos = mul(Output.OutPos, matView);");
	ADD_LINE("Output.OutPos = mul(Output.OutPos, matProjection);");

	if(info.GetFlags()->lightingModel != MaterialLightingModel::None)
		ADD_LINE("Output.Normal = mul(Input.Normal, (float3x3)matWorld);");

	if(info.diffuseMap.GetType() != TextureType::Unknown || 
		info.alphaMap.GetType() != TextureType::Unknown)
		ADD_LINE("Output.Texcoord = Input.Texcoord;");

	if(info.GetFlags()->useVertexColors)
		ADD_LINE("Output.Color = Input.Color;");

	ADD_LINE("return Output;");
	ADD_LINE("}");
}

void ShiftEngine::D3D10ShaderGenerator::CreatePS( ostringstream & stream, const VertexSemantic & verticesInfo, const MaterialInfo & info )
{
	ADD_LINE("float4 PS(VS_OUT Input) : SV_TARGET" << endl << "{");
	
	if(info.GetFlags()->lightingModel == MaterialLightingModel::Phong)
	{
		ADD_LINE("float4 resultColor = float4(0.0f, 0.0f, 0.0f, 1.0f);");

		if(info.diffuseMap.GetType() != TextureType::Unknown)
			ADD_LINE("float4 sampledColor = diffuseMap.Sample(SS, Input.Texcoord);");
		else
			ADD_LINE("float4 sampledColor = float4(1.0f, 1.0f, 1.0f, 1.0f);");

		ADD_LINE("resultColor.xyz += ambientColor;");
		ADD_LINE("resultColor *= sampledColor;");
		ADD_LINE("float3 CurrentNormal = normalize(Input.Normal);");
		ADD_LINE("float3 localEyePos = normalize(eyePos - Input.Position);");
		ADD_LINE("for(int i = 0; i < lightsCount; i++) {");
		//directional lights
		ADD_LINE("if(lights[i].lightType < 1.0) {"); 
		ADD_LINE("float diffuseIntensity = dot(CurrentNormal, -normalize(lights[i].lightDir));");
		ADD_LINE("resultColor.xyz += max(diffuseIntensity, 0.0) * lights[i].lightColor * diffuseColor.xyz * sampledColor.xyz;");
		ADD_LINE("float3 reflectedLight = normalize(reflect(normalize(lights[i].lightDir), CurrentNormal));");
		ADD_LINE("float specProduct = pow(max(dot(localEyePos, reflectedLight), 0.0f), shininess);");
		ADD_LINE("resultColor.xyz += lights[i].lightColor * specularColor.xyz * specProduct;");
		//point lights
		ADD_LINE("} else {"); 
		ADD_LINE("float d = distance(lights[i].lightPos, Input.Position);");
		ADD_LINE("if(d < lights[i].lightRadius) {");
		ADD_LINE("float diffuseIntensity = dot(CurrentNormal, normalize(lights[i].lightPos - Input.Position)) / pow(d, 1.0);");
		ADD_LINE("resultColor.xyz += (max(diffuseIntensity, 0.0) * lights[i].lightColor * diffuseColor.xyz * sampledColor.xyz);");
		ADD_LINE("float3 reflectedLight = normalize(reflect(-normalize(lights[i].lightPos - Input.Position), CurrentNormal));");
		ADD_LINE("float specProduct = pow(max(dot(localEyePos, reflectedLight), 0.0f), shininess) / pow(d, 1.0);");
		ADD_LINE("resultColor.xyz += lights[i].lightColor * diffuseColor.xyz * specProduct;");
		ADD_LINE("}");
		ADD_LINE("}");
		ADD_LINE("}");
	}
	else
	{
		ADD_LINE("float4 resultColor = float4(1.0f, 1.0f, 1.0f, 1.0f);");
		if(info.diffuseMap.GetType() != TextureType::Unknown)
			ADD_LINE("resultColor *= diffuseMap.Sample(SS, Input.Texcoord);");
	}

	if(info.GetFlags()->useVertexColors)
		ADD_LINE("resultColor *= Input.Color;");

	if(info.GetFlags()->isTransparent)
		ADD_LINE("resultColor.a *= opacity;");

	ADD_LINE("resultColor.xyz += emissionColor.xyz;");

	if(info.alphaMap.GetType() != TextureType::Unknown)
	{
		ADD_LINE("float4 alphaColor = alphaMap.Sample(SS, Input.Texcoord);");
		ADD_LINE("resultColor.a *= alphaColor.r;");
	}

	ADD_LINE("return resultColor;");
	ADD_LINE("}");
}

std::string ShiftEngine::D3D10ShaderGenerator::GetNameBySemantic( const VertexInfo & info )
{
	switch (info.semantic)
	{
	case ShiftEngine::ES_Position:
		return "Position";
	case ShiftEngine::ES_Normal:
		return "Normal";
	case ShiftEngine::ES_Texcoord:
		return "Texcoord";
	case ShiftEngine::ES_Color:
		return "Color";
	case ShiftEngine::ES_Custom:
		return info.name;
	default:
		assert(false);
	}

	return "";
}

std::string ShiftEngine::D3D10ShaderGenerator::GetSemanticBySemantic( ElemSemantic sem )
{
	switch (sem)
	{
	case ShiftEngine::ES_Position:
		return "POSITION";
	case ShiftEngine::ES_Normal:
		return "NORMAL";
	case ShiftEngine::ES_Texcoord:
		return "TEXCOORD";
	case ShiftEngine::ES_Color:
		return "COLOR";
	case ShiftEngine::ES_Custom:
		assert(false);
	default:
		assert(false);
	}

	return "";
}

std::string ShiftEngine::D3D10ShaderGenerator::GetTypeBySemantic( ElemType type, unsigned count )
{
	assert(count != 0);

	std::ostringstream os;

	switch (type)
	{
	case ShiftEngine::ET_FLOAT:
		os << "float";
		break;
	default:
		assert(false);
		break;
	}

	if(count > 1)
		os << count;

	return os.str();
}
