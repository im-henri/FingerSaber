#include "FingerSaber.hpp"

#include "main.hpp"

#include "ModSettingsViewController.hpp"
#include "ModConfig.hpp"

#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "HMUI/Touchable.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"


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

void FingerSaberSettings::ModSettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if (firstActivation)
    {
        getLogger().info("First Activation of settings");

        get_gameObject()->AddComponent<HMUI::Touchable*>();
        UnityEngine::GameObject* container = QuestUI::BeatSaberUI::CreateScrollableSettingsContainer(get_transform());

        // ---------
        // Mod enabled Toggle
        this->modEnabledToggle = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Mod Enabled", getModConfig().ModEnabled.GetValue(), [](bool value) {
            getModConfig().ModEnabled.SetValue(value, true);
        });
        QuestUI::BeatSaberUI::AddHoverHint(this->modEnabledToggle->get_gameObject(),  "Disables the mod. (Disable from BMBF to completely disable the mod)");

        // OtherHandClicks Toggle
        auto OtherHandClicksToggle = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Pinch-Click With Other Hand", getModConfig().OtherHandClicks.GetValue(), [](bool value) {
            getModConfig().OtherHandClicks.SetValue(value, true);
        });
        QuestUI::BeatSaberUI::AddHoverHint(OtherHandClicksToggle->get_gameObject(), "Point with one hand and pinch-click with other hand. (To ease clicking in menus.)");

        // Auto pause Toggle
        auto AutoPauseToggle = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Auto Pause", getModConfig().AutoPause.GetValue(), [](bool value) {
            getModConfig().AutoPause.SetValue(value, true);
        });
        QuestUI::BeatSaberUI::AddHoverHint(AutoPauseToggle->get_gameObject(), "Pause automatically when hands are lost.");

        // Hands only mode Toggle
        auto handModeToggle = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Hands-Only mode", getModConfig().HandMode.GetValue(), [](bool value) {
            getModConfig().HandMode.SetValue(value, true);
        });
        QuestUI::BeatSaberUI::AddHoverHint(handModeToggle->get_gameObject(),  "Normal BeatSaber with hand-tracking. Sabers follow \"wrists\" instead of fingers. And player is not Supersized");

        auto smootMenuToggle = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Smoothed Hands In Menus", getModConfig().SmoothedMenuHands.GetValue(), [](bool value) {
            getModConfig().SmoothedMenuHands.SetValue(value, true);
        });
        QuestUI::BeatSaberUI::AddHoverHint(smootMenuToggle->get_gameObject(), "Only smooths hands menu, including in-game pause menu, but NOT in actual gameplay.");

        // Spacing + Simple text
        QuestUI::BeatSaberUI::CreateText(container->get_transform(), "");
        //QuestUI::BeatSaberUI::CreateText(container->get_transform(), "== Saber Target Finger Tips ==");


    //#define BeatSaberUI::CreateDropdown(parent, enumConfigValue.GetName(), enumReverseMap.at(clamp(0, (int) enumMap.size() - 1, (int) enumConfigValue.GetValue())), getKeys(enumMap), [](const std::string& value) {enumConfigValue.SetValue((int) enumMap.at(value));} )

        // RHand Finger Target Bone - Dropdown
        auto RTarg_dropdown_stringw = std::vector<StringW>();
        for (auto& target : Right_Hand_Targets){
            RTarg_dropdown_stringw.push_back(StringW(target));
        }
        HMUI::SimpleTextDropdown* RTarg_dropdown = QuestUI::BeatSaberUI::CreateDropdown(container->get_transform(), "Right Saber Target", StringW(Right_Hand_Targets[getModConfig().RightHandTargetIdx.GetValue()]), RTarg_dropdown_stringw, [&](auto value) {
            int i = 0;
            for (i = 0; i < Right_Hand_Targets.size(); i++){
                if (Right_Hand_Targets[i] == value) break;
            }
            getModConfig().RightHandTargetIdx.SetValue(i, true);

            modManager.update_LRTargetBone();
        });
        QuestUI::BeatSaberUI::AddHoverHint(RTarg_dropdown->get_gameObject(), "Right saber will follow this finger tip");

        // LHand Finger Target Bone - Dropdown
        auto LTarg_dropdown_stringw = std::vector<StringW>();
        for (auto& target : Left_Hand_Targets){
            LTarg_dropdown_stringw.push_back(StringW(target));
        }
        HMUI::SimpleTextDropdown* LTarg_dropdown = QuestUI::BeatSaberUI::CreateDropdown(container->get_transform(), "Left  Saber Target", StringW(Left_Hand_Targets[getModConfig().LeftHandTargetIdx.GetValue()]), LTarg_dropdown_stringw, [&](auto value) {
               int i = 0;
               for (i = 0; i < Left_Hand_Targets.size(); i++){
                   if (Left_Hand_Targets[i] == value) break;
               }
               getModConfig().LeftHandTargetIdx.SetValue(i, true);

               modManager.update_LRTargetBone();
        });
        QuestUI::BeatSaberUI::AddHoverHint(LTarg_dropdown->get_gameObject(), "Left saber will follow this finger tip");

        // Platform Height offset
        //float platformHeightOffsetMeters   = -0.34;
        QuestUI::SliderSetting* heightOffsetIncrement = QuestUI::BeatSaberUI::CreateSliderSetting(container->get_transform(), "Platform Height Offset (m)", 0.01f, getModConfig().PlatformHeightOffsetMeters.GetValue(), -0.5f, 0.0f, [](float value) {
             getModConfig().PlatformHeightOffsetMeters.SetValue(value, true);
        });
        QuestUI::BeatSaberUI::AddHoverHint(heightOffsetIncrement->get_gameObject(), "Platform height offset from head level.");

        // Platform Height offset
        //float platformDistanceOffsetMeters = -0.25;
        QuestUI::SliderSetting* distanceOffsetIncrement = QuestUI::BeatSaberUI::CreateSliderSetting(container->get_transform(), "Platform Distance Offset (m)", 0.01f, getModConfig().PlatformDistanceOffsetMeters.GetValue(), -0.5f, 0.0f, [](float value) {
             getModConfig().PlatformDistanceOffsetMeters.SetValue(value, true);
        });
        QuestUI::BeatSaberUI::AddHoverHint(distanceOffsetIncrement->get_gameObject(), "Platform Z distance offset.");

        // Spacing
        //QuestUI::BeatSaberUI::CreateText(container->get_transform(), "");

    }
    else{
        this->modEnabledToggle->set_isOn(getModConfig().ModEnabled.GetValue());
    }
}

void FingerSaberSettings::ModSettingsViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling) {

}
