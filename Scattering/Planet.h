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

    // Sets up the shaders for the object
    virtual void setupShaders(
        ID3D11Device *d3dDevice, 
        ID3D11DeviceContext* d3dDeviceContext
    );

    /* Draws the planet on the screen */
    void draw(
        ID3D11Device *d3dDevice,
        ID3D11DeviceContext *d3dDeviceContext
    );

private:

    /* The planet's atmospheric shell */
    //Atmosphere _atmosphere;

    /* Planet's radius */
    float _r;

    /* The upper limit of the planet's atmosphere */
    float _karmanLine;

    ID3D11VertexShader* _vertexShader;
    ID3D11PixelShader* _pixelShader;

	ID3D11InputLayout* _inputLayout;

};

}