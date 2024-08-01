#include "FingerSaber.hpp"
#include "main.hpp"
#include "logging.hpp"
#include "ModConfig.hpp"

#include "GlobalNamespace/MultiplayerSpectatorController.hpp"
#include "GlobalNamespace/IMultiplayerSpectatingSpot.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"

MAKE_HOOK_MATCH(
    MultiplayerSpectatorController_SwitchToSpectatingSpot,
    &GlobalNamespace::MultiplayerSpectatorController::SwitchToSpectatingSpot,
    void,
    GlobalNamespace::MultiplayerSpectatorController* self,

    GlobalNamespace::IMultiplayerSpectatingSpot* spectatingSpot
) {
    MultiplayerSpectatorController_SwitchToSpectatingSpot(self, spectatingSpot);

    // Early exit when mod not enabled
    if (getModConfig().ModEnabled.GetValue() == false)
        return;

    // Moving hands to the target spectation spot
    auto tf = spectatingSpot->get_transform();
    modManager.handTrackingObjectsParent->get_transform()->set_position(tf->get_position());
    modManager.handTrackingObjectsParent->get_transform()->set_rotation(tf->get_rotation());
}

void FingerSaber::_Hook_MultiplayerSpectatorController_SwitchToSpectatingSpot(){
    INSTALL_HOOK(Logger, MultiplayerSpectatorController_SwitchToSpectatingSpot);
}
