#pragma once
#include "AnimatorParameter.h"

class AnimationState;

class Transition
{
public:
    Transition(shared_ptr<AnimationState> targetState);

    void AddCondition(const string& parameterName, ParameterConditionType type, float value);

    bool IsConditionMet(const unordered_map<string, AnimatorParameter>& parameters) const;

    shared_ptr<AnimationState> GetTargetState() const { return _targetState; }

private:
    shared_ptr<AnimationState> _targetState;

    struct Condition
    {
        string parameterName;
        ParameterConditionType type;
        float value;
    };

    vector<Condition> _conditions;
};

