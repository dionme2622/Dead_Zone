#include "pch.h"
#include "PhysicsSystem.h"


void PhysicsSystem::Init() {
    // 1) 충돌 구성
    _collisionConfig = make_unique<btDefaultCollisionConfiguration>();
    _dispatcher      = make_unique<btCollisionDispatcher>(_collisionConfig.get());
    // 2) 광역 단계 (Broadphase)
    _broadphase      = make_unique<btDbvtBroadphase>();
    // 3) 제약 해석기 (Solver)
    _solver          = make_unique<btSequentialImpulseConstraintSolver>();
    // 4) 월드 생성
    _dynamicsWorld = make_unique<btDiscreteDynamicsWorld>(
        _dispatcher.get(),
        _broadphase.get(),
        _solver.get(),
        _collisionConfig.get());
    // 5) 중력 설정 (예: 아래 방향 Y=-9.8)
    _dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
}

void PhysicsSystem::Shutdown() {
    if (_dynamicsWorld) {
        // Body 제거 등 필요하다면 여기서 해 주고
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