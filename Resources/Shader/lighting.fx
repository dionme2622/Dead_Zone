#ifndef _LIGHTING_FX_
#define _LIGHTING_FX_

#include "params.fx"
#include "utils.fx"

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

struct PS_OUT
{
    float4 diffuse : SV_Target0;
    float4 specular : SV_Target1;
};

// [Directional Light]
// g_int_0 : Light index
// g_tex_0 : Position RT
// g_tex_1 : Normal RT
// g_tex_2 : Shadow RT
// g_mat_0 : ShadowCamera VP
// Mesh : Rectangle




VS_OUT VS_DirLight(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.pos = float4(input.pos * 2.f, 1.f);
    output.uv = input.uv;

    return output;
}

PS_OUT PS_DirLight(VS_OUT input)
{
    PS_OUT output = (PS_OUT) 0;

    float3 viewPos = g_tex_0.Sample(g_sam_0, input.uv).xyz;
    if (viewPos.z <= 0.f)
        clip(-1);

    float3 viewNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;

    LightColor color = CalculateLightColor(g_int_0, viewNormal, viewPos);

    // 그림자
    if (length(color.diffuse) != 0)
    {
        matrix shadowCameraVP = g_mat_0;

        float4 worldPos = mul(float4(viewPos.xyz, 1.f), g_matViewInv);
        float4 shadowClipPos = mul(worldPos, shadowCameraVP);
        float depth = shadowClipPos.z;

        // x [-1 ~ 1] -> u [0 ~ 1]
        // y [1 ~ -1] -> v [0 ~ 1]
        float2 uv = shadowClipPos.xy;
        uv.y = -uv.y;
        uv = uv * 0.5 + 0.5;

        if (0 < uv.x && uv.x < 1 && 0 < uv.y && uv.y < 1)
        {
            float shadowDepth = g_tex_2.Sample(g_sam_0, uv).x;
            if (shadowDepth > 0 && depth > shadowDepth + 0.00001f)
            {
                color.diffuse *= 0.5f;
                color.specular = (float4) 0.f;
            }
        }
    }

    output.diffuse = color.diffuse + color.ambient;
    output.specular = color.specular;

    return output;
}

// [Point Light]
// g_int_0 : Light index
// g_tex_0 : Position RT
// g_tex_1 : Normal RT
// g_vec2_0 : RenderTarget Resolution
// Mesh : Sphere

VS_OUT VS_PointLight(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.pos = mul(float4(input.pos, 1.f), g_matWVP);
    output.uv = input.uv;

    return output;
}

PS_OUT PS_PointLight(VS_OUT input)
{
    PS_OUT output = (PS_OUT) 0;

    // input.pos = SV_Position = Screen 좌표
    float2 uv = float2(input.pos.x / g_vec2_0.x, input.pos.y / g_vec2_0.y);
    float3 viewPos = g_tex_0.Sample(g_sam_0, uv).xyz;
    if (viewPos.z <= 0.f)
        clip(-1);

    int lightIndex = g_int_0;
    float3 viewLightPos = mul(float4(g_light[lightIndex].position.xyz, 1.f), g_matView).xyz;
    float distance = length(viewPos - viewLightPos);
    if (distance > g_light[lightIndex].range)
        clip(-1);

    float3 viewNormal = g_tex_1.Sample(g_sam_0, uv).xyz;

    LightColor color = CalculateLightColor(g_int_0, viewNormal, viewPos);

    output.diffuse = color.diffuse + color.ambient;
    output.specular = color.specular;

    return output;
}

//// SSAO 패스
//static const int SSAO_KERNEL_SIZE = 16;
//static const float SSAO_RADIUS = 0.5f;
//static const float SSAO_BIAS = 0.025f;
//static const float SSAO_POWER = 2.0f;

//static const float3 SSAO_KERNEL[16];

VS_OUT VS_SSAO(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    output.pos = float4(input.pos * 2.f, 1.f);
    output.uv = input.uv;
    return output;
}

// SSAO 패스
static const int SSAO_KERNEL_SIZE = 16;
static const float SSAO_RADIUS = 1.0f; // 장면 크기에 맞게 조정
static const float SSAO_BIAS = 0.05f; // 깊이 범위에 맞게 조정
static const float SSAO_POWER = 1.5f; // 효과 강도 조정

static float3 SSAO_KERNEL[16] =
{
    float3(0.5381, 0.1856, -0.4319),
    float3(0.1379, 0.2486, 0.4430),
    float3(0.3371, 0.5679, -0.0057),
    float3(-0.6999, -0.0451, -0.0019),
    float3(0.0689, -0.1598, -0.8547),
    float3(0.0560, 0.0069, -0.1843),
    float3(-0.0146, 0.1402, 0.0762),
    float3(0.0100, -0.1924, -0.0344),
    float3(-0.3577, -0.5301, -0.4358),
    float3(-0.3169, 0.1063, 0.0158),
    float3(0.0103, -0.5869, 0.0046),
    float3(-0.0897, -0.4940, 0.3287),
    float3(0.7119, -0.0154, -0.0918),
    float3(-0.0533, 0.0596, -0.5411),
    float3(0.0352, -0.0632, 0.5460),
    float3(-0.4776, 0.2847, -0.0271)
};

float4 PS_SSAO(VS_OUT input) : SV_Target
{
    float3 viewPos = g_tex_0.Sample(g_sam_0, input.uv).xyz;
    if (viewPos.z <= 0.f)
        return float4(1.0f, 1.0f, 1.0f, 0.0f);

    float3 viewNormal = normalize(g_tex_1.Sample(g_sam_0, input.uv).xyz);

    float2 noiseUV = input.uv * g_vec2_0 / 16.0;
    float2 randomVec = g_tex_2.Sample(g_sam_0, noiseUV).xy;
    randomVec = randomVec * 2.0 - 1.0;
    float3 randomVec3 = float3(randomVec, 0.0);
    float3 tangent = normalize(randomVec3 - viewNormal * dot(randomVec3, viewNormal));
    float3 bitangent = cross(viewNormal, tangent);
    float3x3 TBN = float3x3(tangent, bitangent, viewNormal);

    
    
    float occlusion = 0.0;
    for (int i = 0; i < SSAO_KERNEL_SIZE; ++i)
    {
        float3 samplePos = viewPos + mul(SSAO_KERNEL[i], TBN) * SSAO_RADIUS;
        float4 offset = float4(samplePos, 1.0);
        offset = mul(offset, g_mat_1); // g_mat_0 -> g_mat_1으로 변경
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;

        float sampleDepth = g_tex_0.Sample(g_sam_0, offset.xy).z;
        float rangeCheck = smoothstep(0.0, 1.0, SSAO_RADIUS / abs(viewPos.z - sampleDepth));
        occlusion += (sampleDepth <= samplePos.z + SSAO_BIAS ? 1.0 : 0.0) * rangeCheck;
    }
    occlusion = 1.0 - (occlusion / SSAO_KERNEL_SIZE);
    float finalOcclusion = pow(occlusion, SSAO_POWER);
    return float4(finalOcclusion, finalOcclusion, finalOcclusion, 1.0f);
}


// [Final]
// g_tex_0 : Diffuse Color Target
// g_tex_1 : Diffuse Light Target
// g_tex_2 : Specular Light Target
// Mesh : Rectangle

VS_OUT VS_Final(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.pos = float4(input.pos * 2.f, 1.f);
    output.uv = input.uv;

    return output;
}

float4 PS_Final(VS_OUT input) : SV_Target
{
    float4 output = (float4) 0;

    float4 lightPower = g_tex_1.Sample(g_sam_0, input.uv);
    if (lightPower.x == 0.f && lightPower.y == 0.f && lightPower.z == 0.f)
        clip(-1);

    float4 color = g_tex_0.Sample(g_sam_0, input.uv);
    float4 specular = g_tex_2.Sample(g_sam_0, input.uv);

    output = (color * lightPower) + specular;
    return output;
    
    
    
    //float4 output = (float4) 0;

    //float4 lightPower = g_tex_1.Sample(g_sam_0, input.uv);
    //if (lightPower.x == 0.f && lightPower.y == 0.f && lightPower.z == 0.f)
    //    clip(-1);

    //float4 color = g_tex_0.Sample(g_sam_0, input.uv);
    //float4 specular = g_tex_2.Sample(g_sam_0, input.uv);
    //float occlusion = g_tex_3.Sample(g_sam_0, input.uv).r;

    //output = (color * lightPower * occlusion) + specular;
    //return output;
}

#endif