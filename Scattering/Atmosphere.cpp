#include "stdafx.h"
#include "Atmosphere.h"
#include "ConstantBuffers.h"

using namespace DirectX;
using namespace Scattering;


/* Thickness of atmosphere w/ uniform density (km) */
const float Atmosphere::SCALE_HEIGHT = 0.25f;

/* Index of refraction of air - source Wikipedia */
const float Atmosphere::REFRACTION_INDEX = 1.000293f;

/* Molecular density of standard atmosphere */
const float Atmosphere::MOLECULAR_DENSITY = 2.54743e19f;

/* Constant for atmospheric conditions */
const float Atmosphere::U = 0.7f;


Atmosphere::Atmosphere( float planetRadius, float karmanLine )
    : Sphere( planetRadius + karmanLine ),
      _vertexShader( nullptr ),
      _pixelShader( nullptr ),
      _planetRadius( planetRadius ),
      _karmanLine( karmanLine )
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
void Atmosphere::setConstants(
    ID3D11Device *d3dDevice,
    ID3D11DeviceContext *d3dDeviceContext,
    XMFLOAT3 sunIntensity
) {

    // Fill in a buffer description.
    D3D11_BUFFER_DESC cbDesc;
    cbDesc.ByteWidth = sizeof( StaticConstants );
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbDesc.MiscFlags = 0;
    cbDesc.StructureByteStride = 0;

    StaticConstants constants;
    ZeroMemory( &constants, sizeof( StaticConstants ) );
    constants.refractionIndex = Atmosphere::REFRACTION_INDEX;
    constants.scaleHeight = Atmosphere::SCALE_HEIGHT;
    constants.planetRadius = _planetRadius;
    constants.atmosphereRadius = _planetRadius + _karmanLine;
    constants.atmosScale = 1 / (constants.atmosphereRadius - _planetRadius);
    constants.atmosScaleOverScaleHeight = constants.atmosScale / 0.25;
    constants.invWavelength = XMFLOAT4(
        1.0f / pow( 0.650f, 4 ),
        1.0f / pow( 0.570f, 4 ),
        1.0f / pow( 0.475f, 4 ),
        1
    );

    constants.sunIntensity = XMFLOAT4(
        sunIntensity.x,
        sunIntensity.y,
        sunIntensity.z,
        1
    );

    constants.KrESun = 0.025f;
    constants.KmESun = 0.15f;
    constants.Kr4Pi = 0.00025f * 4 * XM_PI;
    constants.Km4Pi = 0.00015f * 4 * XM_PI;

    // Fill in the subresource data.
    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = &constants;
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

    d3dDeviceContext->VSSetConstantBuffers( 2, 1, &constantBuffer );
    d3dDeviceContext->PSSetConstantBuffers( 2, 1, &constantBuffer );
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

    d3dDeviceContext->DrawIndexed( static_cast<UINT>( _indices.size() ), 0, 0);

}

void Atmosphere::generateIndices() {

    // Swap the first and third vertices of every triangle
    for( UINT i = 0; i < _indices.size(); i += 3 ){
        std::swap( _indices[i], _indices[i+2] );
    }

}


/* Calculates the attenuation coefficient for the specified wavelength */
inline float Atmosphere::attenuation( int wavelength ) {
    const float n = Atmosphere::REFRACTION_INDEX;
    const float Ns = Atmosphere::MOLECULAR_DENSITY;
    const float lambda = static_cast<float>( wavelength );

    return 8.0f * pow( XM_PI, 3.0f ) * pow( pow( n, 3.0f ) - 1, 2.0f )
        / 3.0f * Ns * pow( lambda, 4.0f );
}

/* Calculates the phase function */
float Atmosphere::phaseFunction( float angle ) {
    
    const float x = (5.0f/9.0f) * U + (125.0f/729.0f) * pow(U,3)
        + pow( (64.0f/27.0f) - (325.0f/243.0f) * pow(U,2) + (1250.0f/2187.0f) * pow(U,4), 0.5f );

    const float g = (5.0f/9.0f) * U - (4.0f/3.0f - (25.0f/81.0f) * pow(U, 2)) * pow(x, -1.0f/3.0f)
        + pow(x, 1.0f/3.0f);

    const float result = ((3.0f * (1-(pow(g,2))/(2.0f*(2.0f+pow(g,2)))))
        * ((1.0f + pow(cos(angle),2)) / pow(1.0f + pow(g,2)+2.0f * g * cos(angle), 3.0f/2.0f)));

    return result;
}
