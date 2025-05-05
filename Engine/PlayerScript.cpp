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
#include "Animator.h"
#include "RigidBody.h"
#include "PhysicsSystem.h"
PlayerScript::PlayerScript(HWND hwnd, bool isLocal, int playerId, shared_ptr<CharacterController> controller)
{
	_hwnd = hwnd;
	_isLocal = isLocal;
	_playerId = playerId;
	_controller = controller;
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
	// 1) RigidBody & 현재 Y 속도
//	auto body = GetRigidBody()->GetBody();
//	btVector3 vel = body->getLinearVelocity();
//	float     yVel = vel.getY();

	// 1) 이전 위치 저장
	Vec3 oldPos = GetTransform()->GetLocalPosition();
	Vec3 newPos = oldPos;
	
	// 2) 입력에 따라 newPos 변경	
	//if (INPUT->GetButton(KEY_TYPE::W))
	//	newPos += GetTransform()->GetLook() * _speed * DELTA_TIME;
	//if (INPUT->GetButton(KEY_TYPE::S))
	//	newPos -= GetTransform()->GetLook() * _speed * DELTA_TIME;
	//if (INPUT->GetButton(KEY_TYPE::A))
	//	newPos -= GetTransform()->GetRight() * _speed * DELTA_TIME;
	//if (INPUT->GetButton(KEY_TYPE::D))
	//	newPos += GetTransform()->GetRight() * _speed * DELTA_TIME;
	//if (INPUT->GetButton(KEY_TYPE::Q))
	//	newPos += GetTransform()->GetLook() * _speed * 2 * DELTA_TIME;
	//if (INPUT->GetButton(KEY_TYPE::CTRL))
	//	newPos -= GetTransform()->GetUp() * _speed * DELTA_TIME;

	// 2) WASD 입력에 따른 방향(dir) 계산
	Vec3 dir(0, 0, 0);
	if (INPUT->GetButton(KEY_TYPE::W)) dir += GetTransform()->GetLook();
	if (INPUT->GetButton(KEY_TYPE::S)) dir -= GetTransform()->GetLook();
	if (INPUT->GetButton(KEY_TYPE::A)) dir -= GetTransform()->GetRight();
	if (INPUT->GetButton(KEY_TYPE::D)) dir += GetTransform()->GetRight();

	_controller->Move(dir * _speed * DELTA_TIME);

	// 3) 땅에 붙어 있을 때만 점프
	if (INPUT->GetButtonDown(KEY_TYPE::SPACE) && _controller->IsOnGround())
	{
		_controller->Jump();
	}
	//if (IsGrounded(body.get(), GET_SINGLE(PhysicsSystem)->GetDynamicsWorld())
	//	&& INPUT->GetButtonDown(KEY_TYPE::SPACE))
	//{
	//	yVel = _jumpVelocity;   // 예: 8.0f
	//}
	// 4) 수평 방향 속도 설정
	const float EPS = 1e-6f;
	if (dir.LengthSquared() > EPS)
	{
		dir.Normalize();
		dir *= _speed;
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
	//printf("속도: %f\n", currentSpeed);

	// 5) 최종 속도 세팅 (Y 속도는 점프/중력 유지)
//	body->setLinearVelocity(btVector3(dir.x, yVel, dir.z));
	//GetTransform()->SetLocalPosition(newPos);
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

bool PlayerScript::IsGrounded(btRigidBody* body, btDiscreteDynamicsWorld* world)
{
	// 1) 리지드바디 현재 위치
	btTransform trans;
	body->getMotionState()->getWorldTransform(trans);
	btVector3 origin = trans.getOrigin();

	// 2) 레이 시작점과 끝점 설정 (Y축 아래로 e.g. 0.1m 만큼)
	btVector3 from = origin;
	btVector3 to = origin - btVector3(0, /*down*/ 0.12f, 0);

	// 3) 레이캐스트 콜백
	btCollisionWorld::ClosestRayResultCallback rayCB(from, to);
	world->rayTest(from, to, rayCB);

	// 4) 땅에 닿았으면 fraction < 1.0
	return rayCB.hasHit() && rayCB.m_closestHitFraction < 1.0f;
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






