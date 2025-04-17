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

#include "Resources.h"
#include "KeyInput.h"
#include "PlayerStats.h"
#include "Timer.h"

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();


	/*if (INPUT->GetButtonDown(KEY_TYPE::LEFTCLICK))
	{
		LoadScene(SCENETYPE((ACTIVESCENE + 1)));
	}*/
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




