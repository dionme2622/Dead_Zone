#include "pch.h"
#include "MeshData.h"
#include "Mesh.h"
#include "Material.h"
#include "Resources.h"
#include "Transform.h"
#include "MeshRenderer.h"




MeshData::MeshData() : Object(OBJECT_TYPE::MESH_DATA)
{
}

MeshData::~MeshData()
{
}

shared_ptr<MeshData> MeshData::LoadModelFromBinary(const char* path)
{
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, path, "rb");
	::rewind(pInFile);

	shared_ptr<MeshData> meshData = make_shared<MeshData>();

	char pstrToken[64] = { '\0' };

	for (; ; )
	{
		if (::ReadStringFromFile(pInFile, pstrToken))
		{
			if (!strcmp(pstrToken, "<Hierarchy>:"))
			{
				MeshData::LoadFrameHierarchyFromFile(meshData, pInFile);
				::ReadStringFromFile(pInFile, pstrToken); //"</Hierarchy>"
			}
			/*else if (!strcmp(pstrToken, "<Animation>:"))
			{
				CGameObject::LoadAnimationFromFile(pInFile, pLoadedModel);
				pLoadedModel->PrepareSkinning();
			}
			else if (!strcmp(pstrToken, "</Animation>:"))
			{
				break;
			}*/
		}
		else
		{
			break;
		}
	}
	return meshData;
}

void MeshData::LoadFrameHierarchyFromFile(shared_ptr<MeshData> meshData, FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	int nFrame = 0, nTextures = 0;

	MeshRenderInfo info = {};
	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	// 임시
	char pstrFrameName[64] = {};
	Matrix matrix = Matrix::Identity;
	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Frame>:"))
		{
			nFrame = ::ReadIntegerFromFile(pInFile);
			nTextures = ::ReadIntegerFromFile(pInFile);
			::ReadStringFromFile(pInFile, pstrFrameName);
		}
		else if (!strcmp(pstrToken, "<Transform>:"))
		{
			XMFLOAT3 xmf3Position, xmf3Rotation, xmf3Scale;
			XMFLOAT4 xmf4Rotation;
			nReads = (UINT)::fread(&xmf3Position, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf3Rotation, sizeof(float), 3, pInFile); //Euler Angle
			nReads = (UINT)::fread(&xmf3Scale, sizeof(float), 3, pInFile);
			nReads = (UINT)::fread(&xmf4Rotation, sizeof(float), 4, pInFile); //Quaternion
		}
		else if (!strcmp(pstrToken, "<TransformMatrix>:"))
		{
			// TODO : Transform Local Matrix를 가져온다.
			nReads = (UINT)::fread(&matrix, sizeof(float), 16, pInFile);
		}
		else if (!strcmp(pstrToken, "<Mesh>:"))
		{
			 info.mesh = mesh->LoadMeshFromFile(pInFile);
			 info.mesh = Mesh::CreateBuffer();
		}
		else if (!strcmp(pstrToken, "<SkinningInfo>:"))
		{
			//if (pnSkinnedMeshes) (*pnSkinnedMeshes)++;

			//CSkinnedMesh* pSkinnedMesh = new CSkinnedMesh(pd3dDevice, pd3dCommandList);
			//pSkinnedMesh->LoadSkinInfoFromFile(pd3dDevice, pd3dCommandList, pInFile);
			//pSkinnedMesh->CreateShaderVariables(pd3dDevice, pd3dCommandList);

			//::ReadStringFromFile(pInFile, pstrToken); //<Mesh>:
			//if (!strcmp(pstrToken, "<Mesh>:")) pSkinnedMesh->LoadMeshFromFile(pd3dDevice, pd3dCommandList, pInFile);

			//pGameObject->SetMesh(pSkinnedMesh);
		}
		else if (!strcmp(pstrToken, "<Materials>:"))
		{
			meshData->GetMaterial()->LoadMaterialsFromFile(pInFile);
			info.materials = meshData->GetMaterials();

		}
		else if (!strcmp(pstrToken, "<Children>:"))
		{
			int nChilds = ::ReadIntegerFromFile(pInFile);
			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					// TODO : 계층구조를 사용하여 자식의 수 만큼 파일을 재귀함수로 읽는다.
					/*CGameObject* pChild = CGameObject::LoadFrameHierarchyFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, pGameObject, pInFile, pShader, pnSkinnedMeshes);
					if (pChild) pGameObject->SetChild(pChild);*/
				}
			}
		}
		else if (!strcmp(pstrToken, "</Frame>"))
		{
			meshData->GetMeshRenderInfo().push_back(info);
			break;
		}
	}
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


	return v;
}

