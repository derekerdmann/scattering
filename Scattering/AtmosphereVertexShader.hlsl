#include "Structures.hlsli"
#include "Constants.hlsli"

// Approximates actual scattering
float scale(float c)
{
	float x = 1.0 - c;  
	return scaleHeight * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

ATMOS_PS_INPUT main( in PLANET_VS_INPUT input )
{
    int samples = 3;

    float3 lightVector = cameraPosition - sunPosition;
    
    float3 pos = input.Position.xyz;
    float3 ray = pos - cameraPosition.xyz;
    float far = length( ray );
    ray /= far;

    float3 start = cameraPosition.xyz;

    // Initialize the scattering loop variables
    float sampleLength = far / float( samples );
    float scaledLength = sampleLength * atmosScale;
    float3 sampleRay = ray * sampleLength;
    float3 samplePoint = start + sampleRay * 0.5;

    float h = length( samplePoint );
    float startAngle = dot( ray, start ) / h;
    float startDepth = exp(atmosScaleOverScaleHeight * (planetRadius - h));
    float startOffset = startDepth * scale( startAngle );

    // Calculate scattering
    // Work with each sample point
    float3 frontColor = float3( 0, 0, 0 );
    for( int i = 0; i < samples; i++ ){

        h = length( samplePoint );
        float depth = exp( atmosScaleOverScaleHeight * (planetRadius - h) );
        float lightAngle = dot( lightDirection, samplePoint ) / h;
        float cameraAngle = dot( ray, samplePoint ) / h;
        float scatter = startOffset + depth * (scale(lightAngle) - scale(cameraAngle));
        float3 attenuate = exp( -scatter * invWavelength.xyz * Kr4Pi + Km4Pi );
        frontColor += attenuate * (depth * scaledLength);
        samplePoint += sampleRay;
    }

    // Prep values for pixel shader
    ATMOS_PS_INPUT output;
    output.Ray = float4( ray, 1 );
    output.Position = mul(  worldViewProjection, float4( pos, 1 ) );
    output.c0 = float4( frontColor * (invWavelength.xyz * KrESun), 1 );
    output.c1 = float4( frontColor * KmESun, 1 );

    return output;
}