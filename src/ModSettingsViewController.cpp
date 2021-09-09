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

//extern config_t config;

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
        auto modEnabledToggle = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Mod enabled", getModConfig().ModEnabled.GetValue(), [](bool value) { 
            getModConfig().ModEnabled.SetValue(value, true);
        });
        QuestUI::BeatSaberUI::AddHoverHint(modEnabledToggle->get_gameObject(),  "Disables the mod. (Disable from BMBF to completely disable the mod)");

        // OtherHandClicks Toggle
        auto OtherHandClicksToggle = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Pinch-Click With Other Hand", getModConfig().OtherHandClicks.GetValue(), [](bool value) { 
            getModConfig().OtherHandClicks.SetValue(value, true);
        });
        QuestUI::BeatSaberUI::AddHoverHint(OtherHandClicksToggle->get_gameObject(), "Point with one hand and pinch-click with other hand. (To ease clicking in menus.)");

        // Spacing + Simple text
        QuestUI::BeatSaberUI::CreateText(container->get_transform(), "");
        QuestUI::BeatSaberUI::CreateText(container->get_transform(), "== Saber Target Finger Tips ==");

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

        // Spacing
        QuestUI::BeatSaberUI::CreateText(container->get_transform(), "");

        // Hands only mode Toggle
        auto handModeToggle = QuestUI::BeatSaberUI::CreateToggle(container->get_transform(), "Hands-Only mode", getModConfig().HandMode.GetValue(), [](bool value) { 
            getModConfig().HandMode.SetValue(value, true);
        });
        QuestUI::BeatSaberUI::AddHoverHint(handModeToggle->get_gameObject(),  "Normal BeatSaber with hand-tracking. Sabers follow \"wrists\" instead of fingers. And player is not Supersized");


        /*QuestUI::IncrementSetting* zmax = QuestUI::BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Title", 2, 0.2f, getModConfig().SomeKindOfFloat.GetValue(), 0.0, 1000, [](float value) {
                
        });
        QuestUI::BeatSaberUI::AddHoverHint(zmax->get_gameObject(), "Hover info");
        */
        
        
    }
}

void FingerSaberSettings::ModSettingsViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling) {

}
