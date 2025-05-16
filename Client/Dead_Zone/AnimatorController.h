// AnimatorController.h
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "AnimatorParameter.h"
#include "AnimationState.h"

class AnimatorController
{
public:
    void AddParameter(const AnimatorParameter& param)
    {
        int idx = (int)_paramDefs.size();
        _paramMap[param.name] = idx;
        _paramDefs.push_back(param);
    }

    void AddState(shared_ptr<AnimationState> state)
    {
        _states[state->GetName()] = state;
    }

    void SetEntryState(const wstring& name)
    {
        _entryState = _states[name];
        _currentState = _entryState;
    }

    void SetAnyState(const wstring& name)
    {
        _anyState = _states[name];
    }

    void SetCurrentState(const wstring& name)
    {
        _currentState = _states[name];
    }
    shared_ptr<AnimationState> GetCurrentState() const { return _currentState; }
    shared_ptr<AnimationState> GetEntryState() const { return _entryState; }
    shared_ptr<AnimationState> GetAnyState() const { return _anyState; }

    const vector<AnimatorParameter>& GetParamDefs() const { return _paramDefs; }
    int GetParamIndex(const string& name) const
    {
        auto it = _paramMap.find(name);
        return it == _paramMap.end() ? -1 : it->second;
    }

private:
    vector<AnimatorParameter> _paramDefs;
    unordered_map<string, int> _paramMap;

    unordered_map<wstring, shared_ptr<AnimationState>> _states;
    shared_ptr<AnimationState> _entryState;
    shared_ptr<AnimationState> _anyState;
    shared_ptr<AnimationState> _currentState;
};