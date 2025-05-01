#pragma once
#include "BaseCollider.h"

class CapsuleCollider : public BaseCollider
{
public:
	CapsuleCollider(float radius, float height);

	virtual ~CapsuleCollider();

	virtual void FinalUpdate() override;

public:
	// PhysicsBody ������Ʈ�� �� shape�� �����ؼ� rigidbody ����
	virtual btCollisionShape* GetShape() const { return _shape.get(); }
	// ���� shape ����, DebugDraw(), RaycastIntersect() ��
	virtual bool Intersects(const Vec4& rayOrigin, const Vec4& rayDir, float& outDist) override;
	virtual shared_ptr<Mesh> GetColliderMesh() override;

	void SetRadius(float radius) { _radius = radius; };
	void SetHeight(float height) { _height = height; };


private:
	float		_radius = 0.5f;
	float		_height = 1.0f;

	shared_ptr<btCapsuleShape>		_shape;

};

