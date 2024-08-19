#include "FingerSaber.hpp"
#include "main.hpp"

#include "ModConfig.hpp"
#include "logging.hpp"
#include "GlobalNamespace/MultiplayerLocalActivePlayerGameplayManager.hpp"

#include "GlobalNamespace/IMultiplayerSpectatingSpot.hpp"
#include "GlobalNamespace/MultiplayerSpectatingSpotManager.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"

MAKE_HOOK_MATCH(
    MultiplayerLocalActivePlayerGameplayManager_PerformPlayerFail,
    &GlobalNamespace::MultiplayerLocalActivePlayerGameplayManager::PerformPlayerFail,
    void,
    GlobalNamespace::MultiplayerLocalActivePlayerGameplayManager* self
) {
    MultiplayerLocalActivePlayerGameplayManager_PerformPlayerFail(self);

    // Early exit when mod not enabled
    if (getModConfig().ModEnabled.GetValue() == false)
        return;

    // Must re-create hands when failed in MP
    modManager._InitializeOculusHands();

    modManager.multiplayerGameFailed = true;
}

void FingerSaber::_Hook_MultiplayerLocalActivePlayerGameplayManager_PerformPlayerFail(){
    INSTALL_HOOK(Logger, MultiplayerLocalActivePlayerGameplayManager_PerformPlayerFail);
}