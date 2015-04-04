#include "Material.h"

#include <cassert>

ShiftEngine::Material::Material(IProgramPtr _program)
    : program(_program)
{
    for (int i = 0; i < SV_Elems; i++)
        builtinVarIndices[i] = 0;

    for (int i = 0; i < ST_Elems; i++)
        builtinTextures[i] = std::make_pair(UINT_MAX, nullptr);

    if (_program)
        LinkMaterial();
}

ShiftEngine::Material::Material(MaterialInfo & mtlParams)
    : info(mtlParams)
{
    for (int i = 0; i < SV_Elems; i++)
        builtinVarIndices[i] = 0;

    for (int i = 0; i < ST_Elems; i++)
        builtinTextures[i] = std::make_pair(UINT_MAX, nullptr);
}

ShiftEngine::Material::Material(IProgramPtr _program, MaterialInfo & mtlParams)
    : program(_program)
    , info(mtlParams)
{
    for (int i = 0; i < SV_Elems; i++)
        builtinVarIndices[i] = 0;

    for (int i = 0; i < ST_Elems; i++)
        builtinTextures[i] = std::make_pair(UINT_MAX, nullptr);

    if (_program)
        LinkMaterial();
}

void ShiftEngine::Material::LinkMaterial()
{
    program->GetVariableIndex("matWorld", &builtinVarIndices[SV_MatWorld]);
    program->GetVariableIndex("matView", &builtinVarIndices[SV_MatView]);
    program->GetVariableIndex("matProjection", &builtinVarIndices[SV_MatProj]);

    program->GetVariableIndex("eyePos", &builtinVarIndices[SV_EyePos]);
    program->GetVariableIndex("ambientColor", &builtinVarIndices[SV_AmbientColor]);

    program->GetVariableIndex("lights", &builtinVarIndices[SV_LightsArray]);
    program->GetVariableIndex("lightsCount", &builtinVarIndices[SV_LightsCount]);

    program->GetVariableIndex("diffuseColor", &builtinVarIndices[SV_DiffuseColor]);
    program->GetVariableIndex("specularColor", &builtinVarIndices[SV_SpecularColor]);
    program->GetVariableIndex("emissionColor", &builtinVarIndices[SV_EmissionColor]);
    program->GetVariableIndex("opacity", &builtinVarIndices[SV_Transparency]);
    program->GetVariableIndex("shininess", &builtinVarIndices[SV_Shininess]);

    for (int i = 0; i < SV_Elems; i++)
    {
        if (builtinVarIndices[i] != (unsigned)-1)
        {
            knownUniforms.push_back((engineUniforms)i);
        }
    }

    program->GetResourceIndex("diffuseMap", &builtinTextures[ST_Diffuse].first);
    program->GetResourceIndex("alphaMap", &builtinTextures[ST_Alpha].first);
}

// void ShiftEngine::cMaterial::SetCustomTexture( cTexturePtr texture, const char * name )
// {
// 	textures[name] = texture;
// }

// void ShiftEngine::cMaterial::UnbindAllTextures()
// {
// 	for(auto & elem : builtinTexIndices)
// 		elem = UINT_MAX;
// }

void ShiftEngine::Material::SetDiffuseTexture(const TexturePtr & texture)
{
    if (builtinTextures[ST_Diffuse].first != UINT_MAX)
    {
        builtinTextures[ST_Diffuse].second = texture;
    }
    else
    {
        builtinTextures[ST_Diffuse].second = texture;
        program = nullptr;
        info.diffuseMap = MaterialTextureDescription(texture->GetType());
    }
}

void ShiftEngine::Material::SetAlphaTexture(const TexturePtr & texture)
{
    if (builtinTextures[ST_Alpha].first != UINT_MAX)
    {
        builtinTextures[ST_Alpha].second = texture;
    }
    else
    {
        builtinTextures[ST_Alpha].second = texture;
        program = nullptr;
        info.alphaMap = MaterialTextureDescription(texture->GetType());
    }
}

void ShiftEngine::Material::SetNamedParam(const std::string & parameter, const float param)
{
    unsigned int var = 0;
    if (program)
        program->GetVariableIndex(parameter.c_str(), &var);
    floatParams[parameter] = param;
}

void ShiftEngine::Material::SetNamedParam(const std::string & parameter, const vec2<float> & param)
{
    unsigned int var = 0;
    if (program)
        program->GetVariableIndex(parameter.c_str(), &var);
    float2Params[parameter] = param;
}

void ShiftEngine::Material::SetNamedParam(const std::string & parameter, const vec3<float> & param)
{
    unsigned int var = 0;
    if (program)
        program->GetVariableIndex(parameter.c_str(), &var);
    float3Params[parameter] = param;
}

void ShiftEngine::Material::SetNamedParam(const std::string & parameter, const vec4<float> & param)
{
    unsigned int var = 0;
    if (program)
        program->GetVariableIndex(parameter.c_str(), &var);
    float4Params[parameter] = param;
}

void ShiftEngine::Material::SetZState(bool zState)
{
    ZState = zState;
}

void ShiftEngine::Material::BindLights(const std::vector<LightInfo> & lights)
{
    //must be equal to shader struct with 4-byte pack!
    struct shaderLightRefl
    {
        shaderLightRefl()
        {
            memset(this, 0, sizeof(*this));
        }
        vec3<float> color;
        float type;
        vec3<float> dir;
        float radius;
        vec3<float> pos;
        float pack;
    };

    if (!program || lights.size() > 4)
        return;

    shaderLightRefl binder[4];
    for (unsigned int i = 0; i < lights.size(); i++)
    {
        binder[i].color = lights[i].Color;

        if (lights[i].Type == LNT_Directional)
        {
            binder[i].type = 0.5f;
            binder[i].dir = lights[i].Direction;
        }
        else if (lights[i].Type == LNT_Point)
        {
            binder[i].type = 1.5f;
            binder[i].pos = lights[i].Position;
            binder[i].radius = lights[i].Radius;
        }
    }

    return program->SetArrayConstantByIndex(builtinVarIndices[SV_LightsArray], binder);
}

ShiftEngine::MaterialInfo * ShiftEngine::Material::GetMaterialInfo()
{
    return &info;
}

unsigned int ShiftEngine::Material::GetUniformIndex(engineUniforms var) const
{
    return builtinVarIndices[var];
}

const std::vector<ShiftEngine::engineUniforms> & ShiftEngine::Material::GetUniforms() const
{
    return knownUniforms;
}

void ShiftEngine::Material::SetDiffuseColor(const MathLib::Vector4F & diffuseColor)
{
    info.diffuseColor = diffuseColor;
}

void ShiftEngine::Material::SetSpecularColor(const MathLib::Vector4F & specularColor)
{
    info.specularColor = specularColor;
}

void ShiftEngine::Material::SetEmissionColor(const MathLib::Vector4F & emissionColor)
{
    info.emissionColor = emissionColor;
}
