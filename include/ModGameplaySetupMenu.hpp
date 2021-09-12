  
#pragma once
#include "UnityEngine/MonoBehaviour.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(FingerSaberSettings, ModGameplaySetupMenu, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, modEnabledToggle);

    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
)