#include "Structures.hlsli"

// Information related to the 
cbuffer viewingData
{
    float3 cameraPosition;
};

// updated as the sun moves through the sky
cbuffer sunDependent
{
    float sunAngle;              // theta
    float phaseFunctionResult;   // F( theta, g )
}

// Should only be set once
cbuffer atmosphericConstants 
{
    float3 sunIntensity;            // I0 [R, G, B]
    float3 attenuationCoefficient;  // beta [R, G, B]
    float refractionIndex;          // n
    float scaleHeight;              // H0
};


// Calculates optical depth along the vector S
float opticalDepth( float3 S, float lambda )
{
    
}

// rho( h ) - Computes the density ratio at the specified height
float densityRatio( float h )
{
    return exp( -h / scaleHeight );
}


float4 main( in PS_INPUT input ) : SV_TARGET
{
    return input.Normal;
}

