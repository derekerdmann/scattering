#include "Structures.hlsli"
#include "Constants.hlsli"

float4 main( in PLANET_PS_INPUT input ) : SV_TARGET
{
    // RGB  146, 111, 91
    float4 color = float4( 0.5725f, 0.4353f, 0.3569f, 0 );

    float3 toEye = cameraPosition.xyz - input.Position.xyz;

    float3 light = normalize( sunPosition.xyz - input.Position.xyz );

    float4 ambient = color * 0.3f;
    float diffuseFactor = dot( light, input.Normal.xyz );

    float4 diffuse = float4( 0, 0, 0, 0 );
    float4 specular = float4( 0, 0, 0, 0 );

    [flatten]
    if( diffuseFactor > 0.0f ){
        float3 v = reflect( -light, input.Normal.xyz );
        diffuse = diffuseFactor * color;
        float d = dot( v, toEye );
    }

    return ambient + diffuse + specular;

    return color;
}