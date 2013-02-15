#include "Structures.hlsli"
#include "Constants.hlsli"

ATMOS_VS_OUTPUT main( in VS_INPUT input )
{
    ATMOS_VS_OUTPUT output;

    output.Position = mul(  worldViewProjection, float4( input.Position, 1.0 ) );
    output.Pc = normalize( sunPosition ) * planetRadius;
    output.Color = input.Color;

    return output;
}