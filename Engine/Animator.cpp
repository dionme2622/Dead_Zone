#include "pch.h"
#include "Animator.h"
#include "Timer.h"
#include "Resources.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "StructuredBuffer.h"

Animator::Animator() : Component(COMPONENT_TYPE::ANIMATOR)
{
	_computeMaterial = GET_SINGLE(Resources)->Get<Material>(L"ComputeAnimation");
	_boneFinalMatrix = make_shared<StructuredBuffer>();
	_bonekeyFrameMatrix = make_shared<StructuredBuffer>();

}

Animator::~Animator()
{
}

void Animator::FinalUpdate()
{
	_updateTime += DELTA_TIME * _speed;

	const AnimClipInfo& animClip = _animClips->at(_clipIndex);  // 몇 번째 클립을 틀고있는지
	if (_updateTime >= animClip.duration)
		_updateTime = 0.0f;

	_frameCount = animClip.frameCount;
	_bonesCount = _bones->size();

	const int32 ratio = static_cast<int32>(animClip.frameCount / animClip.duration);
	_frame = static_cast<int32>(_updateTime * ratio);
	_frame = min(_frame, animClip.frameCount - 1);
	_nextFrame = min(_frame + 1, animClip.frameCount - 1);
	_frameRatio = static_cast<float>(_nextFrame - _frame);		// [질문] _nextFrame을 넣어야하지않나?

}

void Animator::SetAnimClip(const vector<AnimClipInfo>* animClips)
{
	_animClips = animClips;
}

void Animator::PushData()
{
	if (_boneFinalMatrix->GetElementCount() < _bonesCount)
		_boneFinalMatrix->Init(sizeof(Matrix), _bonesCount);

	if (_bonekeyFrameMatrix->GetElementCount() < _bonesCount)
		_bonekeyFrameMatrix->Init(sizeof(Matrix), _bonesCount);

	// Compute Shader
	shared_ptr<Mesh> mesh = GetGameObject()->GetMeshRenderer()->GetMesh();
	mesh->GetBoneFrameDataBuffer(_clipIndex)->PushComputeSRVData(SRV_REGISTER::t8);
	mesh->GetBoneOffsetBuffer()->PushComputeSRVData(SRV_REGISTER::t9);

	_boneFinalMatrix->PushComputeUAVData(UAV_REGISTER::u0);
	_bonekeyFrameMatrix->PushComputeUAVData(UAV_REGISTER::u1);


	_computeMaterial->SetInt(0, _bonesCount);
	_computeMaterial->SetInt(1, _frame);
	_computeMaterial->SetInt(2, _nextFrame);
	_computeMaterial->SetInt(3, _frameCount);
	_computeMaterial->SetFloat(0, _frameRatio);

	uint32 groupCount = (_frameCount / 256) + 1;
	_computeMaterial->Dispatch(groupCount, 1, 1);
	
	// Graphics Shader
	_boneFinalMatrix->PushGraphicsData(SRV_REGISTER::t7);		// 얘를 총 오브젝트에도 넘겨야 함
}

void Animator::Play(uint32 idx)
{
	assert(idx < _animClips->size());
	_clipIndex = idx;
	_updateTime = 0.0f;
}

