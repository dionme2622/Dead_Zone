#pragma once
#include "MonoBehaviour.h"
#include <bullet3\btBulletDynamicsCommon.h>

class CharacterController;
class PlayerScript : public MonoBehaviour
{
public:
	PlayerScript(HWND hwnd, bool isLocal, int playerId, shared_ptr<CharacterController> controller);
	virtual ~PlayerScript();

	virtual void LateUpdate() override;

	void UpdatePlayerInput();

	void UpdateRotation(float deltaX, float deltaY);

	void UpdateKeyInput();

	void UpdateMouseInput();

	bool IsGrounded(btRigidBody* body, btDiscreteDynamicsWorld* world);
	// 이동 관련
public:

private:
	HWND _hwnd;
	bool _isLocal;
	int  _playerId;

	shared_ptr<btRigidBody> _body = nullptr;

	float _speed;     // 기존에 있던 이동 속도
	float _jumpVelocity;    // 점프 초기 속도
	float _currentVelocity; // 현재 수직 속도
	float _gravity;         // 중력 가속도
	bool _isGrounded;       // 땅에 닿아있는지 여부
	shared_ptr<CharacterController> _controller;

private:
	float sensitivity = 0.001f;

	float _pitch = 0.0f; // 위/아래 각도
	float _yaw = 0.0f; // 좌우 각도
	Vec3 rotation; // 카메라 회전

	bool _mouseMove;
	shared_ptr<Transform> _cameraTransform;
};

