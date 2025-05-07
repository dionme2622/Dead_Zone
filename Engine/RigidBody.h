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

    // ���� ���忡 ���
    virtual void OnEnable();
    virtual void OnDisable();

    // �� ������ Transform ����ȭ
    virtual void FinalUpdate() override;

    shared_ptr<btRigidBody> GetBody() const { return _body; }

private:
    float                               _mass;
    bool                                _isKinematic = false;
    shared_ptr<BaseCollider>            _collider;
    shared_ptr<btDefaultMotionState>    _motionState;
    shared_ptr<btRigidBody>             _body;
    Matrix                              _matrix;

    weak_ptr<GameObject>                _weakGameObject;        // �� ������Ʈ�� ���� ��ü
};

class CharacterController : public Component
{
public:
    // radius: ĸ�� ������, height: ĸ�� ����, stepHeight: �� ���� �ִ� ������ ����
    CharacterController(shared_ptr<GameObject> gameObject, float radius, float height, float stepHeight);
    ~CharacterController();

    // �̵� ���� ���� ����(���� �����̽�)
    void Move(const Vec3& direction);
    // ����

    void Jump();

    // Component �������̽�
    virtual void OnEnable();
    virtual void OnDisable();
    virtual void FinalUpdate() override;

    bool IsOnGround() const {
        return _controller->onGround();
    }
private:
    float _radius, _height, _stepHeight;

    std::shared_ptr<btPairCachingGhostObject>    _ghost;
    std::shared_ptr<btCapsuleShape>              _shape;
    std::shared_ptr<btKinematicCharacterController> _controller;

    weak_ptr<GameObject>                _weakGameObject;        // �� ������Ʈ�� ���� ��ü
};