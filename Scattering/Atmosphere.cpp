#include "stdafx.h"
#include "Atmosphere.h"

using namespace DirectX;
using namespace Scattering;

Atmosphere::Atmosphere( float planetRadius, float karmanLine )
    : Sphere( planetRadius + karmanLine ),
      _vertexShader( nullptr ),
      _pixelShader( nullptr ),
      _planetRadius( planetRadius )
{
}


Atmosphere::~Atmosphere(void)
{
}

/* Initializes the vertex layout */
void Atmosphere::setupShaders(
    ID3D11Device *d3dDevice,
    ID3D11DeviceContext *d3dDeviceContext
) {

    ID3DBlob *vertexShaderBuffer = nullptr;
    HRESULT hr = S_OK;

    hr = D3DReadFileToBlob( L"AtmosphereVertexShader.cso", &vertexShaderBuffer );
    assert( SUCCEEDED( hr ) );

    hr = d3dDevice->CreateVertexShader(
        vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(),
        nullptr,
        &_vertexShader
    );
    assert( SUCCEEDED( hr ) );

    D3D11_INPUT_ELEMENT_DESC vertexDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    
	hr = d3dDevice->CreateInputLayout(
        vertexDesc, 
        ARRAYSIZE(vertexDesc),
        vertexShaderBuffer->GetBufferPointer(), 
        vertexShaderBuffer->GetBufferSize(), 
        &_inputLayout
    );
    assert( SUCCEEDED( hr ) );

    Release( vertexShaderBuffer );

    ID3DBlob *pixelShaderBuffer = nullptr;

    hr = D3DReadFileToBlob( L"AtmospherePixelShader.cso", &pixelShaderBuffer );
    assert( SUCCEEDED( hr ) );

    hr = d3dDevice->CreatePixelShader(
        pixelShaderBuffer->GetBufferPointer(),
        pixelShaderBuffer->GetBufferSize(),
        nullptr,
        &_pixelShader
    );

    assert( SUCCEEDED( hr ) );

    Release( pixelShaderBuffer );
}


/* Sets constants at the start of the program */
void Atmosphere::setConstants( ID3D11Device *d3dDevice ) {

    // Fill in a buffer description.
    //D3D11_BUFFER_DESC cbDesc;
    //cbDesc.ByteWidth = sizeof( XMFLOAT4X4 );
    //cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    //cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    //cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    //cbDesc.MiscFlags = 0;
    //cbDesc.StructureByteStride = 0;

    //// Fill in the subresource data.
    //D3D11_SUBRESOURCE_DATA data;
    //data.pSysMem = &wvp;
    //data.SysMemPitch = 0;
    //data.SysMemSlicePitch = 0;

    //// Create the buffer.
    //ID3D11Buffer *constantBuffer = nullptr;
    //HRESULT hr = d3dDevice->CreateBuffer(
    //    &cbDesc,
    //    &data,
    //    &constantBuffer
    //);

    //assert( SUCCEEDED( hr ) );
}


void Atmosphere::draw(
    ID3D11Device *d3dDevice,
    ID3D11DeviceContext *d3dDeviceContext
) {
    
	UINT stride = sizeof(Vertex);
    UINT offset = 0;

    d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3dDeviceContext->IASetIndexBuffer(getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
    d3dDeviceContext->IASetVertexBuffers(0, 1, getVertexBuffer(), &stride, &offset);

    d3dDeviceContext->IASetInputLayout( _inputLayout );

    d3dDeviceContext->PSSetShader( _pixelShader, nullptr, 0 );
    d3dDeviceContext->VSSetShader( _vertexShader, nullptr, 0 );

    d3dDeviceContext->DrawIndexed( _indices.size(), 0, 0);

}

void Atmosphere::generateIndices() {

    // Swap the first and third vertices of every triangle
    for( UINT i = 0; i < _indices.size(); i += 3 ){
        std::swap( _indices[i], _indices[i+2] );
    }

}