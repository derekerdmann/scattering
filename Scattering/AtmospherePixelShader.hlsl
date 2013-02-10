#include "Structures.hlsli"
#include "Constants.hlsli"

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

