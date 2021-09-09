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

class UnNamedMod{
    public:
        
        void InstallHooks();
        
        // --- Public Methods ---

        //void FixedUpdate(GlobalNamespace::OculusVRHelper* self); 
        void _InitializeOculusHands();
        //void _InitializeOculusHandInGame(GlobalNamespace::Saber* saber);
        void ChangeRightSkeletonRendererColor(UnityEngine::Color col);
        void ChangeLeftSkeletonRendererColor(UnityEngine::Color col);
        
        bool createNewSkeletonMaterials(); 

        bool getRHandClickRequested() { return _rHandClickRequested; }
        bool getLHandClickRequested() { return _lHandClickRequested; }

        bool getEitherHandIsTracked() { return _oculusLHandIsTracked || _oculusRHandIsTracked; }

        void update_LRHandIsTracked();
        void update_LRHandClickRequested();


        // Public variables
        bool oculusHandsExist = false;
        bool is_scene_GameCore = false;
        bool is_GamePaused = false;
        
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

extern UnNamedMod modManager;