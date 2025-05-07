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
    btVector3 m_bulletPosition; // 레이 시작점
    btVector3 m_bulletDirection; // 레이 방향
    float m_range; // 레이 사거리
    bool m_isReloading; // 재장전 상태
    float m_reloadTime; // 재장전 시간
    float m_reloadTimer; // 재장전 타이머

    void ApplyDamageToHitObject(const btCollisionObject* hitObject, btDiscreteDynamicsWorld* dynamicsWorld);
};
