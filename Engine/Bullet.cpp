#include "pch.h"
#include "EnginePch.h"
#include "Bullet.h"
#include "Transform.h"
#include "Timer.h"
#include "SceneManager.h"


Bullet::Bullet() : Component(COMPONENT_TYPE::BULLET), _isFired(false)
{
}

Bullet::~Bullet()
{
}

void Bullet::DeleteBullet()
{
    if(_lifeDistance >= 500.0)
        _isFired = false;
}

void Bullet::FinalUpdate()
{
    if (_isFired)
    {
        Vec3 pos = GetTransform()->GetLocalPosition();
        pos += _direction * _speed * DELTA_TIME;
        GetTransform()->SetLocalPosition(pos);

        _lifeDistance += Vec3::Distance(_direction * _speed * DELTA_TIME, pos);

        DeleteBullet();
    }
}

