#include "pch.h"
#include "BattleScene.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Timer.h"
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
#include <bullet3/btBulletDynamicsCommon.h>
#include "CapsuleCollider.h"
#include "RigidBody.h"
#include "PhysicsSystem.h"
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
	//{
	//	_playerCamera = make_shared<GameObject>();
	//	_playerCamera->SetName(L"Debug_Camera");
	//	_playerCamera->AddComponent(make_shared<Transform>());
	//	_playerCamera->AddComponent(make_shared<Camera>());
	//	_playerCamera->AddComponent(make_shared<TestAnimation>(_hwnd));

	//	_playerCamera->GetTransform()->SetLocalPosition(Vec3(0.0f, 100.0f, 100.f));
	//	_playerCamera->GetTransform()->LookAt(Vec3(0.f, 0.f, 1.f));
	//	_playerCamera->GetTransform()->SetLocalRotation(Vec3(0.f, 180.f, 0.f));
	//	uint8 layerIndex = LayerNameToIndex(L"UI");
	//	_playerCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI�� �� ����
	//	AddGameObject(_playerCamera);
	//}
#pragma endregion

#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->SetLayerIndex(LayerNameToIndex(L"Battle"));
		skybox->AddComponent(make_shared<Transform>());
		skybox->SetCheckFrustum(false);
		skybox->SetStatic(true);

		skybox->GetTransform()->SetLocalScale(Vec3(1.0f, 1.0f, 1.0f));
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

	shared_ptr<MeshData> FemaleSoldier_data = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\Character\\SA_Character_FemaleSoldier.bin", PLAYER); // MeshData* meshData

	vector<shared_ptr<GameObject>> FemaleSoldier = FemaleSoldier_data->Instantiate(PLAYER, NONE);

	shared_ptr<GameObject> player1 = FemaleSoldier[23];
	for (auto& gameObject : FemaleSoldier)
	{
		//gameObject->SetName(L"FemaleSoldier");
		gameObject->SetCheckFrustum(false);
		gameObject->SetStatic(false);
		gameObject->GetTransform()->FinalUpdate();
		AddGameObject(gameObject);
	}

	player1->GetTransform()->SetLocalPosition(Vec3(15, 100.f, 0));
	player1->AddComponent(make_shared<WeaponManager>());													// Add Weapon Manager
	player1->AddComponent(make_shared<PlayerStats>());
	player1->AddComponent(make_shared<CharacterController>(0.5, 3.0, 0.3f));
	player1->GetCharacterController()->OnEnable();
	player1->AddComponent(make_shared<PlayerScript>(_hwnd, islocal, _theirID, player1->GetCharacterController()));								// Add Player Controller
	_player.push_back(player1);
#pragma endregion

#pragma region Player2
	_theirID = 2;
	islocal = (_theirID == _myID);

	shared_ptr<MeshData> FemaleHero_data = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\Character\\SA_Character_FemaleHero.bin", PLAYER); // MeshData* meshData

	vector<shared_ptr<GameObject>> FemaleHero = FemaleHero_data->Instantiate(PLAYER);

	shared_ptr<GameObject> player2 = FemaleHero[23];
	for (auto& gameObject : FemaleHero)
	{	
		//gameObject->SetName(L"FemaleSoldier");
		gameObject->SetCheckFrustum(false);
		gameObject->SetStatic(true);
		gameObject->GetTransform()->FinalUpdate();
		AddGameObject(gameObject);
	}

	player2->GetTransform()->SetLocalPosition(Vec3(20, 300.f, 0));
	//player2->AddComponent(make_shared<WeaponManager>());													// Add Weapon Manager
	player2->AddComponent(make_shared<PlayerStats>());
	player2->AddComponent(make_shared<CharacterController>(0.5, 1.5, 0.3f));
	player2->GetCharacterController()->OnEnable();
	player2->AddComponent(make_shared<PlayerScript>(_hwnd, islocal, _theirID, player2->GetCharacterController()));										// Add Weapon Manager

	_player.push_back(player2);

#pragma endregion


#pragma region PlayerCamera
	{
		_playerCamera = make_shared<GameObject>();
		_playerCamera->SetName(L"Main_Camera");
		_playerCamera->AddComponent(make_shared<Transform>());
		_playerCamera->AddComponent(make_shared<Camera>());
		_playerCamera->GetTransform()->SetLocalPosition(Vec3(0.01f, 2.03f, -6.65f));
		_playerCamera->GetTransform()->LookAt(Vec3(0.f, 0.f, 1.f));
		uint8 layerIndex = LayerNameToIndex(L"UI");
		_playerCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI�� �� ����
		AddGameObject(_playerCamera);
	}
	_playerCamera->GetTransform()->SetParent(_player[_myID - 1]->GetTransform());						// Player���� Camera �� ���δ�.

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
		_uiCamera->GetCamera()->SetCullingMaskAll(); // �� ����
		_uiCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI�� Ŵ
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



#pragma region UI_Test
	//for (int32 i = 0; i < 6; i++)
	//{
	//	shared_ptr<GameObject> obj = make_shared<GameObject>();
	//	obj->SetLayerIndex(LayerNameToIndex(L"UI")); // UI
	//	obj->AddComponent(make_shared<Transform>());
	//	obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
	//	obj->GetTransform()->SetLocalPosition(Vec3(-350.f + (i * 120), 250.f, 500.f));
	//	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
	//	{
	//		shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
	//		meshRenderer->SetMesh(mesh);
	//	}
	//	{
	//		shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");

	//		shared_ptr<Texture> texture;
	//		if (i < 3)
	//			texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
	//		else if (i < 5)
	//			texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
	//		else
	//			texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);

	//		shared_ptr<Material> material = make_shared<Material>();
	//		material->SetShader(shader);
	//		material->SetTexture(0, texture);
	//		meshRenderer->SetMaterial(material);
	//	}
	//	obj->AddComponent(meshRenderer);
	//	AddGameObject(obj);
	//}
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
	//{
	//	for (int i = 0; i < 10; ++i)
	//	{
	//		shared_ptr<MeshData> Zombie = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Zombie_Cheerleader.bin", ZOMBIE); // MeshData* meshData

	//		vector<shared_ptr<GameObject>> gameObjects = Zombie->Instantiate(ZOMBIE);
	//		for (auto& gameObject : gameObjects)
	//		{
	//			gameObject->SetCheckFrustum(true);
	//			gameObject->SetStatic(true);
	//			AddGameObject(gameObject);
	//		}
	//			gameObjects[0]->GetTransform()->SetLocalPosition(Vec3(i * 5.0f, 0.0f, 0.0f));
	//			//gameObjects[0]->GetTransform()->SetLocalRotation(Vec3(0.f, 0.0f, 0.0f));
	//	}
	//}

#pragma endregion


#pragma region Map
	{
		shared_ptr<MeshData> scene = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\Map\\EnvDemo.bin"); // MeshData* meshData

		vector<shared_ptr<GameObject>> gameObjects = scene->Instantiate(OBJECT, MESH);

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetCheckFrustum(true);
			gameObject->SetStatic(false);
			AddGameObject(gameObject);
		}
	}

	{
		shared_ptr<MeshData> scene = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\Map\\BldDemo.bin"); // MeshData* meshData

		vector<shared_ptr<GameObject>> gameObjects = scene->Instantiate(OBJECT, MESH);

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetCheckFrustum(true);
			gameObject->SetStatic(true);
			AddGameObject(gameObject);
		}
	}

	{
		shared_ptr<MeshData> scene = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\Map\\PropDemo.bin"); // MeshData* meshData

		vector<shared_ptr<GameObject>> gameObjects = scene->Instantiate(OBJECT, MESH);

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetCheckFrustum(true);
			gameObject->SetStatic(true);
			AddGameObject(gameObject);
		}
	}

	{
		shared_ptr<MeshData> scene = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\Map\\Wall.bin"); // MeshData* meshData

		vector<shared_ptr<GameObject>> gameObjects = scene->Instantiate(OBJECT, BOX);

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetCheckFrustum(false);
			gameObject->SetStatic(true);
			AddGameObject(gameObject);
		}
	}

	{
		// ���⿡ �� ������Ʈ ����
		shared_ptr<MeshData> scene = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SkyDome.bin"); // MeshData* meshData

		vector<shared_ptr<GameObject>> gameObjects = scene->Instantiate(OBJECT, MESH);

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetCheckFrustum(true);
			gameObject->SetStatic(true);
			AddGameObject(gameObject);
			/*if (gameObject->GetName() == L"Sun_1")
				_sunObject = gameObject*/;
		}

		// �̷��� �ؾ� ���� �߾��� ��
		//_sunObject->GetTransform()->SetLocalRotation(Vec3(-45, 225, 0));
	}
#pragma endregion


#pragma region Directional Light
	{
		// �¾� ������Ʈ �̸� : Sun_1
		_mainLight = make_shared<GameObject>();
		_mainLight->AddComponent(make_shared<Transform>());
		_mainLight->AddComponent(make_shared<Light>());
		_mainLight->GetLight()->SetLightDirection(Vec3(0, -1.0, 0.f));
		_mainLight->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		_mainLight->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		_mainLight->GetLight()->SetAmbient(Vec3(0.2f, 0.2f, 0.2f));
		_mainLight->GetLight()->SetSpecular(Vec3(0.2f, 0.2f, 0.2f));
		_mainLight->GetLight()->SetSunObject(_sunObject);
		_mainLight->GetTransform()->SetLocalPosition(Vec3(0, 200, 50));
		//_sunObject->GetTransform()->LookAt(Vec3(0, -1, 0));
		AddGameObject(_mainLight);

	}

#pragma endregion

}




void BattleScene::Update()
{
	/*Vec3 pos = _player[1]->GetTransform()->GetLocalPosition();
	printf("%f %f %f\n", pos.x, pos.y, pos.z);*/



	Scene::Update();
	GET_SINGLE(PhysicsSystem)->Update(DELTA_TIME);
	//UpdateSunOrbit();


	Vec3 direction = Vec3(0.f, 100.f, 100.f) - _mainLight->GetTransform()->GetLocalPosition();
	direction.Normalize();

	_mainLight->GetLight()->SetLightDirection(Vec3(direction));
}



void BattleScene::CreateZombie()
{
	
}

void BattleScene::UpdateSunOrbit()
{
	// ���� �߽� (�� �߾�)
	Vec3 center = Vec3(0.0f, 100.0f, 100.0f);

	// ���� ������ (������ �Ÿ�)

	float orbitRadius = Vec3::Distance(Vec3(0, 100, 100), _sunObject->GetTransform()->GetLocalPosition()); // �߽ɿ��� 50 ���� �Ÿ� (�ʿ信 ���� ����)

	// ���� ȸ�� ���� (������ �� ����)
	static float currentAngle = 0.0f;
	float rotationSpeed = 20.0f; // �ʴ� 20��
	float angle = rotationSpeed * DELTA_TIME; // �̹� �������� ȸ�� ����
	currentAngle += angle; // ���� ȸ�� ����

	// �������� ��ȯ
	float radians = XMConvertToRadians(currentAngle);

	// ���ο� ��ġ ��� (xz ��鿡�� ���� ����, y�� ���� ����)
	Vec3 newPosition = center;
	newPosition.x = center.x + orbitRadius * cosf(radians); // x�� �̵�
	newPosition.y = _sunObject->GetTransform()->GetLocalPosition().y - 10;
	newPosition.z = center.z + orbitRadius * sinf(radians); // z�� �̵�
	// y���� center.y (100.0f)�� ����

	// _sunObject�� ��ġ ������Ʈ
	_sunObject->GetTransform()->SetLocalPosition(newPosition);

	// _sunObject�� �߽��� �ٶ󺸵��� ���� (�� �߾�: 0, 100, 100)
	_sunObject->GetTransform()->LookAt(center);



}
