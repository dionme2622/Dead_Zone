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

CharacterController::CharacterController(float radius, float height, float stepHeight) : Component(COMPONENT_TYPE::RIGIDBODY)
, _radius(radius)
, _height(height)
, _stepHeight(stepHeight)
{
    // 1) Ghost Object
    _ghost = std::make_shared<btPairCachingGhostObject>();
    _ghost->setCollisionFlags(
        btCollisionObject::CF_CHARACTER_OBJECT);

    // 2) Capsule Shape
    _shape = std::make_shared<btCapsuleShape>(_radius, _height - 2 * _radius);

    // ghost에 shape 장착
    _ghost->setCollisionShape(_shape.get());

    // 3) Kinematic Controller
    _controller = std::make_shared<btKinematicCharacterController>(
        _ghost.get(), _shape.get(), _stepHeight);
}

CharacterController::~CharacterController()
{
    OnDisable();
}

void CharacterController::Move(const Vec3& direction)
{
}

void CharacterController::Jump()
{
}

void CharacterController::OnEnable()
{
    //auto world = GET_SINGLE(PhysicsSystem)->GetDynamicsWorld();

    //// 4) 월드에 ghost와 controller 등록
    //world->addCollisionObject(
    //    _ghost.get(),
    //    btBroadphaseProxy::CharacterFilter,
    //    btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

    //world->addAction(_controller.get());

    //// 초기 위치 세팅: Transform→Bullet
    //auto t = GetTransform();
    //Vec3 pos = t->GetLocalPosition();
    //btTransform start;
    //start.setIdentity();
    //start.setOrigin(btVector3(pos.x, pos.y, pos.z));
    //_ghost->setWorldTransform(start);
    //_controller->reset();  // 내부 상태 초기화
}

void CharacterController::OnDisable()
{
}

void CharacterController::FinalUpdate()
{
}
