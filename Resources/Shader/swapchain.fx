#ifndef _SWAPCHAIN_FX_
#define _SWAPCHAIN_FX_

#include "params.fx"

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};


VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.pos = mul(float4(input.pos, 1.f), g_matWVP);
    output.uv = input.uv;

    return output;
}


float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    if (g_tex_on_0)
        color = g_tex_0.Sample(g_sam_0, input.uv);
    
    //return color;
    return 0;
}





RWTexture2D<float4> g_Output : register(u0);
Texture2D g_Input : register(t0);
SamplerState g_Sampler : register(s0);

[numthreads(16, 16, 1)]
void CS_Main(uint3 DTid : SV_DispatchThreadID)
{
    float4 color = float4(0, 0, 0, 0);
    float weight[5] = { 0.227027, 0.194594, 0.121621, 0.054054, 0.016216 };

    for (int i = -2; i <= 2; i++)
    {
        color += g_Input.Load(int3(DTid.xy + int2(i, 0), 0)) * weight[i + 2];
    }
    
    g_Output[DTid.xy] = color;
}

#endif