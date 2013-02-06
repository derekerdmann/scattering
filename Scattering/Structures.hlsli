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