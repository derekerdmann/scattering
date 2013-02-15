// Information related to the camera and viewing
cbuffer viewingData : register( b0 )
{
    float4 cameraPosition;
    float4x4 worldViewProjection;
};

// updated as the sun moves through the sky
cbuffer sunDependent : register( b1 )
{
    float sunAngle;              // theta
    float4 sunPosition;
    float phaseFunctionResult;   // F( theta, g )
}

// Should only be set once
cbuffer constants : register( b2 )
{
    float4 sunIntensity;            // I0 [R, G, B]
    float4 attenuationCoefficient;  // beta [R, G, B]
    float refractionIndex;          // n
    float scaleHeight;              // H0
    float planetRadius;
};
