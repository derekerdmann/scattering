// Information related to the camera and viewing
cbuffer viewingData : register( cb0 )
{
    float4 cameraPosition;
    float4x4 worldViewProjection;
};

// updated as the sun moves through the sky
cbuffer sunDependent : register( cb1 )
{
    float sunAngle;              // theta
    float phaseFunctionResult;   // F( theta, g )
}

// Should only be set once
cbuffer constants : register( cb2 )
{
    float3 sunIntensity;            // I0 [R, G, B]
    float3 attenuationCoefficient;  // beta [R, G, B]
    float refractionIndex;          // n
    float scaleHeight;              // H0
    float planetRadius;
};
