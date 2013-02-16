#include "Structures.hlsli"
#include "Constants.hlsli"


float4 main( in ATMOS_PS_INPUT input ) : SV_TARGET
{
    float fCos = dot( angleAndPhaseResult.x, input.Ray) / length(input.Ray);
    float fCos2 = fCos * fCos;
    float4 color = input.c0 * angleAndPhaseResult.y;
    color.z = 1;
    return color;
}

