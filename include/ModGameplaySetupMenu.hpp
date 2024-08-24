
#pragma once
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "HMUI/ViewController.hpp"
#include "custom-types/shared/macros.hpp"
#include "bsml/shared/BSML/Components/Settings/ToggleSetting.hpp"

DECLARE_CLASS_CODEGEN(FingerSaberSettings, ModGameplaySetupMenu, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(UnityW<BSML::ToggleSetting>, modEnabledToggle);
    DECLARE_INSTANCE_FIELD(UnityW<BSML::ToggleSetting>, handsOnlyToggle);
    DECLARE_INSTANCE_FIELD(UnityW<BSML::ToggleSetting>, handRenderingToggle);

    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
)