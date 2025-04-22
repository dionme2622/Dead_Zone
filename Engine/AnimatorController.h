#pragma once
#include "Mesh.h"

class Animator;
class AnimationState;
class AnimatorParameter;

class AnimatorController
{
public:
    AnimatorController();
    ~AnimatorController();

    void Update(shared_ptr<Animator> animator);

    void AddState(shared_ptr<AnimationState> state);
    void SetAnyState(shared_ptr<AnimationState> state);
    void SetDefaultState(const wstring& name);
    void SetState(const wstring& name);

    shared_ptr<AnimationState> GetCurrentState() const { return _currentState; }
    shared_ptr<AnimClipInfo> GetCurrentClip() const;
    const int GetCurrentClipIndex() const;

    void AddParameter(const AnimatorParameter& parameter);
    void SetBool(const string& name, bool value);
    void SetFloat(const string& name, float value);
    void SetInt(const string& name, int value);
    bool GetBool(const string& name) const;
    float GetFloat(const string& name) const;
    int GetInt(const string& name) const;

private:
    unordered_map<wstring, shared_ptr<AnimationState>> _states;
    shared_ptr<AnimationState> _defaultState;
    shared_ptr<AnimationState> _anyState;
    shared_ptr<AnimationState> _currentState;

    unordered_map<string, AnimatorParameter> _parameters;
};