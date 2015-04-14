#include "D3D11Mesh.h"

#include <cassert>

ShiftEngine::D3D11MeshData::D3D11MeshData(ID3D11Buffer * _VB /*= nullptr*/, ID3D11Buffer * _IB /*= nullptr*/, 
                                          ID3D11Device * pDevice /*= nullptr*/, ID3D11DeviceContext * pDeviceContext /*= nullptr*/)
    : VertexBuffer(_VB)
    , IndexBuffer(_IB)
    , pDevice(pDevice)
    , pDeviceContext(pDeviceContext)
{
}

ShiftEngine::D3D11MeshData::D3D11MeshData(const D3D11MeshData & ref)
    : pDevice(ref.pDevice)
    , pDeviceContext(ref.pDeviceContext)
{
    indicesCount = ref.indicesCount;
    vertexSize = ref.vertexSize;
    verticesCount = ref.verticesCount;
    vertexDeclaration = ref.vertexDeclaration;
    vertexSemantic = ref.vertexSemantic;

    IndexBuffer = ref.IndexBuffer;
    if (IndexBuffer)
        IndexBuffer->AddRef();

    VertexBuffer = ref.VertexBuffer;
    if (VertexBuffer)
        VertexBuffer->AddRef();
}

ShiftEngine::D3D11MeshData& ShiftEngine::D3D11MeshData::operator = (const D3D11MeshData & ref)
{
    indicesCount = ref.indicesCount;
    vertexSize = ref.vertexSize;
    verticesCount = ref.verticesCount;

    IndexBuffer = ref.IndexBuffer;
    if (IndexBuffer)
        IndexBuffer->AddRef();

    VertexBuffer = ref.VertexBuffer;
    if (VertexBuffer)
        VertexBuffer->AddRef();

    vertexDeclaration = ref.vertexDeclaration;
    vertexSemantic = ref.vertexSemantic;

    pDevice = ref.pDevice;
    pDeviceContext = ref.pDeviceContext;

    return *this;
}

ShiftEngine::D3D11MeshData::~D3D11MeshData()
{
    Clear();
}

bool ShiftEngine::D3D11MeshData::CreateBuffers(bool dynamic, const uint8_t * vData, size_t vDataSize, const uint32_t * iData, size_t iDataSize, const VertexSemantic * semantic, const IVertexDeclarationPtr & declaration)
{
    assert(pDevice);
    assert(pDeviceContext);

    if (!vData || !vDataSize || !semantic)
        return false;

    D3D11_BUFFER_DESC vBuffDesc;
    ZeroMemory(&vBuffDesc, sizeof(D3D11_BUFFER_DESC));
    D3D11_BUFFER_DESC iBuffDesc;
    ZeroMemory(&iBuffDesc, sizeof(D3D11_BUFFER_DESC));

    vBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vBuffDesc.ByteWidth = vDataSize;
    vBuffDesc.MiscFlags = NULL;

    if (dynamic)
    {
        vBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        vBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
    }
    else
    {
        vBuffDesc.CPUAccessFlags = NULL;
        vBuffDesc.Usage = D3D11_USAGE_DEFAULT;
    }

    if (vData)
    {
        D3D11_SUBRESOURCE_DATA data;
        ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
        data.pSysMem = vData;

        if (FAILED(pDevice->CreateBuffer(&vBuffDesc, &data, &VertexBuffer)))
            return false;
    }

    iBuffDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    iBuffDesc.ByteWidth = iDataSize;
    iBuffDesc.MiscFlags = NULL;

    if (dynamic)
    {
        iBuffDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
        iBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
    }
    else
    {
        iBuffDesc.CPUAccessFlags = NULL;
        iBuffDesc.Usage = D3D11_USAGE_DEFAULT;
    }

    if (iData)
    {
        D3D11_SUBRESOURCE_DATA data;
        ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
        data.pSysMem = iData;

        if (FAILED(pDevice->CreateBuffer(&iBuffDesc, &data, &IndexBuffer)))
            return false;
    }

    vertexSemantic = semantic;
    vertexDeclaration = declaration;
    vertexSize = vertexSemantic->getVertexSize();
    verticesCount = vDataSize / vertexSize;
    indicesCount = iDataSize / sizeof(uint32_t);

    return true;
}

void ShiftEngine::D3D11MeshData::Clear()
{
    if (VertexBuffer)
    {
        VertexBuffer->Release();
        VertexBuffer = nullptr;
    }

    if (IndexBuffer)
    {
        IndexBuffer->Release();
        IndexBuffer = nullptr;
    }
}

size_t ShiftEngine::D3D11MeshData::Draw()
{
    if (!pDevice)
        return 0;

    if (!vertexSize || !verticesCount)
        return 0;

    unsigned int stride = vertexSize;
    unsigned int offset = 0;
    pDeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
    if (indicesCount > 0)
    {
        pDeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        pDeviceContext->DrawIndexed(indicesCount, 0, 0);
    }
    else
    {
        pDeviceContext->Draw(verticesCount, 0);
    }

    return indicesCount / 3;
}
