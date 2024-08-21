#pragma once

#include "main.hpp"

#include "GlobalNamespace/OculusVRHelper.hpp"

#include "GlobalNamespace/OVRSkeleton.hpp"
#include "GlobalNamespace/OVRBone.hpp"
#include "GlobalNamespace/OVRPlugin.hpp"
#include "GlobalNamespace/OVRHand.hpp"
#include "GlobalNamespace/Saber.hpp"

#include "GlobalNamespace/PauseController.hpp"
#include "UnityEngine/Color.hpp"

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"

#include "GlobalNamespace/SaberManager.hpp"
#include "GlobalNamespace/MultiplayerSpectatorController.hpp"
#include "GlobalNamespace/VRController.hpp"

class FingerSaber
{
public:
    void InstallHooks();

    // --- Public Methods ---

    // Creates new oculus hand things
    void _InitializeOculusHands();
    // Tries to destory oculus hand things if they are found. Returns true if destroy call was initiated
    void _Destroy_OculusHands();

    void ChangeRightSkeletonRendererColor(UnityEngine::Color col);
    void ChangeLeftSkeletonRendererColor(UnityEngine::Color col);

    void createNewSkeletonMaterials();

    bool getRHandClickRequested() { return _rHandClickRequested; }
    bool getLHandClickRequested() { return _lHandClickRequested; }

    bool getEitherHandIsTracked() { return _oculusLHandIsTracked || _oculusRHandIsTracked; }

    void update_LRHandIsTracked();
    void update_LRHandClickRequested();

    void update_LRTargetBone();

    // Public variables
    bool is_scene_GameCore = false;
    bool is_GamePaused = false;

    GlobalNamespace::OVRSkeleton::BoneId rightTargetBone = 0;
    GlobalNamespace::OVRSkeleton::BoneId leftTargetBone = 0;
    bool leftHand_isTargetHandRight = false;
    bool rightHand_isTargetHandLeft = false;

    GlobalNamespace::PauseController *pauseController = nullptr;

    UnityEngine::GameObject *handTrackingObjectsParent = nullptr;

    GlobalNamespace::OVRHand *rightOVRHand = nullptr;
    GlobalNamespace::OVRHand *leftOVRHand = nullptr;
    GlobalNamespace::OVRSkeleton *rightOVRSkeleton = nullptr;
    GlobalNamespace::OVRSkeleton *leftOVRSkeleton = nullptr;

    GlobalNamespace::OVRSkeletonRenderer *rightOVRSkeletonRenderer = nullptr;
    GlobalNamespace::OVRSkeletonRenderer *leftOVRSkeletonRenderer = nullptr;

    UnityEngine::Material *rightHandSkeletonMat = nullptr;
    UnityEngine::Material *leftHandSkeletonMat = nullptr;

    UnityEngine::Quaternion menu_l_quaternion = UnityEngine::Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    UnityEngine::Quaternion menu_r_quaternion = UnityEngine::Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    UnityEngine::Vector3 menu_l_vector3 = UnityEngine::Vector3(0.0f, 0.0f, 0.0f);
    UnityEngine::Vector3 menu_r_vector3 = UnityEngine::Vector3(0.0f, 0.0f, 0.0f);

    GlobalNamespace::VRController *vrcontroller_r = nullptr;
    GlobalNamespace::VRController *vrcontroller_l = nullptr;

    // In multiplayer, there are many sabers. These help detect
    // local player sabers.
    GlobalNamespace::Saber *local_player_saber_l = nullptr;
    GlobalNamespace::Saber *local_player_saber_r = nullptr;
    // Also needed for multiplayer. When player dies, must enable
    // the menu saber movement again.
    bool multiplayerGameFailed = false;
    GlobalNamespace::MultiplayerSpectatorController *mp_spectatorController = nullptr;

private:
    bool _rHandClickRequested = false;
    bool _lHandClickRequested = false;

    bool _oculusRHandIsTracked = false;
    bool _oculusLHandIsTracked = false;

    // Hook Install Calls
    void _Hook_SceneManager_SetActiveScene();
    void _Hook_MultiplayerSpectatorController_SwitchToSpectatingSpot();
    void _Hook_SaberModelController_Init();
    void _Hook_GamePause_Pause();
    void _Hook_GamePause_WillResume();
    void _Hook_MultiplayerLocalActivePlayerGameplayManager_PerformPlayerFail();
    void _Hook_BoneVisualization_Update();

    void _Hook_menu_saber_functionality();
};

extern FingerSaber modManager;