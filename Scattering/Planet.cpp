#include "stdafx.h"
#include "Planet.h"

using namespace DirectX;
using namespace Scattering;

Planet::Planet( float radius, float karmanLine )
    : Sphere( radius ), _atmosphere( radius, karmanLine )
{
}


Planet::~Planet(void)
{
}
