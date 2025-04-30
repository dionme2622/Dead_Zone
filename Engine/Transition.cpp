#include "pch.h"
#include "Transition.h"

//Transition::Transition(shared_ptr<AnimationState> targetState)
//    : _targetState(targetState)
//{}
//
//void Transition::AddCondition(const string& parameterName, ParameterConditionType type, float value)
//{
//    _conditions.push_back({ parameterName, type, value });
//}
//
//bool Transition::IsConditionMet(const unordered_map<string, AnimatorParameter>& parameters) const
//{
//    // 모든 조건이 만족해야 전이
//    for (auto& cond : _conditions)
//    {
//        auto it = parameters.find(cond.parameterName);
//        if (it == parameters.end())
//            return false;
//        const AnimatorParameter& p = it->second;
//
//        bool pass = false;
//        switch (cond.type)
//        {
//        case ParameterConditionType::EQUAL:
//            if (p.type == ParameterType::FLOAT && fabs(p.floatValue - cond.value) < 1e-4f)
//                pass = true;
//            else if (p.type == ParameterType::INT && p.intValue == static_cast<int>(cond.value))
//                pass = true;
//            else if (p.type == ParameterType::BOOL && p.boolValue == (cond.value != 0.0f))
//                pass = true;
//            break;
//        case ParameterConditionType::GREATER:
//            if ((p.type == ParameterType::FLOAT && p.floatValue > cond.value) ||
//                (p.type == ParameterType::INT && p.intValue > static_cast<int>(cond.value)))
//                pass = true;
//            break;
//        case ParameterConditionType::LESS:
//            if ((p.type == ParameterType::FLOAT && p.floatValue < cond.value) ||
//                (p.type == ParameterType::INT && p.intValue < static_cast<int>(cond.value)))
//                pass = true;
//            break;
//        }
//        if (!pass)
//            return false;
//    }
//    return !_conditions.empty();
//}
