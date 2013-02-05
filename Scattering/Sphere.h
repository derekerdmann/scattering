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

    /* Compiles the shape's effect from file and creates the effect pointer */
    virtual void compileEffect( ID3D11Device *d3dDevice );

    /* Allows subclasses to retrieve any necessary effect variables once the
     * effect has been compiled */
    virtual void storeEffectVariables();

    /* Initializes the vertex layout */
    virtual void createVertexLayout( ID3D11Device *d3dDevice ) = 0;

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