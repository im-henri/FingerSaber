#include "ModGameplaySetupMenu.hpp"

#include "ModConfig.hpp"


DEFINE_TYPE(FingerSaberSettings, ModGameplaySetupMenu);

void FingerSaberSettings::ModGameplaySetupMenu::DidActivate(bool firstActivation){
    if (firstActivation){
        auto vertical = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(this->get_transform());
        vertical->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
        vertical->get_rectTransform()->set_anchoredPosition({0, 20});
        auto horizontal = QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(vertical->get_transform());
        horizontal->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);

        const float width = 75.0f;
        horizontal->template GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(width);

        modEnabledToggle = QuestUI::BeatSaberUI::CreateToggle(horizontal->get_transform(), "Mod Enabled", getModConfig().ModEnabled.GetValue(), [](bool value) {
            getModConfig().ModEnabled.SetValue(value, true);
        });
        QuestUI::BeatSaberUI::AddHoverHint(modEnabledToggle->get_gameObject(),  "Disables the mod. (Disable from BMBF to completely disable the mod)");


        handsOnlyToggle = QuestUI::BeatSaberUI::CreateToggle(horizontal->get_transform(), "Hand Mode", getModConfig().HandMode.GetValue(), [](bool value) {
            getModConfig().HandMode.SetValue(value, true);
        });
        QuestUI::BeatSaberUI::AddHoverHint(handsOnlyToggle->get_gameObject(),  "Use Hands instead of fingers.");


    }
    else{
        modEnabledToggle->set_isOn(getModConfig().ModEnabled.GetValue());
        handsOnlyToggle->set_isOn(getModConfig().HandMode.GetValue());

    }

}