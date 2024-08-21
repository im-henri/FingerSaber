#pragma once

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"

static inline UnityEngine::Quaternion operator*(UnityEngine::Quaternion lhs, UnityEngine::Quaternion rhs)
{
    return UnityEngine::Quaternion(
        lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
        lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
}

static inline UnityEngine::Vector3 operator-(UnityEngine::Vector3 a, UnityEngine::Vector3 b)
{
    return UnityEngine::Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline UnityEngine::Vector3 operator+(UnityEngine::Vector3 a, UnityEngine::Vector3 b)
{
    return UnityEngine::Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline UnityEngine::Vector3 operator*(UnityEngine::Vector3 a, float_t d)
{
    return UnityEngine::Vector3(a.x * d, a.y * d, a.z * d);
}

static inline UnityEngine::Vector3 operator*(float_t d, UnityEngine::Vector3 a)
{
    return UnityEngine::Vector3(a.x * d, a.y * d, a.z * d);
}

static inline float mathf_Clamp01(float value)
{
    bool flag = value < 0.0f;
    float result;
    if (flag)
    {
        result = 0.0f;
    }
    else
    {
        bool flag2 = value > 1.0f;
        if (flag2)
        {
            result = 1.0f;
        }
        else
        {
            result = value;
        }
    }
    return result;
}

static inline UnityEngine::Vector3 Vector3_Lerp(UnityEngine::Vector3 a, UnityEngine::Vector3 b, float t)
{
    t = mathf_Clamp01(t);
    return UnityEngine::Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
}
