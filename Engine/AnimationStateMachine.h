#pragma once

class Animator;

enum class EAnimState
{
    Idle = 0,
    Walk,
    Run,
    Jump,
    Attack,
    END
};

struct AnimTransition
{
    EAnimState    from;
    EAnimState    to;
    // ������ true �̸� ����
    function<bool()>  condition;
    float          exitTime;       // ���� Ŭ�� ����(0~1) ���Ŀ��� ����
    float          blendDuration;  // ���� �ð�(��)
};

class AnimationStateMachine
{
public:
    AnimationStateMachine(shared_ptr<Animator> animator);

    // �� ������ ȣ��
    void Update(float deltaTime);

    // �ܺο��� �Ķ���� ����
    void SetSpeed(float s) { _speed = s; }
    void SetJumped(bool j) { _didJump = j; }
    void TriggerAttack() { _attackTriggered = true; }

private:
    void CheckTransitions();
    void DoTransition(const AnimTransition& t);

private:
    shared_ptr<Animator> _animator;
    EAnimState    _currentState = EAnimState::Idle;
    EAnimState    _nextState = EAnimState::Idle;
    bool          _inTransition = false;
    float         _stateTime = 0.f;
    float         _blendTime = 0.f;

    // �Ķ����
    float _speed = 0.f;
    bool  _didJump = false;
    bool  _attackTriggered = false;

    vector<AnimTransition> _transitions;
    float _blendDurationDefault = 0.2f;

    // Helper: ������Ʈ �� �ִϸ��̼� Ŭ�� �ε��� ����
    int  StateToClipIndex(EAnimState s);
};
