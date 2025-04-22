#pragma once
#include "Component.h"
#include "Mesh.h" 

class Material;
class StructuredBuffer;
class AnimatorController;

class Animator : public Component, public enable_shared_from_this<Animator>
{
public:
	Animator();
	virtual ~Animator();

public:
	void SetController(shared_ptr<AnimatorController> controller) { _controller = controller; }
	void SetBones(const vector<BoneInfo>* bones) { _bones = bones; }
	void PushData();

	//int32 GetAnimCount() const { return static_cast<int32>(_animClips->size()); }
	shared_ptr<StructuredBuffer> GetBoneFinalMatrix() const { return _boneFinalMatrix; }
	shared_ptr<StructuredBuffer> GetBoneKeyFrameMatrix() const { return _boneKeyFrameMatrix; }

	void ResetTime() { _updateTime = 0.0f; } // Transition 발생 시 호출

public:
	virtual void FinalUpdate() override;

private:
	const vector<BoneInfo>*			_bones = nullptr;
	//const vector<AnimClipInfo>*		_animClips = nullptr;

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

	shared_ptr<AnimatorController>	_controller;
};
