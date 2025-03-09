#include "pch.h"
#include "PlayerScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "KeyInput.h"
#include "Timer.h"

PlayerScript::PlayerScript()
{
	_speed = 1.0f;
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
	UpdatePlayerInput();

	UpdatePlayerOnTerrain();
}



void PlayerScript::UpdatePlayerInput()
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


	GetTransform()->SetLocalPosition(pos);
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




