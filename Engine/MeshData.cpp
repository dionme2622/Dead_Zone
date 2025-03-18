#include "pch.h"
#include "MeshData.h"
#include "Mesh.h"
#include "Material.h"
#include "Resources.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "BinaryLoader.h"
#include "Animator.h"
#include "TestAnimation.h"

MeshData::MeshData() : Object(OBJECT_TYPE::MESH_DATA)
{
}

MeshData::~MeshData()
{
}

shared_ptr<MeshData> MeshData::LoadModelFromBinary(const char* path)
{
	BinaryLoader loader;
	loader.LoadModelFromBinary(path);

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	for (int32 i = 0; i < loader.GetMeshCount(); i++)
	{
		MeshRenderInfo info = {};

		if (!loader.GetMesh(i).vertices.empty())
		{
			shared_ptr<Mesh> mesh = Mesh::CreateFromBinary(&loader.GetMesh(i), loader);
			mesh->SetName(loader.GetMesh(i).meshName);
			GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

			info.mesh = GET_SINGLE(Resources)->Get<Mesh>(mesh->GetName());
		}
		shared_ptr<Transform> transform = loader.GetMesh(i).transform;
		// Material ã�Ƽ� ����
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).meshName);

			//material->SetInt(0, 1);
			materials.push_back(material);
		}
		info.objName = loader.GetMesh(i).objName;
		info.materials = materials;
		info.transform = transform;
		// TODO : AABB �ٿ�� �ڽ� ������ �Ѱܾ� ��

		////////////////////////////////////////
		meshData->_meshRenders.push_back(info);
	}

	return meshData;
}


vector<shared_ptr<GameObject>> MeshData::Instantiate()
{
	vector<shared_ptr<GameObject>> v;

	for (MeshRenderInfo& info : _meshRenders)
	{
		shared_ptr<GameObject> gameObject = make_shared<GameObject>();
		gameObject->SetName(info.objName);									// Object �̸� ����
		gameObject->AddComponent(info.transform);
		if (info.mesh != nullptr)
		{
			//shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->Get<Mesh>(mesh->GetName(), info;
			gameObject->AddComponent(make_shared<MeshRenderer>());
			gameObject->GetMeshRenderer()->SetMesh(info.mesh);
			for (uint32 i = 0; i < info.materials.size(); i++)
				gameObject->GetMeshRenderer()->SetMaterial(info.materials[i], i);


			// TODO : AABB �ٿ�� �ڽ� ������ �Ѱܾ� ��

			///////////////////////////////////////////

			if (info.mesh->IsAnimMesh())				// Mesh�� �ִϸ��̼��� ������ �ִٸ�?
			{
				shared_ptr<Animator> animator = make_shared<Animator>();
				animator->SetBones(info.mesh->GetBones());
				animator->SetAnimClip(info.mesh->GetAnimClip());
				gameObject->AddComponent(animator);
				gameObject->AddComponent(make_shared<TestAnimation>());
			}
		}
		v.push_back(gameObject);
	}

	return v;
}

