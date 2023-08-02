
#pragma once
#include "HMUI/ViewController.hpp"
#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(FingerSaberSettings, ModSettingsViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, modEnabledToggle);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, handsOnlyToggle);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool screenSystemDisabling);
)