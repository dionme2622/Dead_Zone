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
	int32					parentIdx;
	Matrix					matOffset;
};

struct KeyFrameInfo
{
	double	time;
	int32	frame;
	Vec3	scale;
	Vec4	rotation;
	Vec3	translate;
};

struct AnimClipInfo
{
	wstring							animName;
	int32							frameCount;
	double							duration;
	vector<vector<KeyFrameInfo>>	keyFrames;
};

class Mesh : public Object
{
public:
	Mesh();
	virtual ~Mesh();

	void Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexbuffer);
	void Render(uint32 instanceCount = 1, uint32 idx = 0);
	//void Render(shared_ptr<class InstancingBuffer>& buffer, uint32 idx = 0);
	static shared_ptr<Mesh> CreateFromBinary(const struct BinaryMeshInfo* meshInfo, class BinaryLoader& loader);


private:
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<uint32>& buffer);


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

