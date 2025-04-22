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
    // 조건이 true 이면 전이
    function<bool()>  condition;
    float          exitTime;       // 현재 클립 진행(0~1) 이후에만 전이
    float          blendDuration;  // 블렌딩 시간(초)
};

class AnimationStateMachine
{
public:
    AnimationStateMachine(shared_ptr<Animator> animator);

    // 매 프레임 호출
    void Update(float deltaTime);

    // 외부에서 파라미터 세팅
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

    // 파라미터
    float _speed = 0.f;
    bool  _didJump = false;
    bool  _attackTriggered = false;

    vector<AnimTransition> _transitions;
    float _blendDurationDefault = 0.2f;

    // Helper: 스테이트 → 애니메이션 클립 인덱스 매핑
    int  StateToClipIndex(EAnimState s);
};
