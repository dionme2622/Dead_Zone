#pragma once
#include "Object.h"

class Material;
class StructuredBuffer;

struct IndexBufferInfo
{
	ComPtr<ID3D12Resource>		buffer;
	D3D12_INDEX_BUFFER_VIEW		bufferView;
	DXGI_FORMAT					format;
	uint32						count;
};

struct BoneInfo
{
	wstring					boneName;
	Matrix					matOffset;
};

struct KeyFrameInfo
{
	wstring boneName;
	double	time;

	Vec4	scale;
	Vec4	rotation;
	Vec4	translate;
};

struct AnimClipInfo
{
	wstring							animName;
	double							duration;
	int32							frameCount;
	vector<vector<KeyFrameInfo>>	keyFrames;
};

class Mesh : public Object
{
public:
	Mesh();
	virtual ~Mesh();

	void Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexbuffer);
	void Render(uint32 instanceCount = 1, uint32 idx = 0);
	void Render(shared_ptr<class InstancingBuffer>& buffer, uint32 idx = 0);
	static shared_ptr<Mesh> CreateFromBinary(const struct BinaryMeshInfo* meshInfo, class BinaryLoader& loader);


private:
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<uint32>& buffer);
	void CreateBonesAndAnimations(class BinaryLoader& loader);


public:
	uint32 GetSubsetCount() { return static_cast<uint32>(_vecIndexInfo.size()); }
	const vector<BoneInfo>* GetBones() { return &_bones; }
	uint32						GetBoneCount() { return static_cast<uint32>(_bones.size()); }
	const vector<AnimClipInfo>* GetAnimClip() { return &_animClips; }

	bool							IsAnimMesh() { return !_animClips.empty(); }
	shared_ptr<StructuredBuffer>	GetBoneFrameDataBuffer(int32 index = 0) { return _frameBuffer[index]; } // 전체 본 프레임 정보
	shared_ptr<StructuredBuffer>	GetBoneOffsetBuffer() { return  _offsetBuffer; }
private:
	ComPtr<ID3D12Resource>		_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};
	uint32 _vertexCount = 0;

	vector<IndexBufferInfo>		_vecIndexInfo;

	// Animation
	vector<AnimClipInfo>			_animClips;
	vector<BoneInfo>				_bones;

	shared_ptr<StructuredBuffer>	_offsetBuffer; // 각 뼈의 offset 행렬
	vector<shared_ptr<StructuredBuffer>> _frameBuffer; // 전체 본 프레임 정보
};

