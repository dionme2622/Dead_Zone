#include "pch.h"
#include "BattleScene.h"

#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "PlayerScript.h"
#include "Engine.h"
#include "Resources.h"
#include "MeshData.h"
#include "TestAnimation.h"
#include "ParticleSystem.h"
#include "BoxCollider.h"
#include "Animator.h"
#include "WeaponManager.h"
#include "Weapon.h"
#include "PlayerStats.h"
#include "StructuredBuffer.h"
// TEST
#include "KeyInput.h"

BattleScene::BattleScene()
{
}



void BattleScene::LoadScene()
{
	int _myID = 1;
	int _theirID = 0;
#pragma region LayerMask
	SetLayerName(0, L"Battle");
	SetLayerName(1, L"UI");
#pragma endregion

#pragma region DebugCamera
	{
		//_playerCamera = make_shared<GameObject>();
		//_playerCamera->SetName(L"Debug_Camera");
		//_playerCamera->AddComponent(make_shared<Transform>());
		//_playerCamera->AddComponent(make_shared<Camera>());
		////_playerCamera->AddComponent(make_shared<PlayerScript>(_hwnd));

		//_playerCamera->GetTransform()->SetLocalPosition(Vec3(0.0f, 0.0f, 100.f));
		//_playerCamera->GetTransform()->LookAt(Vec3(0.f, 0.f, 1.f));
		//_playerCamera->GetTransform()->SetLocalRotation(Vec3(0.f, 180.f, 0.f));
		//uint8 layerIndex = LayerNameToIndex(L"UI");
		//_playerCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI´Â ¾È ÂïÀ½
		//AddGameObject(_playerCamera);
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

#pragma region Player1
	_theirID = 1;
	bool islocal = (_theirID == _myID);

	shared_ptr<MeshData> FemaleSoldier_data = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Character_FemaleSoldier.bin"); // MeshData* meshData

	vector<shared_ptr<GameObject>> FemaleSoldier = FemaleSoldier_data->Instantiate();

	shared_ptr<GameObject> player1 = FemaleSoldier[23];
	for (auto& gameObject : FemaleSoldier)
	{
		//gameObject->SetName(L"FemaleSoldier");
		gameObject->SetCheckFrustum(true);
		gameObject->SetStatic(true);
		gameObject->GetTransform()->FinalUpdate();
		AddGameObject(gameObject);
	}

	player1->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
	player1->AddComponent(make_shared<PlayerScript>(_hwnd, islocal, _theirID));								// Add Player Controller
	//player1->AddComponent(make_shared<WeaponManager>());													// Add Weapon Manager
	player1->AddComponent(make_shared<PlayerStats>());

	_player.push_back(player1);
#pragma endregion

#pragma region Player2
	_theirID = 2;
	islocal = (_theirID == _myID);

	shared_ptr<MeshData> FemaleHero_data = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Character_FemaleHero.bin"); // MeshData* meshData

	vector<shared_ptr<GameObject>> FemaleHero = FemaleHero_data->Instantiate();

	shared_ptr<GameObject> player2 = FemaleHero[23];
	for (auto& gameObject : FemaleHero)
	{
		//gameObject->SetName(L"FemaleSoldier");
		gameObject->SetCheckFrustum(true);
		gameObject->SetStatic(true);
		gameObject->GetTransform()->FinalUpdate();
		AddGameObject(gameObject);
	}

	player2->GetTransform()->SetLocalPosition(Vec3(5.f, 0.f, 0.f));
	//player2->AddComponent(make_shared<PlayerScript>(_hwnd, islocal, _theirID));

	_player.push_back(player2);

#pragma endregion

#pragma region PlayerCamera
	{
		_playerCamera = make_shared<GameObject>();
		_playerCamera->SetName(L"Main_Camera");
		_playerCamera->AddComponent(make_shared<Transform>());
		_playerCamera->AddComponent(make_shared<Camera>());
		_playerCamera->GetTransform()->SetLocalPosition(Vec3(0.01f, 2.03f, -3.65f));
		_playerCamera->GetTransform()->LookAt(Vec3(0.f, 0.f, 1.f));
		uint8 layerIndex = LayerNameToIndex(L"UI");
		_playerCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI´Â ¾È ÂïÀ½
		AddGameObject(_playerCamera);

		_playerCamera->GetTransform()->SetParent(_player[_myID - 1]->GetTransform());						// Player¿¡°Ô Camera ¸¦ ºÙÀÎ´Ù.

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
		light->GetTransform()->SetLocalPosition(Vec3(0, 800.f, 0));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0, -1.f, 0.f));
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
		for (int i = 0; i < 10; ++i)
		{
			shared_ptr<MeshData> Zombie = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Zombie_Cheerleader.bin"); // MeshData* meshData

			vector<shared_ptr<GameObject>> gameObjects = Zombie->Instantiate();
			for (auto& gameObject : gameObjects)
			{
				gameObject->SetCheckFrustum(true);
				gameObject->SetStatic(true);
				//gameObject->SetTransform();  gameObject->GetTransform();
				AddGameObject(gameObject);
			}
			gameObjects[23]->GetTransform()->SetLocalPosition(Vec3(i * 5.0f, 0.0f, 0.0f));
			gameObjects[23]->GetTransform()->GetLocalMatrix();
			_player.push_back(gameObjects[23]);
		}
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
	/*Vec3 pos = _player->GetTransform()->GetLocalPosition();
	printf("%f %f %f\n", pos.x, pos.y, pos.z);*/

	Scene::Update();
}