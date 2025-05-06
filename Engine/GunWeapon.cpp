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

// Vector3를 btVector3로 변환하는 유틸리티 함수
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
        return; // 재장전 중에는 발사 불가

    auto* dynamicsWorld = GET_SINGLE(PhysicsSystem)->GetDynamicsWorld();
    if (!dynamicsWorld)
        return;

    // 발사 시작점과 끝점 계산
    btVector3 from = m_bulletPosition;
    btVector3 to = m_bulletPosition + m_bulletDirection * m_range;

    // 레이캐스트 수행
    btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);
    dynamicsWorld->rayTest(from, to, rayCallback);
   
    
    // 충돌 처리
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
    // 무기가 GameObject에 부착되어 있다고 가정

    auto camera = GET_SINGLE(SceneManager)->GetActiveScene()->GetMainCamera();
    auto transform = camera->GetTransform();

    m_bulletPosition = ToBtVector3(transform->GetWorldPosition());
}

void GunWeapon::SetBulletDirection()
{
    // 메인 카메라의 Look 벡터를 기준으로 방향 설정
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