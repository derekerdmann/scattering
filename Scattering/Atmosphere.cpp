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