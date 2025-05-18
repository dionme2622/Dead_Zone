#pragma once
#include "MonoBehaviour.h"

class TestAnimation : public MonoBehaviour
{
public:
	TestAnimation(HWND hwnd) : _hwnd(hwnd) {}

	virtual void Update() override;
	void UpdateRotation(float deltaX, float deltaY);
	//virtual void LateUpdate() override;

private:
	HWND _hwnd;
	float sensitivity = 0.001f;

	float _pitch = 0.0f; // 위/아래 각도
	float _yaw = 0.0f; // 좌우 각도
	Vec3 rotation; // 카메라 회전

	bool _mouseMove;

};

