#include "stdafx.h"
#include "Planet.h"

using namespace DirectX;
using namespace Scattering;

Planet::Planet( float radius, float karmanLine )
    : Sphere( radius, L"planet.fx" ),
      //_atmosphere( radius, karmanLine ),
      _vertexShader( nullptr ),
      _pixelShader( nullptr )
{

}


Planet::~Planet(void)
{
}


/* Initializes the vertex layout */
void Planet::setupShaders( ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext ) {

    //_atmosphere.createVertexLayout( d3dDevice );

    ID3DBlob *vertexShaderBuffer = nullptr;
    HRESULT hr = S_OK;

    hr = D3DReadFileToBlob( L"PlanetVertexShader.cso", &vertexShaderBuffer );
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
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

    hr = D3DReadFileToBlob( L"PlanetPixelShader.cso", &pixelShaderBuffer );
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


void Planet::draw( ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext, XMFLOAT4X4 *viewMatrix ) {

	UINT stride = sizeof(Vertex);
    UINT offset = 0;
    d3dDeviceContext->IASetVertexBuffers(0, 1, getVertexBuffer(), &stride, &offset);
	d3dDeviceContext->IASetIndexBuffer(getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Set constants
	XMMATRIX world = XMMatrixIdentity();
	XMMATRIX view  = XMLoadFloat4x4( viewMatrix );
	XMMATRIX proj  = XMMatrixIdentity();
	XMMATRIX worldViewProj = world * view * proj;

    XMFLOAT4X4 wvp;
    XMStoreFloat4x4( &wvp, worldViewProj );

    d3dDeviceContext->VSSetShader( _vertexShader, nullptr, 0 );
    d3dDeviceContext->PSSetShader( _pixelShader, nullptr, 0 );

    // Fill in a buffer description.
    D3D11_BUFFER_DESC cbDesc;
    cbDesc.ByteWidth = sizeof( XMFLOAT4X4 );
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbDesc.MiscFlags = 0;
    cbDesc.StructureByteStride = 0;

    // Fill in the subresource data.
    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = &wvp;
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    // Create the buffer.
    ID3D11Buffer *constantBuffer = nullptr;
    HRESULT hr = d3dDevice->CreateBuffer(
        &cbDesc,
        &data,
        &constantBuffer
    );

    assert( SUCCEEDED( hr ) );

    // Set the buffer.
    d3dDeviceContext->VSSetConstantBuffers( 0, 1, &constantBuffer );

}