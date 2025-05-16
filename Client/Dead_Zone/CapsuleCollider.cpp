#include "pch.h"
#include "CapsuleCollider.h"
#include "Transform.h"
#include "PhysicsSystem.h"

CapsuleCollider::CapsuleCollider(float radius, float height) : BaseCollider(ColliderType::Capsule), _radius(radius), _height(height)
{
    // 1) Capsule Shape »ý¼º
    _shape = make_shared<btCapsuleShape>(_radius, _height);
}


CapsuleCollider::~CapsuleCollider()
{

}

void CapsuleCollider::FinalUpdate()
{
    GET_SINGLE(DebugRenderer)->AddCapsule(_radius, _height, GetTransform()->GetLocalToWorldMatrix(), {1,0,0,1});
}

bool CapsuleCollider::Intersects(const Vec4& rayOrigin, const Vec4& rayDir, float& outDistance) {
  
    auto* world = GET_SINGLE(PhysicsSystem)->GetCollisionWorld();
    btVector3 from(rayOrigin.x, rayOrigin.y, rayOrigin.z);
    btVector3 to(rayOrigin.x + rayDir.x, rayOrigin.y + rayDir.y, rayOrigin.z + rayDir.z);

    btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);
    world->rayTest(from, to, rayCallback);
    if (rayCallback.hasHit()) {
        outDistance = rayCallback.m_closestHitFraction;
        return true;
    }
    return false;
}

shared_ptr<Mesh> CapsuleCollider::GetColliderMesh()
{
    return shared_ptr<Mesh>();
}
