#pragma once

#include <string>
#include <sstream>

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/HideFlags.hpp"

#include "UnityEngine/Component.hpp"

template <typename T>
void Log_GetComponents(UnityEngine::GameObject* gameobj, Logger& logger);
template <typename T>
void Log_GetComponentsInChildren(UnityEngine::GameObject* gameobj, Logger& logger);

void Write_To_Log(std::stringstream& logStream, std::string logName, Logger& logger);
void Write_To_Log_AllGameObjectsInScene(std::string logName, Logger& logger);