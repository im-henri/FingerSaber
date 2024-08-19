#pragma once

#include <string>
#include <sstream>

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/HideFlags.hpp"

#include "UnityEngine/Component.hpp"

//template <typename T>
//void Log_GetComponents(UnityEngine::GameObject* gameobj);
//template <typename T>
//void Log_GetComponentsInChildren(UnityEngine::GameObject* gameobj);
//
void Write_To_Log(std::stringstream& logStream, std::string logName);
void Log_AllGameObjectsInScene();