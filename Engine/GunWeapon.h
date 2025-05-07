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
    virtual void Update(); // �߰�: ���� ���� ������Ʈ

private:
    btVector3 m_bulletPosition; // ���� ������
    btVector3 m_bulletDirection; // ���� ����
    float m_range; // ���� ��Ÿ�
    bool m_isReloading; // ������ ����
    float m_reloadTime; // ������ �ð�
    float m_reloadTimer; // ������ Ÿ�̸�

    void ApplyDamageToHitObject(const btCollisionObject* hitObject, btDiscreteDynamicsWorld* dynamicsWorld);
};
