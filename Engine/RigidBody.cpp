#include "pch.h"
#include "RigidBody.h"
#include "PhysicsSystem.h"
#include "Transform.h"


RigidBody::RigidBody(shared_ptr<GameObject> gameObject, float mass, shared_ptr<BaseCollider> collider, Vec3 pos, bool isKinematic)
    : Component(COMPONENT_TYPE::RIGIDBODY)
    , _weakGameObject(gameObject)
    , _mass(mass)
    , _isKinematic(isKinematic)
    , _collider(collider)
{
    // Get initial transform
    btTransform start;
    start.setIdentity();
    
    start.setOrigin(btVector3(pos.x, pos.y, pos.z));
    _motionState = make_shared<btDefaultMotionState>(start);

    // Calculate inertia (질량·관성 계산)
    btCollisionShape* shape = _collider->GetShape();
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

void RigidBody::FinalUpdate() {
    btTransform trans;
    _body->getMotionState()->getWorldTransform(trans);
    Vec3 p(trans.getOrigin().x(), trans.getOrigin().y(), trans.getOrigin().z());
    GetTransform()->SetLocalPosition(p);

}

