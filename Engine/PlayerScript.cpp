#include "pch.h"
#include "PlayerScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "KeyInput.h"
#include "MouseInput.h"
#include "Timer.h"
#include "Engine.h"
#include "WeaponManager.h"


PlayerScript::PlayerScript(HWND hwnd, bool isLocal, int playerId)
{
	_hwnd = hwnd;
	_isLocal = isLocal;
	_playerId = playerId;
	// Player에 대한 정보 초기화 단계

	_speed = 50.0f;
	_jumpVelocity = 500.0f;
	_currentVelocity = 0.0f;
	_gravity = 9.8f;
	_isGrounded = true;


}

PlayerScript::~PlayerScript()
{
}

void PlayerScript::LateUpdate()
{
	

	//UpdatePlayerOnTerrain();

	if (_isLocal)
	{
		// 1) 로컬 입력 읽어서 내 캐릭터 움직임 계산 → 클라이언트 예측
		UpdatePlayerInput();
	}
	else
	{
		// 2) 원격 플레이어: 네트워크 상태를 받아서 Transform에 적용
		// TODO : 여기서 다른 플레이어 객체들의 pos 값을 받아와서 GetTransform()->SetLocalPosition(pos); 을 하면 된다.
		// EX) 서버로 부터 자기 ID에 맞는 객체의 pos 값을 받아와서 GetTransform()->SetLocalPosition(pos); 을 하면 된다.
		std::lock_guard<std::mutex> lock(g_posMutex);

		std::cout << "[LateUpdate] 전체 위치 정보\n";
		long long key = _playerId-1;
		auto it = g_otherPlayerPositions.find(key);
		if (it != g_otherPlayerPositions.end())
		{
			auto [x, y, z] = it->second;
			std::cout << "Player 1 위치: (" << x << ", " << y << ", " << z << ")\n";
			GetTransform()->SetLocalPosition(Vec3(x, y, z));
		}
		else
		{
			std::cout << "키 1인 항목이 없습니다.\n";
		}
		/*for (const auto& [id, pos] : g_otherPlayerPositions)
		{
			float x = std::get<0>(pos);
			float y = std::get<1>(pos);
			float z = std::get<2>(pos);

			std::cout << "- ID: " << id << " 위치: (" << x << ", " << y << ", " << z << ")\n";
			GetTransform()->SetLocalPosition(Vec3(x, y, z));
		}*/


		
		/*auto state = NetworkManager::Get()->GetPlayerState(_playerId);
		ApplyNetworkState(state.position, state.rotationEuler, state.equippedWeapon);*/
	}

	

}



void PlayerScript::UpdatePlayerInput()
{
	UpdateKeyInput();

	UpdateMouseInput();
}

void PlayerScript::UpdateKeyInput()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::A))
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::D))
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::CTRL))
		pos -= GetTransform()->GetUp() * _speed * DELTA_TIME;

	if (_isGrounded && INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		_currentVelocity = _jumpVelocity;
		pos.y += _currentVelocity * DELTA_TIME;
		_isGrounded = false;
	}

	if (INPUT->GetButton(KEY_TYPE::KEY_1))
		GetWeaponManager()->EquipWeapon(0);
	
	if (INPUT->GetButton(KEY_TYPE::KEY_2))
		GetWeaponManager()->EquipWeapon(1);
	
	if (INPUT->GetButton(KEY_TYPE::KEY_3))
		GetWeaponManager()->EquipWeapon(2);


	// TODO : pos 값을 서버로 보낸다.

	ctos_packet_position ctos_pos{};
	ctos_pos.size = sizeof(ctos_pos);
	ctos_pos.type = CToS_PLAYER_POS;
	ctos_pos.x = pos.x;
	ctos_pos.y = pos.y;
	ctos_pos.z = pos.z;
	send(sock, reinterpret_cast<char*>(&ctos_pos), sizeof(ctos_pos), 0);
	//std::this_thread::sleep_for(std::chrono::seconds(1));
	 
	// TODO : pos 값을 서버로 받아.


	GetTransform()->SetLocalPosition(pos);
}


void PlayerScript::UpdateMouseInput()
{
	if (INPUT->GetButtonDown(KEY_TYPE::SHIFT))
		_mouseMove = !_mouseMove;

	POINT mousePos;
	SetCapture(_hwnd);
	GetCursorPos(&mousePos);
	ScreenToClient(_hwnd, &mousePos);

	POINT center = { WINDOW.width / 2, WINDOW.height / 2 };
	float deltaX = (float)(mousePos.x - center.x);
	float deltaY = (float)(mousePos.y - center.y);

	if (_mouseMove == false)
	{
		if (deltaX != 0 || deltaY != 0)
		{
			UpdateRotation(deltaX, deltaY);
		}

		// 마우스 위치를 중앙으로 고정
		POINT screenCenter = { center.x, center.y };
		ClientToScreen(_hwnd, &screenCenter);
		SetCursorPos(screenCenter.x, screenCenter.y);

	}

}


void PlayerScript::UpdateRotation(float deltaX, float deltaY)
{
	// X축 회전 (Pitch, 위아래)
	_pitch += deltaY * sensitivity;
	_pitch = max(-90 * XM_PI / 180, min(90 * XM_PI / 180, _pitch));

	_yaw += deltaX * sensitivity;

	rotation.x = _pitch * 50;
	rotation.y = _yaw * 50;
	rotation.z = 0.0;

	GetTransform()->SetLocalRotation(rotation);
}

void PlayerScript::UpdatePlayerOnTerrain()
{
	Vec3 pos = GetTransform()->GetLocalPosition();
	float terrainHeight = -130.f;  // 수정해야됨

	if (pos.y <= terrainHeight)
	{
		pos.y = terrainHeight;
		_currentVelocity = 0.0f;
		_isGrounded = true;
	}
	else
	{
		_currentVelocity -= _gravity * DELTA_TIME * 100;
		pos.y += _currentVelocity * DELTA_TIME;
		_isGrounded = false;
	}
	GetTransform()->SetLocalPosition(pos);
}




