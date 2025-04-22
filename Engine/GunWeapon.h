#pragma once
#include "Weapon.h"
#include "Camera.h"

class GunWeapon : public Weapon
{
public:
	GunWeapon();
	virtual ~GunWeapon() {};

public:
	virtual void Attack();
	virtual void Reload();
	virtual void DeleteBullet();


public:
	void SetBulletPosition();
	void SetBulletDirection();


private:
	vector<shared_ptr<GameObject>> _bulletObjects;
	const Vec3					   _muzzleOffset = { 0, 1, 2 };	// �ӽ� �ѱ� ��ġ


	weak_ptr<Camera>			   _camera;
};

