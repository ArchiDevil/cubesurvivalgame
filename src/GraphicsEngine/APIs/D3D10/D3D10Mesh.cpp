#include "D3D10Mesh.h"

#include <cassert>

ShiftEngine::D3D10MeshData::D3D10MeshData(ID3D10Device * pDevice)
    : IMeshData()
    , pDevice(pDevice)
{
}

ShiftEngine::D3D10MeshData::D3D10MeshData(ID3D10Buffer * _VB /*= nullptr*/, ID3D10Buffer * _IB /*= nullptr*/, ID3D10Device * pDevice /*= nullptr*/)
    : IMeshData()
    , VertexBuffer(_VB)
    , IndexBuffer(_IB)
    , pDevice(pDevice)
{
}

ShiftEngine::D3D10MeshData::D3D10MeshData(const D3D10MeshData & ref)
    : pDevice(ref.pDevice)
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
}

ShiftEngine::D3D10MeshData& ShiftEngine::D3D10MeshData::operator = (const D3D10MeshData & ref)
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

    return *this;
}

ShiftEngine::D3D10MeshData::~D3D10MeshData()
{
    Clear();
}

bool ShiftEngine::D3D10MeshData::CreateBuffers(bool dynamic,
    const uint8_t * vData,
    size_t vDataSize,
    const uint32_t * iData,
    size_t iDataSize,
    const VertexSemantic * semantic,
    const IVertexDeclarationPtr & declaration)
{
    assert(pDevice);

    if (!vData || !vDataSize || !semantic)
        return false;

    D3D10_BUFFER_DESC vBuffDesc;
    ZeroMemory(&vBuffDesc, sizeof(D3D10_BUFFER_DESC));
    D3D10_BUFFER_DESC iBuffDesc;
    ZeroMemory(&iBuffDesc, sizeof(D3D10_BUFFER_DESC));

    vBuffDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vBuffDesc.ByteWidth = vDataSize;
    vBuffDesc.MiscFlags = NULL;

    if (dynamic)
    {
        vBuffDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
        vBuffDesc.Usage = D3D10_USAGE_DYNAMIC;
    }
    else
    {
        vBuffDesc.CPUAccessFlags = NULL;
        vBuffDesc.Usage = D3D10_USAGE_DEFAULT;
    }

    if (vData)
    {
        D3D10_SUBRESOURCE_DATA data;
        ZeroMemory(&data, sizeof(D3D10_SUBRESOURCE_DATA));
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
        iBuffDesc.Usage = D3D10_USAGE_DYNAMIC;
    }
    else
    {
        iBuffDesc.CPUAccessFlags = NULL;
        iBuffDesc.Usage = D3D10_USAGE_DEFAULT;
    }

    if (iData)
    {
        D3D10_SUBRESOURCE_DATA data;
        ZeroMemory(&data, sizeof(D3D10_SUBRESOURCE_DATA));
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

void ShiftEngine::D3D10MeshData::Clear()
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

size_t ShiftEngine::D3D10MeshData::Draw()
{
    if (!pDevice)
        return 0;

    if (!vertexSize || !verticesCount)
        return 0;

    unsigned int stride = vertexSize;
    unsigned int offset = 0;
    pDevice->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
    if (indicesCount > 0)
    {
        pDevice->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        pDevice->DrawIndexed(indicesCount, 0, 0);
    }
    else
    {
        pDevice->Draw(verticesCount, 0);
    }

    return indicesCount / 3;
}
