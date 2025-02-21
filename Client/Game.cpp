#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "SceneManager.h"
#include "KeyInput.h"


void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);
	
	GET_SINGLE(SceneManager)->LoadScene(SCENETYPE::EBATTLESCENE);
}

void Game::Update()
{
	if (INPUT->GetButtonDown(KEY_TYPE::ESC)) exit(0);
	GEngine->Update();
}
