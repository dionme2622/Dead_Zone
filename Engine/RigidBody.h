#pragma once
#include "Component.h"
#include "BaseCollider.h"
#include "GameObject.h"

class RigidBody : public Component
{
public:

	RigidBody(shared_ptr<GameObject> gameObject, float mass, shared_ptr<BaseCollider> collider, Vec3 pos, bool isKinematic = false);
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

    weak_ptr<GameObject>                _weakGameObject;        // �� ������Ʈ�� ���� ��ü
};

