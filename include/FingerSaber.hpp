#pragma once

#include "main.hpp"

#include "GlobalNamespace/OculusVRHelper.hpp"

#include "GlobalNamespace/OVRSkeleton.hpp"
#include "GlobalNamespace/OVRSkeleton_SkeletonPoseData.hpp"
#include "GlobalNamespace/OVRBone.hpp"
#include "GlobalNamespace/OVRPlugin_Skeleton2.hpp"
#include "GlobalNamespace/OVRHand.hpp"
#include "GlobalNamespace/Saber.hpp"

#include "GlobalNamespace/PauseController.hpp"
#include "UnityEngine/Color.hpp"

class FingerSaber{
    public:
        
        void InstallHooks();
        
        // --- Public Methods ---

        // Creates new oculus hand things
        void _InitializeOculusHands();
        // Tries to destory oculus hand things if they are found. Returns true if destroy call was initiated
        bool _Destroy_OculusHands();

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
        bool oculusHandsExist = false;
        bool is_scene_GameCore = false;
        bool is_GamePaused = false;

        GlobalNamespace::OVRSkeleton::BoneId rightTargetBone = 0;
        GlobalNamespace::OVRSkeleton::BoneId  leftTargetBone = 0;
        bool leftHand_isTargetHandRight = false;
        bool rightHand_isTargetHandLeft = false;

        GlobalNamespace::PauseController* pauseController = nullptr;

        UnityEngine::GameObject* handTrackingObjectsParent  = nullptr; 

        GlobalNamespace::OVRHand*     rightOVRHand = nullptr;
        GlobalNamespace::OVRHand*     leftOVRHand  = nullptr;
        GlobalNamespace::OVRSkeleton* rightOVRSkeleton = nullptr;
        GlobalNamespace::OVRSkeleton* leftOVRSkeleton  = nullptr;

        UnityEngine::Material * rightHandSkeletonMat = nullptr; // Createdd win new_ctor
        UnityEngine::Material * leftHandSkeletonMat  = nullptr; // Createdd win new_ctor
        
        UnityEngine::Transform* r_saber_TF = nullptr;
        UnityEngine::Transform* l_saber_TF = nullptr;

    private:
        bool _rHandClickRequested = false;
        bool _lHandClickRequested = false;

        bool _oculusRHandIsTracked = false;
        bool _oculusLHandIsTracked = false;

        // Hook Install Calls
        void _Hook_SceneManager_SetActiveScene();
        //void _Hook_OculusVRHelper_VRControllersInputManager();
        void _Hook_Saber_ManualUpdate();
        void _Hook_SaberModelController_Init();
        void _Hook_GamePause_Pause();
        void _Hook_GamePause_WillResume();

        void _Hook_SOME_HOOK_METHOD();

       
        
};

extern FingerSaber modManager;