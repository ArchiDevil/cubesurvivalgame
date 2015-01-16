#include "ShaderGenerator.h"

#include <cassert>
#include <sstream>

#define ADD_LINE(x) stream << ##x << endl
#define SERIALIZE(buffer)                           \
            if (!buffer.empty())                    \
            {                                       \
                ADD_LINE("cbuffer "#buffer" {");    \
                for (auto & elem : buffer)          \
                    ADD_LINE(elem);                 \
                ADD_LINE("};");                     \
            }

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

std::string ShiftEngine::D3D10ShaderGenerator::CreateShaderCode(const VertexSemantic & verticesInfo, const MaterialInfo & info)
{
    ostringstream stream;
    ADD_LINE("//generated with D3D10 shader generator");
    CreateUniforms(stream, info);
    CreateSamplers(stream, info);
    CreateInput(stream, verticesInfo);
    CreateOutput(stream, verticesInfo, info);
    CreateVS(stream, verticesInfo, info);
    CreatePS(stream, verticesInfo, info);
    return std::move(stream.str());
}

void ShiftEngine::D3D10ShaderGenerator::CreateUniforms(ostringstream & stream, const MaterialInfo & info)
{
    vector<string> perObject;
    vector<string> perFrame;
    vector<string> perLight;
    vector<string> perMaterial;

    perObject.push_back("float4x4 matWorld;");

    perFrame.push_back("float4x4 matView;");
    perFrame.push_back("float4x4 matProjection;");

    if (info.GetFlags()->isTransparent)
    {
        perMaterial.push_back("float opacity;");
    }

    if (info.GetFlags()->lightingModel != MaterialLightingModel::None)
    {
        perFrame.push_back("float3 ambientColor;");
        perFrame.push_back("float3 eyePos;");

        perMaterial.push_back("float shininess;");
        perMaterial.push_back("float4 diffuseColor;");
        perMaterial.push_back("float4 specularColor;");
        perMaterial.push_back("float4 emissionColor;");

        ADD_LINE("struct Light {");
        ADD_LINE("float3 lightColor;");
        ADD_LINE("float lightType;");
        ADD_LINE("float3 lightDir;");
        ADD_LINE("float lightRadius;");
        ADD_LINE("float3 lightPos;");
        ADD_LINE("float packFloat;");
        ADD_LINE("};");

        perLight.push_back("Light lights[4];");
        perLight.push_back("float lightsCount;");
    }

    //generating code

    SERIALIZE(perObject);
    SERIALIZE(perFrame);
    SERIALIZE(perLight);
    SERIALIZE(perMaterial);
}

void ShiftEngine::D3D10ShaderGenerator::CreateSamplers(ostringstream & stream, const MaterialInfo & info)
{
    ADD_LINE("SamplerState SS;");

    if (info.diffuseMap.GetType() == TextureType::Texture2D)
    {
        ADD_LINE("Texture2D diffuseMap;");
    }
    else if (info.diffuseMap.GetType() == TextureType::Texture2DArray)
    {
        ADD_LINE("Texture2DArray diffuseMap;");
    }

    if (info.alphaMap.GetType() == TextureType::Texture2D)
    {
        ADD_LINE("Texture2D alphaMap;");
    }
    else if (info.alphaMap.GetType() == TextureType::Texture2DArray)
    {
        ADD_LINE("Texture2DArray alphaMap;");
    }
}

void ShiftEngine::D3D10ShaderGenerator::CreateInput(ostringstream & stream, const VertexSemantic & verticesInfo)
{
    ADD_LINE("struct VS_IN" << endl << "{");

    for (const VertexInfo & elem : verticesInfo.getInnerRepresentation())
    {
        stream << GetTypeBySemantic(elem.type, elem.count) << " " << GetNameBySemantic(elem) << " : " << GetSemanticBySemantic(elem.semantic) << ";" << endl;
    }

    ADD_LINE("};");
}

void ShiftEngine::D3D10ShaderGenerator::CreateOutput(ostringstream & stream, const VertexSemantic & verticesInfo, const MaterialInfo & info)
{
    ADD_LINE("struct VS_OUT" << endl << "{");
    ADD_LINE("float4 OutPos : SV_Position;");

    if (info.GetFlags()->lightingModel != MaterialLightingModel::None)
    {
        ADD_LINE("float3 Position : POSITION;");
        ADD_LINE("float3 Normal : NORMAL;");
    }

    if (info.diffuseMap.GetType() != TextureType::Unknown ||
        info.alphaMap.GetType() != TextureType::Unknown)
    {
        if (info.diffuseMap.GetType() == TextureType::Texture2DArray)
        {
            ADD_LINE("float3 Texcoord : TEXCOORD;");
        }
        else
        {
            ADD_LINE("float2 Texcoord : TEXCOORD;");
        }
    }

    if (info.GetFlags()->useVertexColors)
    {
        ADD_LINE("float3 Color : COLOR;");
    }

    ADD_LINE("};");
}

void ShiftEngine::D3D10ShaderGenerator::CreateVS(ostringstream & stream, const VertexSemantic & verticesInfo, const MaterialInfo & info)
{
    ADD_LINE("VS_OUT VS(VS_IN Input)" << endl << "{");
    ADD_LINE("VS_OUT Output;");
    ADD_LINE("Output.OutPos = mul(float4(Input.Position, 1.0f), matWorld);");

    if (info.GetFlags()->lightingModel != MaterialLightingModel::None)
    {
        ADD_LINE("Output.Position = Output.OutPos.xyz;");
    }

    ADD_LINE("Output.OutPos = mul(Output.OutPos, matView);");
    ADD_LINE("Output.OutPos = mul(Output.OutPos, matProjection);");

    if (info.GetFlags()->lightingModel != MaterialLightingModel::None)
    {
        ADD_LINE("Output.Normal = mul(Input.Normal, (float3x3)matWorld);");
    }

    if (info.diffuseMap.GetType() != TextureType::Unknown ||
        info.alphaMap.GetType() != TextureType::Unknown)
    {
        ADD_LINE("Output.Texcoord = Input.Texcoord;");
    }

    if (info.GetFlags()->useVertexColors)
    {
        ADD_LINE("Output.Color = Input.Color;");
    }

    ADD_LINE("return Output;");
    ADD_LINE("}");
}

void ShiftEngine::D3D10ShaderGenerator::CreatePS(ostringstream & stream, const VertexSemantic & verticesInfo, const MaterialInfo & info)
{
    ADD_LINE("float4 PS(VS_OUT Input) : SV_TARGET" << endl << "{");

    if (info.GetFlags()->lightingModel == MaterialLightingModel::Phong)
    {
        ADD_LINE("float4 resultColor = float4(0.0f, 0.0f, 0.0f, 1.0f);");

        if (info.diffuseMap.GetType() != TextureType::Unknown)
        {
            ADD_LINE("float4 sampledColor = diffuseMap.Sample(SS, Input.Texcoord);");
        }
        else
        {
            ADD_LINE("float4 sampledColor = float4(1.0f, 1.0f, 1.0f, 1.0f);");
        }

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
        if (info.diffuseMap.GetType() != TextureType::Unknown)
        {
            ADD_LINE("resultColor *= diffuseMap.Sample(SS, Input.Texcoord);");
        }
    }

    if (info.GetFlags()->useVertexColors)
    {
        ADD_LINE("resultColor.x *= Input.Color.x;");
        ADD_LINE("resultColor.y *= Input.Color.y;");
        ADD_LINE("resultColor.z *= Input.Color.z;");
    }

    if (info.GetFlags()->isTransparent)
    {
        ADD_LINE("resultColor.a *= opacity;");
    }

    ADD_LINE("resultColor.xyz += emissionColor.xyz;");

    if (info.alphaMap.GetType() != TextureType::Unknown)
    {
        ADD_LINE("float4 alphaColor = alphaMap.Sample(SS, Input.Texcoord);");
        ADD_LINE("resultColor.a *= alphaColor.r;");
    }

    ADD_LINE("return resultColor;");
    ADD_LINE("}");
}

std::string ShiftEngine::D3D10ShaderGenerator::GetNameBySemantic(const VertexInfo & info)
{
    switch (info.semantic)
    {
    case ES_Position:
        return "Position";
    case ES_Normal:
        return "Normal";
    case ES_Binormal:
        return "Binormal";
    case ES_Tangent:
        return "Tangent";
    case ES_Texcoord:
        return "Texcoord";
    case ES_Color:
        return "Color";
    case ES_Custom:
        return info.name;
    default:
        assert(false);
    }

    return "";
}

std::string ShiftEngine::D3D10ShaderGenerator::GetSemanticBySemantic(ElemSemantic sem)
{
    switch (sem)
    {
    case ES_Position:
        return "POSITION";
    case ES_Normal:
        return "NORMAL";
    case ES_Binormal:
        return "BINORMAL";
    case ES_Tangent:
        return "TANGENT";
    case ES_Texcoord:
        return "TEXCOORD";
    case ES_Color:
        return "COLOR";
    case ES_Custom:
        assert(false);
    default:
        assert(false);
    }

    return "";
}

std::string ShiftEngine::D3D10ShaderGenerator::GetTypeBySemantic(ElemType type, unsigned count)
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

    if (count > 1)
        os << count;

    return os.str();
}
