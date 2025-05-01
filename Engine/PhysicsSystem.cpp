#include "pch.h"
#include "PhysicsSystem.h"


void PhysicsSystem::Init() {
    // 1) �浹 ����
    _collisionConfig = make_unique<btDefaultCollisionConfiguration>();
    _dispatcher      = make_unique<btCollisionDispatcher>(_collisionConfig.get());
    // 2) ���� �ܰ� (Broadphase)
    _broadphase      = make_unique<btDbvtBroadphase>();
    // 3) ���� �ؼ��� (Solver)
    _solver          = make_unique<btSequentialImpulseConstraintSolver>();
    // 4) ���� ����
    _dynamicsWorld = make_unique<btDiscreteDynamicsWorld>(
        _dispatcher.get(),
        _broadphase.get(),
        _solver.get(),
        _collisionConfig.get());
    // 5) �߷� ���� (��: �Ʒ� ���� Y=-9.8)
    _dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
}

void PhysicsSystem::Shutdown() {
    if (_dynamicsWorld) {
        // Body ���� �� �ʿ��ϴٸ� ���⼭ �� �ְ�
        _dynamicsWorld.reset();
        _solver.reset();
        _broadphase.reset();
        _dispatcher.reset();
        _collisionConfig.reset();
    }
}

void PhysicsSystem::Update(float deltaTime) {
    if (_dynamicsWorld) {
        _dynamicsWorld->stepSimulation(deltaTime);
    }
}