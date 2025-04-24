#include "pch.h"
#include "AnimatorController.h"
#include "AnimationState.h"
#include "AnimatorParameter.h"
#include "Animator.h"
#include "Transition.h"

//AnimatorController::AnimatorController() {}
//AnimatorController::~AnimatorController() {}
//
//void AnimatorController::Update(shared_ptr<Animator> animator)
//{
//    /*if (_currentState == nullptr)
//        return;
//
//    for (auto& transition : _currentState->GetTransitions())
//    {
//        if (transition->IsConditionMet(_parameters))
//        {
//            _currentState = transition->GetTargetState();
//            animator->ResetTime();
//            return;
//        }
//    }
//
//    if (_anyState)
//    {
//        for (auto& transition : _anyState->GetTransitions())
//        {
//            if (transition->IsConditionMet(_parameters))
//            {
//                _currentState = transition->GetTargetState();
//                animator->ResetTime();
//                return;
//            }
//        }
//    }*/
//}
//
//void AnimatorController::AddState(shared_ptr<AnimationState> state)
//{
//    _states[state->GetName()] = state;
//}
//
//void AnimatorController::SetAnyState(shared_ptr<AnimationState> state)
//{
//    _anyState = state;
//}
//
//void AnimatorController::SetDefaultState(const wstring& name)
//{
//    _defaultState = _states[name];
//    _currentState = _defaultState;
//}
//
//void AnimatorController::SetState(const wstring& name)
//{
//    auto iter = _states.find(name);
//    if (iter != _states.end())
//        _currentState = iter->second;
//}
//
//shared_ptr<AnimClipInfo> AnimatorController::GetCurrentClip() const
//{
//    return _currentState->GetClip();
//}
//
//const int AnimatorController::GetCurrentClipIndex() const
//{
//    return _currentState->GetClipIndex();
//}
//
//
//// Parameter Handling
//void AnimatorController::AddParameter(const AnimatorParameter& parameter)
//{
//    _parameters[parameter.name] = parameter;
//}
//
//void AnimatorController::SetBool(const string& name, bool value)
//{
//    _parameters[name].boolValue = value;
//}
//
//void AnimatorController::SetFloat(const string& name, float value)
//{
//    _parameters[name].floatValue = value;
//}
//
//void AnimatorController::SetInt(const string& name, int value)
//{
//    _parameters[name].intValue = value;
//}
//
//bool AnimatorController::GetBool(const string& name) const
//{
//    return _parameters.at(name).boolValue;
//}
//
//float AnimatorController::GetFloat(const string& name) const
//{
//    return _parameters.at(name).floatValue;
//}
//
//int AnimatorController::GetInt(const string& name) const
//{
//    return _parameters.at(name).intValue;
//}