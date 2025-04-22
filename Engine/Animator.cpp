#include "pch.h"
#include "Animator.h"
#include "Timer.h"
#include "Resources.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "StructuredBuffer.h"
#include "AnimatorController.h"
#include "AnimationState.h"

Animator::Animator() : Component(COMPONENT_TYPE::ANIMATOR)
{
	_computeMaterial = GET_SINGLE(Resources)->Get<Material>(L"ComputeAnimation");
	_boneFinalMatrix = make_shared<StructuredBuffer>();
	_boneKeyFrameMatrix = make_shared<StructuredBuffer>();

	_controller = GET_SINGLE(Resources)->LoadAnimatorController();
}

Animator::~Animator()
{
}

void Animator::FinalUpdate()
{
	if (_controller == nullptr || _bones == nullptr )
		return;

	// 1. 상태 전이 갱신
	//_controller->Update(shared_from_this());

	auto state = _controller->GetCurrentState();				// 컨트롤러의 현재 State를 가져온다
	auto currentClip = _controller->GetCurrentClip();			// 컨트롤러의 현재 State의 애니메이션 클립을 가져온다
	if (state)
	{
		_speed = state->GetSpeed();
		_loop = state->IsLooping();  // bool _loop 멤버 추가해주면 좋아

		// 그리고 아래처럼 애니메이션 시간을 조절해
		_updateTime += DELTA_TIME * _speed;

		
		if (_loop)
		{
			if (_updateTime >= currentClip->duration)
				_updateTime = 0.f;
		}
		else
		{
			_updateTime = min(_updateTime, currentClip->duration);
		}
	}

	// 4. 프레임 계산
	_frameCount = currentClip->frameCount;
	_bonesCount = static_cast<int32>(_bones->size());

	float frameDuration = currentClip->duration / static_cast<float>(_frameCount);
	_frame = static_cast<int32>(_updateTime / frameDuration);
	_nextFrame = min(_frame + 1, _frameCount - 1);
	_frameRatio = (_updateTime - (_frame * frameDuration)) / frameDuration;
}


void Animator::PushData()
{
	if (_boneFinalMatrix->GetElementCount() < _bonesCount)
		_boneFinalMatrix->Init(sizeof(Matrix), _bonesCount);

	if (_boneKeyFrameMatrix->GetElementCount() < _bonesCount)
		_boneKeyFrameMatrix->Init(sizeof(Matrix), _bonesCount);


	// Compute Shader에 클립 데이터 전송
	auto currentClipIndex = _controller->GetCurrentClipIndex();									// 컨트롤러의 현재 State의 애니메이션 클립 Index 값을 가져온다
	shared_ptr<Mesh> mesh = GetGameObject()->GetMeshRenderer()->GetMesh();
	mesh->GetBoneFrameDataBuffer(currentClipIndex)->PushComputeSRVData(SRV_REGISTER::t8);		// 컨트롤러의 현재 State의 몇 번째 애니메이션 클립인지 보내야 한다.
	mesh->GetBoneOffsetBuffer()->PushComputeSRVData(SRV_REGISTER::t9);						// 그대로 유지

	_boneFinalMatrix->PushComputeUAVData(UAV_REGISTER::u0);
	_boneKeyFrameMatrix->PushComputeUAVData(UAV_REGISTER::u1);

	// 파라미터 설정
	_computeMaterial->SetInt(0, _bonesCount);
	_computeMaterial->SetInt(1, _frame);
	_computeMaterial->SetInt(2, _nextFrame);
	_computeMaterial->SetInt(3, _frameCount);
	_computeMaterial->SetFloat(0, _frameRatio);

	uint32 groupCount = (_frameCount / 256) + 1;
	_computeMaterial->Dispatch(groupCount, 1, 1);

	// 그래픽 셰이더용 데이터 전달
	_boneFinalMatrix->PushGraphicsData(SRV_REGISTER::t7);
}

