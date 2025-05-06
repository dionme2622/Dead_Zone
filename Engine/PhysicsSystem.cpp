#include "pch.h"
#include "PhysicsSystem.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


void PhysicsSystem::Init() {
    // 1) 충돌 구성
    _collisionConfig = make_unique<btDefaultCollisionConfiguration>();
    _dispatcher      = make_unique<btCollisionDispatcher>(_collisionConfig.get());
    // 2) 광역 단계 (Broadphase)
    _broadphase      = make_unique<btDbvtBroadphase>();
    _broadphase->getOverlappingPairCache()
        ->setInternalGhostPairCallback(new btGhostPairCallback());
    // 3) 제약 해석기 (Solver)
    _solver          = make_unique<btSequentialImpulseConstraintSolver>();
    // 4) 월드 생성
    _dynamicsWorld = make_unique<btDiscreteDynamicsWorld>(
        _dispatcher.get(),
        _broadphase.get(),
        _solver.get(),
        _collisionConfig.get());
    // 5) 중력 설정 (예: 아래 방향 Y=-9.8)
    _dynamicsWorld->setGravity(btVector3(0, 0.f, 0));
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
    // (1) 프레임마다 들어온 실제 경과 시간
    _accumulator += deltaTime;

    // (2) 물리는 항상 fixedStep 으로 연속 처리
    const float fixedStep = 1.0f / 60.0f;  // 60Hz 물리
    int   maxSubSteps = 5;             // 최대 서브스텝 개수

    // 경과 시간을 fixedStep 단위로 쪼개서 물리 시뮬 돌리기
    while (_accumulator >= fixedStep && maxSubSteps-- > 0)
    {
        _dynamicsWorld->stepSimulation(fixedStep, 0);
        _accumulator -= fixedStep;
    }
}