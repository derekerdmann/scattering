#include "Structures.hlsli"
#include "Constants.hlsli"

// rho( h ) - Computes the density ratio at the specified height
float densityRatio( float h )
{
    return exp( -h / scaleHeight );
}

// Calculates optical depth along the vector S
float opticalDepth( float3 p1, float3 p2, float beta )
{
    float3 S = p1 - p2;
    float3 sDirection = normalize( S );
    float dP = length( S ) / 5;

    float approx = 0;

    for( uint i = 0; i < 5; i++ ){
        float3 p = sDirection * dP * (i+1);
        float h = length( p ) - planetRadius;

        approx += beta * densityRatio( h );
    }

    return approx;

}

// Performs most of the scattering work
float scatteringIntegral( float4 Pa, float4 Pb, float4 Pc, float beta )
{
    float3 S = float3( Pa.x - Pb.x, Pa.y - Pb.y, Pa.z - Pb.z );

    float3 sDirection = normalize( S );
    float len = length( S );

    float dP = length( S ) / 5;

    float approx = 0;

    for( uint i = 0; i < 5; i++ ) {
        float3 p = (dP * i + 1) * sDirection;

        float h = length( p ) - planetRadius;

        approx += densityRatio( h )
            * exp( -opticalDepth( p, Pc, beta ) - opticalDepth( p, Pa, beta ) )
            * dP;
    }

    return approx;
}


float4 main( in ATMOS_PS_INPUT input ) : SV_TARGET
{
    float sunAngle = angleAndPhaseResult.x;
    float phaseFunctionOutput = angleAndPhaseResult.y;

    float R = sunAngle.x * sunIntensity.x
        * attenuationCoefficient.x
        * phaseFunctionOutput;
        //* scatteringIntegral(
        //    cameraPosition,
        //    pos,
        //    input.Pc,
        //    attenuationCoefficient.x
        //);

    float G = sunIntensity.y
        * attenuationCoefficient.y
        * phaseFunctionOutput;
        //* scatteringIntegral(
        //    cameraPosition,
        //    pos,
        //    input.Pc,
        //    attenuationCoefficient.y
        //);

    float B = sunIntensity.z
        * attenuationCoefficient.z
        * phaseFunctionOutput;
        //* scatteringIntegral(
        //    cameraPosition,
        //    pos,
        //    input.Pc,
        //    attenuationCoefficient.z
        //);

    return float4( R, G, B, 1 );
}

