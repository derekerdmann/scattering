#pragma once

#include "stdafx.h"

namespace Scattering {

/* Wavelengths for RGB colors, specified in nanometers */
enum ColorWavelengths {
    RED = 650,
    GREEN = 532,
    BLUE = 473
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
    float sunAngle;
    float phaseFunctionResult;
};


/* Shader constants that are set once and are not changed
 */
struct StaticConstants {
    DirectX::XMFLOAT3 sunIntensity;
    DirectX::XMFLOAT3 attenuationCoefficient;
    float refractionIndex;
    float scaleHeight;
    float planetRadius;
};

}