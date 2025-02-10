#pragma once
#include "Object.h"

class Mesh;
class Material;
class GameObject;

struct MeshRenderInfo
{
	shared_ptr<Mesh>				mesh;
	vector<shared_ptr<Material>>	materials;
};

class MeshData : public Object
{
public:
	MeshData();
	virtual ~MeshData();

public:
	shared_ptr<Mesh> GetMesh() { return _mesh; };
	shared_ptr<Material> GetMaterial(int idx = 0) { return _materials[idx]; };

	static shared_ptr<MeshData> LoadModelFromBinary(const char* path);
	static void LoadFrameHierarchyFromFile(shared_ptr<MeshData>, FILE*);
	//static void LoadFrameHierarchyFromFile(shared_ptr<MeshData> mesh, FILE* file);
	//virtual void Load(const wstring& path);
	//virtual void Save(const wstring& path);

	vector<shared_ptr<GameObject>> Instantiate();

private:
	shared_ptr<Mesh>				_mesh;
	vector<shared_ptr<Material>>	_materials;

	vector<MeshRenderInfo> _meshRenders;
};

