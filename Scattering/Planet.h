#pragma once

#include "Sphere.h"
#include "Atmosphere.h"

namespace Scattering {

/* Represents a planet with an atmosphere */
class Planet : public Sphere {
public:

    /* Constructor
     * radius - The radius of the planet
     * karmanLine - The upper boundary of the planet's atmosphere
     */
    Planet( float radius, float karmanLine );

    /* Destructor */
    virtual ~Planet(void);

private:

    /* The planet's atmospheric shell */
    Atmosphere _atmosphere;

    /* Planet's radius */
    float _r;

    /* The upper limit of the planet's atmosphere */
    float _karmanLine;
};

}