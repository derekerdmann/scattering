#pragma once

#include "stdafx.h"
#include <vector>

namespace Scattering {

/* Records single points for a shape vertex */
struct Vertex {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT4 color;
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

    /* Returns the vertex buffer */
    virtual ID3D11Buffer **getVertexBuffer();

    /* Returns the index buffer */
    virtual ID3D11Buffer *getIndexBuffer();

    // Sets up the shaders for the object
    virtual void setupShaders(
        ID3D11Device *d3dDevice, 
        ID3D11DeviceContext* d3dDeviceContext
    ) = 0;

protected:
    /* compiled DirectX11 effect */
	ID3DX11Effect* _fx;

    /* generates the indices for the sphere, allows this to be used for
     * both planet and atmosphere */
    virtual void generateIndices() = 0;

    std::vector<Vertex> _vertices;
    std::vector<UINT> _indices;

private:

    ID3D11Buffer *_vertexBuffer;
    ID3D11Buffer *_indexBuffer;

    /* Effect file name */
    std::wstring _effectFile;

    float _r;
};

}