#include "D3D11ShaderManager.h"

#include <D3Dcompiler.h>
#include <sstream>
#include <cstring>
#include <locale>

#include <Utilities/ut.h>

#if defined (NDEBUG) || (_NDEBUG)
    uint32_t flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#else
    uint32_t flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_WARNINGS_ARE_ERRORS | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#endif

    ShiftEngine::D3D11ShaderManager::D3D11ShaderManager(ID3D11Device * _pDevice)
    : pDevice(_pDevice)
    , shaderGenerator(new D3D11ShaderGenerator())
{
}

ShiftEngine::D3D11ShaderManager::~D3D11ShaderManager()
{
}

ShiftEngine::D3D11ShaderPtr ShiftEngine::D3D11ShaderManager::CompileVSFromSource(const std::string & source)
{
    ID3DBlob * compiledShader = nullptr;
    ID3DBlob * errors = nullptr;
    HRESULT hr = S_OK;

    hr = D3DCompile(source.c_str(), source.size(), NULL, NULL, NULL, "VS", "vs_5_0", flags, 0, &compiledShader, &errors);
    if (hr != S_OK && errors != nullptr)
    {
        LOG_ERROR("Unable to compile custom shader");
        std::string temp = (char*)errors->GetBufferPointer();
        LOG_FATAL_ERROR(temp);
    }

    ID3D10VertexShader * finalShader = nullptr;
    pDevice->CreateVertexShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), &finalShader);
    ID3D10ShaderReflection* pReflector = NULL;
    hr = D3D10ReflectShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), &pReflector);
    if (FAILED(hr))
    {
        LOG_FATAL_ERROR("Unable to parse shader");
    }
    D3D10ShaderPtr out = std::make_shared<D3D10Shader>(finalShader, ShaderType::ST_Vertex, pReflector);

    return out;
}

ShiftEngine::D3D11ShaderPtr ShiftEngine::D3D11ShaderManager::CompilePSFromSource(const std::string & source)
{
    ID3DBlob * compiledShader = nullptr;
    ID3DBlob * errors = nullptr;
    HRESULT hr = S_OK;

    hr = D3DCompile(source.c_str(), source.size(), NULL, NULL, NULL, "PS", "ps_5_0", flags, 0, &compiledShader, &errors);
    if (hr != S_OK && errors != nullptr)
    {
        LOG_ERROR("Unable to compile custom shader");
        std::string temp = (char*)errors->GetBufferPointer();
        LOG_FATAL_ERROR(temp);
    }

    ID3D10PixelShader * finalShader = nullptr;
    pDevice->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), &finalShader);
    ID3D10ShaderReflection* pReflector = NULL;
    hr = D3D10ReflectShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), &pReflector);
    if (FAILED(hr))
    {
        LOG_FATAL_ERROR("Unable to parse shader");
    }
    D3D10ShaderPtr out = std::make_shared<D3D10Shader>(finalShader, ShaderType::ST_Pixel, pReflector);
    return out;
}

ShiftEngine::IProgramPtr ShiftEngine::D3D11ShaderManager::CreateProgramFromFile(const std::wstring & fileName)
{
    auto program = filePrograms.find(fileName);
    if (program != filePrograms.end())
    {
        return program->second;
    }
    else
    {
        std::ifstream in(fileName);

        if (in.fail())
        {
            LOG_ERROR("Unable to open ", utils::Narrow(fileName));
            return nullptr;
        }

        std::stringstream buffer;
        buffer << in.rdbuf();
        std::string content = buffer.str();

        auto vs = CompileVSFromSource(content);
        auto ps = CompilePSFromSource(content);
        auto ptr = std::make_shared<D3D10Program>(vs, ps, pDevice);
        filePrograms[fileName] = ptr;
        return ptr;
    }
}

ShiftEngine::IProgramPtr ShiftEngine::D3D11ShaderManager::CreateProgramFromSource(const std::string & source)
{
    auto program = sourcePrograms.find(source);
    if (program != sourcePrograms.end())
    {
        return program->second;
    }
    else
    {
        auto vs = CompileVSFromSource(source);
        auto ps = CompilePSFromSource(source);

        auto ptr = std::make_shared<D3D10Program>(vs, ps, pDevice);
        sourcePrograms[source] = ptr;

        return ptr;
    }
}

ShiftEngine::IProgramPtr ShiftEngine::D3D11ShaderManager::CreateProgramFromMaterialFlags(const MaterialInfo & mtlInfo, const VertexSemantic & verticesInfo)
{
    auto program = materialPrograms.find(std::make_pair(mtlInfo, verticesInfo));
    if (program != materialPrograms.end())
    {
        return program->second;
    }
    else
    {
        auto shaderCode = shaderGenerator->CreateShaderCode(verticesInfo, mtlInfo);

        D3D10VertexDeclaration vd(nullptr, pDevice);
        auto vs = CompileVSFromSource(shaderCode);
        auto ps = CompilePSFromSource(shaderCode);

        auto ptr = std::make_shared<D3D10Program>(vs, ps, pDevice);
        materialPrograms[std::make_pair(mtlInfo, verticesInfo)] = ptr;

        return ptr;
    }
}
