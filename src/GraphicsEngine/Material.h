#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>

#include "SceneGraph/LightNode.h"
#include "IProgram.h"
#include "MaterialInfo.h"
#include "MiscTypes.h"

using MathLib::vec2;
using MathLib::vec3;
using MathLib::vec4;

namespace ShiftEngine
{
    class Renderer;

    //just move LightNode?
    struct LightInfo
    {
        LightInfo()
        {
            memset(this, 0, sizeof(LightInfo));
        }

        Vector3F Color;
        Vector3F Position;
        Vector3F Direction;
        LightNodeType Type;
        float Radius;
    };

    enum engineUniforms
    {
        SV_MatWorld = 0,
        SV_MatView,
        SV_MatProj,
        SV_EyePos,
        SV_AmbientColor,
        SV_LightsArray,
        SV_LightsCount,
        SV_DiffuseColor,
        SV_SpecularColor,
        SV_EmissionColor,
        SV_Transparency,
        SV_Shininess,
        SV_Elems    //must be LAST!
    };

    enum engineTextures
    {
        ST_Diffuse,
        ST_Alpha,
        //ST_Normal,
        //ST_Specular,
        ST_Elems    //must be LAST!
    };

    //just proxy to MaterialInfo structure?
    class Material
    {
        friend class Renderer;

    public:
        Material(IProgramPtr _program = nullptr);
        Material(MaterialInfo & mtlParams);
        Material(IProgramPtr _program, MaterialInfo & mtlParams);

        void SetNamedParam(const std::string & parameter, const float param);
        void SetNamedParam(const std::string & parameter, const vec2<float> & param);
        void SetNamedParam(const std::string & parameter, const vec3<float> & param);
        void SetNamedParam(const std::string & parameter, const vec4<float> & param);

        void SetZState(bool zState);

        void SetDiffuseColor(const MathLib::Vector4F & diffuseColor);
        void SetSpecularColor(const MathLib::Vector4F & specularColor);
        void SetEmissionColor(const MathLib::Vector4F & emissionColor);

        void SetDiffuseTexture(const ITexturePtr & texture);
        void SetAlphaTexture(const ITexturePtr & texture);
        //void SetNormalTexture(const cTexturePtr & texture);
        //void SetSpecularTexture(const cTexturePtr & texture);
        //void SetEmissionTexture(const cTexturePtr & texture);
        //void SetCustomTexture(cTexturePtr texture, const char * name);
        //void UnbindAllTextures();

        void SetOpacityLevel(float opacity);
        void SetShininessLevel(float shininess);

        MaterialInfo * GetMaterialInfo();

        unsigned int GetUniformIndex(engineUniforms var) const;
        const std::vector<engineUniforms> & GetUniforms() const;

    protected:
        void BindLights(const std::vector<LightInfo> & lights);
        void LinkMaterial();

    private:
        IProgramPtr                                     program = nullptr;

        std::map<std::string, float>                    floatParams;
        std::map<std::string, vec2<float>>              float2Params;
        std::map<std::string, vec3<float>>              float3Params;
        std::map<std::string, vec4<float>>              float4Params;

        bool                                            ZState = true;
        std::vector<engineUniforms>                     knownUniforms;
        unsigned int                                    builtinVarIndices[SV_Elems];
        std::pair<unsigned int, ITexturePtr>            builtinTextures[ST_Elems];
        MaterialInfo                                    info;
    };

    typedef std::shared_ptr<Material> MaterialPtr;
}
