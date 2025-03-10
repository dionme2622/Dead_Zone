#pragma once
#include "MonoBehaviour.h"

class PlayerScript : public MonoBehaviour
{
public:
	PlayerScript();
	virtual ~PlayerScript();

	virtual void LateUpdate() override;

	void UpdatePlayerInput();

	void UpdateRotation(float deltaX, float deltaY);

	void UpdateKeyInput();

	void UpdateMouseInput();


	// 이동 관련
public:
	void UpdatePlayerOnTerrain();

private:
	float _speed;     // 기존에 있던 이동 속도
	float _jumpVelocity;    // 점프 초기 속도
	float _currentVelocity; // 현재 수직 속도
	float _gravity;         // 중력 가속도
	bool _isGrounded;       // 땅에 닿아있는지 여부


private:
	float sensitivity = 0.001f;

	float _pitch = 0.0f; // 위/아래 각도
	float _yaw = 0.0f; // 좌우 각도
	Vec3 rotation; // 카메라 회전



	HWND _hwnd;
	bool _mouseMove;
};

