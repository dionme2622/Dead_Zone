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
#include "KeyInput.h"

Animator::Animator(shared_ptr<AnimatorController> controller) : Component(COMPONENT_TYPE::ANIMATOR), _controller(controller)
{
	// ��Ʈ�ѷ��� ���ǵ� �Ķ���� ������ŭ ���� �ʱ�ȭ
	int paramCount = (int)_controller->GetParamDefs().size();
	_floatParams.assign(paramCount, 0.0f);
	_boolParams.assign(paramCount, false);
	_triggerParams.assign(paramCount, false);

	// �⺻�� ä���
	for (int i = 0; i < paramCount; ++i) {
		const auto& def = _controller->GetParamDefs()[i];
		_floatParams[i] = def.defaultValue;
		if (def.type == ParameterType::Bool)
			_boolParams[i] = (def.defaultValue != 0.0f);
	}

	// �ʱ� ������Ʈ
	_currentState = _controller->GetEntryState();

	// GPU ��Ű�� ���ҽ�
	_computeMaterial = GET_SINGLE(Resources)->Get<Material>(L"ComputeAnimation");
	_boneFinalMatrix = make_shared<StructuredBuffer>();
	_boneKeyFrameMatrix = make_shared<StructuredBuffer>();
}



void Animator::FinalUpdate()
{
	if (!_controller)
		return;

	if (INPUT->GetButton(KEY_TYPE::Q))
		_controller->SetCurrentState(L"Walk");
	if (INPUT->GetButton(KEY_TYPE::E))
		_controller->SetCurrentState(L"Idle");


	

	
	// 1) ���� ��
	EvaluateTransitions();
	// 2) �ð� ���� �� ���� ó��
	AdvanceTime();
	// 3) ������ �ε��� ���
	ComputeFrameValues();
}

void Animator::EvaluateTransitions()
{
	// ���� ������Ʈ ����
	for (auto& t : _currentState->GetTransitions()) {
		if (t->IsConditionMet(_floatParams, _boolParams, _triggerParams)) {
			_nextState = t->GetTarget();
			_inTransition = true;
			_transitionTime = 0.0f;

			// <-- Ʈ���� �Ķ���Ͱ� �ִٸ� �Һ�(Reset)�ϱ�
			for (auto& cond : t->GetConditions())
			{
				if (_controller->GetParamDefs()[cond.paramIndex].type == ParameterType::Trigger)
				{
					_triggerParams[cond.paramIndex] = false;
				}
			}

			return;
		}
	}
	// Any-State ����
	auto anyState = _controller->GetAnyState();
	if (anyState) {
		for (auto& t : anyState->GetTransitions()) {
			if (t->IsConditionMet(_floatParams, _boolParams, _triggerParams)) {
				_nextState = t->GetTarget();
				_inTransition = true;
				_transitionTime = 0.0f;
				return;
			}
		}
	}
}

void Animator::AdvanceTime()
{
	float dt = DELTA_TIME * _currentState->GetSpeed();
	_stateTime += dt;

	
	auto clip = _currentState->GetClip();
	if (_currentState->IsLooping()) {
		if (_stateTime >= clip->duration)
			_stateTime = fmodf(_stateTime, clip->duration);
	}
	else {
		_stateTime = (_stateTime < clip->duration) ? _stateTime : clip->duration;
	}

	if (_inTransition) {
		_transitionTime += dt;
		float dur =  /* transition duration �������� */ 0.f;
		if (_transitionTime >= dur) {
			_inTransition = false;
			_currentState = _nextState;
			_stateTime = _transitionTime - dur;
		}
	}

}

void Animator::ComputeFrameValues()
{
	auto clip = _currentState->GetClip();
	_frameCount = clip->frameCount;
	_bonesCount = static_cast<int>(_bones->size());

	float frameDuration = clip->duration / static_cast<float>(_frameCount);
	_frame = ((_stateTime / frameDuration) < (_frameCount - 1)) ? static_cast<int>(_stateTime / frameDuration) : (_frameCount - 1);
	_nextFrame = (_frame + 1 < _frameCount) ? _frame + 1 : _frame;
	_frameRatio = (_stateTime - (_frame * frameDuration)) / frameDuration;
}


void Animator::PushData()
{
	if (_boneFinalMatrix->GetElementCount() < _bonesCount)
		_boneFinalMatrix->Init(sizeof(Matrix), _bonesCount);

	if (_boneKeyFrameMatrix->GetElementCount() < _bonesCount)
		_boneKeyFrameMatrix->Init(sizeof(Matrix), _bonesCount);


	// Compute Shader�� Ŭ�� ������ ����
	auto currentClipIndex = _currentState->GetClipIndex();					// ��Ʈ�ѷ��� ���� State�� �ִϸ��̼� Ŭ�� Index ���� �����´�
	shared_ptr<Mesh> mesh = GetGameObject()->GetMeshRenderer()->GetMesh();
	mesh->GetBoneFrameDataBuffer(currentClipIndex)->PushComputeSRVData(SRV_REGISTER::t8);					// ��Ʈ�ѷ��� ���� State�� �� ��° �ִϸ��̼� Ŭ������ ������ �Ѵ�.
	mesh->GetBoneOffsetBuffer()->PushComputeSRVData(SRV_REGISTER::t9);						// �״�� ����

	_boneFinalMatrix->PushComputeUAVData(UAV_REGISTER::u0);
	_boneKeyFrameMatrix->PushComputeUAVData(UAV_REGISTER::u1);

	// �Ķ���� ����
	_computeMaterial->SetInt(0, _bonesCount);
	_computeMaterial->SetInt(1, _frame);
	_computeMaterial->SetInt(2, _nextFrame);
	_computeMaterial->SetInt(3, _frameCount);
	_computeMaterial->SetFloat(0, _frameRatio);

	uint32 groupCount = (_frameCount / 256) + 1;
	_computeMaterial->Dispatch(groupCount, 1, 1);

	// �׷��� ���̴��� ������ ����
	_boneFinalMatrix->PushGraphicsData(SRV_REGISTER::t7);
}

void Animator::SetFloat(const string& name, float value)
{
	int idx = _controller->GetParamIndex(name);
	if (idx >= 0) _floatParams[idx] = value;
}

void Animator::SetBool(const string& name, bool value)
{
	int idx = _controller->GetParamIndex(name);
	if (idx >= 0) _boolParams[idx] = value;
}

void Animator::SetTrigger(const string& name)
{
	int idx = _controller->GetParamIndex(name);
	if (idx >= 0) _triggerParams[idx] = true;
}

void Animator::ResetTrigger(const string& name)
{
	int idx = _controller->GetParamIndex(name);
	if (idx >= 0) _triggerParams[idx] = false;
}