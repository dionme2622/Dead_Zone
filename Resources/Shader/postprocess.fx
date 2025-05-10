#include "params.fx"

Texture2D inputTexture : register(t0);
SamplerState samplerState : register(s0);

struct VS_TEX_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_TEX_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

VS_TEX_OUT VS_PostProccessing(VS_TEX_IN input)
{
    VS_TEX_OUT output = (VS_TEX_OUT) 0;

    output.pos = mul(float4(input.pos, 1.f), g_matWVP);
    output.uv = input.uv;

    return output;
}

float4 PS_PostProccessing(float4 position : SV_Position, float2 texCoord : TEXCOORD) : SV_Target
{
    float4 color = inputTexture.Sample(samplerState, texCoord);
    return float4(1.0 - color.rgb, color.a); // »ö ¹ÝÀü
}