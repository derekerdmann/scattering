#pragma once

#include "stdafx.h"

namespace Scattering {

/* Wavelengths for RGB colors, specified in nanometers */
enum ColorWavelengths {
    RED = 650,
    GREEN = 570,
    BLUE = 475
};


/* Viewing Data constant buffer. Should be bound to register 0 */
struct ViewingData {
    DirectX::XMFLOAT4 cameraPosition;
    DirectX::XMFLOAT4X4 wordViewProjection;
};


/* Shader constants that depend on the sun, and will change with the sun's
 * position. Should be bound to register 1
 */
struct SunData {
    DirectX::XMFLOAT4 sunPosition;
    float sunAngle;
    float phaseFunctionResult;
    float a;
    float b;
};


/* Shader constants that are set once and are not changed
 */
struct StaticConstants {
    DirectX::XMFLOAT4 sunIntensity;
    DirectX::XMFLOAT4 attenuationCoefficient;
    float refractionIndex;
    float scaleHeight;
    float planetRadius;
};

}