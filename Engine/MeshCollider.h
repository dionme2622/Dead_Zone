#pragma once
#include "BaseCollider.h"

class MeshCollider : public BaseCollider {
public:
    /// vertices: ������� ���������� ����, indices: �ﰢ�� �ε��� ����Ʈ
    MeshCollider(const vector<btVector3>& vertices, const vector<uint32>& indices, bool isDynamic = false);
    ~MeshCollider();

    virtual void FinalUpdate() override;
    virtual bool Intersects(const Vec4& rayOrigin, const Vec4& rayDir, float& outDistance) override;
    virtual std::shared_ptr<Mesh> GetColliderMesh() override;

    btCollisionShape* GetShape() const { return _shape.get(); }

private:
    bool _isDynamic;
    std::shared_ptr<btTriangleMesh>        _triMesh;
    std::shared_ptr<btCollisionShape>      _shape;
    std::vector<btVector3>                 _verts;
    std::vector<uint32>                       _inds;
};

