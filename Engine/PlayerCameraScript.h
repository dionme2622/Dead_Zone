#pragma once
#include "MonoBehaviour.h"

class PlayerCameraScript : public MonoBehaviour
{
public:
	PlayerCameraScript();
	virtual ~PlayerCameraScript();

	virtual void LateUpdate() override;

private:
	float		_speed = 100.f;
};

