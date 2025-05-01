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

	float _pitch = 0.0f; // ��/�Ʒ� ����
	float _yaw = 0.0f; // �¿� ����
	Vec3 rotation; // ī�޶� ȸ��

	bool _mouseMove;

};

