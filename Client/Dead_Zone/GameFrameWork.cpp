#include "pch.h"
#include "GameFrameWork.h"
#include "Engine.h"
#include "SceneManager.h"
#include "KeyInput.h"

void GameFrameWork::Init(const WindowInfo& info)
{
	if (!ConnectAndLogin()) {
		std::cout << "���� ���� �Ǵ� �α��� ����\n";
		return;
	}
	std::thread(recv_thread, sock).detach();


	// DirectX12 ��ġ�ʱ�ȭ
	GEngine->Init(info);

	// TODO : Scene Load
	GET_SINGLE(SceneManager)->LoadScene(SCENETYPE::EBATTLESCENE);
}

void GameFrameWork::Update()
{
	/* ------------------------------ Ű �Է� Debug -----------------------------------------*/
	if (INPUT->GetButtonDown(KEY_TYPE::ESC)) exit(0);

	if (INPUT->GetButtonDown(KEY_TYPE::B))
		_WIRE_FRAME_MODE = !_WIRE_FRAME_MODE;
	if (INPUT->GetButtonDown(KEY_TYPE::C))
		_DEBUG_COLLIDER = !_DEBUG_COLLIDER;
	/* --------------------------------------------------------------------------------------*/

	GEngine->Update();
}
