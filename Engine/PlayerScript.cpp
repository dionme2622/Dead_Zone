#include "pch.h"
#include "PlayerScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "KeyInput.h"
#include "MouseInput.h"
#include "Timer.h"
#include "Engine.h"
#include "BoxCollider.h"
#include "SceneManager.h"

PlayerScript::PlayerScript(HWND hwnd, shared_ptr<Transform> playerTransform) :
	_hwnd(hwnd), _speed(30.0f), _jumpVelocity(50.0f), _currentVelocity(0.0f), 
	_gravity(-9.8f), _isGrounded(true), _pitch(0.0f), _yaw(0.0f), _mouseMove(false), 
	_cameraTransform(playerTransform)
{
}

PlayerScript::~PlayerScript()
{
}

void PlayerScript::LateUpdate()
{
	UpdatePlayerInput();

	//UpdatePlayerOnTerrain();


	// 카메라 위치 업데이트
	/*Vec3 cameraPosition = GetTransform()->GetLocalPosition();
	_cameraTransform->SetLocalPosition(cameraPosition);*/

	//Vec3 cameraRotate = GetTransform()->GetLocalRotation();
	//_cameraTransform->SetLocalRotation(cameraRotate);


	// 디버그용
	// 플레이어의 위치와 룩 벡터를 가져옴
	Vec3 playerPosition = GetTransform()->GetLocalPosition();
	Vec3 lookVector = GetTransform()->GetLook();

	// 카메라가 플레이어의 뒤쪽에 위치하도록 설정 (예: 플레이어의 뒤쪽 5 단위 거리)
	Vec3 cameraOffset = lookVector * -7.0f; // 뒤쪽으로 5 단위 거리
	Vec3 cameraPosition = playerPosition + cameraOffset;
	//cameraPosition.y += 10;
	// 카메라 위치 업데이트
	_cameraTransform->SetLocalPosition(cameraPosition);

	// 카메라가 플레이어를 바라보도록 설정
	//_cameraTransform->LookAt(playerPosition);

	// 카메라 위치 업데이트
	//Vec3 cameraPosition = GetTransform()->GetLocalPosition();

	Vec3 cameraRotate = GetTransform()->GetLocalRotation();
	Vec3 pos = _cameraTransform->GetLocalPosition();
	pos.y += 3;
	_cameraTransform->SetLocalPosition(pos);
	_cameraTransform->SetLocalRotation(cameraRotate);
}


void PlayerScript::UpdatePlayerInput()
{
	UpdateKeyInput();
	
	UpdateMouseInput();
}

void PlayerScript::UpdateKeyInput()
{
	Vec3 pos = GetTransform()->GetLocalPosition();
	Vec3 scale = GetTransform()->GetLocalScale();

	float adjustedSpeed = _speed / scale.x; // 스케일에 따라 속도를 조정

	if (INPUT->GetButton(KEY_TYPE::W))
		pos += GetTransform()->GetLook() * adjustedSpeed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::S))
		pos -= GetTransform()->GetLook() * adjustedSpeed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::A))
		pos -= GetTransform()->GetRight() * adjustedSpeed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::D))
		pos += GetTransform()->GetRight() * adjustedSpeed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::CTRL))
		pos -= GetTransform()->GetUp() * adjustedSpeed * DELTA_TIME;

	if (_isGrounded && INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		_currentVelocity = _jumpVelocity;
		pos.y += _currentVelocity * DELTA_TIME;
		_isGrounded = false;
	}

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

	// 카메라 위치 업데이트
	
}




void PlayerScript::UpdateRotation(float deltaX, float deltaY)
{
    // X축 회전 (Pitch, 위아래)
    _pitch += deltaY * sensitivity;
    _pitch = max(-90 * XM_PI / 180, min(90 * XM_PI / 180, _pitch));

    // Y축 회전 (Yaw, 좌우)
    _yaw += deltaX * sensitivity;

    // 플레이어의 Yaw 회전만 적용
	rotation.x = _pitch * 50;
    rotation.y = _yaw * 50;

    GetTransform()->SetLocalRotation(rotation);

    // 카메라의 Pitch, _yaw 회전만 적용
    Vec3 cameraRotation = _cameraTransform->GetLocalRotation();
    cameraRotation.x = _pitch * 50;
    cameraRotation.y = _yaw * 50;
    _cameraTransform->SetLocalRotation(cameraRotation);
}

void PlayerScript::UpdatePlayerOnTerrain()
{
	Vec3 pos = GetTransform()->GetLocalPosition();
	float terrainHeight = -330.f;  // 수정해야됨

	if (pos.y <= terrainHeight)
	{
		pos.y = terrainHeight;
		_currentVelocity = 0.0f;
		_isGrounded = true;
	}
	else
	{
		_currentVelocity += _gravity * DELTA_TIME * 10;
		pos.y += _currentVelocity * DELTA_TIME;
		_isGrounded = false;
	}
	GetTransform()->SetLocalPosition(pos);
}






