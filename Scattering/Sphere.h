#pragma once

#include "stdafx.h"
#include <vector>

namespace Scattering {

/* Records single points for a shape vertex */
struct Vertex {
    Vertex( const DirectX::XMFLOAT3& pos ) {
        DirectX::XMVECTOR n = DirectX::XMVector3Normalize(XMLoadFloat3(&pos));
        XMStoreFloat3( &normal, n );
    }

    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
};

/* Generates points for a sphere */
class Sphere {
public:
    
    /* Constructor */
    Sphere( float radius );

    /* Destructor */
    virtual ~Sphere(void);

private:
    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;

    float _r;
};

}