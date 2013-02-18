#include "Structures.hlsli"
#include "Constants.hlsli"

// Calculates the result of the Rayleigh scattering phase function
float rayleighPhase( float angle )
{
    return 0.75 * (1.0 + angle * angle);
}

float miePhase( float angle )
{

    float g =-0.90f;
    float g2 = 0.81f;

    float phase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + angle*angle) / pow(1.0 + g2 - 2.0*g*angle, 1.5);

    return phase;

    float U = 0.7;

    //float x = (5.0/9.0) * U + (125.0/729.0) * pow(U,3)
    //    + pow( (64.0/27.0) - (325.0/243.0) * pow(U,2) + (1250.0/2187.0) * pow(U,4), 0.5f );

    //float g = (5.0/9.0) * U - (4.0/3.0 - (25.0/81.0) * pow(U, 2)) * pow(x, -1.0/3.0)
    //    + pow(x, 1.0/3.0);

    //float result = ((3.0 * (1-(pow(g,2))/(2.0*(2.0+pow(g,2)))))
    //    * ((1.0 + pow(cos(angle),2)) / pow(1.0 + pow(g,2)+2.0 * g * cos(angle), 3.0/2.0)));

    //return result;
}


float4 main( in ATMOS_PS_INPUT input ) : SV_TARGET
{

    float fCos = dot( lightDirection, input.Ray) / length(input.Ray);

    float4 color = (rayleighPhase( fCos ) * input.c0) + (miePhase( fCos ) * input.c1);
    color.w = max(color.x, color.z);

    float4 result = 1 - exp(-2 * color);
    return result;
}

