#include "Structures.hlsli"
#include "Constants.hlsli"

PS_INPUT main( in VS_INPUT input )
{
    PS_INPUT output;

    output.Position = mul(  worldViewProjection, float4( input.Position, 1.0 ) );

    return output;
}