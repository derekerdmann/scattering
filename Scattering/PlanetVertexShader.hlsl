#include "Structures.hlsli"
#include "Constants.hlsli"

PLANET_PS_INPUT main( in PLANET_VS_INPUT input )
{
    PLANET_PS_INPUT output;

    output.Position = mul(  worldViewProjection, float4( input.Position, 1.0 ) );
    output.Normal = mul( invWorldViewProjection, float4( input.Normal, 1.0 ) );

    return output;
}