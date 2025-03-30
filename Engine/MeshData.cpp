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
#include "BoxCollider.h"

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

		// Mesh
		if (!loader.GetMesh(i).vertices.empty())
		{
			shared_ptr<Mesh> mesh = Mesh::CreateFromBinary(&loader.GetMesh(i), loader);
			mesh->SetName(loader.GetMesh(i).meshName);
			GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

			info.mesh = GET_SINGLE(Resources)->Get<Mesh>(mesh->GetName());
		}

		// Transform
		shared_ptr<Transform> transform = loader.GetMesh(i).transform;

		// Material
		vector<shared_ptr<Material>> materials;
		for (size_t j = 0; j < loader.GetMesh(i).materials.size(); j++)
		{
			shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(loader.GetMesh(i).meshName);

			materials.push_back(material);
		}

		// Collider 
		Vec3 center = loader.GetMesh(i).AABBCenter;
		Vec3 extents = loader.GetMesh(i).AABBExtents;
		shared_ptr<BoxCollider> boxCollider = make_shared<BoxCollider>(center, extents);


		info.objName = loader.GetMesh(i).objName;
		info.materials = materials;
		info.transform = transform;
		info.boxCollider = boxCollider;

		// TODO : AABB 바운딩 박스 데이터 넘겨야 함

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
		gameObject->SetName(info.objName);									// Object 이름 설정
		gameObject->AddComponent(info.transform);
		if (info.mesh != nullptr)
		{
			gameObject->AddComponent(make_shared<MeshRenderer>());
			gameObject->GetMeshRenderer()->SetMesh(info.mesh);
			for (uint32 i = 0; i < info.materials.size(); i++)
				gameObject->GetMeshRenderer()->SetMaterial(info.materials[i], i);


#ifdef _DEBUG_COLLIDER
			// TODO : AABB 바운딩 박스 데이터 넘겨야 함
			if (info.boxCollider != nullptr)
			{
				shared_ptr<BoxCollider> collider = info.boxCollider;
				gameObject->AddComponent(collider);

				shared_ptr<Mesh> mesh = gameObject->GetCollider()->GetColliderMesh();
				GET_SINGLE(Resources)->Add<Mesh>(gameObject->GetMeshRenderer()->GetMesh()->GetName() + L"collider", mesh);

				shared_ptr<GameObject> boundingBox = make_shared<GameObject>();
				boundingBox->AddComponent(make_shared<Transform>());
				boundingBox->AddComponent(make_shared<MeshRenderer>());
				{
					shared_ptr<Mesh> colliderMesh = GET_SINGLE(Resources)->Get<Mesh>(info.mesh->GetName() + L"collider");
					boundingBox->GetMeshRenderer()->SetMesh(colliderMesh);
				}
				{
					shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Collider");
					shared_ptr<Material> material = make_shared<Material>();
					material->SetShader(shader);
					boundingBox->GetMeshRenderer()->SetMaterial(material);
				}
				boundingBox->GetTransform()->SetParent(gameObject->GetTransform());
				v.push_back(boundingBox);
				if (collider->DebugDraw())				// Collider를 그리는가?
				{
					
				}
			}
#endif
			///////////////////////////////////////////
			if (info.mesh->IsAnimMesh())				// Mesh가 애니메이션을 가지고 있다면?
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

