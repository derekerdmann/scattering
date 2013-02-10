#include "Structures.hlsli"

float4 main( in PS_INPUT input ) : SV_TARGET
{
    return abs( input.Normal );
}