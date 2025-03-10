#pragma once
#include "Object.h"

class Mesh;
class Material;
class GameObject;
class Transform;

struct MeshRenderInfo
{
	wstring							objName;
	shared_ptr<Mesh>				mesh;
	vector<shared_ptr<Material>>	materials;
	shared_ptr<Transform>			transform;
};

class MeshData : public Object
{
public:
	MeshData();
	virtual ~MeshData();

public:
	
	vector<MeshRenderInfo> GetMeshRenderInfo() { return _meshRenders; };

	static shared_ptr<MeshData> LoadModelFromBinary(const char* path);
	vector<shared_ptr<GameObject>> Instantiate();

private:
	vector<MeshRenderInfo> _meshRenders;
};

