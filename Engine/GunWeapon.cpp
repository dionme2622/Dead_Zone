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
    if (!m_isReloading)
    {
        m_isReloading = true;
        m_reloadTimer = 0.0f;
    }
}

void GunWeapon::SetBulletPosition()
{
    auto camera = GET_SINGLE(SceneManager)->GetActiveScene()->GetMainCamera();
    auto transform = camera->GetTransform();
    m_bulletPosition = ToBtVector3(transform->GetWorldPosition());
}

void GunWeapon::SetBulletDirection()
{
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

void GunWeapon::ApplyDamageToHitObject(const btCollisionObject* hitObject, btDiscreteDynamicsWorld* dynamicsWorld)
{
    if (!hitObject || !hitObject->getUserPointer())
    {
        std::cout << "No valid userPointer for hit object" << std::endl;
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