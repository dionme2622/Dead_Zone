#include "pch.h"
#include "RigidBody.h"
#include "PhysicsSystem.h"
#include "Transform.h"

RigidBody::RigidBody(float mass, shared_ptr<BaseCollider> collider, Matrix matrix, bool isKinematic)
    : Component(COMPONENT_TYPE::RIGIDBODY)
    , _mass(mass)
    , _isKinematic(isKinematic)
    , _collider(collider)
    , _matrix(matrix)
{   
    Vec3 scale, translation;
    SimpleMath::Quaternion rotation;
    _matrix.Decompose(scale, rotation, translation);

    btCollisionShape* shape = _collider->GetShape();
    shape->setLocalScaling(ToBt(scale));

    // Get initial transform
    btTransform start;
    start.setIdentity();
 
    start.setOrigin(ToBt(translation));
    start.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
    _motionState = make_shared<btDefaultMotionState>(start);

    // Calculate inertia (질량·관성 계산)


    btVector3 inertia(0, 0, 0);
    if (!_isKinematic && _mass > 0.0f) {
        shape->calculateLocalInertia(_mass, inertia);
    }

    // RigidBody 생성
    btRigidBody::btRigidBodyConstructionInfo info(
        _mass,
        _motionState.get(),
        shape,
        inertia
    );
    _body = make_shared<btRigidBody>(info);
    _body->setRestitution(0.0f);
    _body->setFriction(1.0f);
    _body->setDamping(0.2f, 0.2f);
    // TODO : 키네마틱을 사용하는 가?
    if (_isKinematic) {
        _body->setCollisionFlags(_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        _body->setActivationState(DISABLE_DEACTIVATION);
    }
}
RigidBody::~RigidBody()
{
    auto world = GET_SINGLE(PhysicsSystem)->GetDynamicsWorld();
    if (_body) world->removeRigidBody(_body.get());
}

void RigidBody::OnEnable() {
    PhysicsSystem::GET_SINGLE(PhysicsSystem)->GetDynamicsWorld()->addRigidBody(_body.get());
}

void RigidBody::OnDisable() {
    PhysicsSystem::GET_SINGLE(PhysicsSystem)->GetDynamicsWorld()->removeRigidBody(_body.get());
}

void RigidBody::FinalUpdate() {
    btTransform trans;
    _body->getMotionState()->getWorldTransform(trans);
    Vec3 p(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
    GetTransform()->SetNo(true);
    GetTransform()->SetLocalPosition(p);
    /*std::cout
        << "[Collider] Position: ("
        << p.x << ", "
        << p.y << ", "
        << p.z << ")\n";*/
}

CharacterController::CharacterController(float radius, float height, float stepHeight) : Component(COMPONENT_TYPE::CHARACTER_CONTROLLER)
, _radius(radius)
, _height(height)
, _stepHeight(stepHeight)
{
    // 1) Ghost Object 생성 (충돌 검사 전용)
    _ghost = std::make_unique<btPairCachingGhostObject>();
    _ghost->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

    // 2) 캡슐 콜라이더 생성 (height 에서 상·하 반경 제외)
    float capsuleHeight = _height - 2.0f * _radius;
    _shape = std::make_unique<btCapsuleShape>(_radius, capsuleHeight);

    _ghost->setCollisionShape(_shape.get());

    // 3) Kinematic Character Controller 생성
    _controller = std::make_unique<btKinematicCharacterController>(
        _ghost.get(),
        _shape.get(),
        _stepHeight
    );

    _controller->setGravity(btVector3(0, -9.8f, 0));      // 월드 단위 중력
    _controller->setFallSpeed(55.0f);   // max fall speed
    _controller->setJumpSpeed(8.0f);    // 원하는 점프 속도
}
CharacterController::~CharacterController()
{
    OnDisable();
}

void CharacterController::Move(const Vec3& direction)
{
    // direction.y 무시, 속도는 외부에서 곱해서 넘겨 주세요
    btVector3 dir(direction.x, 0.0f, direction.z);
    _controller->setWalkDirection(dir);
}

void CharacterController::Jump()
{
    if (_controller->onGround())  // 바닥에 닿아 있을 때만
        _controller->jump();
}

void CharacterController::OnEnable()
{
    auto world = GET_SINGLE(PhysicsSystem)->GetDynamicsWorld();

    // GhostObject 등록
    world->addCollisionObject(
        _ghost.get(),
        btBroadphaseProxy::CharacterFilter,
        btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter
    );

    // Controller 등록
    world->addAction(_controller.get());

    // 4) 초기 위치 동기화: Transform → GhostObject
    Vec3 pos = GetTransform()->GetLocalPosition();
    btVector3 startPos(pos.x, pos.y, pos.z);
    _ghost->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), startPos));

    // warp() 로 내부 상태까지 초기화
    _controller->warp(startPos);
}

void CharacterController::OnDisable()
{
    if (auto world = GET_SINGLE(PhysicsSystem)->GetDynamicsWorld())
    {
        world->removeAction(_controller.get());
        world->removeCollisionObject(_ghost.get());
    }
}

void CharacterController::FinalUpdate()
{
    btTransform btTrans = _ghost->getWorldTransform();
    btVector3 btPos = btTrans.getOrigin();
    Vec3 worldPos(btPos.x(), btPos.y(), btPos.z());

    GetTransform()->SetNo(true);
    GetTransform()->SetLocalPosition(worldPos);

    GET_SINGLE(DebugRenderer)->AddCapsule(_radius, _height, GetTransform()->GetLocalToWorldMatrix(), { 1,0,0,1 });
}
