#include "ModGameplaySetupMenu.hpp"

#include "ModConfig.hpp"
#include "logging.hpp"
#include "bsml/shared/BSML-Lite/Creation/Layout.hpp"
#include "bsml/shared/BSML-Lite/Creation/Buttons.hpp"
#include "bsml/shared/BSML-Lite/Creation/Settings.hpp"
#include "bsml/shared/BSML-Lite/Creation/Text.hpp"

DEFINE_TYPE(FingerSaberSettings, ModGameplaySetupMenu);

void FingerSaberSettings::ModGameplaySetupMenu::DidActivate(bool firstActivation){
    if (firstActivation){
        auto vertical = BSML::Lite::CreateVerticalLayoutGroup(this->get_transform());
        vertical->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
        vertical->get_rectTransform()->set_anchoredPosition({0, 20});
        auto horizontal = BSML::Lite::CreateHorizontalLayoutGroup(vertical->get_transform());
        horizontal->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);

        const float width = 75.0f;
        horizontal->template GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(width);

        modEnabledToggle = BSML::Lite::CreateToggle(horizontal->get_transform(), "Mod Enabled", getModConfig().ModEnabled.GetValue(), [](bool value) {
            getModConfig().ModEnabled.SetValue(value, true);
        });
        BSML::Lite::AddHoverHint(modEnabledToggle->get_gameObject(),  "Disables the mod.");


        handsOnlyToggle = BSML::Lite::CreateToggle(horizontal->get_transform(), "Hand Mode", getModConfig().HandMode.GetValue(), [](bool value) {
            getModConfig().HandMode.SetValue(value, true);
        });
        BSML::Lite::AddHoverHint(handsOnlyToggle->get_gameObject(),  "Use Hands instead of fingers.");


    }
    else{
        modEnabledToggle->set_Value(getModConfig().ModEnabled.GetValue());
        handsOnlyToggle->set_Value(getModConfig().HandMode.GetValue());
    }

}