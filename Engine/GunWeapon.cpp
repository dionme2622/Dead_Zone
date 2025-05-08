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

// Vector3를 btVector3로 변환하는 유틸리티 함수
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
    , _fireRate(4.0f)      // 초당 2발
    , _fireTimer(0.0f)
{
}

void GunWeapon::Attack()
{
    if (_isReloading || _fireTimer > 0.0f)
        return; // 재장전 중이거나 발사 대기 시간 중에는 발사 불가

    if (_bulletNum <= 0)
    {
        Reload();
        return;
    }

    Beep(1000, 100);


    _fireTimer = 1.0f / _fireRate; // 다음 발사까지 대기 시간 설정
    --_bulletNum;

    auto* dynamicsWorld = GET_SINGLE(PhysicsSystem)->GetDynamicsWorld();
    if (!dynamicsWorld)
        return;

    // 발사 시작점과 끝점 계산
    btVector3 from = _bulletPosition;
    btVector3 to = _bulletPosition + _bulletDirection * _range;

    // 레이캐스트 수행 (CharacterFilter와 충돌하도록 설정)
    btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);
    rayCallback.m_collisionFilterGroup = btBroadphaseProxy::DefaultFilter;
    rayCallback.m_collisionFilterMask = btBroadphaseProxy::CharacterFilter | btBroadphaseProxy::DefaultFilter;
    dynamicsWorld->rayTest(from, to, rayCallback);

    // 충돌 처리
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

    // CharacterController가 있는 경우
    if (auto characterController = gameObjectPtr->GetCharacterController())
    {
        // PlayerStats 컴포넌트를 통해 데미지 적용 (가정)
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