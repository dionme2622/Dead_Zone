#pragma once

class Mesh;
class Material;
class Transform;

struct MaterialInfo
{
	Vec4			albedo;
	Vec4			emissive;
	Vec4			specular;
	float			glossiness;
	float			smoothness;
	float			metalic;
	float			specularhighlight;
	float			glossyreflection;

	wstring			name;
	wstring			albedoTexName;
	wstring			normalTexName;
	wstring			specularTexName;
};

struct BoneWeight
{
	using Pair = pair<int32, double>;
	vector<Pair> boneWeights;

	void AddWeights(uint32 index, double weight)
	{
		if (weight <= 0.f)
			return;

		auto findIt = std::find_if(boneWeights.begin(), boneWeights.end(),
			[=](const Pair& p) { return p.second < weight; });

		if (findIt != boneWeights.end())
			boneWeights.insert(findIt, Pair(index, weight));
		else
			boneWeights.push_back(Pair(index, weight));

		// 가중치는 최대 4개
		if (boneWeights.size() > 4)
			boneWeights.pop_back();
	}

	void Normalize()
	{
		double sum = 0.f;
		std::for_each(boneWeights.begin(), boneWeights.end(), [&](Pair& p) { sum += p.second; });
		std::for_each(boneWeights.begin(), boneWeights.end(), [=](Pair& p) { p.second = p.second / sum; });
	}
};

struct BinaryMeshInfo
{
	wstring								objName;
	wstring								meshName;
	vector<Vertex>						vertices;
	vector<vector<uint32>>				indices;
	Vec3								AABBCenter;
	Vec3								AABBExtents;
	vector<MaterialInfo>				materials;
	shared_ptr<Transform>				transform;
	vector<BoneWeight>					boneWeights; // 뼈 가중치
	bool								hasMesh = false;
	bool								hasAnimation = false;
};

struct BinaryBoneInfo
{
	wstring					boneName;
	Matrix					matOffset;
};

struct BinaryKeyFrameInfo
{
	wstring		boneName;
	Matrix		matTransform;
	double		time;
};

struct BinaryAnimClipInfo
{
	wstring								name;
	double								duration;
	int32								frameCount;
	vector<vector<BinaryKeyFrameInfo>>	keyFrames;
};

class BinaryLoader
{
public:
	BinaryLoader();
	~BinaryLoader();

public:
	void LoadModelFromBinary(const char* path);
	void LoadFrameHierarchyFromFile(shared_ptr<Transform>, FILE*);
	void LoadMeshFromFile(BinaryMeshInfo&, FILE*);
	void LoadMaterialFromFile(BinaryMeshInfo&, FILE*);
	void LoadSkinInfoFromFile(BinaryMeshInfo&, FILE*);
	void LoadAnimationFromFile(FILE*);

	void CreateTextures();
	void CreateMaterials();

public:
	int32 GetMeshCount() { return static_cast<int32>(_meshes.size()); }
	const BinaryMeshInfo& GetMesh(int32 idx) { return _meshes[idx]; }

	vector<shared_ptr<BinaryBoneInfo>>& GetBones() { return _bones; }
	vector<shared_ptr<BinaryAnimClipInfo>>& GetAnimClip() { return _animClips; }
private:
	void LoadMesh(Mesh* mesh);
	void LoadMaterial(Material* surfaceMaterial);

private:
	vector<BinaryMeshInfo>					_meshes;
	vector<shared_ptr<BinaryBoneInfo>>		_bones;
	vector<shared_ptr<BinaryAnimClipInfo>>	_animClips;


};

