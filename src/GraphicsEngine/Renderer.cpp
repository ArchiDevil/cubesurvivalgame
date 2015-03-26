#include "Renderer.h"

#include <sstream>
#include <cassert>

#include "ShiftEngine.h"

#include "SceneGraph/CameraSceneNode.h"
#include "SceneGraph/LightNode.h"

#include <Utilities/logger.hpp>
#include <Utilities/ut.h>

ShiftEngine::Renderer::Renderer(IShaderManager * _pShaderManager, IShaderGenerator * _pShaderGenerator)
    : FPS(0)
    , FPSCounter(0)
    , elapsedTime(0)
    , millisecondsPerFrame(0)
    , pShaderManager(_pShaderManager)
    , pShaderGenerator(_pShaderGenerator)
{
    FPSTimer.Start();
    FPSTimer.Tick();
}

void ShiftEngine::Renderer::DrawAll(RenderQueue & rq, double /*dt*/)
{
    static double baseTime = 0.0f;
    if (FPSTimer.GetRunningTime() - baseTime >= 1.0f)
    {
        baseTime += 1.0f;
        FPS = FPSCounter;
        millisecondsPerFrame = 1000.0f / FPSCounter;
        FPSCounter = 0;
    }

    //	PreProcess();
    Process(rq);
    //	PostProcess();

    FPSCounter++;
}

void ShiftEngine::Renderer::drawSky(RenderQueue &rq)
{
    SkySceneNode * skyNode = rq.GetActiveSky();
    if (skyNode)
    {
        currentState.ShaderChanges++;
        auto pos = rq.GetActiveCamera()->GetPosition();
        skyNode->SetPosition(Vector3F(pos.x, pos.y, pos.z));
        skyNode->SetScale(Vector3F(10.0f, 10.0f, 10.0f));
        GetContextManager()->SetZState(false);

        Material * mat = skyNode->GetMaterialPtr();
        currentState.currentProgram = mat->program;

        bindEngineUniforms(skyNode, rq);
        bindCustomUniforms(skyNode, rq);

        currentState.currentProgram->Apply(true);
        currentState.PolygonsCount += skyNode->Render();
        currentState.DrawCalls++;
    }
}

bool sortFunctor(ShiftEngine::MeshNode * left, ShiftEngine::MeshNode * right)
{
    bool leftTr = left->GetMaterialPtr()->GetMaterialInfo()->GetFlags()->isTransparent;
    bool rightTr = right->GetMaterialPtr()->GetMaterialInfo()->GetFlags()->isTransparent;
    if (!leftTr && rightTr)
        return true;
    else
        return false;
}

void ShiftEngine::Renderer::Process(RenderQueue &rq)
{
    this->currentState.Reset();

    //sky first
    drawSky(rq);
    auto & renderVec = rq.GetRenderableNodes();

    std::sort(renderVec.begin(), renderVec.end(), sortFunctor);

    GetContextManager()->SetZState(true);

    for (auto iter = renderVec.begin(); iter != renderVec.end(); ++iter)
    {
        MeshNode * currentNode = (*iter);

        //bind material
        Material * mat = currentNode->GetMaterialPtr();

        if (mat->program == nullptr)
        {
            //PANIC COMPILE SHADER!!!
            mat->program = pShaderManager->CreateProgramFromMaterialFlags(mat->info, *currentNode->GetDataPtr()->vertexSemantic);
            mat->LinkMaterial();
        }

        if (mat->program != currentState.currentProgram)
        {
            currentState.currentProgram = mat->program;
            currentState.shaderChanged = true;
            currentState.ShaderChanges++;
        }

        //check already set textures
        for (int i = 0; i < ST_Elems; i++)
        {
            if (mat->builtinTextures[i].first != UINT_MAX &&
                currentState.texturesCache[(engineTextures)i] != mat->builtinTextures[i].second)
            {
                if (!currentState.currentProgram->SetTextureByIndex(mat->builtinTextures[i].first, mat->builtinTextures[i].second))
                {
                    LOG_ERROR("Unable to set texture for material");
                }
                currentState.texturesCache[(engineTextures)i] = mat->builtinTextures[i].second;
                currentState.TextureBindings++;
            }
        }

        bindEngineUniforms(currentNode, rq);
        bindCustomUniforms(currentNode, rq);

        currentState.currentProgram->Apply(currentState.shaderChanged);
        currentState.PolygonsCount += currentNode->Render();
        currentState.DrawCalls++;

        currentState.shaderChanged = false;
    }
}

void ShiftEngine::Renderer::PreProcess()
{
    throw std::exception("The method or operation is not implemented.");
}

void ShiftEngine::Renderer::PostProcess()
{
    throw std::exception("The method or operation is not implemented.");
}

void ShiftEngine::Renderer::bindEngineUniforms(MeshNode * currentNode, const RenderQueue & list)
{
    if (!currentNode || !currentNode->GetMaterialPtr() || !currentNode->GetMaterialPtr()->program)
        return;

    const auto & uniforms = currentNode->GetMaterialPtr()->GetUniforms();
    size_t uniformsNum = uniforms.size();
    for (unsigned i = 0; i != uniformsNum; ++i)
    {
        auto uniformId = uniforms[i];
        auto uniformIndex = currentNode->GetMaterialPtr()->GetUniformIndex(uniformId);

        Material * curMaterial = currentNode->GetMaterialPtr();
        IProgramPtr & program = curMaterial->program;

        switch (uniformId)
        {
        case SV_MatWorld:
            currentState.MatricesBindings++;
            program->SetMatrixConstantByIndex(uniformIndex, currentNode->GetWorldMatrix());
            break;
        case SV_MatView:
            if (currentState.shaderChanged)
            {
                currentState.MatricesBindings++;
                program->SetMatrixConstantByIndex(uniformIndex, list.GetActiveCamera()->GetViewMatrix());
            }
            break;
        case SV_MatProj:
            if (currentState.shaderChanged)
            {
                currentState.MatricesBindings++;
                program->SetMatrixConstantByIndex(uniformIndex, list.GetActiveCamera()->GetProjectionMatrix());
            }
            break;
        case SV_EyePos:
            if (currentState.shaderChanged)
            {
                currentState.UniformsBindings++;
                program->SetVectorConstantByIndex(uniformIndex, list.GetActiveCamera()->GetPosition());
            }
            break;
        case SV_AmbientColor:
            if (currentState.shaderChanged)
            {
                currentState.UniformsBindings++;
                program->SetVectorConstantByIndex(uniformIndex, list.GetAmbientColor().ptr());
            }
            break;
        case SV_LightsArray:
            if (currentState.shaderChanged)
            {
                currentState.UniformsBindings++;
                bindLights(list.GetLights(), 0, list.GetLights().size(), curMaterial);
            }
            break;
        case SV_LightsCount:
            if (currentState.shaderChanged)
            {
                currentState.UniformsBindings++;
                float lightsCount = (float)list.GetLights().size();
                program->SetScalarConstantByIndex(uniformIndex, &lightsCount);
            }
            break;
        case SV_DiffuseColor:
            currentState.UniformsBindings++;
            program->SetVectorConstantByIndex(uniformIndex, curMaterial->GetMaterialInfo()->diffuseColor.ptr());
            break;
        case SV_SpecularColor:
            currentState.UniformsBindings++;
            program->SetVectorConstantByIndex(uniformIndex, curMaterial->GetMaterialInfo()->specularColor.ptr());
            break;
        case SV_EmissionColor:
            currentState.UniformsBindings++;
            program->SetVectorConstantByIndex(uniformIndex, curMaterial->GetMaterialInfo()->emissionColor.ptr());
            break;
        case SV_Transparency:
            currentState.UniformsBindings++;
            program->SetScalarConstantByIndex(uniformIndex, &curMaterial->GetMaterialInfo()->opacity);
            break;
        case SV_Shininess:
            currentState.UniformsBindings++;
            program->SetScalarConstantByIndex(uniformIndex, &curMaterial->GetMaterialInfo()->shininess);
            break;
        default:
            assert(false);
            break;
        }
    }
}

void ShiftEngine::Renderer::bindCustomUniforms(MeshNode * currentNode, const RenderQueue & /*list*/)
{
    auto mat = currentNode->GetMaterialPtr();

    if (!currentState.shaderChanged)
        return;

    if (!mat->floatParams.empty())
    {
        for (auto & elem : mat->floatParams)
        {
            mat->program->SetScalarConstantByName(elem.first.c_str(), &elem.second);
            currentState.UniformsBindings++;
        }
    }

    if (!mat->float2Params.empty())
    {
        for (auto & elem : mat->float2Params)
        {
            mat->program->SetVectorConstantByName(elem.first.c_str(), elem.second.ptr());
            currentState.UniformsBindings++;
        }
    }

    if (!mat->float3Params.empty())
    {
        for (auto & elem : mat->float3Params)
        {
            mat->program->SetVectorConstantByName(elem.first.c_str(), elem.second.ptr());
            currentState.UniformsBindings++;
        }
    }

    if (!mat->float4Params.empty())
    {
        for (auto & elem : mat->float4Params)
        {
            mat->program->SetVectorConstantByName(elem.first.c_str(), elem.second.ptr());
            currentState.UniformsBindings++;
        }
    }
}

void ShiftEngine::Renderer::bindLights(const LightsVector & lv, unsigned int startIndex, unsigned int count, Material * matPtr)
{
    if (!matPtr)
        return;

    if (currentState.shaderChanged)
    {
        std::vector<LightInfo> li;
        LightInfo temp;
        for (size_t i = startIndex; i < startIndex + count; i++)
        {
            temp.Type = lv[i]->GetType();
            temp.Color = lv[i]->GetColor();
            temp.Direction = lv[i]->GetDirection();
            temp.Position = lv[i]->GetPosition();
            temp.Radius = lv[i]->GetRadius();
            li.push_back(temp);
        }

        currentState.UniformsBindings += 4 + 1 + 1;

        matPtr->BindLights(li);
    }
}

unsigned int ShiftEngine::Renderer::GetTextureBindings() const
{
    return currentState.TextureBindings;
}

unsigned int ShiftEngine::Renderer::GetUniformsBindings() const
{
    return currentState.UniformsBindings;
}

unsigned int ShiftEngine::Renderer::GetMatricesBindings() const
{
    return currentState.MatricesBindings;
}

double ShiftEngine::Renderer::GetElapsedTime() const
{
    return elapsedTime;
}

int ShiftEngine::Renderer::GetFPS() const
{
    return FPS;
}

double ShiftEngine::Renderer::GetMSPF() const
{
    return millisecondsPerFrame;
}

unsigned int ShiftEngine::Renderer::GetDrawCalls() const
{
    return currentState.DrawCalls;
}

unsigned int ShiftEngine::Renderer::GetDrawnPolygonsCount() const
{
    return currentState.PolygonsCount;
}

unsigned int ShiftEngine::Renderer::GetShaderChanges() const
{
    return currentState.ShaderChanges;
}
