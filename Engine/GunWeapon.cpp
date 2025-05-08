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
#include "RigidBody.h"

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

    //Beep(1000, 100);


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
        ApplyDamageToHitObject(hitObject, dynamicsWorld, rayCallback.m_hitPointWorld);
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

void GunWeapon::ApplyDamageToHitObject(const btCollisionObject* hitObject, btDiscreteDynamicsWorld* dynamicsWorld, const btVector3& hitPoint)
{
    if (!hitObject || !hitObject->getUserPointer())
    {
        return;
    }

    GameObject* hitGameObject = static_cast<GameObject*>(hitObject->getUserPointer());
    shared_ptr<GameObject> gameObjectPtr;
    gameObjectPtr = hitGameObject->shared_from_this();

    // CharacterController가 있는 경우
    if (auto characterController = gameObjectPtr->GetCharacterController())
    {
        // PlayerStats 컴포넌트를 통해 데미지 적용 (가정)
        if (auto playerStats = gameObjectPtr->GetPlayerStats())
        {
            playerStats->ApplyDamage(40.0f);
            // 총알 방향에 따라 뒤로 밀림 효과 적용
            btVector3 pushDirection = _bulletDirection.normalized(); // 총알 반대 방향
            float pushSpeed = 0.5f; // 밀림 속도 (조정 가능)
            float pushDuration = 0.05f; // 밀림 지속 시간 (초)

            // 밀림 효과 적용
            Vec3 walkDirection(pushDirection.x(), 0.0f, pushDirection.z()); // Y축 무시
            characterController->Move(walkDirection * pushSpeed); // Move 호출
            characterController->SetPushDirection(btVector3(walkDirection.x * pushSpeed, 0.0f, walkDirection.z * pushSpeed)); // 밀림 방향 저장
            characterController->SetPushTimer(pushDuration); // 밀림 타이머 설정
            characterController->SetIsPushing(true); // 밀림 상태 활성화

            if (playerStats->IsDead())
            {
                //characterController->OnDisable();
                GET_SINGLE(SceneManager)->GetActiveScene()->RemoveGameObject(gameObjectPtr);
            }
        }
    }
}