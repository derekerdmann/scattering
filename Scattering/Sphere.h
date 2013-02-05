#pragma once

#include "stdafx.h"
#include <vector>

namespace Scattering {

/* Records single points for a shape vertex */
struct Vertex {
    Vertex( const DirectX::XMFLOAT3& pos )
        : color( 0, 0, 0 )
    {
        DirectX::XMVECTOR n = DirectX::XMVector3Normalize(XMLoadFloat3(&pos));
        XMStoreFloat3( &normal, n );
    }

    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT3 color;
};

/* Generates points for a sphere */
class Sphere {
public:
    
    /* Constructor */
    Sphere( float radius, std::wstring effectFileName );

    /* Destructor */
    virtual ~Sphere(void);

    /* Initializes the vertex and index buffers */
    virtual void createBuffer( ID3D11Device *d3dDevice );

    // Sets up the shaders for the object
    virtual void setupShaders(
        ID3D11Device *d3dDevice, 
        ID3D11DeviceContext* d3dDeviceContext
    ) = 0;

protected:
    /* compiled DirectX11 effect */
	ID3DX11Effect* _fx;

private:
    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;

    ID3D11Buffer *_vertexBuffer;
    ID3D11Buffer *_indexBuffer;

    /* Effect file name */
    std::wstring _effectFile;

    float _r;
};

}