// Vertex shader input format
struct VS_INPUT {
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float4 Color : COLOR;
};

// Vertex shader output format
struct PS_INPUT {
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
    float4 Color : COLOR;
};

// Vertex shader output format
struct ATMOS_VS_OUTPUT {
    float4 Position : SV_POSITION;
    float4 Pc : TEXCOORD;
    float4 Color : COLOR;
};

// Vertex shader output format
struct ATMOS_PS_INPUT {
    float4 Position : SV_POSITION;
    float4 Pc : TEXCOORD;
    float4 Color : COLOR;
};