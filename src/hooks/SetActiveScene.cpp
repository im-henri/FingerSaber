#include "main.hpp"
#include "logging.hpp"
#include "FingerSaber.hpp"

#include "UnityEngine/SceneManagement/SceneManager.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Component.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/HideFlags.hpp"
#include "UnityEngine/Resources.hpp"

#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/ColorManager.hpp"
#include "GlobalNamespace/ColorManagerInstaller.hpp"
#include "GlobalNamespace/ColorSchemeSO.hpp"
#include "GlobalNamespace/PlayerData.hpp"

#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/ColorSchemesSettings.hpp"
#include "System/Collections/Generic/Dictionary_2.hpp"
#include <string>

MAKE_HOOK_MATCH(
    SceneManager_SetActiveScene,
    &UnityEngine::SceneManagement::SceneManager::SetActiveScene,
    bool,
    UnityEngine::SceneManagement::Scene scene
) {

    INFO("SetActiveScene Hook()");

    auto sceneName_il2str = scene.get_name();
    auto sceneName = sceneName_il2str;

    std::string sceneName_str = std::string(sceneName);

    bool is_MainMenu        = sceneName_str == "MainMenu";
    bool is_ShaderWarmup    = sceneName_str == "ShaderWarmup";
    bool is_HealthWarning   = sceneName_str == "HealthWarning";
    bool is_GameCore        = sceneName_str == "GameCore";
    bool is_EmptyTransition = sceneName_str == "EmptyTransition";

    // Actual function call
    bool ret = SceneManager_SetActiveScene(scene);
    // Actual function call

    // Re-set things at scene change
    modManager.pauseController = nullptr;
    modManager.is_GamePaused = false;
    modManager.local_player_saber_l = nullptr;
    modManager.local_player_saber_r = nullptr;
    modManager.multiplayerGameFailed = false;
    modManager.mp_spectatorController = nullptr;
    modManager.vrcontroller_r = nullptr;
    modManager.vrcontroller_l = nullptr;
    if(ret == true){
        INFO("New scene name: {}", sceneName_str);

        // Initialize things on every scene change
        modManager._InitializeOculusHands();

        modManager.is_scene_GameCore = is_GameCore;
    }

    return ret;
}

void FingerSaber::_Hook_SceneManager_SetActiveScene(){

    INSTALL_HOOK(Logger, SceneManager_SetActiveScene);
}