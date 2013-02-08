#include "stdafx.h"
#include "Sphere.h"
#include <algorithm>

using namespace DirectX;
using namespace Scattering;
using namespace std;


/* Constructor */
Sphere::Sphere( float radius )
    : _r( radius ),
      _vertexBuffer( nullptr ),
      _indexBuffer( nullptr )
{
	const float X = 0.525731f; 
	const float Z = 0.850651f;

	XMFLOAT3 points[12] = {
		XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),  
		XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),    
		XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X), 
		XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),    
		XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f), 
		XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
	};

    _vertices.resize( ARRAYSIZE( points ) );
    for( unsigned int i = 0; i < ARRAYSIZE( points ); ++i ){
        XMVECTOR n = XMVector3Normalize( XMLoadFloat3( &points[i] ) );
        XMVECTOR p = _r * n;
        XMStoreFloat3( &_vertices[i].normal, n );
        XMStoreFloat3( &_vertices[i].position, p );
        _vertices[i].color = XMFLOAT4( 1, 0, 0, 1 );
    }

}


/* Destructor */
Sphere::~Sphere(void) {
    if( _vertexBuffer ) _vertexBuffer->Release();
    if( _indexBuffer ) _indexBuffer->Release();
}


/* Create a buffer for the shape */
void Sphere::createBuffer( ID3D11Device *d3dDevice ) {

    generateIndices();
    
    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof( Vertex ) * static_cast<UINT>( _vertices.size() );
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    vbd.StructureByteStride = 0;
    
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &_vertices[0];
    d3dDevice->CreateBuffer( &vbd, &vinitData, &_vertexBuffer );
    
    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof( UINT ) * static_cast<UINT>( _indices.size() );
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    ibd.StructureByteStride = 0;
    
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &_indices[0];
    HRESULT hr = d3dDevice->CreateBuffer( &ibd, &iinitData, &_indexBuffer );
    assert( SUCCEEDED( hr ) );
}


/* Returns the vertex buffer */
ID3D11Buffer **Sphere::getVertexBuffer() {
    return &_vertexBuffer;
}

/* Returns the index buffer */
ID3D11Buffer *Sphere::getIndexBuffer() {
    return _indexBuffer;
}