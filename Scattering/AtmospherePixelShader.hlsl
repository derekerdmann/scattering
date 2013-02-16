#include "Structures.hlsli"
#include "Constants.hlsli"


float4 main( in ATMOS_PS_INPUT input ) : SV_TARGET
{

    float fCos = dot( angleAndPhaseResult.x, input.Ray) / length(input.Ray);
    float fCos2 = fCos * fCos;

    float g =-0.90;
    float g2 = 0.81;

    float fRayleighPhase = 0.75 * (1.0 + fCos*fCos);
	float fMiePhase= 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);

    float4 color = (fRayleighPhase * input.c0) + (fMiePhase * input.c1);
    color.z = max(color.x, color.z);

    return 1 - exp(-2 * color);
}

