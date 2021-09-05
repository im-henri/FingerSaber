#include "main.hpp"
#include "UnNamedMod.hpp"

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



#include "beatsaber-hook/shared/utils/logging.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/ColorManager.hpp"
#include "GlobalNamespace/ColormanagerInstaller.hpp"
#include "GlobalNamespace/ColorSchemeSO.hpp"
#include "GlobalNamespace/PlayerData.hpp"

#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/colorSchemesSettings.hpp"
#include "codegen/include/System/Collections/Generic/Dictionary_2.hpp"


#include <sstream>
#include <string>

MAKE_HOOK_MATCH(
    SceneManager_SetActiveScene, 
    &UnityEngine::SceneManagement::SceneManager::SetActiveScene, 
    bool,
    UnityEngine::SceneManagement::Scene scene
) {

    getLogger().info("SetActiveScene Hook()");

    auto sceneName_il2str = scene.get_name();
    auto sceneName = to_utf8(csstrtostr( sceneName_il2str ));

    bool is_MainMenu        = sceneName.compare("MainMenu") == 0;
    //bool is_ShaderWarmup    = sceneName.compare("ShaderWarmup") == 0;
    bool is_HealthWarning   = sceneName.compare("HealthWarning") == 0;
    bool is_GameCore        = sceneName.compare("GameCore") == 0;
    //bool is_EmptyTransition = sceneName.compare("EmptyTransition") == 0;

    // Actual function call
    bool ret = SceneManager_SetActiveScene(scene);
    // Actual function call

    getLogger().info("New scene name: %s", sceneName.c_str());
    if( (is_GameCore == true) || (is_MainMenu == true) || (is_HealthWarning == true) ){
        if(modManager.oculusHandsExist == false){
            modManager._InitializeOculusHands();
            modManager.oculusHandsExist = true; 
        }
    } 
    
    modManager.is_scene_GameCore = is_GameCore;

    //WriteToLog_AllGameObjectsInScene();

    return ret;
}

void UnNamedMod::_Hook_SceneManager_SetActiveScene(){
    
    INSTALL_HOOK(getLogger(), SceneManager_SetActiveScene);
}