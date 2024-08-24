#include "ModGameplaySetupMenu.hpp"

#include "ModConfig.hpp"
#include "logging.hpp"
#include "bsml/shared/BSML-Lite/Creation/Layout.hpp"
#include "bsml/shared/BSML-Lite/Creation/Buttons.hpp"
#include "bsml/shared/BSML-Lite/Creation/Settings.hpp"
#include "bsml/shared/BSML-Lite/Creation/Text.hpp"

#include "FingerSaber.hpp"

DEFINE_TYPE(FingerSaberSettings, ModGameplaySetupMenu);

void FingerSaberSettings::ModGameplaySetupMenu::DidActivate(bool firstActivation){
    if (firstActivation){
        UnityEngine::GameObject* container = BSML::Lite::CreateScrollableSettingsContainer(get_transform());

        modEnabledToggle = BSML::Lite::CreateToggle(container->get_transform(), "Mod Enabled", getModConfig().ModEnabled.GetValue(), [](bool value) {
            getModConfig().ModEnabled.SetValue(value, true);
            modManager.update_scoreSubmission();
        });
        BSML::Lite::AddHoverHint(modEnabledToggle->get_gameObject(),  "Disables the mod.");

        handsOnlyToggle = BSML::Lite::CreateToggle(container->get_transform(), "Hand Mode", getModConfig().HandMode.GetValue(), [](bool value) {
            getModConfig().HandMode.SetValue(value, true);
        });
        BSML::Lite::AddHoverHint(handsOnlyToggle->get_gameObject(),  "Use Hands instead of fingers.");

        handRenderingToggle = BSML::Lite::CreateToggle(container->get_transform(), "Hand-Lines Rendering", getModConfig().handRendering.GetValue(), [](bool value) {
            getModConfig().handRendering.SetValue(value, true);
            // If we went from ON->OFF then must manually disable lines
            if(value == false)
            {
                modManager.disable_skeletonRender_lines(modManager.rightOVRSkeletonRenderer);
                modManager.disable_skeletonRender_lines(modManager.leftOVRSkeletonRenderer);
            }
        });
        BSML::Lite::AddHoverHint(handRenderingToggle->get_gameObject(), "Toggle the hand-lines rendering");
    }
    else{
        modEnabledToggle->set_Value(getModConfig().ModEnabled.GetValue());
        handsOnlyToggle->set_Value(getModConfig().HandMode.GetValue());
    }

}