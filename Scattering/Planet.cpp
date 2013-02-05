#include "stdafx.h"
#include "Planet.h"

using namespace DirectX;
using namespace Scattering;

Planet::Planet( float radius, float karmanLine )
    : Sphere( radius, L"planet.fx" ), _atmosphere( radius, karmanLine )
{
}


Planet::~Planet(void)
{
}


/* Retrieve the effect technique */
void Planet::storeEffectVariables() {
    _atmosphere.storeEffectVariables();
	_technique = _fx->GetTechniqueByName("ColorTech");
}

/* Initializes the vertex layout */
void Planet::createVertexLayout( ID3D11Device *d3dDevice ) {

    _atmosphere.createVertexLayout( d3dDevice );

    D3D11_INPUT_ELEMENT_DESC vertexDesc[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    // Build the actual vertex layout
    D3DX11_PASS_DESC passDesc;
    _technique->GetPassByIndex(0)->GetDesc(&passDesc);

	HRESULT hr = d3dDevice->CreateInputLayout(
        vertexDesc, 
        2, 
        passDesc.pIAInputSignature, 
		passDesc.IAInputSignatureSize, 
        &_inputLayout
    );

    assert( SUCCEEDED( hr ) );
}