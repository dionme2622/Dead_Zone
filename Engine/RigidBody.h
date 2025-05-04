#pragma once
#include "Component.h"
#include "BaseCollider.h"
#include "GameObject.h"

class RigidBody : public Component
{
public:

	RigidBody(shared_ptr<GameObject> gameObject, float mass, shared_ptr<BaseCollider> collider, Vec3 pos, bool isKinematic = false);
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

    weak_ptr<GameObject>                _weakGameObject;        // 이 컴포넌트를 가진 객체
};

