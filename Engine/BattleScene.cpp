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

// TEST
#include "KeyInput.h"

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
		_playerCamera->AddComponent(make_shared<Camera>());
		//_playerCamera->AddComponent(make_shared<PlayerScript>(_hwnd));
		_playerCamera->GetTransform()->SetLocalPosition(Vec3(0.f, 2.3f, 0.9f));
		_playerCamera->GetTransform()->LookAt(Vec3(0.f, 0.f, 1.f));
		uint8 layerIndex = LayerNameToIndex(L"UI");
		_playerCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI´Â ¾È ÂïÀ½
		AddGameObject(_playerCamera);
	}
#pragma endregion

#pragma region UI_Camera
	{
		_uiCamera = make_shared<GameObject>();
		_uiCamera->SetName(L"Orthographic_Camera");
		_uiCamera->AddComponent(make_shared<Transform>());
		_uiCamera->AddComponent(make_shared<Camera>());
		_uiCamera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		_uiCamera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = LayerNameToIndex(L"UI");
		_uiCamera->GetCamera()->SetCullingMaskAll(); // ´Ù ²ô°í
		_uiCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI¸¸ Å´
		AddGameObject(_uiCamera);
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
	/*_player = make_shared<Player>();
	_player->GetGameObject()->SetLayerIndex(LayerNameToIndex(L"Battle"));
	_player->GetGameObject()->AddComponent(make_shared<PlayerScript>(_hwnd));
	AddGameObject(_player->GetGameObject());
	_playerCamera->GetTransform()->SetParent(_player->GetGameObject()->GetTransform());*/

	shared_ptr<MeshData> FemaleHero = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Character_FemaleHero.bin"); // MeshData* meshData

	vector<shared_ptr<GameObject>> gameObjects = FemaleHero->Instantiate();

	for (auto& gameObject : gameObjects)
	{
		gameObject->SetLayerIndex(LayerNameToIndex(L"Battle"));
		gameObject->SetCheckFrustum(true);
		gameObject->SetStatic(true);
		AddGameObject(gameObject);
	}
	shared_ptr<GameObject> rootObject = gameObjects[0];

	//rootObject->AddComponent(make_shared<Player>());
	rootObject->AddComponent(make_shared<PlayerScript>(_hwnd));
	rootObject->GetTransform()->SetLocalPosition(Vec3(0.0, 0.0f, 0.f));
	rootObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

	_playerCamera->GetTransform()->SetParent(rootObject->GetTransform());

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
		light->GetTransform()->SetLocalPosition(Vec3(0, 500, 0));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0, -1, 0.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
		uint8 layerIndex = LayerNameToIndex(L"UI");
		//light->GetShadowCamera()->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI´Â ¾È ÂïÀ½
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

#pragma region Character
	{
		shared_ptr<MeshData> FemaleSoldier = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Character_FemaleSoldier1.bin"); // MeshData* meshData

		vector<shared_ptr<GameObject>> gameObjects1 = FemaleSoldier->Instantiate();

		for (auto& gameObject : gameObjects1)
		{
			gameObject->SetCheckFrustum(true);
			gameObject->SetStatic(true);
			AddGameObject(gameObject);
		}
		shared_ptr<GameObject> rootObject1 = gameObjects1[0];

		rootObject->GetTransform()->SetLocalPosition(Vec3(0.0, 0.0f, 0.f));
		//rootObject->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
	

	
		shared_ptr<MeshData> Rifle = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Wep_AssaultRifle01.bin"); // MeshData* meshData

		vector<shared_ptr<GameObject>> gameObjects = Rifle->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetCheckFrustum(true);
			gameObject->SetStatic(true);
			AddGameObject(gameObject);
		}
		shared_ptr<GameObject> rootObject = gameObjects[0];

		rootObject->GetTransform()->SetLocalPosition(Vec3(0.0, 0.0f, 0.f));
		rootObject->GetTransform()->SetParent(gameObjects1[14]->GetTransform());
	}

#pragma endregion


#pragma region Map
	{
		shared_ptr<MeshData> scene = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\EnvDemo.bin"); // MeshData* meshData

		vector<shared_ptr<GameObject>> gameObjects = scene->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetCheckFrustum(true);
			gameObject->SetStatic(true);
			AddGameObject(gameObject);
		}

		shared_ptr<GameObject> rootObject = gameObjects[0];

		//rootObject->GetTransform()->SetLocalPosition(Vec3(0.0, 0.f, 0.f));
		rootObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	}

	{
		shared_ptr<MeshData> scene = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\BldDemo.bin"); // MeshData* meshData

		vector<shared_ptr<GameObject>> gameObjects = scene->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetCheckFrustum(true);
			gameObject->SetStatic(true);
			AddGameObject(gameObject);
		}

		shared_ptr<GameObject> rootObject = gameObjects[0];

		//rootObject->GetTransform()->SetLocalPosition(Vec3(0.0, 0.f, 0.f));
		rootObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	}

	{
		shared_ptr<MeshData> scene = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\PropDemo.bin"); // MeshData* meshData

		vector<shared_ptr<GameObject>> gameObjects = scene->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetCheckFrustum(true);
			gameObject->SetStatic(true);
			AddGameObject(gameObject);
		}

		shared_ptr<GameObject> rootObject = gameObjects[0];

		//rootObject->GetTransform()->SetLocalPosition(Vec3(0.0, 0.f, 0.f));
		rootObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	}

	{
		shared_ptr<MeshData> scene = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\VehDemo.bin"); // MeshData* meshData

		vector<shared_ptr<GameObject>> gameObjects = scene->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetCheckFrustum(true);
			gameObject->SetStatic(true);
			AddGameObject(gameObject);
		}

		shared_ptr<GameObject> rootObject = gameObjects[0];

		//rootObject->GetTransform()->SetLocalPosition(Vec3(0.0, 0.f, 0.f));
		rootObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	}
#pragma endregion
}



void BattleScene::Update()
{
	Scene::Update();
}