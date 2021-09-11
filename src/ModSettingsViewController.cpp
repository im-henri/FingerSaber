#include "FingerSaber.hpp"

#include "main.hpp"

#include "ModSettingsViewController.hpp"
#include "Config.hpp"

#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "HMUI/Touchable.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"

#include "Config.hpp"


DEFINE_TYPE(FingerSaberSettings, ModSettingsViewController);

std::vector<std::string> Left_Hand_Targets = {
    "Left_Thumb",
    "Left_Index",
    "Left_Middle",
    "Left_Ring",
    "Left_Pinky",

    "Right_Thumb",
    "Right_Index",
    "Right_Middle",
    "Right_Ring",
    "Right_Pinky"
};

std::vector<std::string> Right_Hand_Targets = {
    "Right_Thumb",
    "Right_Index",
    "Right_Middle",
    "Right_Ring",
    "Right_Pinky",

    "Left_Thumb",
    "Left_Index",
    "Left_Middle",
    "Left_Ring",
    "Left_Pinky"
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
        auto modEnabledToggle = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Mod Enabled", getModConfig().ModEnabled.GetValue(), [](bool value) { 
            getModConfig().ModEnabled.SetValue(value, true);
        });
        QuestUI::BeatSaberUI::AddHoverHint(modEnabledToggle->get_gameObject(),  "Disables the mod. (Disable from BMBF to completely disable the mod)");

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
        
        // Spacing + Simple text
        QuestUI::BeatSaberUI::CreateText(container->get_transform(), "");
        //QuestUI::BeatSaberUI::CreateText(container->get_transform(), "== Saber Target Finger Tips ==");

        // RHand Finger Target Bone - Dropdown
        HMUI::SimpleTextDropdown* RTarg_dropdown = QuestUI::BeatSaberUI::CreateDropdown(container->get_transform(), u"Right Saber Target", Right_Hand_Targets[getModConfig().RightHandTargetIdx.GetValue()], Right_Hand_Targets, [&](auto value) {
            int i = 0;
            for (i = 0; i < Right_Hand_Targets.size(); i++){
                if (Right_Hand_Targets[i] == value) break;
            }
            getModConfig().RightHandTargetIdx.SetValue(i, true);

            modManager.update_LRTargetBone();
        });
        QuestUI::BeatSaberUI::AddHoverHint(RTarg_dropdown->get_gameObject(), "Right saber will follow this finger tip");

        // LHand Finger Target Bone - Dropdown
        HMUI::SimpleTextDropdown* LTarg_dropdown = QuestUI::BeatSaberUI::CreateDropdown(container->get_transform(), u"Left  Saber Target", Left_Hand_Targets[getModConfig().LeftHandTargetIdx.GetValue()], Left_Hand_Targets, [&](auto value) {
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
}

void FingerSaberSettings::ModSettingsViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling) {

}
