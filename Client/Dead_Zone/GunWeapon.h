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
    void ApplyDamageToHitObject(const btCollisionObject* hitObject, btDiscreteDynamicsWorld* dynamicsWorld, const btVector3& hitPoint);

private:
    btVector3   _bulletPosition;    // ���� ������
    btVector3   _bulletDirection;   // ���� ����
    float       _range;             // ���� ��Ÿ�
    bool        _isReloading;       // ������ ����
    float       _reloadTime;        // ������ �ð�
    float       _reloadTimer;       // ������ Ÿ�̸�
    int         _bulletNum;         // �Ѿ� ����
    const int   _maxBulletNum = 12; // �ִ� �Ѿ� ����
    float       _fireRate;          // �ʴ� �߻� �ӵ� (��: 2.0f = �ʴ� 2��)
    float       _fireTimer;         // �߻� ���� Ÿ�̸�

};
