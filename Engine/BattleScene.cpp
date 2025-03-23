#include "pch.h"
#include "BattleScene.h"

#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "Player.h"
#include "PlayerScript.h"
#include "Engine.h"
#include "Resources.h"
#include "MeshData.h"
#include "TestAnimation.h"
#include "ParticleSystem.h"
#include "BoxCollider.h"


BattleScene::BattleScene()
{
}



void BattleScene::LoadScene()
{
#pragma region LayerMask
	SetLayerName(0, L"Battle");
	SetLayerName(1, L"UI");
#pragma endregion

#pragma region PlayerCamera
	{
		_playerCamera = make_shared<GameObject>();
		_playerCamera->SetName(L"Main_Camera");
		_playerCamera->AddComponent(make_shared<Transform>());
		_playerCamera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=90도
		_playerCamera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		_playerCamera->GetTransform()->LookAt(Vec3(0.f, 0.f, 1.f));
		uint8 layerIndex = LayerNameToIndex(L"UI");
		_playerCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI는 안 찍음
		AddGameObject(_playerCamera);
	}
#pragma endregion

#pragma region UI_Camera
	{
		//shared_ptr<GameObject> camera = make_shared<GameObject>();
		//camera->SetName(L"Orthographic_Camera");
		//camera->AddComponent(make_shared<Transform>());
		//camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, 800*600
		//camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		//camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		//uint8 layerIndex = LayerNameToIndex(L"UI");
		//camera->GetCamera()->SetCullingMaskAll(); // 다 끄고
		//camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI만 찍음
		//AddGameObject(camera);
	}
#pragma endregion

#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->SetLayerIndex(LayerNameToIndex(L"Battle"));
		skybox->AddComponent(make_shared<Transform>());
		skybox->SetCheckFrustum(false);
		skybox->GetTransform()->SetLocalScale(Vec3(100.0f, 100.0f, 100.0f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resources\\Texture\\SimpleSky.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		AddGameObject(skybox);
	}
#pragma endregion

#pragma region Player
	_player = make_shared<Player>();
	_player->GetGameObject()->SetLayerIndex(LayerNameToIndex(L"Battle"));
	_player->GetGameObject()->AddComponent(make_shared<PlayerScript>());
	AddGameObject(_player->GetGameObject());
	_playerCamera->GetTransform()->SetParent(_player->GetGameObject()->GetTransform());
#pragma endregion

#pragma region Object
	{
		for (int32 i = 0; i < 1; i++)
		{
			shared_ptr<GameObject> obj = make_shared<GameObject>();
			obj->AddComponent(make_shared<Transform>());
			obj->GetTransform()->SetLocalScale(Vec3(25.f, 25.f, 25.f));
			obj->GetTransform()->SetLocalPosition(Vec3(300.f, 0.f, 0.0f));
			obj->AddComponent(make_shared<BoxCollider>());
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			{
				shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
				meshRenderer->SetMesh(sphereMesh);
			}
			{
				shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
				shared_ptr<Material> material = make_shared<Material>();
				material->SetShader(shader);
				meshRenderer->SetMaterial(material);
			}
			obj->AddComponent(meshRenderer);
			AddGameObject(obj);

			// 여기서 Collider 컴포넌트의 정점 데이터를 가져와서 새로운 메쉬를 생성한다.
			if (obj->GetCollider() != nullptr)	// Collider Component가 존재한다면
			{
				shared_ptr<Mesh> mesh = obj->GetCollider()->GetColliderMesh();
				GET_SINGLE(Resources)->Add<Mesh>(obj->GetMeshRenderer()->GetMesh()->GetName() + L"collider", mesh);
			}
			


			shared_ptr<GameObject> obj2 = make_shared<GameObject>();
			obj2->AddComponent(make_shared<Transform>());
			obj2->AddComponent(make_shared<BoxCollider>());
			shared_ptr<MeshRenderer> meshRenderer2 = make_shared<MeshRenderer>();
			{
				shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->Get<Mesh>(L"playercollider");
				meshRenderer2->SetMesh(sphereMesh);
			}
			{
				shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Collider");
				shared_ptr<Material> material = make_shared<Material>();
				material->SetShader(shader);
				meshRenderer2->SetMaterial(material);
			}
			obj2->AddComponent(meshRenderer2);
			obj2->GetTransform()->SetParent(obj->GetTransform());
			AddGameObject(obj2);
		}
		// Bounding Box를 그리기 위한 객체를 하나 더 만들고 부모 설정을 해야한다. 어떻게 만들어야 하는가??
		// Bounding Box의 정점 데이터를 기반으로 메쉬를 만들고 자식으로 설정한다.
		// 객체의 Local Position은 부모의 World Position 으로 한다.
		// SetMesh와 SetMaterial을 해주면 끝

		
	}
#pragma endregion

#pragma region BoundingBox
	{
		

	}


#pragma region Plane
	{
		/*shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->SetLayerIndex(LayerNameToIndex(L"Battle"));
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(10000.f, 1.f, 10000.f));
		obj->GetTransform()->SetLocalPosition(Vec3(0.f, -150.f, 0.f));
		obj->SetStatic(true);
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadCubeMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Metal_Pattern", L"..\\Resources\\Texture\\Metal_Pattern_basecolor.png");
			shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Metal_Pattern_Normal", L"..\\Resources\\Texture\\Metal_Pattern_normal.png");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			material->SetTexture(1, texture2);
			meshRenderer->SetMaterial(material);
		}
		obj->AddComponent(meshRenderer);
		AddGameObject(obj);
	}*/


#pragma endregion

#pragma region UI_Test
		for (int32 i = 0; i < 6; i++)
		{
			shared_ptr<GameObject> obj = make_shared<GameObject>();
			obj->SetLayerIndex(LayerNameToIndex(L"UI")); // UI
			obj->AddComponent(make_shared<Transform>());
			obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
			obj->GetTransform()->SetLocalPosition(Vec3(-350.f + (i * 120), 250.f, 500.f));
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			{
				shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
				meshRenderer->SetMesh(mesh);
			}
			{
				shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");

				shared_ptr<Texture> texture;
				if (i < 3)
					texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
				else if (i < 5)
					texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
				else
					texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);

				shared_ptr<Material> material = make_shared<Material>();
				material->SetShader(shader);
				material->SetTexture(0, texture);
				meshRenderer->SetMaterial(material);
			}
			obj->AddComponent(meshRenderer);
			AddGameObject(obj);
		}
#pragma endregion

#pragma region Aiming Point

		{
			shared_ptr<GameObject> obj = make_shared<GameObject>();
			obj->SetLayerIndex(LayerNameToIndex(L"UI")); // UI
			obj->AddComponent(make_shared<Transform>());
			obj->GetTransform()->SetLocalScale(Vec3(30.f, 30.f, 1.f));
			obj->GetTransform()->SetLocalPosition(Vec3(0, 0, 1.f));
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			{
				shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
				meshRenderer->SetMesh(mesh);
			}
			{
				shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"UI");
				shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"AimPoint", L"..\\Resources\\Texture\\AimPoint.png");
				shared_ptr<Material> material = make_shared<Material>();
				material->SetShader(shader);
				material->SetTexture(0, texture);
				meshRenderer->SetMaterial(material);
			}
			obj->AddComponent(meshRenderer);
			AddGameObject(obj);
		}
#pragma endregion

#pragma region Directional Light
		{
			shared_ptr<GameObject> light = make_shared<GameObject>();
			light->AddComponent(make_shared<Transform>());
			light->GetTransform()->SetLocalPosition(Vec3(0, 500, 500));
			light->AddComponent(make_shared<Light>());
			light->GetLight()->SetLightDirection(Vec3(0, -1, -1.f));
			light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
			light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
			light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
			light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
			AddGameObject(light);
		}
#pragma endregion

#pragma region ParticleSystem
		{
			/*shared_ptr<GameObject> particle = make_shared<GameObject>();
			particle->AddComponent(make_shared<Transform>());
			particle->AddComponent(make_shared<ParticleSystem>());
			particle->SetCheckFrustum(false);
			particle->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 100.f));
			AddGameObject(particle);*/
		}
#pragma endregion

#pragma region Model

		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\EnvDemo1.bin"); // MeshData* meshData

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetCheckFrustum(true);
				gameObject->SetStatic(true);
				AddGameObject(gameObject);
			}

			shared_ptr<GameObject> rootObject = gameObjects[0];

			rootObject->GetTransform()->SetLocalPosition(Vec3(0.0, 0.f, 0.f));
			rootObject->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
		}

		{
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Character_FemaleHero.bin"); // MeshData* meshData

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				gameObject->SetCheckFrustum(true);
				gameObject->SetStatic(true);
				AddGameObject(gameObject);
			}
			shared_ptr<GameObject> rootObject = gameObjects[0];

			rootObject->GetTransform()->SetLocalPosition(Vec3(0.0, -150.0f, 0.f));
			rootObject->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
		}


#pragma endregion


	}
}


void BattleScene::Update()
{
	Scene::Update();
}