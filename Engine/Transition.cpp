#include "pch.h"
//#include "Transition.h"
//
//Transition::Transition(shared_ptr<AnimationState> targetState)
//    : _targetState(targetState) {}
//
//void Transition::AddCondition(const string& parameterName, ParameterConditionType type, float value)
//{
//    _conditions.push_back({ parameterName, type, value });
//}
//
//bool Transition::IsConditionMet(const unordered_map<string, AnimatorParameter>& parameters) const
//{
//    for (const auto& cond : _conditions)
//    {
//        auto iter = parameters.find(cond.parameterName);
//        if (iter == parameters.end())
//            return false;
//
//        const auto& param = iter->second;
//
//        switch (cond.type)
//        {
//        case ParameterConditionType::EQUAL:
//            if (param.floatValue != cond.value) return false;
//            break;
//        case ParameterConditionType::GREATER:
//            if (param.floatValue <= cond.value) return false;
//            break;
//        case ParameterConditionType::LESS:
//            if (param.floatValue >= cond.value) return false;
//            break;
//        }
//    }
//    return true;
//}