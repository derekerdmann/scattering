// Information related to the camera and viewing
cbuffer viewingData : register( b0 )
{
    float3 cameraPosition;
    float4x4 worldViewProjection;
};

// updated as the sun moves through the sky
cbuffer sunDependent : register( b1 )
{
    float3 sunPosition;
    // The sun angle and phase function need to be packed into a single
    // float4, since I couldn't get the correct values out of the buffer 
    // when they were individual floats
    // x component is angle
    // y component is phase function result
    float4 angleAndPhaseResult;              // theta
    //float phaseFunctionResult;   // F( theta, g )
}

// Should only be set once
cbuffer constants : register( b2 )
{
    float4 sunIntensity;            // I0 [R, G, B]
    float4 attenuationCoefficient;  // beta [R, G, B]
    float refractionIndex;          // n
    float scaleHeight;              // H0
    float planetRadius;
    float atmosphereRadius;
    float atmosScale;
    float atmosScaleOverScaleHeight;
};
