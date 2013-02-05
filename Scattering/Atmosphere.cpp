#include "stdafx.h"
#include "Atmosphere.h"

using namespace DirectX;
using namespace Scattering;

Atmosphere::Atmosphere( float planetRadius, float karmanLine )
    : Sphere( planetRadius + karmanLine, L"FX/Atmosphere.fx" )
{
}


Atmosphere::~Atmosphere(void)
{
}
