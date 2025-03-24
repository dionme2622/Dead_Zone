#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Engine.h"
#include "Material.h"
#include "Shader.h"
#include "ParticleSystem.h"
#include "InstancingManager.h"
#include "BaseCollider.h"
#include "BoxCollider.h"

Matrix Camera::S_MatView;
Matrix Camera::S_MatProjection;

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
	_width = static_cast<float>(GEngine->GetWindow().width);
	_height = static_cast<float>(GEngine->GetWindow().height);
}

Camera::~Camera()
{
}

void Camera::FinalUpdate()
{

	_matView = GetTransform()->GetLocalToWorldMatrix().Invert();
	if (_type == PROJECTION_TYPE::PERSPECTIVE) {

	}

	if (_type != PROJECTION_TYPE::PERSPECTIVE) {
		//printf("%f", GetTransform()->GetLocalToWorldMatrix().Forward().X)
	}


	if (_type == PROJECTION_TYPE::PERSPECTIVE) 
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, _width / _height, _near, _far);
	else
		_matProjection = ::XMMatrixOrthographicLH(_width * _scale, _height * _scale, _near, _far);

	_frustum.FinalUpdate();

}

void Camera::SortGameObject()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	_vecForward.clear();
	_vecDeferred.clear();
	_vecParticle.clear();

	int a = 0;
	for (auto& gameObject : gameObjects)
	{
		
		if (gameObject->GetMeshRenderer() == nullptr && gameObject->GetParticleSystem() == nullptr) {
			continue;
		}
		
		if (IsCulled(gameObject->GetLayerIndex())) {
			continue;
		}

		if (gameObject->GetCheckFrustum())
		{
			shared_ptr<BaseCollider> baseCollider = gameObject->GetCollider();
			shared_ptr<BoxCollider> boxCollider = dynamic_pointer_cast<BoxCollider>(baseCollider);
			/*if (boxCollider)
				cout << boxCollider->GetBoundingBox().Center.x << endl;*/

			if (boxCollider) {
				
				Vec3 scale = gameObject->GetTransform()->GetLocalScale();
				float scaledExtentX = boxCollider->_extents.x * scale.x;
				float scaledExtentY = boxCollider->_extents.y * scale.y;
				float scaledExtentZ = boxCollider->_extents.z * scale.z;
				//cout << scaledExtentX << endl;

				if (_frustum.ContainsSphere(
					gameObject->GetTransform()->GetWorldPosition(),
					max(max(scaledExtentX, scaledExtentY), scaledExtentZ) + 80) == false)
				{
					continue;
				}
			}
			++a;
			/*if (_frustum.ContainsSphere(
				gameObject->GetTransform()->GetWorldPosition(),
				gameObject->GetTransform()->GetBoundingSphereRadius()) == false)
			{
				continue;
			}*/
		}

		if (gameObject->GetMeshRenderer())
		{
			SHADER_TYPE shaderType = gameObject->GetMeshRenderer()->GetMaterial()->GetShader()->GetShaderType();
			switch (shaderType)
			{
			case SHADER_TYPE::DEFERRED:
				_vecDeferred.emplace_back(gameObject);
				break;
			case SHADER_TYPE::FORWARD:
				_vecForward.emplace_back(gameObject);
				break;
			}
		}
		else
		{
			_vecParticle.emplace_back(gameObject);
		}
	}
	cout << a << endl;
}

void Camera::SortShadowObject()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	_vecShadow.clear();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetMeshRenderer() == nullptr)
			continue;

		if (gameObject->IsStatic())
			continue;

		if (IsCulled(gameObject->GetLayerIndex()))
			continue;

		if (gameObject->GetCheckFrustum())
		{
			if (_frustum.ContainsSphere(
				gameObject->GetTransform()->GetWorldPosition(),
				gameObject->GetTransform()->GetBoundingSphereRadius()) == false)
			{
				continue;
			}
		}

		_vecShadow.emplace_back(gameObject);
	}
}

void Camera::Render_Deferred()
{
	//if (GetProjectionType() == PROJECTION_TYPE::PERSPECTIVE) {
		S_MatView = _matView;
		S_MatProjection = _matProjection;
	//}


#ifdef _INSTANCING_MODE
	GET_SINGLE(InstancingManager)->Render(_vecDeferred);

#else
	for (auto& gameObject : _vecDeferred)
	{
		gameObject->GetMeshRenderer()->Render();
	}	
#endif

}

void Camera::Render_Forward()
{
	S_MatView = _matView;
	if (_type == PROJECTION_TYPE::PERSPECTIVE) {
		//printf("%f\n", GetTransform()->GetLocalRotation().x);
	}
	else {
		Vec3 pos = GetTransform()->GetLocalRotation();
		//S_MatView = ::XMMatrixTranslation(-pos.x, -pos.y, -pos.z); // 회전 없이 위치만 반영
	}
	S_MatProjection = _matProjection;
	//if (GetProjectionType() == PROJECTION_TYPE::PERSPECTIVE) {
		S_MatView = _matView;
		S_MatProjection = _matProjection;
	//}
	/*S_MatView = _matView;
	S_MatProjection = _matProjection;*/

#ifdef _INSTANCING_MODE
	GET_SINGLE(InstancingManager)->Render(_vecForward);

#else
	for (auto& gameObject : _vecForward)
	{
		gameObject->GetMeshRenderer()->Render();
	}

#endif
	for (auto& gameObject : _vecParticle)
	{
		gameObject->GetParticleSystem()->Render();
	}

	// Bounding Box 그리기
	/*for (auto& gameObject : _vecForward)
	{
		gameObject->GetCollider()->Render();
	}*/
}


void Camera::Render_Shadow()
{

	//if (GetProjectionType() == PROJECTION_TYPE::PERSPECTIVE) {
		S_MatView = _matView;
		S_MatProjection = _matProjection;
	//}

	for (auto& gameObject : _vecShadow)
	{
		gameObject->GetMeshRenderer()->RenderShadow();
	}
}