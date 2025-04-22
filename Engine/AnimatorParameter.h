#pragma once
#include <string>

enum class ParameterConditionType
{
    EQUAL,
    GREATER,
    LESS
};

enum class ParameterType
{
    BOOL,
    FLOAT,
    INT,
};

struct AnimatorParameter
{
    string name;
    ParameterType type;

    bool boolValue = false;
    float floatValue = 0.0f;
    int intValue = 0;
};