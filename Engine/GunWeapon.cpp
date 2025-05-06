#include "pch.h"
#include "GunWeapon.h"
#include "SceneManager.h"
#include "Transform.h"
#include "Resources.h"
#include "Bullet.h"
#include "Timer.h"
#include "RigidBody.h"
#include "PhysicsSystem.h"
#include <bullet3\btBulletDynamicsCommon.h>

// Vector3�� btVector3�� ��ȯ�ϴ� ��ƿ��Ƽ �Լ�
inline btVector3 ToBtVector3(const Vec3& vec)
{
    return btVector3(vec.x, vec.y, vec.z);
}

GunWeapon::GunWeapon()
    : m_bulletPosition(btVector3(0, 0, 0))
    , m_bulletDirection(btVector3(0, 0, -1))
    , m_range(1000.0f)
    , m_isReloading(false)
    , m_reloadTime(2.0f)
    , m_reloadTimer(0.0f)
{
}

void GunWeapon::Attack()
{
    if (m_isReloading)
        return; // ������ �߿��� �߻� �Ұ�

    auto* dynamicsWorld = GET_SINGLE(PhysicsSystem)->GetDynamicsWorld();
    if (!dynamicsWorld)
        return;

    // �߻� �������� ���� ���
    btVector3 from = m_bulletPosition;
    btVector3 to = m_bulletPosition + m_bulletDirection * m_range;

    // ����ĳ��Ʈ ����
    btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);
    dynamicsWorld->rayTest(from, to, rayCallback);
   
    
    // �浹 ó��
    if (rayCallback.hasHit())
    {
        const btCollisionObject* hitObject = rayCallback.m_collisionObject;
        RemoveHitObject(hitObject, dynamicsWorld);
    }
}

void GunWeapon::Reload()
{
    if (!m_isReloading)
    {
        m_isReloading = true;
        m_reloadTimer = 0.0f;
    }
}

void GunWeapon::SetBulletPosition()
{
    // ���Ⱑ GameObject�� �����Ǿ� �ִٰ� ����

    auto camera = GET_SINGLE(SceneManager)->GetActiveScene()->GetMainCamera();
    auto transform = camera->GetTransform();

    m_bulletPosition = ToBtVector3(transform->GetWorldPosition());
}

void GunWeapon::SetBulletDirection()
{
    // ���� ī�޶��� Look ���͸� �������� ���� ����
    auto camera = GET_SINGLE(SceneManager)->GetActiveScene()->GetMainCamera();
    auto mat = camera->GetTransform()->GetLocalToWorldMatrix();

    m_bulletDirection = ToBtVector3(mat.Backward());
}

void GunWeapon::Update()
{
    if (m_isReloading)
    {
        m_reloadTimer += DELTA_TIME; 
        if (m_reloadTimer >= m_reloadTime)
        {
            m_isReloading = false;
        }
    }
}

void GunWeapon::RemoveHitObject(const btCollisionObject* hitObject, btDiscreteDynamicsWorld* dynamicsWorld)
{
    if (!hitObject)
        return;

    auto mutableHitObject = const_cast<btCollisionObject*>(hitObject);
    dynamicsWorld->removeCollisionObject(mutableHitObject);

    void* userPointer = mutableHitObject->getUserPointer();
    if (userPointer)
    {
        GameObject* hitGameObject = static_cast<GameObject*>(userPointer);
        shared_ptr<GameObject> gameObjectPtr = hitGameObject->shared_from_this();
        if (gameObjectPtr)
        {
            GET_SINGLE(SceneManager)->GetActiveScene()->RemoveGameObject(gameObjectPtr);
        }
    }
}