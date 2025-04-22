#include "pch.h"
#include "AnimationState.h"
#include "Transition.h"

AnimationState::AnimationState(const wstring& name, shared_ptr<AnimClipInfo> clip, int index, float speed = 1.0f, bool loop = true)
    : _name(name), _clip(clip), _clipIndex(index), _speed(speed), _loop(loop) {}

void AnimationState::AddTransition(shared_ptr<Transition> transition)
{
    _transitions.push_back(transition);
}
