#include "FingerSaber.hpp"

#include "main.hpp"
#include "logging.hpp"
#include "ModSettingsViewController.hpp"
#include "ModConfig.hpp"

#include "HMUI/Touchable.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "bsml/shared/BSML-Lite/Creation/Buttons.hpp"
#include "bsml/shared/BSML-Lite/Creation/Text.hpp"
#include "bsml/shared/BSML-Lite/Creation/Settings.hpp"
#include "bsml/shared/BSML-Lite/Creation/Layout.hpp"

DEFINE_TYPE(FingerSaberSettings, ModSettingsViewController);

std::vector<std::string> Left_Hand_Targets = {
    "Left Thumb",
    "Left Index",
    "Left Middle",
    "Left Ring",
    "Left Pinky",

    "Right Thumb",
    "Right Index",
    "Right Middle",
    "Right Ring",
    "Right Pinky"
};

std::vector<std::string> Right_Hand_Targets = {
    "Right Thumb",
    "Right Index",
    "Right Middle",
    "Right Ring",
    "Right Pinky",

    "Left Thumb",
    "Left Index",
    "Left Middle",
    "Left Ring",
    "Left Pinky"
};

std::span<std::string_view> vector_to_span(const std::vector<std::string>& vec) {
    static std::vector<std::string_view> views;
    views.clear();
    views.reserve(vec.size());

    for (const auto& str : vec) {
        views.emplace_back(str);
    }

    return std::span<std::string_view>{views};
}

void FingerSaberSettings::ModSettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if (firstActivation)
    {
        INFO("First Activation of settings");

        get_gameObject()->AddComponent<HMUI::Touchable*>();
        UnityEngine::GameObject* container = BSML::Lite::CreateScrollableSettingsContainer(get_transform());

        // ---------
        // Mod enabled Toggle
       this->modEnabledToggle = BSML::Lite::CreateToggle(container->get_transform(), "Mod Enabled", getModConfig().ModEnabled.GetValue(), [](bool value) {
           getModConfig().ModEnabled.SetValue(value, true);
           modManager.update_scoreSubmission();
       });
       BSML::Lite::AddHoverHint(this->modEnabledToggle->get_gameObject(),  "Disables the mod. (Disable from BMBF to completely disable the mod)");

        // OtherHandClicks Toggle
        auto OtherHandClicksToggle = BSML::Lite::CreateToggle(container->get_transform(), "Pinch-Click With Other Hand", getModConfig().OtherHandClicks.GetValue(), [](bool value) {
            getModConfig().OtherHandClicks.SetValue(value, true);
        });
        BSML::Lite::AddHoverHint(OtherHandClicksToggle->get_gameObject(), "Point with one hand and pinch-click with other hand. (To ease clicking in menus.)");

        // Auto pause Toggle
        auto AutoPauseToggle = BSML::Lite::CreateToggle(container->get_transform(), "Auto Pause", getModConfig().AutoPause.GetValue(), [](bool value) {
            getModConfig().AutoPause.SetValue(value, true);
        });
        BSML::Lite::AddHoverHint(AutoPauseToggle->get_gameObject(), "Pause automatically when hands are lost.");

        // Hands only mode Toggle
        auto handModeToggle = BSML::Lite::CreateToggle(container->get_transform(), "Hands-Only mode", getModConfig().HandMode.GetValue(), [](bool value) {
            getModConfig().HandMode.SetValue(value, true);
        });
        BSML::Lite::AddHoverHint(handModeToggle->get_gameObject(),  "Normal BeatSaber with hand-tracking. Sabers follow \"wrists\" instead of fingers. And player is not Supersized");

        auto smootMenuToggle = BSML::Lite::CreateToggle(container->get_transform(), "Smoothed Hands In Menus", getModConfig().SmoothedMenuHands.GetValue(), [](bool value) {
            getModConfig().SmoothedMenuHands.SetValue(value, true);
        });
        BSML::Lite::AddHoverHint(smootMenuToggle->get_gameObject(), "Only smooths hands menu, including in-game pause menu, but NOT in actual gameplay.");

        // Spacing + Simple text
        BSML::Lite::CreateText(container->get_transform(), "");
        //BSML::Lite::CreateText(container->get_transform(), "== Saber Target Finger Tips ==");


    //#define BeatSaberUI::CreateDropdown(parent, enumConfigValue.GetName(), enumReverseMap.at(clamp(0, (int) enumMap.size() - 1, (int) enumConfigValue.GetValue())), getKeys(enumMap), [](const std::string& value) {enumConfigValue.SetValue((int) enumMap.at(value));} )

        // RHand Finger Target Bone - Dropdown
       std::span<std::string_view> RTarg_dropdown_strings_span = vector_to_span(Right_Hand_Targets);
       auto* RTarg_dropdown = BSML::Lite::CreateDropdown(container->get_transform(), "Right Saber Target", RTarg_dropdown_strings_span[getModConfig().RightHandTargetIdx.GetValue()], RTarg_dropdown_strings_span, [&](auto value) {
           int i = 0;
           for (i = 0; i < Right_Hand_Targets.size(); i++){
               if (Right_Hand_Targets[i] == value) break;
           }
           getModConfig().RightHandTargetIdx.SetValue(i, true);

           modManager.update_LRTargetBone();
       });
       BSML::Lite::AddHoverHint(RTarg_dropdown->get_gameObject(), "Right saber will follow this finger tip");

        // LHand Finger Target Bone - Dropdown
        std::span<std::string_view> LTarg_dropdown_strings_span = vector_to_span(Left_Hand_Targets);
        auto* LTarg_dropdown = BSML::Lite::CreateDropdown(container->get_transform(), "Left  Saber Target", LTarg_dropdown_strings_span[getModConfig().LeftHandTargetIdx.GetValue()], LTarg_dropdown_strings_span, [&](auto value) {
            int i = 0;
            for (i = 0; i < Left_Hand_Targets.size(); i++){
                if (Left_Hand_Targets[i] == value) break;
            }
            getModConfig().LeftHandTargetIdx.SetValue(i, true);

            modManager.update_LRTargetBone();
        });
        BSML::Lite::AddHoverHint(LTarg_dropdown->get_gameObject(), "Left saber will follow this finger tip");

        // Platform Height offset
        //float platformHeightOffsetMeters   = -0.34;
        BSML::SliderSetting* heightOffsetIncrement = BSML::Lite::CreateSliderSetting(container->get_transform(), "Platform Height Offset (m)", 0.01f, getModConfig().PlatformHeightOffsetMeters.GetValue(), -0.5f, 0.0f, [](float value) {
             getModConfig().PlatformHeightOffsetMeters.SetValue(value, true);
        });
        BSML::Lite::AddHoverHint(heightOffsetIncrement->get_gameObject(), "Platform height offset from head level.");

        // Platform Height offset
        //float platformDistanceOffsetMeters = -0.25;
        BSML::SliderSetting* distanceOffsetIncrement = BSML::Lite::CreateSliderSetting(container->get_transform(), "Platform Distance Offset (m)", 0.01f, getModConfig().PlatformDistanceOffsetMeters.GetValue(), -0.5f, 0.0f, [](float value) {
             getModConfig().PlatformDistanceOffsetMeters.SetValue(value, true);
        });
        BSML::Lite::AddHoverHint(distanceOffsetIncrement->get_gameObject(), "Platform Z distance offset.");

        // Spacing
        //BSML::Lite::CreateText(container->get_transform(), "");

    }
    else{
        this->modEnabledToggle->set_Value(getModConfig().ModEnabled.GetValue());
    }
}

void FingerSaberSettings::ModSettingsViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling) {

}
