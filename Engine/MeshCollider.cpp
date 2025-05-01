#include "pch.h"
#include "MeshCollider.h"
#include "PhysicsSystem.h"

MeshCollider::MeshCollider(const std::vector<btVector3>& vertices, const std::vector<int>& indices, bool isDynamic)
    : BaseCollider(ColliderType::MESH)
    , _isDynamic(isDynamic)
    , _verts(vertices)
    , _inds(indices)
{
    // (1) Triangle mesh 생성
    _triMesh = std::make_shared<btTriangleMesh>();
    for (size_t i = 0; i + 2 < _inds.size(); i += 3) {
        btVector3 v0 = _verts[_inds[i + 0]];
        btVector3 v1 = _verts[_inds[i + 1]];
        btVector3 v2 = _verts[_inds[i + 2]];
        _triMesh->addTriangle(v0, v1, v2);
    }

    // (2) Shape 생성
    if (_isDynamic) {
        // Convex 형태로 근사 (볼록 껍질)
        auto convex = std::make_shared<btConvexTriangleMeshShape>(_triMesh.get());
        _shape = std::move(convex);
    }
    else {
        // 정적 구조물용 BVH 가속 메시
        auto meshShape = std::make_shared<btBvhTriangleMeshShape>(_triMesh.get(), true);
        _shape = std::move(meshShape);
    }
}

MeshCollider::~MeshCollider() {}

bool MeshCollider::Intersects(const Vec4& rayOrigin, const Vec4& rayDir, float& outDistance) {
    auto* world = GET_SINGLE(PhysicsSystem)->GetCollisionWorld();
    btVector3 from(rayOrigin.x, rayOrigin.y, rayOrigin.z);
    btVector3 to(rayOrigin.x + rayDir.x, rayOrigin.y + rayDir.y, rayOrigin.z + rayDir.z);
    btCollisionWorld::ClosestRayResultCallback callback(from, to);
    world->rayTest(from, to, callback);
    if (callback.hasHit()) {
        outDistance = callback.m_closestHitFraction;
        return true;
    }
    return false;
}

void MeshCollider::FinalUpdate() {
    // 디버그용으로 메시 에지(Line) 그리기
    /*DirectX::XMMATRIX world = GetTransform()->GetWorldMatrix();
    for (size_t i = 0; i + 2 < _inds.size(); i += 3) {
        btVector3 v0 = _verts[_inds[i + 0]];
        btVector3 v1 = _verts[_inds[i + 1]];
        btVector3 v2 = _verts[_inds[i + 2]];
        DebugRenderer::Get().AddLine({ v0.x(), v0.y(), v0.z() }, { v1.x(), v1.y(), v1.z() }, { 1,1,0,1 });
        DebugRenderer::Get().AddLine({ v1.x(), v1.y(), v1.z() }, { v2.x(), v2.y(), v2.z() }, { 1,1,0,1 });
        DebugRenderer::Get().AddLine({ v2.x(), v2.y(), v2.z() }, { v0.x(), v0.y(), v0.z() }, { 1,1,0,1 });
    }*/
}

std::shared_ptr<Mesh> MeshCollider::GetColliderMesh() {
    // 필요시 메시 자원을 리턴하거나 빈 dummy
    return nullptr;
}