#include "pch.h"
#include "AnimationStateMachine.h"
#include "Animator.h"

AnimationStateMachine::AnimationStateMachine(std::shared_ptr<Animator> animator)
    : _animator(animator)
{
    // Ʈ�����ǵ��� ���
    _transitions = {
        // Idle �� Walk
        { EAnimState::Idle,  EAnimState::Walk,
          [this]() { return _speed > 0.1f && _speed <= 0.8f; },
          0.0f, 0.1f },

          // Walk �� Run
          { EAnimState::Walk,  EAnimState::Run,
            [this]() { return _speed > 0.8f; },
            0.0f, 0.15f },

            // Run �� Walk (����)
            { EAnimState::Run,   EAnimState::Walk,
              [this]() { return _speed <= 0.8f && _speed > 0.1f; },
              0.0f, 0.15f },

              // Walk/Run �� Idle
              { EAnimState::Walk,  EAnimState::Idle,
                [this]() { return _speed <= 0.1f; }, 0.0f, 0.1f },
              { EAnimState::Run,   EAnimState::Idle,
                [this]() { return _speed <= 0.1f; }, 0.0f, 0.1f },

                // Any �� Jump
                { EAnimState::Idle,  EAnimState::Jump,
                  [this]() { return _didJump; }, 0.0f, 0.1f },
                { EAnimState::Walk,  EAnimState::Jump,
                  [this]() { return _didJump; }, 0.0f, 0.1f },
                { EAnimState::Run,   EAnimState::Jump,
                  [this]() { return _didJump; }, 0.0f, 0.1f },

                  // Jump �� Idle (���� ������)
                  { EAnimState::Jump,  EAnimState::Idle,
                    [this]() { return _stateTime >= 1.0f; }, 0.9f, 0.1f },

                    // Any �� Attack (Ʈ����)
                    { EAnimState::Idle,  EAnimState::Attack,
                      [this]() { return _attackTriggered; }, 0.0f, 0.1f },
                    { EAnimState::Walk,  EAnimState::Attack,
                      [this]() { return _attackTriggered; }, 0.0f, 0.1f },
                    { EAnimState::Run,   EAnimState::Attack,
                      [this]() { return _attackTriggered; }, 0.0f, 0.1f },
    };

    // ó���� Idle ���
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
    // 1) �ð� ����
    _stateTime += dt;

    // 2) ���� �˻�
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
        // ���� �ð� ���� & �Ϸ� �˻�
        _blendTime += dt;
        if (_blendTime >= t.blendDuration)
        {
            _inTransition = false;
            _currentState = _nextState;
            _stateTime = 0.f;
            _attackTriggered = false;  // ���� Ʈ���� ����
        }
    }
}

void AnimationStateMachine::DoTransition(const AnimTransition& t)
{
    _inTransition = true;
    _blendTime = 0.f;
    _nextState = t.to;

    // �� Ŭ��(����/����) �ε����� Animator�� �ѱ�� blendRatio�� ����
    _animator->SetTransition(
        StateToClipIndex(t.from),
        StateToClipIndex(t.to),
        t.blendDuration
    );

    // ������GPU �� Compute Shader��
    // currentFrame, nextFrame, ratio �Ķ���� override �ʿ�
}