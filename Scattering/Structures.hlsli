// Vertex shader input format
struct PLANET_VS_INPUT {
    float3 Position : POSITION;
    float3 Normal : NORMAL;
};

// Vertex shader output format
struct PLANET_PS_INPUT {
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
};

// Vertex shader output format
struct ATMOS_PS_INPUT {
    float4 Position : SV_POSITION;
    float4 Ray : TEXCOORD1;
    float4 c0 : TEXCOORD2;
    float4 c1 : TEXCOORD3;
};