#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"

#include "TestCameraScript.h"
#include "Resources.h"
#include "KeyInput.h"
#include "Player.h"
#include "Timer.h"

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();


	if (INPUT->GetButtonDown(KEY_TYPE::LEFTCLICK))
	{
		LoadScene(SCENETYPE((ACTIVESCENE + 1)));
	}
}

// TEMP
void SceneManager::Render()
{
	if (_activeScene)
		_activeScene->Render();
}

void SceneManager::LoadScene(SCENETYPE sceneName)
{
	switch (sceneName)
	{
	case SCENETYPE::ELOBBYSCENE:
		_lobbyScene = make_shared<LobbyScene>();
		_activeScene = static_pointer_cast<Scene>(_lobbyScene);
		break;
	case SCENETYPE::EBATTLESCENE:
		_battleScene = make_shared<BattleScene>();
		_activeScene = static_pointer_cast<Scene>(_battleScene);
		break;
	}

	_activeScene->LoadScene();
	_activeScene->Awake();
	_activeScene->Start();
}

//void SceneManager::SetLayerName(uint8 index, const wstring& name)
//{
//	// ±âÁ¸ µ¥ÀÌÅÍ »èÁ¦
//	const wstring& prevName = _layerNames[index];
//	_layerIndex.erase(prevName);
//
//	_layerNames[index] = name;
//	_layerIndex[name] = index;
//}
//
//uint8 SceneManager::LayerNameToIndex(const wstring& name)
//{
//	auto findIt = _layerIndex.find(name);
//	if (findIt == _layerIndex.end())
//		return 0;
//
//	return findIt->second;
//}



//shared_ptr<Scene> SceneManager::LoadLobbyScene()
//{
//#pragma region LayerMask
//	SetLayerName(0, L"Lobby");
//	SetLayerName(1, L"UI");
//#pragma endregion
//
//	shared_ptr<Scene> scene = make_shared<Scene>();
//
//#pragma region Camera
//	{
//		shared_ptr<GameObject> camera = make_shared<GameObject>();
//		camera->SetName(L"Main_Camera");
//		camera->AddComponent(make_shared<Transform>());
//		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45µµ
//		camera->AddComponent(make_shared<TestCameraScript>());
//		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
//		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
//		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI´Â ¾È ÂïÀ½
//		scene->AddGameObject(camera);
//	}
//#pragma endregion
//
//#pragma region UI_Camera
//	{
//		shared_ptr<GameObject> camera = make_shared<GameObject>();
//		camera->SetName(L"Orthographic_Camera");
//		camera->AddComponent(make_shared<Transform>());
//		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, 800*600
//		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
//		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
//		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
//		camera->GetCamera()->SetCullingMaskAll(); // ´Ù ²ô°í
//		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI¸¸ ÂïÀ½
//		scene->AddGameObject(camera);
//	}
//#pragma endregion
//
//#pragma region SkyBox
//	{
//		shared_ptr<GameObject> skybox = make_shared<GameObject>();
//		skybox->AddComponent(make_shared<Transform>());
//		skybox->SetCheckFrustum(false);
//		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
//			meshRenderer->SetMesh(sphereMesh);
//		}
//		{
//			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");
//			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resources\\Texture\\Sky01.jpg");
//			shared_ptr<Material> material = make_shared<Material>();
//			material->SetShader(shader);
//			material->SetTexture(0, texture);
//			meshRenderer->SetMaterial(material);
//		}
//		skybox->AddComponent(meshRenderer);
//		scene->AddGameObject(skybox);
//	}
//#pragma endregion
//
//#pragma region Object
//	{
//		shared_ptr<GameObject> obj = make_shared<GameObject>();
//		obj->AddComponent(make_shared<Transform>());
//		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
//		obj->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 150.f));
//		obj->SetStatic(false);
//		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
//			meshRenderer->SetMesh(sphereMesh);
//		}
//		{
//			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
//			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Cliff_Rock", L"..\\Resources\\Texture\\Cliff_Rock_basecolor.png");
//			shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Cliff_Rock_Normal", L"..\\Resources\\Texture\\Cliff_Rock_normal.png");
//			shared_ptr<Material> material = make_shared<Material>();
//			material->SetShader(shader);
//			material->SetTexture(0, texture);
//			material->SetTexture(1, texture2);
//			meshRenderer->SetMaterial(material);
//		}
//		obj->AddComponent(meshRenderer);
//		scene->AddGameObject(obj);
//	}
//
//
//#pragma endregion
//
//
//#pragma region UI_Test
//	for (int32 i = 0; i < 6; i++)
//	{
//		shared_ptr<GameObject> obj = make_shared<GameObject>();
//		obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
//		obj->AddComponent(make_shared<Transform>());
//		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
//		obj->GetTransform()->SetLocalPosition(Vec3(-350.f + (i * 120), 250.f, 500.f));
//		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
//			meshRenderer->SetMesh(mesh);
//		}
//		{
//			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
//
//			shared_ptr<Texture> texture;
//			if (i < 3)
//				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
//			else if (i < 5)
//				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
//			else
//				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);
//
//			shared_ptr<Material> material = make_shared<Material>();
//			material->SetShader(shader);
//			material->SetTexture(0, texture);
//			meshRenderer->SetMaterial(material);
//		}
//		obj->AddComponent(meshRenderer);
//		scene->AddGameObject(obj);
//	}
//#pragma endregion
//
//#pragma region Directional Light
//	{
//		shared_ptr<GameObject> light = make_shared<GameObject>();
//		light->AddComponent(make_shared<Transform>());
//		light->GetTransform()->SetLocalPosition(Vec3(0, 1000, 500));
//		light->AddComponent(make_shared<Light>());
//		light->GetLight()->SetLightDirection(Vec3(0, -1, 0.f));
//		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
//		light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
//		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
//		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
//
//		scene->AddGameObject(light);
//	}
//#pragma endregion
//	return scene;
//
//}
//
//shared_ptr<Scene> SceneManager::LoadBattleScene()
//{
//#pragma region LayerMask
//	SetLayerName(0, L"Battle");
//	SetLayerName(1, L"UI");
//#pragma endregion
//
//	shared_ptr<Scene> scene = make_shared<Scene>();
//
//#pragma region Camera
//	{
//		shared_ptr<GameObject> camera = make_shared<GameObject>();
//		camera->SetName(L"Main_Camera");
//		camera->AddComponent(make_shared<Transform>());
//		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45µµ
//		camera->AddComponent(make_shared<TestCameraScript>());
//		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
//		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
//		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI´Â ¾È ÂïÀ½
//		scene->AddGameObject(camera);
//	}
//#pragma endregion
//
//#pragma region UI_Camera
//	{
//		shared_ptr<GameObject> camera = make_shared<GameObject>();
//		camera->SetName(L"Orthographic_Camera");
//		camera->AddComponent(make_shared<Transform>());
//		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, 800*600
//		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
//		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
//		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
//		camera->GetCamera()->SetCullingMaskAll(); // ´Ù ²ô°í
//		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI¸¸ ÂïÀ½
//		scene->AddGameObject(camera);
//	}
//#pragma endregion
//
//#pragma region SkyBox
//	{
//		shared_ptr<GameObject> skybox = make_shared<GameObject>();
//		skybox->AddComponent(make_shared<Transform>());
//		skybox->SetCheckFrustum(false);
//		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
//			meshRenderer->SetMesh(sphereMesh);
//		}
//		{
//			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");
//			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resources\\Texture\\Sky01.jpg");
//			shared_ptr<Material> material = make_shared<Material>();
//			material->SetShader(shader);
//			material->SetTexture(0, texture);
//			meshRenderer->SetMaterial(material);
//		}
//		skybox->AddComponent(meshRenderer);
//		scene->AddGameObject(skybox);
//	}
//#pragma endregion
//
//#pragma region Object
//	{
//		/*shared_ptr<GameObject> obj = make_shared<GameObject>();
//		obj->AddComponent(make_shared<Transform>());
//		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
//		obj->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 150.f));
//		obj->SetStatic(false);
//		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
//			meshRenderer->SetMesh(sphereMesh);
//		}
//		{
//			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
//			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Cliff_Rock", L"..\\Resources\\Texture\\Cliff_Rock_basecolor.png");
//			shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Cliff_Rock_Normal", L"..\\Resources\\Texture\\Cliff_Rock_normal.png");
//			shared_ptr<Material> material = make_shared<Material>();
//			material->SetShader(shader);
//			material->SetTexture(0, texture);
//			material->SetTexture(1, texture2);
//			meshRenderer->SetMaterial(material);
//		}
//		obj->AddComponent(meshRenderer);
//		scene->AddGameObject(obj);*/
//	}
//
//
//#pragma endregion
//
//#pragma region Plane
//	{
//		shared_ptr<GameObject> obj = make_shared<GameObject>();
//		obj->AddComponent(make_shared<Transform>());
//		obj->GetTransform()->SetLocalScale(Vec3(1000.f, 1.f, 1000.f));
//		obj->GetTransform()->SetLocalPosition(Vec3(0.f, -100.f, 500.f));
//		obj->SetStatic(true);
//		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadCubeMesh();
//			meshRenderer->SetMesh(sphereMesh);
//		}
//		{
//			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
//			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Metal_Pattern", L"..\\Resources\\Texture\\Metal_Pattern_basecolor.png");
//			shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Metal_Pattern_Normal", L"..\\Resources\\Texture\\Metal_Pattern_normal.png");
//			shared_ptr<Material> material = make_shared<Material>();
//			material->SetShader(shader);
//			material->SetTexture(0, texture);
//			material->SetTexture(1, texture2);
//			meshRenderer->SetMaterial(material);
//		}
//		obj->AddComponent(meshRenderer);
//		scene->AddGameObject(obj);
//	}
//#pragma endregion
//
//#pragma region Player
//	{
//		_player = make_shared<Player>();
//		_player->GetGameObject()->AddComponent(make_shared<Player>());
//		scene->AddGameObject(_player->GetGameObject());
//	}
//#pragma endregion
//
//#pragma region UI_Test
//	for (int32 i = 0; i < 6; i++)
//	{
//		shared_ptr<GameObject> obj = make_shared<GameObject>();
//		obj->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
//		obj->AddComponent(make_shared<Transform>());
//		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
//		obj->GetTransform()->SetLocalPosition(Vec3(-350.f + (i * 120), 250.f, 500.f));
//		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
//		{
//			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
//			meshRenderer->SetMesh(mesh);
//		}
//		{
//			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");
//
//			shared_ptr<Texture> texture;
//			if (i < 3)
//				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
//			else if (i < 5)
//				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);
//			else
//				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SHADOW)->GetRTTexture(0);
//
//			shared_ptr<Material> material = make_shared<Material>();
//			material->SetShader(shader);
//			material->SetTexture(0, texture);
//			meshRenderer->SetMaterial(material);
//		}
//		obj->AddComponent(meshRenderer);
//		scene->AddGameObject(obj);
//	}
//#pragma endregion
//
//#pragma region Directional Light
//	{
//		shared_ptr<GameObject> light = make_shared<GameObject>();
//		light->AddComponent(make_shared<Transform>());
//		light->GetTransform()->SetLocalPosition(Vec3(0, 1000, 500));
//		light->AddComponent(make_shared<Light>());
//		light->GetLight()->SetLightDirection(Vec3(0, -1, 0.f));
//		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
//		light->GetLight()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
//		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
//		light->GetLight()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
//
//		scene->AddGameObject(light);
//	}
//#pragma endregion
//	return scene;
//
//}

