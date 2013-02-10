#include "Structures.hlsli"

float4x4 worldViewProjection;

PS_INPUT main( in VS_INPUT input )
{
    PS_INPUT output;

    output.Position = mul(  worldViewProjection, float4( input.Position, 1.0 ) );
    output.Normal = -float4( input.Normal, 1.0 );
    output.Color = input.Color;

    return output;
}