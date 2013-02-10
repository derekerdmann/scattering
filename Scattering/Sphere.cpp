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
	//const float X = 0.525731f; 
	//const float Z = 0.850651f;

	//XMFLOAT3 points[12] = {
	//	XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),  
	//	XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),    
	//	XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X), 
	//	XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),    
	//	XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f), 
	//	XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
	//};

 //   _vertices.resize( ARRAYSIZE( points ) );
 //   for( unsigned int i = 0; i < ARRAYSIZE( points ); ++i ){
 //       XMVECTOR n = XMVector3Normalize( XMLoadFloat3( &points[i] ) );
 //       XMVECTOR p = _r * n;
 //       XMStoreFloat3( &_vertices[i].normal, n );
 //       XMStoreFloat3( &_vertices[i].position, p );
 //       _vertices[i].color = XMFLOAT4( 1, 0, 0, 1 );
 //   }


	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
    Vertex topVertex = { XMFLOAT3( 0, _r, 0 ), XMFLOAT3( 0, 1, 0 ) };
    Vertex bottomVertex = { XMFLOAT3( 0, -_r, 0 ), XMFLOAT3( 0, -1, 0 ) };

    const UINT stackCount = 20, sliceCount = 20;

	_vertices.push_back( topVertex );

	float phiStep   = XM_PI/stackCount;
	float thetaStep = 2.0f*XM_PI/sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for(UINT i = 1; i <= stackCount-1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for(UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			Vertex v;

			// spherical to cartesian
			v.position.x = radius*sinf(phi)*cosf(theta);
			v.position.y = radius*cosf(phi);
			v.position.z = radius*sinf(phi)*sinf(theta);

			XMVECTOR p = XMLoadFloat3(&v.position);
            XMStoreFloat3(&v.normal, XMVector3Normalize(p));

			_vertices.push_back( v );
		}
	}

	_vertices.push_back( bottomVertex );

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for(UINT i = 1; i <= sliceCount; ++i)
	{
		_indices.push_back(0);
		_indices.push_back(i+1);
		_indices.push_back(i);
	}
	
	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount+1;
	for(UINT i = 0; i < stackCount-2; ++i)
	{
		for(UINT j = 0; j < sliceCount; ++j)
		{
			_indices.push_back(baseIndex + i*ringVertexCount + j);
			_indices.push_back(baseIndex + i*ringVertexCount + j+1);
			_indices.push_back(baseIndex + (i+1)*ringVertexCount + j);

			_indices.push_back(baseIndex + (i+1)*ringVertexCount + j);
			_indices.push_back(baseIndex + i*ringVertexCount + j+1);
			_indices.push_back(baseIndex + (i+1)*ringVertexCount + j+1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)_vertices.size()-1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;
	
	for(UINT i = 0; i < sliceCount; ++i)
	{
		_indices.push_back(southPoleIndex);
		_indices.push_back(baseIndex+i);
		_indices.push_back(baseIndex+i+1);
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