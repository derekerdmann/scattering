// Vertex shader input format
struct VS_INPUT {
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float4 Color : COLOR;
};

// Vertex shader output format
struct VS_OUTPUT {
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
    float4 Color : COLOR;
};

float4x4 worldViewProjection;

VS_OUTPUT main( VS_INPUT input ) : POSITION
{
    VS_OUTPUT output;

    output.Position = mul( float4( input.Position, 1.0 ), worldViewProjection );
    output.Normal = float4( input.Normal, 1.0 );
    output.Color = input.Color;

    return output;
}