#include "D3D11ContextManager.h"

#include <Utilities/logger.hpp>
#include <Utilities/ut.h>

#include <sstream>

#include <D3Dcompiler.h>

ShiftEngine::D3D11ContextManager::D3D11ContextManager(HWND hwnd)
    : windowHandle(hwnd)
{
    //MOVE TO ICONTEXTMANAGER CONSTRUCTOR
    defaultVertexSemantic.addSemantic(ET_FLOAT, 3, ES_Position);
    defaultVertexSemantic.addSemantic(ET_FLOAT, 3, ES_Normal);
    defaultVertexSemantic.addSemantic(ET_FLOAT, 2, ES_Texcoord);

    extendedVertexSemantic.addSemantic(ET_FLOAT, 3, ES_Position);
    extendedVertexSemantic.addSemantic(ET_FLOAT, 3, ES_Normal);
    extendedVertexSemantic.addSemantic(ET_FLOAT, 3, ES_Texcoord);
    extendedVertexSemantic.addSemantic(ET_FLOAT, 1, ES_Color);

    colorVertexSemantic.addSemantic(ET_FLOAT, 3, ES_Position);
    colorVertexSemantic.addSemantic(ET_FLOAT, 3, ES_Normal);
    colorVertexSemantic.addSemantic(ET_FLOAT, 1, ES_Color);

    plainSpriteVertexSemantic.addSemantic(ET_FLOAT, 2, ES_Position);
    plainSpriteVertexSemantic.addSemantic(ET_FLOAT, 2, ES_Texcoord);
    //plainSpriteVertexSemantic.addVertexSemantic(ET_FLOAT, 1, ES_Color);
}

ShiftEngine::D3D11ContextManager::~D3D11ContextManager()
{
}

bool ShiftEngine::D3D11ContextManager::Initialize(GraphicEngineSettings _Settings, ShiftEngine::PathSettings _Paths)
{
    engineSettings = _Settings;
    enginePaths = _Paths;

    if (enginePaths.FontsPath.empty() || enginePaths.MaterialsPath.empty() || enginePaths.MeshPath.empty() || enginePaths.ShaderPath.empty() || enginePaths.TexturePath.empty())
        LOG_ERROR("Some settings paths are not filled");

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                        //формат буферов
    desc.BufferDesc.Height = engineSettings.screenHeight;                       //высота
    desc.BufferDesc.Width = engineSettings.screenWidth;                         //ширина
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;                    //масштабирование?
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;    //развертка строк?
    desc.BufferDesc.RefreshRate.Denominator = 1;                                //знаменатель?
    desc.BufferDesc.RefreshRate.Numerator = engineSettings.screenRate;          //частота обновления экрана

    desc.SampleDesc.Count = engineSettings.multisampleQuality;                  //мультисемплинг 
    desc.SampleDesc.Quality = 0;                                                //качество его

    desc.BufferCount = 1;                                                       //кол-во задних буферов - один, за глаза
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;                         //использование буфера
    desc.Flags = 0;                                                             //флаги
    desc.OutputWindow = windowHandle;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    desc.Windowed = engineSettings.windowed;

    unsigned int Flags = 0;
#if defined (DEBUG) || (_DEBUG)
    Flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_9_1,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_11_0
    };

    D3D_FEATURE_LEVEL featureLevel;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        Flags,
        featureLevels,
        6,
        D3D11_SDK_VERSION,
        &desc,
        &graphicsContext.SwapChain,
        &graphicsContext.Device,
        &featureLevel,
        &graphicsContext.DeviceContext
        );

    if (FAILED(hr))
        LOG_FATAL_ERROR("Unable to create D3D11 device");

    LOG_INFO("Device created");

    ID3D11Texture2D * tempTex = nullptr;

    graphicsContext.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&tempTex));
    graphicsContext.Device->CreateRenderTargetView(tempTex, 0, &graphicsContext.DefaultRT->rt);
    tempTex->Release();

    /////////////////////////////////////
    // Создание депт и стенсил буферов //
    /////////////////////////////////////

    D3D11_TEXTURE2D_DESC depthStencilDesc;                              //описание глубинно-стенсильного буфера
    depthStencilDesc.Width = engineSettings.screenWidth;                //ширина
    depthStencilDesc.Height = engineSettings.screenHeight;              //высота
    depthStencilDesc.MipLevels = 1;                                     //мип-уровни
    depthStencilDesc.ArraySize = 1;                                     //размер текстуры
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;            //формат
    depthStencilDesc.SampleDesc.Count = engineSettings.multisampleQuality;    //multisampling must match
    depthStencilDesc.SampleDesc.Quality = 0;                            //swap chain values
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;                       //использование
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;              //флаги, куда прибиндивать
    depthStencilDesc.CPUAccessFlags = 0;                                //доступ процессора
    depthStencilDesc.MiscFlags = 0;                                     //прочие флаги

    graphicsContext.Device->CreateTexture2D(&depthStencilDesc, 0, &graphicsContext.DepthStencilBuffer);
    graphicsContext.Device->CreateDepthStencilView(graphicsContext.DepthStencilBuffer, 0, &graphicsContext.DepthStencilView);
    graphicsContext.DeviceContext->OMSetRenderTargets(1, &graphicsContext.DefaultRT->rt, graphicsContext.DepthStencilView); //установка рендер-таргетов

    D3D11_VIEWPORT vp;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.Width = engineSettings.screenWidth;
    vp.Height = engineSettings.screenHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;

    graphicsContext.DeviceContext->RSSetViewports(1, &vp);

    if (!graphicsContext.CreateStates())
        LOG_FATAL_ERROR("Unable to create states for renderer");

    zBufferState = true;
    graphicsContext.DeviceContext->OMSetDepthStencilState(graphicsContext.dsStateZOn, 1);
    graphicsContext.DeviceContext->RSSetState(graphicsContext.rsNormal);
    const float BlendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    graphicsContext.DeviceContext->OMSetBlendState(graphicsContext.bsNormal, BlendFactor, 0xffffffff);

    shaderManager = new D3D11ShaderManager(graphicsContext.Device);
    shaderGenerator = new D3D11ShaderGenerator();
    meshManager = new D3D11MeshManager(graphicsContext.Device);
    textureManager = new D3D11TextureManager(graphicsContext.Device, graphicsContext.DeviceContext, enginePaths.TexturePath);
    materialManager = new MaterialManager(textureManager, shaderManager);
    fontManager = new FontManager();

    graphicsContext.DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //HACK: LOADING SAMPLERS
    D3D11_SAMPLER_DESC sDesc;
    sDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sDesc.BorderColor[0] = sDesc.BorderColor[1] = sDesc.BorderColor[2] = sDesc.BorderColor[3] = 0.0f;
    sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    if (engineSettings.anisotropyLevel > 0)
    {
        sDesc.Filter = D3D11_FILTER_ANISOTROPIC;
        sDesc.MaxAnisotropy = engineSettings.anisotropyLevel;
    }
    else
    {
        sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sDesc.MaxAnisotropy = 0;
    }
    sDesc.MaxLOD = 8;
    sDesc.MinLOD = 0.0f;
    sDesc.MipLODBias = 0.0f;
    ID3D11SamplerState * sampler = nullptr;
    hr = graphicsContext.Device->CreateSamplerState(&sDesc, &sampler);
    if (FAILED(hr))
        LOG_FATAL_ERROR("Unable to create samplers, error code: ", std::to_string(hr));

    graphicsContext.DeviceContext->PSSetSamplers(0, 1, &sampler);

    RegisterVertexSemantic(defaultVertexSemantic);
    RegisterVertexSemantic(extendedVertexSemantic);
    RegisterVertexSemantic(colorVertexSemantic);
    RegisterVertexSemantic(plainSpriteVertexSemantic);

    return true;
}

std::wstring ShiftEngine::D3D11ContextManager::GetGPUDescription()
{
    DXGI_ADAPTER_DESC adapterDesc;
    IDXGIFactory * factory;
    IDXGIAdapter * adapter;
    // Create a DirectX graphics interface factory.
    CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    // Use the factory to create an adapter for the primary graphics interface (video card).
    factory->EnumAdapters(0, &adapter);
    adapter->GetDesc(&adapterDesc);
    std::wstring buffer = adapterDesc.Description;
    factory->Release();
    adapter->Release();
    return buffer;
}

void ShiftEngine::D3D11ContextManager::Shutdown()
{
    fontManager->Shutdown();
    //KILL EVERYTHING
}

void ShiftEngine::D3D11ContextManager::BeginScene()
{
    graphicsContext.DeviceContext->ClearRenderTargetView(graphicsContext.DefaultRT->rt, D3DXCOLOR((float)208 / 255, (float)238 / 255, (float)248 / 255, 1.0f));
    graphicsContext.DeviceContext->ClearDepthStencilView(graphicsContext.DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void ShiftEngine::D3D11ContextManager::EndScene()
{
    if (engineSettings.screenRate > 0)
        graphicsContext.SwapChain->Present(1, 0);
    else
        graphicsContext.SwapChain->Present(0, 0);
}

void ShiftEngine::D3D11ContextManager::ResetPipeline()
{
    UINT null = 0;
    ID3D11Buffer* nullB = nullptr;
    graphicsContext.DeviceContext->IASetVertexBuffers(0, 1, &nullB, &null, &null);
    graphicsContext.DeviceContext->IASetIndexBuffer(nullB, DXGI_FORMAT_UNKNOWN, NULL);
    graphicsContext.DeviceContext->IASetInputLayout(nullptr);
    currentVertexDeclaration = nullptr;
    currentProgram = nullptr;
}

ShiftEngine::ITexturePtr ShiftEngine::D3D11ContextManager::LoadTexture(const std::wstring & FileName)
{
    ITexturePtr out = textureManager->CreateTexture2D(FileName);
    if (out == nullptr)
    {
        LOG_ERROR("Unable to load ", utils::Narrow(FileName), ", loaded empty texture.");
        out = textureManager->GetErrorTexture();
    }
    return out;
}

ShiftEngine::IProgramPtr ShiftEngine::D3D11ContextManager::LoadShader(const std::wstring & FileName)
{
    return shaderManager->CreateProgramFromFile(enginePaths.ShaderPath + FileName);
}

ShiftEngine::D3D11MeshDataPtr ShiftEngine::D3D11ContextManager::LoadMesh(const std::wstring & FileName)
{
    D3D11MeshDataPtr out = meshManager->LoadMesh(enginePaths.MeshPath + FileName);
    if (out == nullptr)
    {
        LOG_ERROR("Unable to load: ", utils::Narrow(FileName), ", trying to use default cube mesh");
        out = meshManager->LoadErrorMesh();
    }
    return out;
}

ShiftEngine::MaterialPtr ShiftEngine::D3D11ContextManager::LoadMaterial(const std::wstring & FileName, const std::wstring & mtlName)
{
    auto ptr = materialManager->LoadMaterial(enginePaths.MaterialsPath + FileName, mtlName);

    if (ptr == nullptr)
    {
        LOG_ERROR("Unable to load ", utils::Narrow(FileName));
        return materialManager->LoadMaterial(enginePaths.MaterialsPath + L"error.mtl");
    }

    return ptr;
}

void ShiftEngine::D3D11ContextManager::SetZState(bool enabled)
{
    zBufferState = enabled;
    if (enabled)
        graphicsContext.DeviceContext->OMSetDepthStencilState(graphicsContext.dsStateZOn, 1);
    else
        graphicsContext.DeviceContext->OMSetDepthStencilState(graphicsContext.dsStateZOff, 1);
}

ID3D11Device * ShiftEngine::D3D11ContextManager::GetDevicePointer()
{
    return graphicsContext.Device;
}

ShiftEngine::GraphicEngineSettings ShiftEngine::D3D11ContextManager::GetEngineSettings() const
{
    return engineSettings;
}

ShiftEngine::PathSettings ShiftEngine::D3D11ContextManager::GetPaths() const
{
    return enginePaths;
}

int ShiftEngine::D3D11ContextManager::DrawMesh(D3D11MeshDataPtr & mesh)
{
    if (mesh && mesh->vertexDeclaration)
    {
        if (mesh->vertexDeclaration.get() != currentVertexDeclaration)
        {
            mesh->vertexDeclaration->Bind();
            currentVertexDeclaration = mesh->vertexDeclaration.get();
        }
        return mesh->Draw();
    }
    else
    {
        return 0;
    }
}

ShiftEngine::D3D11ShaderManager * ShiftEngine::D3D11ContextManager::GetShaderManager()
{
    return shaderManager;
}

ShiftEngine::D3D11ShaderGenerator * ShiftEngine::D3D11ContextManager::GetShaderGenerator()
{
    return shaderGenerator;
}

void ShiftEngine::D3D11ContextManager::SetBlendingState(BlendingState bs)
{
    const float BlendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    switch (bs)
    {
    case ShiftEngine::BS_None:
        graphicsContext.DeviceContext->OMSetBlendState(graphicsContext.bsNormal, BlendFactor, 0xffffffff);
        break;
    case ShiftEngine::BS_AlphaEnabled:
        graphicsContext.DeviceContext->OMSetBlendState(graphicsContext.bsAlpha, BlendFactor, 0xffffffff);
        break;
    case ShiftEngine::BS_Additive:
        graphicsContext.DeviceContext->OMSetBlendState(graphicsContext.bsAdditive, BlendFactor, 0xffffffff);
        break;
    default:
        break;
    }

    currentBlendingState = bs;
}

ShiftEngine::BlendingState ShiftEngine::D3D11ContextManager::GetBlendingState() const
{
    return currentBlendingState;
}

void ShiftEngine::D3D11ContextManager::SetRasterizerState(ShiftEngine::RasterizerState rs)
{
    currentRasterizerState = rs;
    switch (rs)
    {
    case ShiftEngine::RS_Wireframe:
        graphicsContext.DeviceContext->RSSetState(graphicsContext.rsWireframe);
        break;
    case ShiftEngine::RS_Normal:
        graphicsContext.DeviceContext->RSSetState(graphicsContext.rsNormal);
        break;
    case ShiftEngine::RS_NoCulling:
        graphicsContext.DeviceContext->RSSetState(graphicsContext.rsNoCulling);
        break;
    default:
        break;
    }
}

ShiftEngine::RasterizerState ShiftEngine::D3D11ContextManager::GetRasterizerState() const
{
    return currentRasterizerState;
}

ShiftEngine::FontManager * ShiftEngine::D3D11ContextManager::GetFontManager()
{
    return fontManager;
}

void ShiftEngine::D3D11ContextManager::RegisterVertexSemantic(const VertexSemantic & semantic)
{
    auto iter = declarations.find(semantic);
    if (iter == declarations.end())
    {
        auto vd = CreateVDFromDescription(semantic);
        declarations[semantic] = std::make_shared<D3D11VertexDeclaration>(vd);
    }
    else
    {
        LOG_INFO("Semantic is already registered");
    }
}

ShiftEngine::D3D11VertexDeclaration ShiftEngine::D3D11ContextManager::CreateVDFromDescription(const VertexSemantic & semantic)
{
    ID3D11Device * pDevice = graphicsContext.Device;
    ID3D11InputLayout * outIL = nullptr;

    const size_t bufferSize = 2048;
    char shaderCode[bufferSize] = {};
    std::stringstream stream;

    auto repr = semantic.getInnerRepresentation();

    //writing shader
    for (size_t i = 0; i < repr.size(); i++)
    {
        switch (repr[i].semantic)
        {
        case ShiftEngine::ES_Position:
            repr[i].name = "POSITION";
            break;
        case ShiftEngine::ES_Normal:
            repr[i].name = "NORMAL";
            break;
        case ShiftEngine::ES_Texcoord:
            repr[i].name = "TEXCOORD";
            break;
        case ShiftEngine::ES_Color:
            repr[i].name = "COLOR";
            break;
        case ShiftEngine::ES_Binormal:
            repr[i].name = "BINORMAL";
            break;
        case ShiftEngine::ES_Tangent:
            repr[i].name = "TANGENT";
            break;
        case ShiftEngine::ES_Custom:
            break;                      //name should be specified by user
        default:
            throw;
        }

        if (repr[i].count > 4)
        {
            LOG_ERROR("Unable to create input layout for vertex declaration. Reason - wrong byteWidth in vd description. Value is", std::to_string(repr[i].count));
            outIL = nullptr;
        }
        stream << "float" << repr[i].count << " in_" << i << " : " << repr[i].name << ";\n";
    }

    sprintf_s(shaderCode, "struct vs_in {\n %s }; struct vs_out {float4 Pos : SV_Position;}; vs_out f(vs_in input){vs_out o; o.Pos = float4(0.0f, 0.0f, 0.0f, 0.0f); return o;};", stream.str().c_str());
    //end of writing shader

    ID3DBlob * compiledShader = nullptr;    //sue this signature to validate Input Layout
    ID3DBlob * errorMessages = nullptr;
    if (FAILED(D3DCompile(shaderCode, bufferSize, NULL, NULL, NULL, "f", "vs_5_0", 0, 0, &compiledShader, &errorMessages)))
    {
        LOG_ERROR(std::string((char*)errorMessages->GetBufferPointer()));
        LOG_FATAL_ERROR("Internal fatal error");
        outIL = nullptr;
    }

    D3D11_INPUT_ELEMENT_DESC * ilDesc = new D3D11_INPUT_ELEMENT_DESC[repr.size()];
    unsigned int align = 0;

    for (size_t i = 0; i < repr.size(); i++)
    {
        //TEMP
        switch (repr[i].semantic)
        {
        case ShiftEngine::ES_Position:
            ilDesc[i].SemanticName = "POSITION";
            break;
        case ShiftEngine::ES_Normal:
            ilDesc[i].SemanticName = "NORMAL";
            break;
        case ShiftEngine::ES_Texcoord:
            ilDesc[i].SemanticName = "TEXCOORD";
            break;
        case ShiftEngine::ES_Color:
            ilDesc[i].SemanticName = "COLOR";
            break;
        case ShiftEngine::ES_Binormal:
            ilDesc[i].SemanticName = "BINORMAL";
            break;
        case ShiftEngine::ES_Tangent:
            ilDesc[i].SemanticName = "TANGENT";
            break;
        case ShiftEngine::ES_Custom:
        default:
            delete[] ilDesc;
            ilDesc = nullptr;
            throw;
        }
        //TEMP

        ilDesc[i].AlignedByteOffset = align;
        if (repr[i].count == 1)
            ilDesc[i].Format = DXGI_FORMAT_R32_FLOAT;
        if (repr[i].count == 2)
            ilDesc[i].Format = DXGI_FORMAT_R32G32_FLOAT;
        if (repr[i].count == 3)
            ilDesc[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        if (repr[i].count == 4)
            ilDesc[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        ilDesc[i].InputSlot = 0;
        ilDesc[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        ilDesc[i].InstanceDataStepRate = 0;
        ilDesc[i].SemanticIndex = 0;
        align += repr[i].count * 4;
    }

    if (FAILED(pDevice->CreateInputLayout(ilDesc, repr.size(), compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), &outIL)))
    {
        delete[] ilDesc;
        compiledShader->Release();
        outIL = nullptr;
        LOG_FATAL_ERROR("Unable to create input layout");
        return nullptr;
    }

    delete[] ilDesc;
    compiledShader->Release();

    D3D11VertexDeclaration vd(outIL, graphicsContext.DeviceContext);
    return vd;
}

ShiftEngine::D3D11VDPtr ShiftEngine::D3D11ContextManager::GetVertexDeclaration(const VertexSemantic & semantic)
{
    auto iter = declarations.find(semantic);
    if (iter == declarations.end())
    {
        RegisterVertexSemantic(semantic);
        return GetVertexDeclaration(semantic);
    }
    else
    {
        return iter->second;
    }
}

ShiftEngine::D3D11TextureManager * ShiftEngine::D3D11ContextManager::GetTextureManager()
{
    return textureManager;
}
