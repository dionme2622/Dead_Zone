// AnimationState.h
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Transition.h"
#include "Mesh.h"

class AnimationState
{
public:
    AnimationState(const wstring& name, shared_ptr<AnimClipInfo> animClip,int clipIndex, float speed = 1.0f, bool loop = true)
        : _name(name), _animClip(animClip), _clipIndex(clipIndex), _speed(speed), _loop(loop)
    {}

    const wstring& GetName() const { return _name; }
    
    shared_ptr<AnimClipInfo> GetClip() { return _animClip; }
    int GetClipIndex() const { return _clipIndex; }
    float GetSpeed() const { return _speed; }
    bool IsLooping() const { return _loop; }

    void AddTransition(shared_ptr<Transition> t) { _transitions.push_back(t); }
    const vector<shared_ptr<Transition>>& GetTransitions() const { return _transitions; }

private:
    wstring _name;
    shared_ptr<AnimClipInfo> _animClip;
    int _clipIndex;
    float _speed;
    bool _loop;
    vector<shared_ptr<Transition>> _transitions;
};