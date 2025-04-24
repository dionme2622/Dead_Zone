#pragma once
#include <string>

enum class ParameterType
{
    Bool,
    Float,
    Trigger,
};

struct AnimatorParameter
{
    string name;
    ParameterType type;
    float defaultValue;
};