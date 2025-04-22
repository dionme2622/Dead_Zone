#pragma once
#include "Component.h"

class Bullet : public Component
{
public:
	Bullet();
	virtual ~Bullet();


public:
	float GetSpeed() { return _speed; }
	float GetIsFired() { return _isFired; }

	void SetSpeed(float speed) { _speed = speed; }
	void SetIsFired(bool isFired) { _isFired = isFired; }
	void SetDirection(const Vec3& direction) {	_direction = direction;	}
	void DeleteBullet();

public:
	virtual void FinalUpdate() override;


private:
	float	_speed = 100;
	bool	_isFired;
	Vec3	_direction;

	float	_lifeDistance = 0;


};

