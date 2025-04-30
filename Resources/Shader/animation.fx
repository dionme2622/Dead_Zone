#ifndef _ANIMATION_FX_
#define _ANIMATION_FX_

#include "params.fx"
#include "utils.fx"
#include "math.fx"

struct AnimFrameParams
{
    float4 scale;
    float4 rotation;
    float4 translation;
};

StructuredBuffer<AnimFrameParams> g_bone_frame : register(t8);
StructuredBuffer<matrix> g_offset : register(t9);
RWStructuredBuffer<matrix> g_final : register(u0);
RWStructuredBuffer<matrix> g_matBone : register(u1);

// ComputeAnimation
// g_int_0 : BoneCount
// g_int_1 : CurrentFrame
// g_int_2 : NextFrame
// g_float_0 : Ratio
[numthreads(256, 1, 1)]
void CS_Main(int3 threadIdx : SV_DispatchThreadID)
{
    if (g_int_0 <= threadIdx.x)     // 0 ~ bone의 개수 까지만 계산한다, threadIdx.x 은 bone의 Index라고 생각
        return;

    int boneCount = g_int_0;
    int currentFrame = g_int_1;
    int nextFrame = g_int_2;
    int frameCount = g_int_3;
    float ratio = g_float_0;
    
    // 0번 프레임일 때 /
    uint idx = currentFrame + (frameCount * threadIdx.x); // 0   71   142  ...  // 1번 뼈, 2번 뼈, 3번 뼈, ...
    uint nextIdx = nextFrame + (frameCount * threadIdx.x); // 1   72   143  ... 

    float4 quaternionZero = float4(0.f, 0.f, 0.f, 1.f);

    float4 scale = lerp(g_bone_frame[idx].scale, g_bone_frame[nextIdx].scale, ratio);
    float4 rotation = QuaternionSlerp(g_bone_frame[idx].rotation, g_bone_frame[nextIdx].rotation, ratio);
    float4 translation = lerp(g_bone_frame[idx].translation, g_bone_frame[nextIdx].translation, ratio);
    
    g_matBone[threadIdx.x] = MatrixAffineTransformation(scale, quaternionZero, rotation, translation); // Animation KeyFrame의 bone 행렬
    
    
    g_final[threadIdx.x] = mul(g_offset[threadIdx.x], g_matBone[threadIdx.x]);

}

#endif