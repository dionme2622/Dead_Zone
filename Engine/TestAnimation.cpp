#include "pch.h"
#include "TestAnimation.h"
#include "KeyInput.h"
#include "Animator.h"
#include "Transform.h"
#include "Timer.h"
void TestAnimation::Update()
{
	/*if (INPUT->GetButtonDown(KEY_TYPE::KEY_3))
	{
		int32 count = GetAnimator()->GetAnimCount();
		int32 currentIndex = GetAnimator()->GetCurrentClipIndex();

		int32 index = (currentIndex + 1) % count;
		GetAnimator()->Play(index);
	}*/

	if (INPUT->GetButtonDown(KEY_TYPE::KEY_4))
	{
		int32 count = GetAnimator()->GetAnimCount();
		int32 currentIndex = GetAnimator()->GetCurrentClipIndex();

		int32 index = (currentIndex + 1 + count) % count;
		GetAnimator()->Play(index);
	}

	Vec3 pos = GetTransform()->GetLocalPosition();


	/*if (INPUT->GetButton(KEY_TYPE::KEY_4))
		pos += GetTransform()->GetLook() * 1.f * DELTA_TIME;*/


	GetTransform()->SetLocalPosition(pos);

}