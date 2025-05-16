#ifndef _UTILS_FX_
#define _UTILS_FX_

#include "params.fx"

LightColor CalculateLightColor(int lightIndex, float3 viewNormal, float3 viewPos)
{
    LightColor color = (LightColor) 0.f;

    float3 viewLightDir = (float3) 0.f;

    float diffuseRatio = 0.f;
    float specularRatio = 0.f;
    float distanceRatio = 1.f;
    float volumetricRatio = 0.f; // 볼류메트릭 효과를 위한 세기 (상단에서 선언)

    // 메탈릭 값 가져오기 (0.0 ~ 1.0)
    float metallic = g_float_2;
    
    metallic = (metallic < 0.0f || metallic > 1.0f) ? 0.0f : metallic; // 기본값 0.1

    if (g_light[lightIndex].lightType == 0)
    {
        // Directional Light
        viewLightDir = normalize(mul(float4(g_light[lightIndex].direction.xyz, 0.f), g_matView).xyz);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));
    }
    else if (g_light[lightIndex].lightType == 1)
    {
        // Point Light
        float3 viewLightPos = mul(float4(g_light[lightIndex].position.xyz, 1.f), g_matView).xyz;
        viewLightDir = normalize(viewPos - viewLightPos);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));

        float dist = distance(viewPos, viewLightPos);
        if (g_light[lightIndex].range == 0.f)
            distanceRatio = 0.f;
        else
            distanceRatio = saturate(1.f - pow(dist / g_light[lightIndex].range, 2));
    }
    else
    {
        // Spot Light (lightType == 2)
        float3 viewLightPos = mul(float4(g_light[lightIndex].position.xyz, 1.f), g_matView).xyz;
        viewLightDir = normalize(viewPos - viewLightPos);
        diffuseRatio = saturate(dot(-viewLightDir, viewNormal));

        if (diffuseRatio > 0.f)
        {
            float3 viewDir = normalize(-viewPos);
            float3 halfVector = normalize(viewLightDir + viewDir);
            specularRatio = pow(saturate(dot(viewNormal, halfVector)), 16.0f);
        }

        if (g_light[lightIndex].range == 0.f)   
            distanceRatio = 0.f;
        else
        {
            // 스포트라이트 방향과 거리 계산
            float3 viewLightVec = viewPos - viewLightPos;
            float3 viewCenterLightDir = normalize(mul(float4(g_light[lightIndex].direction.xyz, 0.f), g_matView).xyz);
            float centerDist = dot(viewLightVec, viewCenterLightDir);

            // 내/외부 콘 각도 설정
            float outerAngle = g_light[lightIndex].angle / 2; // 외부 콘 반각
            float innerAngle = outerAngle * 0.6f; // 내부 콘 반각 (60%로 조정, 감쇠 구간 확장)
            
            // 각도 계산
            float lightAngle = acos(dot(normalize(viewLightVec), viewCenterLightDir));
            
            // 부드러운 각도 감쇠 (비선형)
            float angleAttenuation = 0.f;
            if (lightAngle < innerAngle)
                angleAttenuation = 1.0f;
            else if (lightAngle < outerAngle)
            {
                float t = (outerAngle - lightAngle) / (outerAngle - innerAngle);
                angleAttenuation = saturate(pow(t, 3.0f)); // 더 부드러운 제곱 감쇠
            }
            else
                angleAttenuation = 0.f;

            // 부드러운 거리 감쇠 (비선형)
            distanceRatio = 0.f;
            if (centerDist >= 0.f && centerDist <= g_light[lightIndex].range)
            {
                float distRatio = centerDist / g_light[lightIndex].range;
                distanceRatio = saturate(pow(1.f - distRatio, 2.0f)); // 지수 기반 감쇠
                
                // 볼류메트릭 효과: 원뿔 내부의 세기 계산
                volumetricRatio = angleAttenuation * distanceRatio * 1.0f; // 세기 강화
            }

            // 최종 감쇠 계산
            distanceRatio *= angleAttenuation;
        }
    }

    // 스페큘러 계산
    float3 reflectionDir = normalize(viewLightDir + 2 * (saturate(dot(-viewLightDir, viewNormal)) * viewNormal));
    float3 eyeDir = normalize(viewPos);
    specularRatio = saturate(dot(-eyeDir, reflectionDir));
    specularRatio = pow(specularRatio, 2);

    // 메탈릭에 따라 디퓨즈와 스페큘러 조정
    float diffuseFactor = 1.0f - metallic;
    float specularFactor = 1.0f + metallic;

    color.diffuse = g_light[lightIndex].color.diffuse * diffuseRatio * distanceRatio * diffuseFactor;
    color.ambient = g_light[lightIndex].color.ambient * distanceRatio;
    color.specular = g_light[lightIndex].color.specular * specularRatio * distanceRatio * specularFactor;

    // 볼류메트릭 효과 추가 (디퓨즈에 혼합)
    if (g_light[lightIndex].lightType == 2) // 스포트라이트에만 적용
        color.diffuse += g_light[lightIndex].color.diffuse * volumetricRatio * 0.5f; // 경계 흐림 효과

    return color;
}

float Rand(float2 co)
{
    return 0.5 + (frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453)) * 0.5;
}
struct SkinningInfo
{
    float3 pos;
    float3 normal;
    float3 tangent;
};

void Skinning(inout float3 pos, inout float3 normal, inout float3 tangent,
    inout float4 weight, inout float4 indices)
{
    SkinningInfo info = (SkinningInfo) 0.f;

    for (int i = 0; i < 4; ++i)
    {
        if (weight[i] == 0.f)
            continue;

        int boneIdx = indices[i];
        matrix matBone = g_mat_bone[boneIdx];
        
        info.pos += (mul(float4(pos, 1.f), matBone) * weight[i]).xyz;
        info.normal += (mul(float4(normal, 0.f), matBone) * weight[i]).xyz;
        info.tangent += (mul(float4(tangent, 0.f), matBone) * weight[i]).xyz;
    }

    pos = info.pos;
    tangent = normalize(info.tangent);
    normal = normalize(info.normal);
}

#endif