#pragma once
#include "Weapon.h"
#include "Camera.h"
#include "PhysicsSystem.h"

class GunWeapon : public Weapon
{
public:
    GunWeapon();
    ~GunWeapon() = default;

    virtual void Attack();
    virtual void Reload();
    virtual void SetBulletPosition();
    virtual void SetBulletDirection();
    virtual void Update(); // 추가: 무기 상태 업데이트



private:
    void ApplyDamageToHitObject(const btCollisionObject* hitObject, btDiscreteDynamicsWorld* dynamicsWorld, const btVector3& hitPoint);

private:
    btVector3   _bulletPosition;    // 레이 시작점
    btVector3   _bulletDirection;   // 레이 방향
    float       _range;             // 레이 사거리
    bool        _isReloading;       // 재장전 상태
    float       _reloadTime;        // 재장전 시간
    float       _reloadTimer;       // 재장전 타이머
    int         _bulletNum;         // 총알 갯수
    const int   _maxBulletNum = 12; // 최대 총알 갯수
    float       _fireRate;          // 초당 발사 속도 (예: 2.0f = 초당 2발)
    float       _fireTimer;         // 발사 간격 타이머

};
