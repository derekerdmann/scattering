// Vertex shader input format
struct VS_INPUT {
    float3 Position : POSITION;
};

// Vertex shader output format
struct PS_INPUT {
    float4 Position : SV_POSITION;
};

// Vertex shader output format
struct ATMOS_PS_INPUT {
    float4 Position : SV_POSITION;
    float4 Pc : TEXCOORD0;
    float4 Ray : TEXCOORD1;
    float4 c0 : TEXCOORD2;
    float4 c1 : TEXCOORD3;
};