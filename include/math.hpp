#pragma once

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"

static UnityEngine::Quaternion operator*(UnityEngine::Quaternion a, UnityEngine::Quaternion b) {
    return UnityEngine::Quaternion::op_Multiply(a, b);
}

static bool operator==(UnityEngine::Quaternion a, UnityEngine::Quaternion b) {
    return UnityEngine::Quaternion::op_Equality(a, b);
}

static UnityEngine::Vector3 operator-(UnityEngine::Vector3 a, UnityEngine::Vector3 b) {
    return UnityEngine::Vector3::op_Subtraction(a,b);
}

static UnityEngine::Vector3 operator+(UnityEngine::Vector3 a, UnityEngine::Vector3 b) {
    return UnityEngine::Vector3::op_Addition(a,b);
}

static UnityEngine::Vector3 operator*(UnityEngine::Vector3 a, float_t b) {
    return UnityEngine::Vector3::op_Multiply(a,b);
}

static UnityEngine::Vector3 operator*(float_t a, UnityEngine::Vector3 b) {
    return UnityEngine::Vector3::op_Multiply(a,b);
}

static UnityEngine::Vector3 operator*(double a, UnityEngine::Vector3 b) {
    return UnityEngine::Vector3::op_Multiply(a,b);
}