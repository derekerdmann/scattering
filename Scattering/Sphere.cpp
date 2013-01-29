#include "stdafx.h"
#include "Sphere.h"
#include <algorithm>

using namespace DirectX;
using namespace Scattering;
using namespace std;

Sphere::Sphere( float radius )
    : _r( radius )
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

    for( unsigned int i = 0; i < 12; ++i ){
        _vertices.push_back( points[i] );
        XMVECTOR n = XMVector3Normalize( XMLoadFloat3( &points[i] ) );
        XMVECTOR p = _r * n;
        XMStoreFloat3( &_vertices[i].normal, n );
        XMStoreFloat3( &_vertices[i].position, p );
    }

	unsigned int indices[60] = {
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,    
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,    
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0, 
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7 
	};

	_indices.resize(60);
    for( unsigned int i = 0; i < 60; ++i ){
        _indices[i] = indices[i];
    }
    
    
}


Sphere::~Sphere(void)
{
}
