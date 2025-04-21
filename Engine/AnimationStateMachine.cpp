#include "pch.h"
#include "AnimationStateMachine.h"
#include "Animator.h"

AnimationStateMachine::AnimationStateMachine(std::shared_ptr<Animator> animator)
    : _animator(animator)
{
    // 트랜지션들을 등록
    _transitions = {
        // Idle → Walk
        { EAnimState::Idle,  EAnimState::Walk,
          [this]() { return _speed > 0.1f && _speed <= 0.8f; },
          0.0f, 0.1f },

          // Walk → Run
          { EAnimState::Walk,  EAnimState::Run,
            [this]() { return _speed > 0.8f; },
            0.0f, 0.15f },

            // Run → Walk (감속)
            { EAnimState::Run,   EAnimState::Walk,
              [this]() { return _speed <= 0.8f && _speed > 0.1f; },
              0.0f, 0.15f },

              // Walk/Run → Idle
              { EAnimState::Walk,  EAnimState::Idle,
                [this]() { return _speed <= 0.1f; }, 0.0f, 0.1f },
              { EAnimState::Run,   EAnimState::Idle,
                [this]() { return _speed <= 0.1f; }, 0.0f, 0.1f },

                // Any → Jump
                { EAnimState::Idle,  EAnimState::Jump,
                  [this]() { return _didJump; }, 0.0f, 0.1f },
                { EAnimState::Walk,  EAnimState::Jump,
                  [this]() { return _didJump; }, 0.0f, 0.1f },
                { EAnimState::Run,   EAnimState::Jump,
                  [this]() { return _didJump; }, 0.0f, 0.1f },

                  // Jump → Idle (점프 끝나면)
                  { EAnimState::Jump,  EAnimState::Idle,
                    [this]() { return _stateTime >= 1.0f; }, 0.9f, 0.1f },

                    // Any → Attack (트리거)
                    { EAnimState::Idle,  EAnimState::Attack,
                      [this]() { return _attackTriggered; }, 0.0f, 0.1f },
                    { EAnimState::Walk,  EAnimState::Attack,
                      [this]() { return _attackTriggered; }, 0.0f, 0.1f },
                    { EAnimState::Run,   EAnimState::Attack,
                      [this]() { return _attackTriggered; }, 0.0f, 0.1f },
    };

    // 처음엔 Idle 재생
    _animator->Play(StateToClipIndex(_currentState));
}

int AnimationStateMachine::StateToClipIndex(EAnimState s)
{
    switch (s)
    {
    case EAnimState::Idle:   return 0;
    case EAnimState::Walk:   return 1;
    case EAnimState::Run:    return 2;
    case EAnimState::Jump:   return 3;
    case EAnimState::Attack: return 4;
    default: return 0;
    }
}

void AnimationStateMachine::Update(float dt)
{
    // 1) 시간 누적
    _stateTime += dt;

    // 2) 전이 검사
    if (!_inTransition)
    {
        for (auto& t : _transitions)
        {
            if (t.from == _currentState && t.condition())
            {
                DoTransition(t);
                break;
            }
        }
    }
    else
    {
        // 블렌딩 시간 누적 & 완료 검사
        _blendTime += dt;
        if (_blendTime >= t.blendDuration)
        {
            _inTransition = false;
            _currentState = _nextState;
            _stateTime = 0.f;
            _attackTriggered = false;  // 공격 트리거 리셋
        }
    }
}

void AnimationStateMachine::DoTransition(const AnimTransition& t)
{
    _inTransition = true;
    _blendTime = 0.f;
    _nextState = t.to;

    // 두 클립(현재/다음) 인덱스를 Animator에 넘기고 blendRatio도 셋팅
    _animator->SetTransition(
        StateToClipIndex(t.from),
        StateToClipIndex(t.to),
        t.blendDuration
    );

    // 서버→GPU 쪽 Compute Shader에
    // currentFrame, nextFrame, ratio 파라미터 override 필요
}