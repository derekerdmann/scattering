#include "Structures.hlsli"

float4 main( in VS_OUTPUT input ) : SV_TARGET
{
    return input.Color;
}