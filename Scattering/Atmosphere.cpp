#include "stdafx.h"
#include "Atmosphere.h"

using namespace DirectX;
using namespace Scattering;

Atmosphere::Atmosphere( float planetRadius, float karmanLine )
    : Sphere( planetRadius + karmanLine, L"atmosphere.fx" )
{
}


Atmosphere::~Atmosphere(void)
{
}

/* Retrieve the effect technique */
void Atmosphere::storeEffectVariables() {
	_technique = _fx->GetTechniqueByName("ColorTech");
}

/* Initializes the vertex layout */
void Atmosphere::createVertexLayout( ID3D11Device *d3dDevice ) {

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