#pragma once
#include "Object.h"

//enum
//{
//	OBJECT = 0, PLAYER = 1, ZOMBIE = 2
//};

class Mesh;
class Material;
class GameObject;
class Transform;
class BoxCollider;
class MeshCollider;

struct MeshRenderInfo
{
	wstring							objName;
	shared_ptr<Mesh>				mesh;
	vector<shared_ptr<Material>>	materials;
	shared_ptr<Transform>			transform;
	shared_ptr<BoxCollider>			boxCollider;
	shared_ptr<MeshCollider>		meshCollider;
};

class MeshData : public Object
{
public:
	MeshData();
	virtual ~MeshData();

public:
	
	vector<MeshRenderInfo> GetMeshRenderInfo() { return _meshRenders; };

	static shared_ptr<MeshData> LoadModelFromBinary(const char* path, int type);
	vector<shared_ptr<GameObject>> Instantiate(int type = OBJECT, int collidertype = NONE);

private:
	vector<MeshRenderInfo> _meshRenders;
};

