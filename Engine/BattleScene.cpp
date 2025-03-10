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
		_playerCamera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=90µµ
		_playerCamera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		_playerCamera->GetTransform()->LookAt(Vec3(0.f, 0.f, 1.f));
		uint8 layerIndex = LayerNameToIndex(L"UI");
		_playerCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI´Â ¾È ÂïÀ½
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
		//camera->GetCamera()->SetCullingMaskAll(); // ´Ù ²ô°í
		//camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI¸¸ ÂïÀ½
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
		/*for (int32 i = 0; i < 50; i++)
		{
			shared_ptr<GameObject> obj = make_shared<GameObject>();
			obj->AddComponent(make_shared<Transform>());
			obj->GetTransform()->SetLocalScale(Vec3(25.f, 25.f, 25.f));
			obj->GetTransform()->SetLocalPosition(Vec3(i * 20.f, 0.f, 50.0f));
			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			{
				shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
				meshRenderer->SetMesh(sphereMesh);
			}
			{
				shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
				meshRenderer->SetMaterial(material);
			}
			obj->AddComponent(meshRenderer);
			AddGameObject(obj);
		}*/
	}


#pragma endregion

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
			shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\Demo.bin"); // MeshData* meshData

			vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

			for (auto& gameObject : gameObjects)
			{
				//gameObject->SetName(L"SA_Character_FemaleHero");
				gameObject->SetCheckFrustum(true);
				gameObject->SetStatic(true);
				//gameObject->AddComponent(make_shared<TestAnimation>());
				AddGameObject(gameObject);
			}


			gameObjects[0]->GetTransform()->SetLocalPosition(Vec3(0.0, 0.f, 0.f));
			gameObjects[0]->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));

		}


#pragma endregion


	}
}


void BattleScene::Update()
{
	Scene::Update();
}