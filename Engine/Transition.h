// Transition.h
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "AnimatorParameter.h"

enum class ConditionMode { Greater, Less, Equals };


struct TransitionCondition
{
    int            paramIndex;
    ParameterType  paramType;   // 새로 추가
    ConditionMode  mode;        // Float 에만 의미 있음
    float          threshold;   // Bool: 1.0f= true, 0.0f=false
    float          exitTime;
    float          duration;
};

class AnimationState;



class Transition
{
public:
    explicit Transition(shared_ptr<AnimationState> target)
        : _target(target)
    {}

    // 파라미터 인덱스를 직접 받아 조건 추가
    void AddCondition(int paramIndex,
        ParameterType paramType,
        ConditionMode mode,
        float threshold,
        float exitTime,
        float duration)
    {
        _conditions.push_back({ paramIndex, paramType, mode, threshold, exitTime, duration });
    }

    bool IsConditionMet(
        const vector<float>& floatParams,
        const vector<bool>& boolParams,
        const vector<bool>& triggerParams) const
    {
        for (auto& c : _conditions)
        {
            bool pass = false;
            switch (c.paramType)
            {
            case ParameterType::Float:
            {
                float v = floatParams[c.paramIndex];
                switch (c.mode)
                {
                case ConditionMode::Greater: pass = (v > c.threshold); break;
                case ConditionMode::Less:    pass = (v < c.threshold); break;
                case ConditionMode::Equals:  pass = (fabsf(v - c.threshold) < 1e-4f); break;
                }
                break;
            }
            case ParameterType::Bool:
            {
                bool v = boolParams[c.paramIndex];
                // threshold는 0.0(false) 또는 1.0(true) 만 쓰도록 약속
                pass = (v == (c.threshold > 0.5f));
                break;
            }
            case ParameterType::Trigger:
            {
                pass = triggerParams[c.paramIndex];
                break;
            }
            }

            if (!pass)
                return false;
        }
        return !_conditions.empty();
    }

    shared_ptr<AnimationState> GetTarget() const { return _target; }
    vector<TransitionCondition> GetConditions() const { return _conditions; }
private:
    shared_ptr<AnimationState>      _target;
    vector<TransitionCondition>     _conditions;
};