#pragma once
#include "Mesh.h"

struct AnimClipInfo;
class Transition;

class AnimationState
{
public:
    AnimationState(const wstring& name, shared_ptr<AnimClipInfo> clip, int32 _clipIndex, float speed, bool loop);

    const wstring& GetName() const { return _name; }
    shared_ptr<AnimClipInfo> GetClip() const { return _clip; }
    float GetSpeed() const { return _speed; }
    bool IsLooping() const { return _loop; }
    int32 GetClipIndex() { return _clipIndex; }
    void AddTransition(shared_ptr<Transition> transition);
    const vector<shared_ptr<Transition>>& GetTransitions() const { return _transitions; }

private:
    wstring _name;
    shared_ptr<AnimClipInfo> _clip;
    int32 _clipIndex;
    float _speed;
    bool _loop;

    vector<shared_ptr<Transition>> _transitions;
};