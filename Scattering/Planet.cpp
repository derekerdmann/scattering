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


void Planet::draw( ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext ) {
    
	UINT stride = sizeof(Vertex);
    UINT offset = 0;

    d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3dDeviceContext->IASetIndexBuffer(getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
    d3dDeviceContext->IASetVertexBuffers(0, 1, getVertexBuffer(), &stride, &offset);

    d3dDeviceContext->IASetInputLayout( _inputLayout );

    d3dDeviceContext->PSSetShader( _pixelShader, nullptr, 0 );
    d3dDeviceContext->VSSetShader( _vertexShader, nullptr, 0 );

    d3dDeviceContext->DrawIndexed(60, 0, 0);

}