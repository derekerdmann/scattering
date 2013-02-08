#include "stdafx.h"
#include "Atmosphere.h"

using namespace DirectX;
using namespace Scattering;

Atmosphere::Atmosphere( float planetRadius, float karmanLine )
    : Sphere( planetRadius + karmanLine ),
      _vertexShader( nullptr ),
      _pixelShader( nullptr )
{
}


Atmosphere::~Atmosphere(void)
{
}

/* Initializes the vertex layout */
void Atmosphere::setupShaders( ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext ) {

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


void Atmosphere::draw( ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext ) {
    
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

void Atmosphere::generateIndices() {

	UINT indices[60] = {
        0, 4, 1,
        0, 9, 4,
        9, 5, 4,
        4, 5, 8,
        4, 8, 1,
        8, 10, 1,
        8, 3, 10,
        5, 3, 8,
        5, 2, 3,
        2, 7, 3,
        7, 10, 3,
        7, 6, 10,
        7, 11, 6,
        11, 0, 6,
        0, 1, 6,
        6, 1, 10,
        9, 0, 11,
        9, 11, 2,
        9, 2, 5,
        7, 2, 11,
	};

	_indices.resize( ARRAYSIZE( indices ) );
    for( unsigned int i = 0; i < ARRAYSIZE( indices ); ++i ){
        _indices[i] = indices[i];
    }
    
}