#pragma once
#include <bullet3\btBulletDynamicsCommon.h>

/// 전역으로 접근 가능한 물리 월드 관리 클래스
class PhysicsSystem {
public:
    DECLARE_SINGLE(PhysicsSystem);

    /// Bullet 월드 초기화 (게임 시작 시 한 번)
    void Init();

    /// Bullet 월드 파괴 (게임 종료 시)
    void Shutdown();

    /// 매 프레임 물리 시뮬레이션 호출
    void Update(float deltaTime);

    /// 동적 물체 등록용 World
    btDiscreteDynamicsWorld* GetDynamicsWorld() const { return _dynamicsWorld.get(); }
    /// 레이캐스트 등 충돌 검사 전용 World
    btCollisionWorld* GetCollisionWorld() const { return _dynamicsWorld.get(); }

private:

    PhysicsSystem(const PhysicsSystem&) = delete;
    PhysicsSystem& operator=(const PhysicsSystem&) = delete;

    unique_ptr<btDefaultCollisionConfiguration>       _collisionConfig;
    unique_ptr<btCollisionDispatcher>                _dispatcher;
    unique_ptr<btBroadphaseInterface>                _broadphase;
    unique_ptr<btSequentialImpulseConstraintSolver>  _solver;
    unique_ptr<btDiscreteDynamicsWorld>              _dynamicsWorld;

    float _accumulator = 0.0f;
};

inline btVector3 ToBt(const Vec3& v) {
    return btVector3(v.x, v.y, v.z);
}

inline Vec3 FromBt(const btVector3& v) {
    return Vec3(v.getX(), v.getY(), v.getZ());
}
