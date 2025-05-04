#pragma once
#include "Component.h"
#include "BaseCollider.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

class RigidBody : public Component
{
public:

	RigidBody(float mass, shared_ptr<BaseCollider> collider, Matrix matrix, bool isKinematic = false);
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
};

class CharacterController : public Component
{
public:
    // radius: 캡슐 반지름, height: 캡슐 높이, stepHeight: 한 걸음 최대 오르막 높이
    CharacterController(float radius, float height, float stepHeight);
    ~CharacterController();

    // 이동 방향 벡터 단위(월드 스페이스)
    void Move(const Vec3& direction);
    // 점프
    void Jump();

    // Component 인터페이스
    virtual void OnEnable();
    virtual void OnDisable();
    virtual void FinalUpdate() override;

private:
    float _radius, _height, _stepHeight;

    std::shared_ptr<btPairCachingGhostObject>    _ghost;
    std::shared_ptr<btCapsuleShape>              _shape;
    std::shared_ptr<btKinematicCharacterController> _controller;
};