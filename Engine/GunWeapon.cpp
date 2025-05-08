#include "pch.h"
#include "GunWeapon.h"
#include "SceneManager.h"
#include "Transform.h"
#include "Resources.h"
#include "Bullet.h"
#include "Timer.h"
#include "PhysicsSystem.h"
#include "PlayerStats.h"
#include <bullet3\btBulletDynamicsCommon.h>

// Vector3�� btVector3�� ��ȯ�ϴ� ��ƿ��Ƽ �Լ�
inline btVector3 ToBtVector3(const Vec3& vec)
{
    return btVector3(vec.x, vec.y, vec.z);
}

GunWeapon::GunWeapon()
    : _bulletPosition(btVector3(0, 0, 0))
    , _bulletDirection(btVector3(0, 0, -1))
    , _range(1000.0f)
    , _isReloading(false)
    , _reloadTime(2.0f)
    , _reloadTimer(0.0f)
    , _bulletNum(12)
    , _maxBulletNum(12)
    , _fireRate(4.0f)      // �ʴ� 2��
    , _fireTimer(0.0f)
{
}

void GunWeapon::Attack()
{
    if (_isReloading || _fireTimer > 0.0f)
        return; // ������ ���̰ų� �߻� ��� �ð� �߿��� �߻� �Ұ�

    if (_bulletNum <= 0)
    {
        Reload();
        return;
    }

    Beep(1000, 100);


    _fireTimer = 1.0f / _fireRate; // ���� �߻���� ��� �ð� ����
    --_bulletNum;

    auto* dynamicsWorld = GET_SINGLE(PhysicsSystem)->GetDynamicsWorld();
    if (!dynamicsWorld)
        return;

    // �߻� �������� ���� ���
    btVector3 from = _bulletPosition;
    btVector3 to = _bulletPosition + _bulletDirection * _range;

    // ����ĳ��Ʈ ���� (CharacterFilter�� �浹�ϵ��� ����)
    btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);
    rayCallback.m_collisionFilterGroup = btBroadphaseProxy::DefaultFilter;
    rayCallback.m_collisionFilterMask = btBroadphaseProxy::CharacterFilter | btBroadphaseProxy::DefaultFilter;
    dynamicsWorld->rayTest(from, to, rayCallback);

    // �浹 ó��
    if (rayCallback.hasHit())
    {
        const btCollisionObject* hitObject = rayCallback.m_collisionObject;
        ApplyDamageToHitObject(hitObject, dynamicsWorld);
    }
}

void GunWeapon::Reload()
{
    _isReloading = true;
    _reloadTimer = 0.0f;
    _bulletNum = _maxBulletNum;
}

void GunWeapon::SetBulletPosition()
{
    auto camera = GET_SINGLE(SceneManager)->GetActiveScene()->GetMainCamera();
    auto transform = camera->GetTransform();
    _bulletPosition = ToBtVector3(transform->GetWorldPosition());
}

void GunWeapon::SetBulletDirection()
{
    auto camera = GET_SINGLE(SceneManager)->GetActiveScene()->GetMainCamera();
    auto mat = camera->GetTransform()->GetLocalToWorldMatrix();
    _bulletDirection = ToBtVector3(mat.Backward());
}

void GunWeapon::Update()
{
    if (_isReloading)
    {
        _reloadTimer += DELTA_TIME;
        if (_reloadTimer >= _reloadTime)
        {
            _isReloading = false;
        }
    }

    if (_fireTimer > 0.0f)
    {
        _fireTimer -= DELTA_TIME;
        if (_fireTimer < 0.0f)
            _fireTimer = 0.0f;
    }

}

void GunWeapon::ApplyDamageToHitObject(const btCollisionObject* hitObject, btDiscreteDynamicsWorld* dynamicsWorld)
{
    if (!hitObject || !hitObject->getUserPointer())
    {
        return;
    }

    GameObject* hitGameObject = static_cast<GameObject*>(hitObject->getUserPointer());
    shared_ptr<GameObject> gameObjectPtr;
    try
    {
        gameObjectPtr = hitGameObject->shared_from_this();
    }
    catch (const std::bad_weak_ptr&)
    {
        std::cerr << "Error: GameObject is not managed by shared_ptr" << std::endl;
        return;
    }

    // CharacterController�� �ִ� ���
    if (auto characterController = gameObjectPtr->GetCharacterController())
    {
        // PlayerStats ������Ʈ�� ���� ������ ���� (����)
        if (auto playerStats = gameObjectPtr->GetPlayerStats())
        {
            playerStats->ApplyDamage(40.0f);
            if (playerStats->IsDead())
            {
                //characterController->OnDisable();
                GET_SINGLE(SceneManager)->GetActiveScene()->RemoveGameObject(gameObjectPtr);
            }
        }
    }
}