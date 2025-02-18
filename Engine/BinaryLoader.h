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

struct MeshInfo
{
	char								name[64];
	vector<Vertex>						vertices;
	vector<vector<uint32>>				indices;
	vector<MaterialInfo>				materials;
	shared_ptr<Transform>				transform;
	//Matrix								matrix;
	//Vec3								positions;
};

class BinaryLoader
{
public:
	BinaryLoader();
	~BinaryLoader();

public:
	void LoadModelFromBinary(const char* path);
	void LoadFrameHierarchyFromFile(shared_ptr<Transform>, FILE*);
	void LoadMeshFromFile(MeshInfo&, FILE*);
	void LoadMaterialFromFile(MeshInfo&, FILE*);

public:
	int32 GetMeshCount() { return static_cast<int32>(_meshes.size()); }
	const MeshInfo& GetMesh(int32 idx) { return _meshes[idx]; }

private:
	void LoadMesh(Mesh* mesh);
	void LoadMaterial(Material* surfaceMaterial);


private:
	vector<MeshInfo>					_meshes;
};

