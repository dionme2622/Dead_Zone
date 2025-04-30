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




bool BattleScene::isPlayerGrounded = false;


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
		_playerCamera->GetCamera()->SetName(L"Main_Camera");
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

	rootObject->GetTransform()->SetLocalPosition(Vec3(0, 200, 60));
	rootObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));

	rootObject->SetLayerIndex(LayerNameToIndex(L"Battle"));

	rootObject->AddComponent(make_shared<PlayerScript>(_hwnd, _playerCamera->GetTransform()));
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
		_mainLight->GetTransform()->SetLocalPosition(Vec3(0, 90, 60));
		_mainLight->AddComponent(make_shared<Light>());
		_mainLight->GetLight()->SetLightDirection(Vec3(0, -1, 0.f));
		_mainLight->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		_mainLight->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
		_mainLight->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		_mainLight->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
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
	//		/*else
	//			texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SSAO)->GetRTTexture(0);*/

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

#pragma region Map
	{
		shared_ptr<MeshData> scene = GET_SINGLE(Resources)->LoadModelFromBinary(L"..\\Resources\\Model\\EnvDemo.bin"); // MeshData* meshData

		vector<shared_ptr<GameObject>> gameObjects = scene->Instantiate();

		for (auto& gameObject : gameObjects)
		{
			gameObject->SetCheckFrustum(true);
			gameObject->SetStatic(false);
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
			gameObject->SetStatic(false);
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
			gameObject->SetStatic(false);
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
			gameObject->SetStatic(false);
			AddGameObject(gameObject);
		}

		shared_ptr<GameObject> rootObject = gameObjects[0];

		//rootObject->GetTransform()->SetLocalPosition(Vec3(0.0, 0.f, 0.f));
		rootObject->GetTransform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
	}

	

	for (auto& gameObject : GetGameObjects()) {
		// 스케일이 음수일 경우 오브젝트 출력
		Vec3 scale = gameObject->GetTransform()->GetLocalScale();
		if (scale.x < 0.f || scale.y < 0.f || scale.z < 0.f) {
			wcout << "Negative Scale: " << gameObject->GetName() << endl;
			cout << scale.x << "," << scale.y << "," << scale.z << endl;


			// 스케일을 절댓값으로 바꾸기
			gameObject->GetTransform()->SetLocalScale(Vec3(abs(scale.x), abs(scale.y), abs(scale.z)));
		}
	}



	SetObjectArea();


#pragma endregion
}

void BattleScene::Update()
{
	Scene::Update();

	
	UpdatePlayerAreas();
	CheckCollisions();



	cout << _player->GetGameObjects()[0]->GetTransform()->GetLocalPosition().x << ","
		<< _player->GetGameObjects()[0]->GetTransform()->GetLocalPosition().z << endl;
}

void BattleScene::FinalUpdate()
{
	Scene::FinalUpdate();
}

void BattleScene::SetObjectArea()
{
	_areaObjects.clear();
	for (int i = 0; i <= static_cast<int>(STAGEAREA::FLAG_NONE); ++i) {
		_areaObjects[static_cast<STAGEAREA>(i)] = vector<shared_ptr<GameObject>>();
	}

	for (auto& gameObject : GetGameObjects()) {
		Vec3 position = gameObject->GetTransform()->GetLocalPosition();
		STAGEAREA area = CalculateObjectArea(position);
		gameObject->SetStageArea(area);
		_areaObjects[area].push_back(gameObject);
	}

}

void BattleScene::UpdatePlayerAreas()
{
	Vec3 position = _player->GetGameObjects()[0]->GetTransform()->GetLocalPosition();
	STAGEAREA area = CalculateObjectArea(position);
	_player->GetGameObjects()[0]->SetStageArea(area);
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
	shared_ptr<GameObject> playerRootObject = _player->GetGameObjects()[0];
	vector<shared_ptr<GameObject>> allGameObjects = GetGameObjects();

	shared_ptr<BoxCollider> playerCollider = dynamic_pointer_cast<BoxCollider>(_player->GetGameObjects()[23]->GetCollider());
	BoundingOrientedBox playerBox = playerCollider->GetBoundingBox();

	Vec3 totalMTV = Vec3(0, 0, 0); // 모든 MTV를 합산
	bool hasCollision = false;

	STAGEAREA playerArea = playerRootObject->GetArea();

	for (auto areas : GetAdjacentAreas(playerArea)) {
		for (auto& otherObject : _areaObjects[areas])
		{
			shared_ptr<BoxCollider> otherCollider = dynamic_pointer_cast<BoxCollider>(otherObject->GetCollider());

			if (!otherCollider || otherCollider == playerCollider)
				continue;

			BoundingOrientedBox otherBox = otherCollider->GetBoundingBox();

			if (playerBox.Intersects(otherBox))
			{
				hasCollision = true;

				Vec3 mtv;
				float minOverlap = FLT_MAX;
				Vec3 collisionNormal;

				// 분리 축 설정 (총 15개: 각 상자 3개 + 외적 9개)
				Vec3 axes[15];

				// playerBox의 Orientation
				XMFLOAT4 playerQuat = playerBox.Orientation;
				XMMATRIX playerRotationMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&playerQuat));
				XMFLOAT4X4 playerOrientation;
				XMStoreFloat4x4(&playerOrientation, playerRotationMatrix);

				axes[0] = Vec3(playerOrientation._11, playerOrientation._12, playerOrientation._13); // Right
				axes[1] = Vec3(playerOrientation._21, playerOrientation._22, playerOrientation._23); // Up
				axes[2] = Vec3(playerOrientation._31, playerOrientation._32, playerOrientation._33); // Forward

				// otherBox의 Orientation
				XMFLOAT4 otherQuat = otherBox.Orientation;
				XMMATRIX otherRotationMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&otherQuat));
				XMFLOAT4X4 otherOrientation;
				XMStoreFloat4x4(&otherOrientation, otherRotationMatrix);

				axes[3] = Vec3(otherOrientation._11, otherOrientation._12, otherOrientation._13); // Right
				axes[4] = Vec3(otherOrientation._21, otherOrientation._22, otherOrientation._23); // Up
				axes[5] = Vec3(otherOrientation._31, otherOrientation._32, otherOrientation._33); // Forward

				// 외적 축 추가
				int axisIndex = 6;
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						axes[axisIndex] = axes[i].Cross(axes[j + 3]);
						axisIndex++;
					}
				}

				// SAT 적용
				for (int i = 0; i < 15; i++)
				{
					if (axes[i].LengthSquared() < 0.0001f) // 유효하지 않은 축 건너뛰기
					{
						//cout << "축이 무시됨\n" << endl;
						continue;
					}
					axes[i].Normalize();

					// 두 박스를 축에 투영
					float playerMin, playerMax, otherMin, otherMax;
					ProjectBoxOntoAxis(playerBox, axes[i], playerMin, playerMax);
					ProjectBoxOntoAxis(otherBox, axes[i], otherMin, otherMax);

					float overlap = GetOverlap(playerMin, playerMax, otherMin, otherMax);
					if (overlap <= 0) // 분리된 경우 충돌 없음
						return;

					if (overlap < minOverlap)
					{
						minOverlap = overlap;
						collisionNormal = axes[i];
						mtv = collisionNormal * overlap;

						// MTV 방향 보정
						Vec3 toOther;
						toOther.x = otherBox.Center.x - playerBox.Center.x;
						toOther.y = otherBox.Center.y - playerBox.Center.y;
						toOther.z = otherBox.Center.z - playerBox.Center.z;
						if (toOther.Dot(collisionNormal) > 0)
							mtv = -mtv;
					}
				}

				// MTV 합산
				totalMTV += mtv;

				//RemoveGameObject(otherObject);
			}
		}

		if (hasCollision)
		{
			// MTV 크기 제한 (플레이어가 과도하게 밀려나는 것을 방지)
			float maxMTVLength = 0.08f; // 최대 MTV 크기
			if (totalMTV.Length() > maxMTVLength)
			{
				totalMTV.Normalize();
				totalMTV *= maxMTVLength;
			}

			// 플레이어 위치 보정
			Vec3 newPosition = playerRootObject->GetTransform()->GetLocalPosition() + totalMTV;
			playerRootObject->GetTransform()->SetLocalPosition(newPosition);




			if (totalMTV.y > 0.01f) // MTV가 아래에서 위로 (즉, 플레이어가 위에서 떨어졌다는 뜻)
			{
				isPlayerGrounded = true;
			}
			else {
				isPlayerGrounded = false;
			}
		}
	}
}

vector<STAGEAREA> BattleScene::GetAdjacentAreas(STAGEAREA playerArea)
{
	vector<STAGEAREA> adjacentAreas;
	int areaIndex = static_cast<int>(playerArea);
	if (areaIndex == static_cast<int>(STAGEAREA::FLAG_NONE)) {
		return adjacentAreas; // 빈 리스트 반환
	}

	// 5x5 격자에서 영역 인덱스(0~24)를 행/열로 변환
	int row = areaIndex / 5; // 0~4
	int col = areaIndex % 5; // 0~4

	// 인접 영역(최대 8개 + 자신)을 추가
	for (int dr = -1; dr <= 1; dr++) {
		for (int dc = -1; dc <= 1; dc++) {
			int newRow = row + dr;
			int newCol = col + dc;
			if (newRow >= 0 && newRow < 5 && newCol >= 0 && newCol < 5) {
				int newIndex = newRow * 5 + newCol;
				adjacentAreas.push_back(static_cast<STAGEAREA>(newIndex));
			}
		}
	}

	return adjacentAreas;
}

STAGEAREA BattleScene::CalculateObjectArea(const Vec3& position)
{
	float x = position.x;
	float z = position.z;
	STAGEAREA area = STAGEAREA::FLAG_NONE;

	// x, z 범위 체크
	if (x >= -280 && x <= 360 && z >= -250 && z <= 360) {
		// x축 인덱스 계산
		int xIndex = -1;
		if (x >= -280 && x < -152) xIndex = 0;
		else if (x >= -152 && x < -24) xIndex = 1;
		else if (x >= -24 && x < 104) xIndex = 2;
		else if (x >= 104 && x < 232) xIndex = 3;
		else if (x >= 232 && x <= 360) xIndex = 4;

		// z축 인덱스 계산
		int zIndex = -1;
		if (z >= -250 && z < -128) zIndex = 0;
		else if (z >= -128 && z < -6) zIndex = 1;
		else if (z >= -6 && z < 116) zIndex = 2;
		else if (z >= 116 && z < 238) zIndex = 3;
		else if (z >= 238 && z <= 360) zIndex = 4;

		// 유효한 인덱스일 때 AREA 계산
		if (xIndex >= 0 && zIndex >= 0) {
			// AREA1 = (0,0), AREA2 = (1,0), ..., AREA25 = (4,4)
			int areaIndex = zIndex * 5 + xIndex;
			area = static_cast<STAGEAREA>(areaIndex);
		}
	}

	return area;
}

void BattleScene::CreateZombie()
{
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
