#include "pch.h"
#include "BinaryLoader.h"
#include "Mesh.h"
#include "Material.h"

BinaryLoader::BinaryLoader()
{
}

BinaryLoader::~BinaryLoader()
{
}

void BinaryLoader::LoadModelFromBinary(const char* path)
{
	FILE* pInFile = NULL;
	::fopen_s(&pInFile, path, "rb");
	::rewind(pInFile);

	char pstrToken[64] = { '\0' };

	_meshes.push_back(MeshInfo());
	MeshInfo& meshInfo = _meshes.back();

	for (; ; )
	{
		if (::ReadStringFromFile(pInFile, pstrToken))
		{
			if (!strcmp(pstrToken, "<Hierarchy>:"))
			{
				LoadFrameHierarchyFromFile(meshInfo, pInFile);
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
}

void BinaryLoader::LoadFrameHierarchyFromFile(MeshInfo meshes, FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	UINT nReads = 0;

	int nFrame = 0, nTextures = 0;

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
			LoadMeshFromFile(meshes, pInFile);
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
			LoadMaterialFromFile(meshes, pInFile);
		}
		else if (!strcmp(pstrToken, "<Children>:"))
		{
			int nChilds = ::ReadIntegerFromFile(pInFile);
			if (nChilds > 0)
			{
				for (int i = 0; i < nChilds; i++)
				{
					// TODO : 계층구조를 사용하여 자식의 수 만큼 파일을 재귀함수로 읽는다.
					LoadFrameHierarchyFromFile(meshes, pInFile);
				}
			}
		}
		else if (!strcmp(pstrToken, "</Frame>"))
		{
			break;
		}
	}
}

void BinaryLoader::LoadMeshFromFile(MeshInfo meshes, FILE* pInFile)
{
	//// TODO : 파일에서 Mesh 데이터를 읽어온다.
	char pstrToken[64] = { '\0' };
	int nPositions = 0, nColors = 0, nNormals = 0, nTangents = 0, nBiTangents = 0, nTextureCoords = 0, nIndices = 0, nSubMeshes = 0, nSubIndices = 0;

	int32 vertexCount = 0;
	UINT nReads = (UINT)::fread(&vertexCount, sizeof(int), 1, pInFile);

	meshes.vertices.resize(vertexCount);
	::ReadStringFromFile(pInFile, meshes.name);			// Mesh의 이름 저장
	// 임시
	XMFLOAT3 temp = {};
	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Bounds>:"))
		{
			nReads = (UINT)::fread(&temp, sizeof(XMFLOAT3), 1, pInFile);
			nReads = (UINT)::fread(&temp, sizeof(XMFLOAT3), 1, pInFile);
		}
		else if (!strcmp(pstrToken, "<Positions>:"))
		{
			nReads = (UINT)::fread(&nPositions, sizeof(int), 1, pInFile);
			if (nPositions > 0)
			{
				XMFLOAT3* m_pxmf3Positions = new XMFLOAT3[nPositions];
				nReads = (UINT)::fread(m_pxmf3Positions, sizeof(XMFLOAT3), nPositions, pInFile);

				for (UINT i = 0; i < nPositions; ++i)
				{
					meshes.vertices[i].pos.x = static_cast<float>(m_pxmf3Positions[i].x);
					meshes.vertices[i].pos.y = static_cast<float>(m_pxmf3Positions[i].y);
					meshes.vertices[i].pos.z = static_cast<float>(m_pxmf3Positions[i].z);
				}

				delete[] m_pxmf3Positions;
			}
		}
		else if (!strcmp(pstrToken, "<Colors>:"))
		{
			nReads = (UINT)::fread(&nColors, sizeof(int), 1, pInFile);
			if (nColors > 0)
			{
				XMFLOAT4* m_pxmf4Colors = new XMFLOAT4[nColors];
				nReads = (UINT)::fread(m_pxmf4Colors, sizeof(XMFLOAT4), nColors, pInFile);

				delete[] m_pxmf4Colors;
			}
		}
		else if (!strcmp(pstrToken, "<TextureCoords0>:"))
		{
			nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
			if (nTextureCoords > 0)
			{
				XMFLOAT2* m_pxmf2TextureCoords0 = new XMFLOAT2[nPositions];
				nReads = (UINT)::fread(m_pxmf2TextureCoords0, sizeof(XMFLOAT2), nTextureCoords, pInFile);

				for (UINT i = 0; i < nTextureCoords; ++i)
				{
					meshes.vertices[i].uv.x = static_cast<float>(m_pxmf2TextureCoords0[i].x);
					meshes.vertices[i].uv.y = static_cast<float>(m_pxmf2TextureCoords0[i].y);
				}
				delete[] m_pxmf2TextureCoords0;
			}
		}
		else if (!strcmp(pstrToken, "<TextureCoords1>:"))
		{
			nReads = (UINT)::fread(&nTextureCoords, sizeof(int), 1, pInFile);
			if (nTextureCoords > 0)
			{
				
				XMFLOAT2* m_pxmf2TextureCoords1 = new XMFLOAT2[nTextureCoords];
				nReads = (UINT)::fread(m_pxmf2TextureCoords1, sizeof(XMFLOAT2), nTextureCoords, pInFile);

				delete[] m_pxmf2TextureCoords1;
			}
		}
		else if (!strcmp(pstrToken, "<Normals>:"))
		{
			nReads = (UINT)::fread(&nNormals, sizeof(int), 1, pInFile);
			if (nNormals > 0)
			{
				XMFLOAT3* m_pxmf3Normals = new XMFLOAT3[nNormals];
				nReads = (UINT)::fread(m_pxmf3Normals, sizeof(XMFLOAT3), nNormals, pInFile);

				for (UINT i = 0; i < nNormals; ++i)
				{
					meshes.vertices[i].normal.x = static_cast<float>(m_pxmf3Normals[i].x);
					meshes.vertices[i].normal.y = static_cast<float>(m_pxmf3Normals[i].y);
					meshes.vertices[i].normal.z = static_cast<float>(m_pxmf3Normals[i].z);

				}
				delete[] m_pxmf3Normals;
			}
		}
		else if (!strcmp(pstrToken, "<Tangents>:"))
		{
			nReads = (UINT)::fread(&nTangents, sizeof(int), 1, pInFile);
			if (nTangents > 0)
			{
				XMFLOAT3* m_pxmf3Tangents = new XMFLOAT3[nTangents];
				nReads = (UINT)::fread(m_pxmf3Tangents, sizeof(XMFLOAT3), nTangents, pInFile);

				for (UINT i = 0; i < nTangents; ++i)
				{
					meshes.vertices[i].tangent.x = static_cast<float>(m_pxmf3Tangents[i].x);
					meshes.vertices[i].tangent.y = static_cast<float>(m_pxmf3Tangents[i].y);
					meshes.vertices[i].tangent.z = static_cast<float>(m_pxmf3Tangents[i].z);

				}
				delete[] m_pxmf3Tangents;
			}
		}
		else if (!strcmp(pstrToken, "<BiTangents>:"))
		{
			nReads = (UINT)::fread(&nBiTangents, sizeof(int), 1, pInFile);
			if (nBiTangents > 0)
			{
				XMFLOAT3* m_pxmf3BiTangents = new XMFLOAT3[nBiTangents];
				nReads = (UINT)::fread(m_pxmf3BiTangents, sizeof(XMFLOAT3), nBiTangents, pInFile);

				for (UINT i = 0; i < nBiTangents; ++i)
				{
					meshes.vertices[i].bitangent.x = static_cast<float>(m_pxmf3BiTangents[i].x);
					meshes.vertices[i].bitangent.y = static_cast<float>(m_pxmf3BiTangents[i].y);
					meshes.vertices[i].bitangent.z = static_cast<float>(m_pxmf3BiTangents[i].z);

				}
				delete[] m_pxmf3BiTangents;
			}
		}
		else if (!strcmp(pstrToken, "<SubMeshes>:"))
		{
			nReads = (UINT)::fread(&nSubMeshes, sizeof(int), 1, pInFile);
			if (nSubMeshes > 0)
			{
				meshes.indices.resize(nSubMeshes);
				int* m_pnSubSetIndices = new int[nSubMeshes];
				UINT** m_ppnSubSetIndices = new UINT * [nSubMeshes];
				for (int i = 0; i < nSubMeshes; i++)
				{
					::ReadStringFromFile(pInFile, pstrToken);
					if (!strcmp(pstrToken, "<SubMesh>:"))
					{
						int nIndex = 0;
						nReads = (UINT)::fread(&nIndex, sizeof(int), 1, pInFile); //i
						nReads = (UINT)::fread(&(m_pnSubSetIndices[i]), sizeof(int), 1, pInFile);
						if (m_pnSubSetIndices[i] > 0)
						{
							m_ppnSubSetIndices[i] = new UINT[m_pnSubSetIndices[i]];
							nReads = (UINT)::fread(m_ppnSubSetIndices[i], sizeof(UINT), m_pnSubSetIndices[i], pInFile);

							for (int j = 0; j < m_pnSubSetIndices[i]; j++)
							{
								meshes.indices[i].push_back(static_cast<uint32>(m_ppnSubSetIndices[i][j]));
							}
						}
					}
				}
			}
		}
		else if (!strcmp(pstrToken, "</Mesh>"))
		{
			break;
		}
	}
}

void BinaryLoader::LoadMaterialFromFile(MeshInfo meshes, FILE* pInFile)
{
	char pstrToken[64] = { '\0' };
	int nMaterial = 0;
	UINT nReads = 0;

	int m_nMaterials = ReadIntegerFromFile(pInFile);
	meshes.materials.resize(m_nMaterials);

	for (; ; )
	{
		::ReadStringFromFile(pInFile, pstrToken);
		if (!strcmp(pstrToken, "<Material>:"))
		{
			nMaterial = ReadIntegerFromFile(pInFile);
		}
		else if (!strcmp(pstrToken, "<AlbedoColor>:"))
		{
			XMFLOAT4* m_xmf4AlbedoColor = new XMFLOAT4[nMaterial];
			nReads = (UINT)::fread(m_xmf4AlbedoColor, sizeof(XMFLOAT4), 1, pInFile);

			//delete[] m_xmf4AlbedoColor;
		}
		else if (!strcmp(pstrToken, "<EmissiveColor>:"))
		{
			XMFLOAT4* m_xmf4EmissiveColor = new XMFLOAT4[nMaterial];
			nReads = (UINT)::fread(m_xmf4EmissiveColor, sizeof(XMFLOAT4), 1, pInFile);

			//delete[] m_xmf4EmissiveColor;
		}
		else if (!strcmp(pstrToken, "<SpecularColor>:"))
		{
			XMFLOAT4* m_xmf4SpecularColor = new XMFLOAT4[nMaterial];
			nReads = (UINT)::fread(m_xmf4SpecularColor, sizeof(XMFLOAT4), 1, pInFile);

			//delete[] m_xmf4SpecularColor;
		}
		else if (!strcmp(pstrToken, "<Glossiness>:"))
		{
			float* m_fGlossiness = new float[nMaterial];
			nReads = (UINT)::fread(m_fGlossiness, sizeof(float), 1, pInFile);

			//delete[] m_fGlossiness;
		}
		else if (!strcmp(pstrToken, "<Smoothness>:"))
		{
			float* m_fSmoothness = new float[nMaterial];
			nReads = (UINT)::fread(m_fSmoothness, sizeof(float), 1, pInFile);

			//delete[] m_fSmoothness;
		}
		else if (!strcmp(pstrToken, "<Metallic>:"))
		{
			float* m_fMetallic = new float[nMaterial];
			nReads = (UINT)::fread(m_fMetallic, sizeof(float), 1, pInFile);

			//delete[] m_fMetallic;
		}
		else if (!strcmp(pstrToken, "<SpecularHighlight>:"))
		{
			float* m_fSpecularHighlight = new float[nMaterial];
			nReads = (UINT)::fread(m_fSpecularHighlight, sizeof(float), 1, pInFile);

			//delete[] m_fSpecularHighlight;
		}
		else if (!strcmp(pstrToken, "<GlossyReflection>:"))
		{
			float* m_fGlossyReflection = new float[nMaterial];
			nReads = (UINT)::fread(m_fGlossyReflection, sizeof(float), 1, pInFile);

			//delete[] m_fGlossyReflection;
		}
		else if (!strcmp(pstrToken, "<AlbedoMap>:"))
		{

		}
		else if (!strcmp(pstrToken, "<SpecularMap>:"))
		{

		}
		else if (!strcmp(pstrToken, "<NormalMap>:"))
		{

		}
		else if (!strcmp(pstrToken, "<MetallicMap>:"))
		{

		}
		else if (!strcmp(pstrToken, "<EmissionMap>:"))
		{

		}
		else if (!strcmp(pstrToken, "<DetailAlbedoMap>:"))
		{

		}
		else if (!strcmp(pstrToken, "<DetailNormalMap>:"))
		{

		}
		else if (!strcmp(pstrToken, "</Materials>"))
		{
			break;
		}

	}
}

void BinaryLoader::LoadMesh(Mesh* mesh)
{
	// TODO : Mesh 데이터를 불러온다.
	_meshes.push_back(MeshInfo());
	MeshInfo& meshInfo = _meshes.back();	


}

void BinaryLoader::LoadMaterial(Material* surfaceMaterial)
{
	// TODO : Material 데이터를 불러온다.
	MaterialInfo material{};

	material.name;

	material.albedo;
	material.emissive;
	material.specular;

	material.albedoTexName;
	material.normalTexName;
	material.specularTexName;

	_meshes.back().materials.push_back(material);
}


