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




bool BattleScene::isPlayerGrounded = false;


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
	//	_playerCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI는 안 찍음
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

	shared_ptr<MeshData> FemaleSoldier_data = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Character_FemaleSoldier.bin", PLAYER); // MeshData* meshData

	vector<shared_ptr<GameObject>> FemaleSoldier = FemaleSoldier_data->Instantiate(PLAYER);

	shared_ptr<GameObject> player1 = FemaleSoldier[23];
	for (auto& gameObject : FemaleSoldier)
	{
		//gameObject->SetName(L"FemaleSoldier");
		gameObject->SetCheckFrustum(false);
		gameObject->SetStatic(false);
		gameObject->GetTransform()->FinalUpdate();
		AddGameObject(gameObject);
	}

	player1->GetTransform()->SetLocalPosition(Vec3(15, 200.f, 0));
	Matrix mat = player1->GetTransform()->GetLocalMatrix();
	player1->AddComponent(make_shared<WeaponManager>());													// Add Weapon Manager
	player1->AddComponent(make_shared<PlayerStats>());
	player1->AddComponent(make_shared<CharacterController>(0.5, 1.0, 0.3f));
	player1->GetCharacterController()->OnEnable();
	player1->AddComponent(make_shared<PlayerScript>(_hwnd, islocal, _theirID, player1->GetCharacterController()));								// Add Player Controller
	_player.push_back(player1);
#pragma endregion

#pragma region Player2
	//_theirID = 2;
	//islocal = (_theirID == _myID);

	//shared_ptr<MeshData> FemaleHero_data = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Character_FemaleHero.bin", PLAYER); // MeshData* meshData

	//vector<shared_ptr<GameObject>> FemaleHero = FemaleHero_data->Instantiate(PLAYER);

	//shared_ptr<GameObject> player2 = FemaleHero[23];
	//for (auto& gameObject : FemaleHero)
	//{	
	//	//gameObject->SetName(L"FemaleSoldier");
	//	gameObject->SetCheckFrustum(false);
	//	gameObject->SetStatic(true);
	//	gameObject->GetTransform()->FinalUpdate();
	//	AddGameObject(gameObject);
	//}

	//player2->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
	//Vec3 pos2 = player2->GetTransform()->GetLocalPosition();
	////player2->AddComponent(make_shared<CapsuleCollider>(0.5f, 1.0f));										// Capsule Collider 생성
	////player2->AddComponent(make_shared<RigidBody>(0.0f, dynamic_pointer_cast<CapsuleCollider>(player2->GetCollider()), pos2, false));			// Rigid Body 생성
	////player2->GetRigidBody()->OnEnable();
	////player2->AddComponent(make_shared<PlayerScript>(_hwnd, islocal, _theirID));
	////player2->AddComponent(make_shared<WeaponManager>());													// Add Weapon Manager

	//_player.push_back(player2);

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
		_playerCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI는 안 찍음
		AddGameObject(_playerCamera);


	}
	_playerCamera->GetTransform()->SetParent(_player[_myID - 1]->GetTransform());						// Player에게 Camera 를 붙인다.

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
		_uiCamera->GetCamera()->SetCullingMaskAll(); // 다 끄고
		_uiCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI만 킴
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
		_mainLight = make_shared<GameObject>();
		_mainLight->AddComponent(make_shared<Transform>());
		_mainLight->GetTransform()->SetLocalPosition(Vec3(0, 150, 90));
		_mainLight->AddComponent(make_shared<Light>());
		_mainLight->GetLight()->SetLightDirection(Vec3(0, -1.0, -0.1f));
		_mainLight->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		_mainLight->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		_mainLight->GetLight()->SetAmbient(Vec3(0.2f, 0.2f, 0.2f));
		_mainLight->GetLight()->SetSpecular(Vec3(0.2f, 0.2f, 0.2f));
		AddGameObject(_mainLight);
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

		vector<shared_ptr<GameObject>> gameObjects = scene->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetCheckFrustum(true);
			gameObject->SetStatic(false);
			AddGameObject(gameObject);
		}
	}

	{
		shared_ptr<MeshData> scene = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\Map\\BldDemo.bin"); // MeshData* meshData

		vector<shared_ptr<GameObject>> gameObjects = scene->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetCheckFrustum(true);
			gameObject->SetStatic(false);
			AddGameObject(gameObject);
		}
	}

	{
		shared_ptr<MeshData> scene = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\Map\\PropDemo.bin"); // MeshData* meshData

		vector<shared_ptr<GameObject>> gameObjects = scene->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetCheckFrustum(true);
			gameObject->SetStatic(false);
			AddGameObject(gameObject);
		}
	}

	//{
	//	shared_ptr<MeshData> scene = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SkyDome.bin"); // MeshData* meshData

	//	vector<shared_ptr<GameObject>> gameObjects = scene->Instantiate();

	//	for (auto& gameObject : gameObjects)
	//	{
	//		gameObject->SetCheckFrustum(true);
	//		gameObject->SetStatic(false);
	//		AddGameObject(gameObject);
	//	}
	//}

}
#pragma endregion



void BattleScene::Update()
{
	/*Vec3 pos = _player[1]->GetTransform()->GetLocalPosition();
	printf("%f %f %f\n", pos.x, pos.y, pos.z);*/



	Scene::Update();
	GET_SINGLE(PhysicsSystem)->Update(DELTA_TIME);

	//CheckCollisions();

	/*if (GET_SINGLE(KeyInput)->GetButtonDown(KEY_TYPE::TAB))
		CreateZombie();*/


	/*if (GET_SINGLE(KeyInput)->GetButton(KEY_TYPE::UP))
	{
		Vec3 Pos = _mainLight->GetTransform()->GetLocalPosition();
		Pos.z += 5.f;
		_mainLight->GetTransform()->SetLocalPosition(Pos);
	}


	if (GET_SINGLE(KeyInput)->GetButton(KEY_TYPE::DOWN))
	{
		Vec3 Pos = _mainLight->GetTransform()->GetLocalPosition();
		Pos.z -= 5.f;
		_mainLight->GetTransform()->SetLocalPosition(Pos);
	}


	if (GET_SINGLE(KeyInput)->GetButton(KEY_TYPE::LEFT))
	{
		Vec3 Pos = _mainLight->GetTransform()->GetLocalPosition();
		Pos.x -= 5.f;
		_mainLight->GetTransform()->SetLocalPosition(Pos);
	}


	if (GET_SINGLE(KeyInput)->GetButton(KEY_TYPE::RIGHT))
	{
		Vec3 Pos = _mainLight->GetTransform()->GetLocalPosition();
		Pos.x += 5.f;
		_mainLight->GetTransform()->SetLocalPosition(Pos);
	}*/


	//{
	//	Vec3 Pos = _mainLight->GetTransform()->GetLocalPosition();
	//	float angle = XMConvertToRadians(1.0f); // 10도 회전 예시
	//	Vec3 center = Vec3(0.f, Pos.y, 0.f);
	//	Vec3 relativePos = Pos - center;
	//	float cosTheta = cos(angle);
	//	float sinTheta = sin(angle);
	//	Vec3 rotatedPos;
	//	rotatedPos.x = relativePos.x * cosTheta - relativePos.z * sinTheta;
	//	rotatedPos.y = relativePos.y; // y축은 그대로 유지
	//	rotatedPos.z = relativePos.x * sinTheta + relativePos.z * cosTheta;
	//	rotatedPos += center;
	//	_mainLight->GetTransform()->SetLocalPosition(rotatedPos);
	//}

	// 라이트가 (0,0,0)을 계속 보도록 설정
	Vec3 direction = Vec3(0.f, 0.f, 0.f) - _mainLight->GetTransform()->GetLocalPosition();
	direction.Normalize();

	_mainLight->GetLight()->SetLightDirection(Vec3(direction));

}


	
// 축에 투영하는 헬퍼 함수
void ProjectBoxOntoAxis(const BoundingOrientedBox& box, const Vec3& axis, float& min_, float& max_)
{
	Vec3 corners[8];
	box.GetCorners(corners);

	min_ = max_ = corners[0].Dot(axis);

		
	for (int i = 1; i < 8; i++)
	{
		float projection = corners[i].Dot(axis);
		min_ = min(min_, projection);
		max_ = max(max_, projection);
	}
}

// 겹침 정도 계산 헬퍼 함수
float GetOverlap(float minA, float maxA, float minB, float maxB)
{
	if (minA > maxB || minB > maxA)
		return 0.0f; // 분리됨
	return min(maxA, maxB) - max(minA, minB);
}


void BattleScene::CheckCollisions()
{
	//shared_ptr<GameObject> playerRootObject = _player[0];
	//vector<shared_ptr<GameObject>> allGameObjects = GetGameObjects();

	//shared_ptr<BoxCollider> playerCollider = dynamic_pointer_cast<BoxCollider>(_player[0]->GetCollider());
	//BoundingOrientedBox playerBox = playerCollider->GetBoundingBox();

	////{
	////	shared_ptr<MeshData> scene = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\Wall2.bin"); // MeshData* meshData
	//Vec3 totalMTV = Vec3(0, 0, 0); // 모든 MTV를 합산
	//bool hasCollision = false;

	//for (auto& otherObject : allGameObjects)
	//{
	//	shared_ptr<BoxCollider> otherCollider = dynamic_pointer_cast<BoxCollider>(otherObject->GetCollider());

	//	if (!otherCollider || otherCollider == playerCollider)
	//		continue;

	//	BoundingOrientedBox otherBox = otherCollider->GetBoundingBox();

	//	if (playerBox.Intersects(otherBox))
	//	{
	//		hasCollision = true;

	//		Vec3 mtv;
	//		float minOverlap = FLT_MAX;
	//		Vec3 collisionNormal;

	//		// 분리 축 설정 (총 15개: 각 상자 3개 + 외적 9개)
	//		Vec3 axes[15];

	//		// playerBox의 Orientation
	//		XMFLOAT4 playerQuat = playerBox.Orientation;
	//		XMMATRIX playerRotationMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&playerQuat));
	//		XMFLOAT4X4 playerOrientation;
	//		XMStoreFloat4x4(&playerOrientation, playerRotationMatrix);

	//		axes[0] = Vec3(playerOrientation._11, playerOrientation._12, playerOrientation._13); // Right
	//		axes[1] = Vec3(playerOrientation._21, playerOrientation._22, playerOrientation._23); // Up
	//		axes[2] = Vec3(playerOrientation._31, playerOrientation._32, playerOrientation._33); // Forward

	//		// otherBox의 Orientation
	//		XMFLOAT4 otherQuat = otherBox.Orientation;
	//		XMMATRIX otherRotationMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&otherQuat));
	//		XMFLOAT4X4 otherOrientation;
	//		XMStoreFloat4x4(&otherOrientation, otherRotationMatrix);

	//		axes[3] = Vec3(otherOrientation._11, otherOrientation._12, otherOrientation._13); // Right
	//		axes[4] = Vec3(otherOrientation._21, otherOrientation._22, otherOrientation._23); // Up
	//		axes[5] = Vec3(otherOrientation._31, otherOrientation._32, otherOrientation._33); // Forward

	//		// 외적 축 추가
	//		int axisIndex = 6;
	//		for (int i = 0; i < 3; i++)
	//		{
	//			for (int j = 0; j < 3; j++)
	//			{
	//				axes[axisIndex] = axes[i].Cross(axes[j + 3]);
	//				axisIndex++;
	//			}
	//		}

	//		// SAT 적용
	//		for (int i = 0; i < 15; i++)
	//		{
	//			if (axes[i].LengthSquared() < 0.0001f) // 유효하지 않은 축 건너뛰기
	//			{
	//				continue;
	//			}
	//			axes[i].Normalize();

	//			// 두 박스를 축에 투영
	//			float playerMin, playerMax, otherMin, otherMax;
	//			ProjectBoxOntoAxis(playerBox, axes[i], playerMin, playerMax);
	//			ProjectBoxOntoAxis(otherBox, axes[i], otherMin, otherMax);

	//			float overlap = GetOverlap(playerMin, playerMax, otherMin, otherMax);
	//			if (overlap <= 0) // 분리된 경우 충돌 없음
	//				return;

	//			if (overlap < minOverlap)
	//			{
	//				minOverlap = overlap;
	//				collisionNormal = axes[i];
	//				mtv = collisionNormal * overlap;

	//				// MTV 방향 보정
	//				Vec3 toOther;
	//				toOther.x = otherBox.Center.x - playerBox.Center.x;
	//				toOther.y = otherBox.Center.y - playerBox.Center.y;
	//				toOther.z = otherBox.Center.z - playerBox.Center.z;
	//				if (toOther.Dot(collisionNormal) > 0)
	//					mtv = -mtv;
	//			}
	//		}

	//		// MTV 합산
	//		totalMTV += mtv;

	//		//RemoveGameObject(otherObject);
	//	}
	//}
	//		gameObject->SetStatic(true);
	//		AddGameObject(gameObject);
	//	}
	//	// MTV 크기 제한 (플레이어가 과도하게 밀려나는 것을 방지)
	//	float maxMTVLength = 1.0f; // 최대 MTV 크기
	//	if (totalMTV.Length() > maxMTVLength)

	//	vector<shared_ptr<GameObject>> gameObjects = scene->Instantiate();

	//	for (auto& gameObject : gameObjects)
	//	{
	//	// 플레이어 위치 보정
	//	Vec3 newPosition = playerRootObject->GetTransform()->GetLocalPosition() + totalMTV;
	//	playerRootObject->GetTransform()->SetLocalPosition(newPosition);






	//	if (totalMTV.y > 0.01f) // MTV가 아래에서 위로 (즉, 플레이어가 위에서 떨어졌다는 뜻)
	//	{
	//		isPlayerGrounded = true;
	//shared_ptr<MeshData> Zombie = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Zombie_Cheerleader.bin"); // MeshData* meshData

	//vector<shared_ptr<GameObject>> zombies = Zombie->Instantiate();

	//for (auto& gameObject : zombies)
	//{
	//	//gameObject->SetName(L"FemaleSoldier");
	//	gameObject->SetCheckFrustum(true);
	//	gameObject->SetStatic(false);
	//	AddGameObject(gameObject); 
	//}

	//shared_ptr<GameObject> rootObject = gameObjects[0];
	//rootObject->GetTransform()->SetLocalPosition(Vec3(rand() % 100, rand() % 100, rand() % 100));
	//rootObject->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
	//rootObject->SetLayerIndex(LayerNameToIndex(L"Battle"));
}

void BattleScene::CreateZombie()
{
	
}