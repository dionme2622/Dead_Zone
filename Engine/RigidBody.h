#pragma once
#include "Component.h"
#include "BaseCollider.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include "GameObject.h"

class RigidBody : public Component
{
public:

	RigidBody(shared_ptr<GameObject> gameObject, float mass, shared_ptr<BaseCollider> collider, Matrix matrix, bool isKinematic = false);
	virtual ~RigidBody();

    // 물리 월드에 등록
    virtual void OnEnable();
    virtual void OnDisable();

    // 매 프레임 Transform 동기화
    virtual void FinalUpdate() override;

    shared_ptr<btRigidBody> GetBody() const { return _body; }

private:
    float                               _mass;
    bool                                _isKinematic = false;
    shared_ptr<BaseCollider>            _collider;
    shared_ptr<btDefaultMotionState>    _motionState;
    shared_ptr<btRigidBody>             _body;
    Matrix                              _matrix;

    weak_ptr<GameObject>                _weakGameObject;        // 이 컴포넌트를 가진 객체
};

class CharacterController : public Component
{
public:
    // radius: 캡슐 반지름, height: 캡슐 높이, stepHeight: 한 걸음 최대 오르막 높이
    CharacterController(shared_ptr<GameObject> gameObject, float radius, float height, float stepHeight);
    ~CharacterController();

    // 이동 방향 벡터 단위(월드 스페이스)
    void Move(const Vec3& direction);
    // 점프

    void Jump();

    // Component 인터페이스
    virtual void OnEnable();
    virtual void OnDisable();
    virtual void FinalUpdate() override;

    shared_ptr<btPairCachingGhostObject>GetGhostObject() { return _ghost; }
    shared_ptr<btKinematicCharacterController> GetController() { return _controller; }
    bool IsOnGround() const {
        return _controller->onGround();
    }

    void  SetPushTimer(float pushTimer) { _pushTimer = pushTimer; }
    void  SetIsPushing(bool isPushing) { _isPushing = isPushing; }
    void  SetPushDirection(const btVector3& direction) { _pushDirection = direction; }

   bool GetIsPushing() { return _isPushing; }

private:
    float _radius, _height, _stepHeight;

    float _pushTimer;
    bool  _isPushing;
    btVector3 _pushDirection;

    shared_ptr<btPairCachingGhostObject>    _ghost;
    shared_ptr<btCapsuleShape>              _shape;
    shared_ptr<btKinematicCharacterController> _controller;

    weak_ptr<GameObject>                _weakGameObject;        // 이 컴포넌트를 가진 객체
};