#pragma once
#include <bullet3\btBulletDynamicsCommon.h>

/// �������� ���� ������ ���� ���� ���� Ŭ����
class PhysicsSystem {
public:
    DECLARE_SINGLE(PhysicsSystem);

    /// Bullet ���� �ʱ�ȭ (���� ���� �� �� ��)
    void Init();

    /// Bullet ���� �ı� (���� ���� ��)
    void Shutdown();

    /// �� ������ ���� �ùķ��̼� ȣ��
    void Update(float deltaTime);

    /// ���� ��ü ��Ͽ� World
    btDiscreteDynamicsWorld* GetDynamicsWorld() const { return _dynamicsWorld.get(); }
    /// ����ĳ��Ʈ �� �浹 �˻� ���� World
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
