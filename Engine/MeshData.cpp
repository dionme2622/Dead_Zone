#include "pch.h"
#include "MeshData.h"
#include "Mesh.h"
#include "Material.h"
#include "Resources.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "BinaryLoader.h"



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
		shared_ptr<Mesh> mesh = Mesh::CreateFromBinary(&loader.GetMesh(i), loader);

		GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

		Matrix matrix = loader.GetMesh(i).matrix;

		// Material 찾아서 연동
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			//shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).materials[j].name);

			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Cliff_Rock2", L"..\\Resources\\Texture\\SimpleApocalypse_Texture.png");
			//shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Cliff_Rock_Normal", L"..\\Resources\\Texture\\Cliff_Rock_normal.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			//material->SetTexture(1, texture2);

			materials.push_back(material);

		}

		MeshRenderInfo info = {};
		info.mesh = mesh;
		info.materials = materials;
		info.matrix = matrix;
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
		gameObject->AddComponent(make_shared<Transform>());
		gameObject->AddComponent(make_shared<MeshRenderer>());
		gameObject->GetMeshRenderer()->SetMesh(info.mesh);
		gameObject->GetTransform()->SetLocalMatrix(info.matrix);
		for (uint32 i = 0; i < info.materials.size(); i++)
			gameObject->GetMeshRenderer()->SetMaterial(info.materials[i], i);

		//if (info.mesh->IsAnimMesh())				// Mesh가 애니메이션을 가지고 있다면?
		//{
		//	shared_ptr<Animator> animator = make_shared<Animator>();
		//	gameObject->AddComponent(animator);
		//	animator->SetBones(info.mesh->GetBones());
		//	animator->SetAnimClip(info.mesh->GetAnimClip());
		//}

		v.push_back(gameObject);
	}

	//for (int i = 0; i < _meshRenders.size(); ++i)
	//{
	//	shared_ptr<GameObject> gameObject = make_shared<GameObject>();
	//	MeshRenderInfo& info = _meshRenders[i];

	//	if (i == 0)		// Root Object
	//	{
	//		gameObject->AddComponent(make_shared<Transform>());
	//		gameObject->AddComponent(make_shared<MeshRenderer>());
	//		gameObject->GetMeshRenderer()->SetMesh(info.mesh);
	//		gameObject->GetTransform()->SetLocalMatrix(info.matrix);
	//		for (uint32 i = 0; i < info.materials.size(); i++)
	//			gameObject->GetMeshRenderer()->SetMaterial(info.materials[i], i);
	//	}
	//	else
	//	{

	//	}
	//}

	return v;
}

