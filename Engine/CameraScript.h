#pragma once
#include "MonoBehaviour.h"

class CameraScript : public MonoBehaviour
{
public:
	CameraScript(HWND hwnd);
	virtual ~CameraScript() {};

	virtual void FinalUpdate() override;

	void UpdateMouseInput();

	void UpdateRotation(float deltaX, float deltaY);

private:
	HWND _hwnd;

private:
	bool _mouseMove;

	float sensitivity = 0.001f;

	float _pitch = 0.0f; // 위/아래 각도
	float _yaw = 0.0f; // 좌우 각도
	Vec3 rotation; // 카메라 회전

};



