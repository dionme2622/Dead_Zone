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
	_isFirstFrame = true;
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
		_playerCamera->GetCamera()->SetName(L"Main_Camera");
		//_playerCamera->AddComponent(make_shared<PlayerScript>(_hwnd));
		_playerCamera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		_playerCamera->GetTransform()->LookAt(Vec3(0.f, 0.f, 1.f));
		uint8 layerIndex = LayerNameToIndex(L"UI");
		_playerCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI는 안 찍음
		AddGameObject(_playerCamera);
	}
#pragma endregion

#pragma region UI_Camera
	{
		_uiCamera = make_shared<GameObject>();
		_uiCamera->SetName(L"Orthographic_Camera");
		_uiCamera->AddComponent(make_shared<Transform>());
		_uiCamera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, 800*600
		_uiCamera->GetCamera()->SetName(L"UI_Camera");
		_uiCamera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		_uiCamera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = LayerNameToIndex(L"UI");
		_uiCamera->GetCamera()->SetCullingMaskAll(); // 다 끄고
		_uiCamera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI만 킴
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
	_player = make_shared<Player>();

	vector<shared_ptr<GameObject>> gameObjects = _player->GetGameObjects();

	for (auto& gameObject : gameObjects)
	{
		gameObject->SetCheckFrustum(false);
		gameObject->SetStatic(false);
		AddGameObject(gameObject);
	}
	shared_ptr<GameObject> rootObject = gameObjects[0];

	rootObject->GetTransform()->SetLocalPosition(Vec3(0.0, 0.0f, 0.f));
	rootObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

	rootObject->SetLayerIndex(LayerNameToIndex(L"Battle"));

	rootObject->AddComponent(make_shared<PlayerScript>(_hwnd, _playerCamera->GetTransform()));
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
		light->GetTransform()->SetLocalPosition(Vec3(0, 0, 0));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0, -1, 0.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
		//uint8 layerIndex = LayerNameToIndex(L"UI");
		//light->GetShadowCamera()->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI는 안 찍음
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
	//{
	//	shared_ptr<MeshData> FemaleSoldier = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Character_FemaleSoldier.bin"); // MeshData* meshData
	//	shared_ptr<MeshData> FemaleHero = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\SA_Character_FemaleHero.bin"); // MeshData* meshData

	//	vector<shared_ptr<GameObject>> gameObjects = FemaleSoldier->Instantiate();

	//	for (auto& gameObject : gameObjects)
	//	{
	//		gameObject->SetCheckFrustum(true);
	//		gameObject->SetStatic(true);
	//		AddGameObject(gameObject);
	//	}
	//	shared_ptr<GameObject> rootObject = gameObjects[0];

	//	rootObject->GetTransform()->SetLocalPosition(Vec3(0.0, -30.0f, 0.f));
	//	rootObject->GetTransform()->SetLocalScale(Vec3(10.f, 10.f, 10.f));
	//}


#pragma endregion

#pragma region Map
	{
		shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\Scene.bin"); // MeshData* meshData

		vector<shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetCheckFrustum(true);
			gameObject->SetStatic(true);
			AddGameObject(gameObject);
		}

		shared_ptr<GameObject> rootObject = gameObjects[0];

		rootObject->GetTransform()->SetLocalPosition(Vec3(0.0, -400.f, 0.f));
		rootObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	}
#pragma endregion
}

void BattleScene::Update()
{
	if (!_isFirstFrame)
	{
		CheckCollisions();
	}
	else
	{
		_isFirstFrame = false; // 첫 프레임을 넘겼음을 표시
	}



	Scene::Update();
}

void BattleScene::FinalUpdate()
{
	Scene::FinalUpdate();
}



void BattleScene::CheckCollisions()
{
	vector<shared_ptr<GameObject>> playerGameObjects = _player->GetGameObjects();
	vector<shared_ptr<GameObject>> allGameObjects = GetGameObjects();

	printf("playerPosY : %f\n", _player->GetGameObjects()[0]->GetTransform()->GetLocalPosition().y);

	for (auto& gameObject : playerGameObjects)
	{
		shared_ptr<BoxCollider> playerCollider = dynamic_pointer_cast<BoxCollider>(gameObject->GetCollider());
		if (!playerCollider)
			continue;

		for (auto& otherObject : allGameObjects)
		{
			if (gameObject == otherObject)
				continue;

			shared_ptr<BoxCollider> otherCollider = dynamic_pointer_cast<BoxCollider>(otherObject->GetCollider());
			if (!otherCollider)
				continue;

			BoundingOrientedBox playerBox = playerCollider->GetBoundingBox();
			BoundingOrientedBox otherBox = otherCollider->GetBoundingBox();

			if (playerBox.Intersects(otherBox))
			{
				//exit(0);
				//RemoveGameObject(otherObject);

				// 충돌 처리 로직: 플레이어 오브젝트가 겹치지 않도록 위치 조정
				Vec3 playerPos = _player->GetGameObjects()[0]->GetTransform()->GetLocalPosition();
				Vec3 otherPos = otherObject->GetTransform()->GetLocalPosition();
				Vec3 direction = playerPos - otherPos;
				direction.Normalize();

				// 겹침 정도 계산
				float overlapX = (playerBox.Extents.x + otherBox.Extents.x * gameObject->GetTransform()->GetLocalScale().x) - abs(playerPos.x - otherPos.x);
				float overlapY = (playerBox.Extents.y + otherBox.Extents.y * gameObject->GetTransform()->GetLocalScale().y) - abs(playerPos.y - otherPos.y);
				float overlapZ = (playerBox.Extents.z + otherBox.Extents.z * gameObject->GetTransform()->GetLocalScale().z) - abs(playerPos.z - otherPos.z);

				Vec3 overlap = Vec3(overlapX, overlapY, overlapZ);

				printf("overlapX : %f, overlapZ : %f, overlapZ : %f\n", otherBox.Extents.x, otherBox.Extents.y, otherBox.Extents.z);

				// 가장 작은 겹침 축을 선택하여 플레이어 오브젝트를 밀어냄
				/*if (overlapX < overlapY && overlapX < overlapZ)
				{
					float directionX = (playerPos.x > otherPos.x) ? 1.0f : -1.0f;
					_player->GetGameObjects()[0]->GetTransform()->SetLocalPosition(playerPos + Vec3(directionX * overlapX, 0, 0));
				}
				else if (overlapY < overlapX && overlapY < overlapZ)
				{
					float directionY = (playerPos.y > otherPos.y) ? 1.0f : -1.0f;
					_player->GetGameObjects()[0]->GetTransform()->SetLocalPosition(playerPos + Vec3(0, directionY * overlapY, 0));
				}
				else
				{
					float directionZ = (playerPos.z > otherPos.z) ? 1.0f : -1.0f;
					_player->GetGameObjects()[0]->GetTransform()->SetLocalPosition(playerPos + Vec3(0, 0, directionZ * overlapZ));
				}*/

				Vec3 direction_ = Vec3((playerPos.x > otherPos.x) ? 1.0f : -1.0f, 
										(playerPos.y > otherPos.y) ? 1.0f : -1.0f, 
										(playerPos.z > otherPos.z) ? 1.0f : -1.0f);
				_player->GetGameObjects()[0]->GetTransform()->SetLocalPosition(playerPos + Vec3(direction_ * overlap / 100));

				//_player->GetGameObjects()[0]->GetTransform()->SetLocalPosition(playerPos + Vec3(0, 0, 0));

			}
		}
	}
}







