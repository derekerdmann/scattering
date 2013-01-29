#pragma once

#include "stdafx.h"
#include "Sphere.h"

namespace Scattering {

/* Represents the atmospheric shell around the planet */
class Atmosphere : public Sphere {
public:

    /* Constructor
     * planetRadius - radius of the planet that owns the atmosphere
     * karmanLine - the elevation of the outer atmosphere
     */
    Atmosphere( float planetRadius, float karmanLine );
    virtual ~Atmosphere(void);
};

}