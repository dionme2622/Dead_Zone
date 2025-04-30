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
#include "BattleScene.h"
#include "WeaponManager.h"
#include "Weapon.h"
#include "Animator.h"

PlayerScript::PlayerScript(HWND hwnd, bool isLocal, int playerId)
{
	_hwnd = hwnd;
	_isLocal = isLocal;
	_playerId = playerId;
	// Player에 대한 정보 초기화 단계

	_speed = 5.0f;
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
		//GetTransform()->SetLocalPosition(Vec3(0.f, 10 * DELTA_TIME, 0.f));		// 임시

		
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
	// 1) 이전 위치 저장
	Vec3 oldPos = GetTransform()->GetLocalPosition();
	Vec3 newPos = oldPos;
	
	// 2) 입력에 따라 newPos 변경	
	Vec3 pos = GetTransform()->GetLocalPosition();
	Vec3 scale = GetTransform()->GetLocalScale();

	float adjustedSpeed = _speed / scale.x; // 스케일에 따라 속도를 조정

	// 룩 벡터를 가져오고, Y 성분을 제거하여 수평 방향만 유지
	Vec3 lookVector = GetTransform()->GetLook();
	Vec3 flatLookVector = Vec3(lookVector.x, 0.0f, lookVector.z); // Y 성분을 0으로 설정
	flatLookVector.Normalize(); // 정규화하여 방향만 유지

	// 오른쪽 벡터도 수평 방향으로 유지
	Vec3 rightVector = GetTransform()->GetRight();
	Vec3 flatRightVector = Vec3(rightVector.x, 0.0f, rightVector.z);
	flatRightVector.Normalize();

	if (INPUT->GetButton(KEY_TYPE::W))
		newPos += GetTransform()->GetLook() * _speed * DELTA_TIME;
	if (INPUT->GetButton(KEY_TYPE::S))
		newPos -= GetTransform()->GetLook() * _speed * DELTA_TIME;
	if (INPUT->GetButton(KEY_TYPE::A))
		newPos -= GetTransform()->GetRight() * _speed * DELTA_TIME;
	if (INPUT->GetButton(KEY_TYPE::D))
		newPos += GetTransform()->GetRight() * _speed * DELTA_TIME;
	if (INPUT->GetButton(KEY_TYPE::Q))
		newPos += GetTransform()->GetLook() * _speed * 2 * DELTA_TIME;
	if (INPUT->GetButton(KEY_TYPE::CTRL))
		newPos -= GetTransform()->GetUp() * _speed * DELTA_TIME;

	if (BattleScene::isPlayerGrounded && INPUT->GetButtonDown(KEY_TYPE::SPACE))
	{
		_currentVelocity = _jumpVelocity;
		newPos.y += _currentVelocity * DELTA_TIME;
		_isGrounded = false;
	}

	if (INPUT->GetButton(KEY_TYPE::KEY_1))
	{
		GetWeaponManager()->EquipWeapon(0);
	}
	
	if (INPUT->GetButton(KEY_TYPE::KEY_2))
	{
		GetWeaponManager()->EquipWeapon(1);
	}
	
	if (INPUT->GetButton(KEY_TYPE::KEY_3))
	{	
		GetWeaponManager()->EquipWeapon(2);
	}

	if (INPUT->GetButton(KEY_TYPE::LEFTCLICK))
	{
		// Shoot 애니메이션 트리거
		GetAnimator()->SetTrigger("Shoot");
		GetWeaponManager()->GetCurrentWeapon()[0]->GetWeapon()->Attack();
	}

	// 3) 실제 이동량 계산
	Vec3 delta = newPos - oldPos;
	// TODO : pos 값을 서버로 보낸다.

	// 4) 초당 속도로 변환
	float currentSpeed = 0.0f;
	if (delta.LengthSquared() > 0.0f)   // >0 이면
	{
		float dist = delta.Length();    // 이동 거리
		currentSpeed = dist / DELTA_TIME;
		// (만약 애니메이터가 0~1 사이 값을 기대하면
		//  currentSpeed /= _speed; // maxSpeed 로 정규화)
	}
	 
	// 5) Animator 에 전달
	GetAnimator()->SetFloat("Speed", currentSpeed);
	printf("속도: %f\n", currentSpeed);
	// 6) 위치 적용
	GetTransform()->SetLocalPosition(newPos);

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
	// X축 회전 (Pitch, 위아래) - 카메라에만 적용
	_pitch += deltaY * sensitivity;
	_pitch = max(-90 * XM_PI / 180, min(90 * XM_PI / 180, _pitch));

	// Y축 회전 (Yaw, 좌우) - 캐릭터와 카메라 모두에 적용
	_yaw += deltaX * sensitivity;

	// 플레이어는 Yaw 회전만 적용 (Pitch는 제외)
	Vec3 rotation = Vec3(0, _yaw * 50, 0.0f); // Pitch는 0으로 유지
	GetTransform()->SetLocalRotation(rotation);
	
	// 디버그용
	/*{
		Vec3 rotation = Vec3(_pitch * 50, _yaw * 50, 0.0f); 
		GetTransform()->SetLocalRotation(rotation);
	}*/

	// 카메라는 Pitch와 Yaw 모두 적용
	//Vec3 cameraRotation = Vec3(_pitch * 50, _yaw * 50, 0.0f); // Pitch와 Yaw 적용
	//_cameraTransform->SetLocalRotation(cameraRotation);
}

void PlayerScript::UpdatePlayerOnTerrain()
{	
	Vec3 pos = GetTransform()->GetLocalPosition();


	if (BattleScene::isPlayerGrounded)
	{
		_currentVelocity = 0.0f;
	}
	else
	{
		_currentVelocity += _gravity * DELTA_TIME;
		pos.y += _currentVelocity * DELTA_TIME;
	}
	GetTransform()->SetLocalPosition(pos);
}