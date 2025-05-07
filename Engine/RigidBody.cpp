#include "pch.h"
#include "RigidBody.h"
#include "PhysicsSystem.h"
#include "Transform.h"
#include "Timer.h"

RigidBody::RigidBody(shared_ptr<GameObject> gameObject, float mass, shared_ptr<BaseCollider> collider, Matrix matrix, bool isKinematic)
    : Component(COMPONENT_TYPE::RIGIDBODY)
    , _weakGameObject(gameObject)
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

    // 사용자 데이터로 GameObject 설정
    if (auto lockedGameObject = _weakGameObject.lock())
    {
        _body->setUserPointer(lockedGameObject.get());
    }

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

void RigidBody::FinalUpdate()
{
    // 1) Bullet world transform
    btTransform    btTrans;
    _body->getMotionState()->getWorldTransform(btTrans);

    // 2) Position
    Vec3 pos(
        btTrans.getOrigin().x(),
        btTrans.getOrigin().y(),
        btTrans.getOrigin().z()
    );

    // 3) Rotation: Bullet quaternion → SimpleMath quaternion
    btQuaternion btQuat = btTrans.getRotation();
    SimpleMath::Quaternion   q(btQuat.x(), btQuat.y(), btQuat.z(), btQuat.w());

    // 4) Quaternion → Rotation matrix
    Matrix   rotMat = Matrix::CreateFromQuaternion(q);

    // 5) Matrix → Euler 각도(라디안) 분해
    Vec3     eulerRad = GetTransform()->DecomposeRotationMatrix(rotMat);

    // 6) 라디안 → 도(°) 로 변환
    const float radToDeg = 180.0f / XM_PI;
    Vec3     eulerDeg(
        eulerRad.x * radToDeg,
        eulerRad.y * radToDeg,
        eulerRad.z * radToDeg
    );

    // 7) Scale: 기존 Transform 스케일 유지
    Vec3     scale = GetTransform()->GetLocalScale();

    // 8) Transform에 한 번에 적용
    auto t = GetTransform();
    t->SetNo(true);
    t->SetLocalPosition(pos);
    t->SetLocalRotation(eulerDeg);
    t->SetLocalScale(scale);
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
    _controller->setFallSpeed(100.0f);   // max fall speed
    _controller->setJumpSpeed(20.0f);    // 원하는 점프 속도
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
    Vec3 p = GetTransform()->GetLocalPosition();
    const float yOffset = 1.5f;
    btVector3 startPos(p.x, p.y + yOffset, p.z);

    btTransform start;
    start.setIdentity();
    start.setOrigin(startPos);
    _ghost->setWorldTransform(start);   

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
    // 1) GhostObject world transform 가져오기
    btTransform btTrans = _ghost->getWorldTransform();
    btVector3 btPos = btTrans.getOrigin();

    // 2) Y축 10만큼 내리기 (Transform 위치 보정)
    const float yOffset = 1.5f;
    Vec3 worldPos(btPos.x(), btPos.y() - yOffset, btPos.z());

    if (!_controller->onGround())
    {
        _controller->setGravity(btVector3(0, -50.f, 0));
        //_controller->setFallSpeed(1500.0f);
    }
    else
    {
        // 땅에 닿으면 원래 값으로 복원
        _controller->setGravity(btVector3(0, -9.8f, 0));
        // _controller->setFallSpeed(550.0f );
    }

    // 3) Transform에 반영
    GetTransform()->SetNo(true);
    GetTransform()->SetLocalPosition(worldPos);

    // 4) 디버그 캡슐 렌더링 (for visualization)
    Matrix capsuleWorld = Matrix::CreateTranslation(
        btPos.x(),
        btPos.y(),
        btPos.z()
    );
    GET_SINGLE(DebugRenderer)
        ->AddCapsule(_radius, _height, capsuleWorld, { 1,0,0,1 });

}
