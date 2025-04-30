// Transition.h
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "AnimatorParameter.h"

enum class ConditionMode { Greater, Less, Equals };

class AnimationState;

struct TransitionCondition
{
    int            paramIndex;  // 파라미터 인덱스
    ConditionMode  mode;        // 비교 방식
    float          threshold;   // 임계값
    float          exitTime;    // normalized [0,1]
    float          duration;    // cross‑fade 시간
};

class Transition
{
public:
    explicit Transition(shared_ptr<AnimationState> target)
        : _target(target)
    {}

    // 파라미터 인덱스를 직접 받아 조건 추가
    void AddCondition(int paramIndex,
        ConditionMode mode,
        float threshold,
        float exitTime,
        float duration)
    {
        _conditions.push_back({ paramIndex, mode, threshold, exitTime, duration });
    }

    bool IsConditionMet(const vector<float>& floatParams,
        const vector<bool>& boolParams,
        const vector<bool>& triggerParams) const
    {
        for (auto& c : _conditions)
        {
            bool pass = false;
            // float 파라미터 먼저 검사
            float v = floatParams[c.paramIndex];
            switch (c.mode)
            {
            case ConditionMode::Greater: pass = (v > c.threshold); break;
            case ConditionMode::Less:    pass = (v < c.threshold); break;
            case ConditionMode::Equals:  pass = (fabsf(v - c.threshold) < 1e-4f); break;
            }
            // bool/trigger 파라미터도 OR 검사
            if (!pass && c.paramIndex < boolParams.size())    pass = boolParams[c.paramIndex];
            if (!pass && c.paramIndex < triggerParams.size()) pass = triggerParams[c.paramIndex];
            if (!pass) return false;
        }
        return !_conditions.empty();
    }

    shared_ptr<AnimationState> GetTarget() const { return _target; }
    vector<TransitionCondition> GetConditions() const { return _conditions; }
private:
    shared_ptr<AnimationState>      _target;
    vector<TransitionCondition>     _conditions;
};