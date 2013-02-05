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

    d3dDevice->CreateVertexShader(
        vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(),
        nullptr,
        &_vertexShader
    );

    D3D11_INPUT_ELEMENT_DESC vertexDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

	hr = d3dDevice->CreateInputLayout(
        vertexDesc, 
        3, 
        vertexShaderBuffer->GetBufferPointer(), 
        vertexShaderBuffer->GetBufferSize(), 
        &_inputLayout
    );

    assert( SUCCEEDED( hr ) );

    Release( vertexShaderBuffer );

    ID3DBlob *pixelShaderBuffer = nullptr;

    D3DReadFileToBlob( L"PlanetPixelShader.cso", &pixelShaderBuffer );
    hr = d3dDevice->CreatePixelShader(
        pixelShaderBuffer->GetBufferPointer(),
        pixelShaderBuffer->GetBufferSize(),
        nullptr,
        &_pixelShader
    );

    assert( SUCCEEDED( hr ) );

    Release( pixelShaderBuffer );

    assert( SUCCEEDED( hr ) );
}