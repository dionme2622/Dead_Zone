#pragma once
#include "Component.h"
#include "Mesh.h"

class Material;
class StructuredBuffer;
class Mesh;

class Animator : public Component
{
public:
	Animator();
	virtual ~Animator();

public:
	void SetBones(const vector<BoneInfo>* bones) { _bones = bones; }
	void SetAnimClip(const vector<AnimClipInfo>* animClips);
	void PushData();

	int32 GetAnimCount() { return static_cast<uint32>(_animClips->size()); }
	int32 GetCurrentClipIndex() { return _clipIndex; }
	shared_ptr<StructuredBuffer> GetBoneFinalMatirx() { return _boneFinalMatrix; }
	shared_ptr<StructuredBuffer> GetBoneKeyFrameMatirx() { return _bonekeyFrameMatrix; }

	void Play(uint32 idx);

public:
	virtual void FinalUpdate() override;

private:
	const vector<BoneInfo>*			_bones;
	const vector<AnimClipInfo>*		_animClips;

	float							_updateTime = 0.0f;
	int32							_clipIndex = 0;
	int32							_frame = 0;
	int32							_frameCount = 0;
	int32							_nextFrame = 0;
	float							_frameRatio = 0;

	float							_speed = 1.0f;

	int32							_bonesCount = 0;
	Matrix							_boneMatrix;

	shared_ptr<Material>			_computeMaterial;

	shared_ptr<StructuredBuffer>	_boneFinalMatrix;  // Ư�� �������� ���� ���
	shared_ptr<StructuredBuffer>	_bonekeyFrameMatrix;  // �� �������� ���� ���

	bool							_boneFinalUpdated = false;
};
