#include "pch.h"
#include "Frustum.h"
#include "Camera.h"

void Frustum::FinalUpdate()
{
    Matrix matViewInv = Camera::S_MatView.Invert();
    Matrix matProjectionInv = Camera::S_MatProjection.Invert();
    Matrix matInv = matProjectionInv * matViewInv;

    vector<Vec3> worldPos =
    {
        ::XMVector3TransformCoord(Vec3(-1.f, 1.f, 0.f), matInv),
        ::XMVector3TransformCoord(Vec3(1.f, 1.f, 0.f), matInv),
        ::XMVector3TransformCoord(Vec3(1.f, -1.f, 0.f), matInv),
        ::XMVector3TransformCoord(Vec3(-1.f, -1.f, 0.f), matInv),
        ::XMVector3TransformCoord(Vec3(-1.f, 1.f, 1.f), matInv),
        ::XMVector3TransformCoord(Vec3(1.f, 1.f, 1.f), matInv),
        ::XMVector3TransformCoord(Vec3(1.f, -1.f, 1.f), matInv),
        ::XMVector3TransformCoord(Vec3(-1.f, -1.f, 1.f), matInv)
    };

    _planes[PLANE_FRONT] = ::XMPlaneFromPoints(worldPos[0], worldPos[1], worldPos[2]); // CW
    _planes[PLANE_BACK] = ::XMPlaneFromPoints(worldPos[4], worldPos[7], worldPos[5]); // CCW
    _planes[PLANE_UP] = ::XMPlaneFromPoints(worldPos[4], worldPos[5], worldPos[1]); // CW
    _planes[PLANE_DOWN] = ::XMPlaneFromPoints(worldPos[7], worldPos[3], worldPos[6]); // CCW
    _planes[PLANE_LEFT] = ::XMPlaneFromPoints(worldPos[4], worldPos[0], worldPos[7]); // CW
    _planes[PLANE_RIGHT] = ::XMPlaneFromPoints(worldPos[5], worldPos[6], worldPos[1]); // CCW
}

bool Frustum::ContainsSphere(const Vec3& pos, float radius)
{
    float cameraDistance = (pos - Vec3(Camera::S_MatView._11, Camera::S_MatView._12, Camera::S_MatView._13) ).Length();
    float margin = max(50.0f, cameraDistance * 0.4f);  // �ּ� 50 ����, �Ÿ� 5% ����

    for (const Vec4& plane : _planes)
    {
        Vec3 normal = Vec3(plane.x, plane.y, plane.z);
        float distance = normal.Dot(pos) + plane.w;

        // ���� ����� �ٱ��� ������ ��ġ�� ��� �ø�
        if (distance > radius + margin)
            return false;
    }

    return true;
}


