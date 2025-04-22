#pragma once
#include "Mesh.h"

struct AnimClipInfo;
class Transition;

class AnimationState
{
public:
    AnimationState(const wstring& name, shared_ptr<AnimClipInfo> clip, int index, float speed, bool loop);

    const wstring& GetName() const { return _name; }
    const shared_ptr<AnimClipInfo> GetClip() const { return _clip; }
    int32 GetClipIndex() const { return _clipIndex; }

    float GetSpeed() const { return _speed; }
    void SetSpeed(float speed) { _speed = speed; }
    bool IsLooping() const { return _loop; }
    void SetLoop(bool loop) { _loop = loop; }

    void AddTransition(shared_ptr<Transition> transition);
    const vector<shared_ptr<Transition>>& GetTransitions() const { return _transitions; }

private:
    wstring                         _name;
    shared_ptr<AnimClipInfo>        _clip;
    int32                           _clipIndex;
    float                           _speed = 1.0f;
    bool                            _loop = true;

    vector<shared_ptr<Transition>> _transitions;
};
