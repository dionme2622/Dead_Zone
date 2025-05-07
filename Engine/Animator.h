#pragma once
#include "Component.h"
#include "Mesh.h" 

class Material;
class StructuredBuffer;
class AnimatorController;

class AnimationState;
class Animator : public Component, public enable_shared_from_this<Animator>
{
public:
	Animator(shared_ptr<AnimatorController> controller);
	virtual ~Animator() = default;

	// 파라미터 API
	void SetFloat(const string& name, float value);
	void SetBool(const string& name, bool value);
	void SetTrigger(const string& name);
	void ResetTrigger(const string& name);

public:
	void SetBones(const vector<BoneInfo>* bones) { _bones = bones; }
	void PushData();

	//int32 GetAnimCount() const { return static_cast<int32>(_animClips->size()); }
	shared_ptr<StructuredBuffer> GetBoneFinalMatrix() const { return _boneFinalMatrix; }
	shared_ptr<StructuredBuffer> GetBoneKeyFrameMatrix() const { return _boneKeyFrameMatrix; }
	shared_ptr<AnimatorController> GetAnimatorController() const { return _controller; }

	void ResetTime() { _updateTime = 0.0f; } // Transition 발생 시 호출

public:
	virtual void FinalUpdate() override;


private:
	const vector<BoneInfo>*			_bones = nullptr;

	float							_updateTime = 0.0f;

	int32							_frame = 0;
	int32							_nextFrame = 0;
	int32							_frameCount = 0;
	float							_frameRatio = 0.0f;

	float							_speed = 1.0f;
	bool							_loop = true;
	int32							_bonesCount = 0;

	shared_ptr<Material>			_computeMaterial;
	shared_ptr<StructuredBuffer>	_boneFinalMatrix;
	shared_ptr<StructuredBuffer>	_boneKeyFrameMatrix;

	// 컨트롤러 에셋
	shared_ptr<AnimatorController> _controller;
	// 런타임 파라미터 값
	vector<float> _floatParams;
	vector<bool>  _boolParams;
	vector<bool>  _triggerParams;

	// 스테이트머신 상태
	shared_ptr<AnimationState> _currentState;
	shared_ptr<AnimationState> _nextState;
	float _stateTime = 0.0f;
	float _transitionTime = 0.0f;
	bool  _inTransition = false;

	void EvaluateTransitions();
	void AdvanceTime();
	void ComputeFrameValues();
};
