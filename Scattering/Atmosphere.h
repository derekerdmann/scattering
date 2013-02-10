#pragma once

#include "stdafx.h"
#include "Sphere.h"

namespace Scattering {

/* Represents the atmospheric shell around the planet */
class Atmosphere : public Sphere {
public:

    /* Thickness of atmosphere w/ uniform density (km) */
    static const float SCALE_HEIGHT;

    /* Index of refraction of air */
    static const float REFRACTION_INDEX;

    /* Molecular density of standard atmosphere */
    static const float MOLECULAR_DENSITY;


    /* Constructor
     * planetRadius - radius of the planet that owns the atmosphere
     * karmanLine - the elevation of the outer atmosphere
     */
    Atmosphere( float planetRadius, float karmanLine );
    virtual ~Atmosphere(void);

    // Sets up the shaders for the object
    virtual void setupShaders(
        ID3D11Device *d3dDevice, 
        ID3D11DeviceContext* d3dDeviceContext
    );

    /* Sets constants that will only be computed once for shaders */
    virtual void setConstants( ID3D11Device *d3dDevice );

    /* Draws the planet on the screen */
    void draw(
        ID3D11Device *d3dDevice,
        ID3D11DeviceContext *d3dDeviceContext
    );

protected:
    /* generates the indices for the planet's sphere */
    virtual void generateIndices();

private:

    float _planetRadius;

    ID3D11VertexShader* _vertexShader;
    ID3D11PixelShader* _pixelShader;

	ID3D11InputLayout* _inputLayout;
};

}