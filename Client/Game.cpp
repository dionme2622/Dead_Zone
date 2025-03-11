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
	/* ------------------------------ Ű �Է� Debug -----------------------------------------*/
	if (INPUT->GetButtonDown(KEY_TYPE::ESC)) exit(0);
	else if (INPUT->GetButtonDown(KEY_TYPE::B))
	{
		if (_WIRE_FRAME_MODE) _WIRE_FRAME_MODE = false;
		else _WIRE_FRAME_MODE = true;
	}
	/* --------------------------------------------------------------------------------------*/

	GEngine->Update();
}
