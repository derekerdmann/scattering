#include "stdafx.h"
#include "Planet.h"

using namespace DirectX;
using namespace Scattering;


const float Planet::EARTH_RADIUS = 6378.101f;

Planet::Planet( float radius, float karmanLine )
    : Sphere( radius ),
      _atmosphere( radius, karmanLine ),
      _vertexShader( nullptr ),
      _pixelShader( nullptr )
{

}


Planet::~Planet(void)
{
}


void Planet::createBuffer( ID3D11Device *d3dDevice ) {
    Sphere::createBuffer( d3dDevice );
    _atmosphere.createBuffer( d3dDevice );
}


/* Initializes the vertex layout */
void Planet::setupShaders( ID3D11Device *d3dDevice, ID3D11DeviceContext *d3dDeviceContext ) {

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

    _atmosphere.setupShaders( d3dDevice, d3dDeviceContext );
}


/* Sets constants for the beginning of the program */
void Planet::setConstants(
    ID3D11Device *d3dDevice,
    ID3D11DeviceContext *d3dDeviceContext,
    XMFLOAT3 sunIntensity
) {
    _atmosphere.setConstants( d3dDevice, d3dDeviceContext, sunIntensity );
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

    d3dDeviceContext->DrawIndexed( static_cast<UINT>( _indices.size() ), 0, 0);

    _atmosphere.draw( d3dDevice, d3dDeviceContext );

}

void Planet::generateIndices() {

	//UINT indices[60] = {
	//	1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,    
	//	1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,    
	//	3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0, 
	//	10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7 
	//};

	//_indices.resize( ARRAYSIZE( indices ) );
 //   for( unsigned int i = 0; i < ARRAYSIZE( indices ); ++i ){
 //       _indices[i] = indices[i];
 //   }
    
}


float Planet::phaseFunction( float angle ) {
    return _atmosphere.phaseFunction( angle );
}