#include "FingerSaber.hpp"

#include "GlobalNamespace/OVRInput.hpp"
#include "GlobalNamespace/OVRInput_Button.hpp"
#include "GlobalNamespace/OVRManager.hpp"


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

FingerSaber modManager;

void FingerSaber::InstallHooks() {
    _Hook_SceneManager_SetActiveScene();
    //_Hook_OculusVRHelper_VRControllersInputManager();
    _Hook_Saber_ManualUpdate();
    _Hook_SaberModelController_Init();
    _Hook_GamePause_Pause();
    _Hook_GamePause_WillResume();
    
    _Hook_SOME_HOOK_METHOD();
}

namespace ModUtils{
    void LogAllFields(Il2CppObject* il2CppObj, std::string prependText = ""){
        auto x = getLogger().WithContext(prependText);
        il2cpp_utils::LogFields(x, il2cpp_utils::ExtractClass(il2CppObj));
    }
    void LogAllProperties(Il2CppObject* il2CppObj, std::string prependText = ""){
        auto x = getLogger().WithContext(prependText);
        il2cpp_utils::LogProperties(x, il2cpp_utils::ExtractClass(il2CppObj));
    }
    void LogAllMethods(Il2CppObject* il2CppObj, std::string prependText = ""){
        auto x = getLogger().WithContext(prependText);
        il2cpp_utils::LogMethods(x, il2cpp_utils::ExtractClass(il2CppObj));
    }

    template <typename T>
    void Log_GetComponents(UnityEngine::GameObject* gameobj){
        auto componentArr = gameobj->GetComponents<T>();
        std::stringstream buff;
        for (int i=0; i<componentArr->Length(); i++){
            auto comp = (*componentArr)[i];
            auto name = comp->get_name();
            auto fullName = comp->GetType()->get_FullName();
            buff << to_utf8(csstrtostr(fullName)).c_str() << " " << to_utf8(csstrtostr(name)).c_str() << "\n";
        }
        getLogger().info("%s",buff.str().c_str());
    }
    template <typename T>
    void Log_GetComponentsInChildren(UnityEngine::GameObject* gameobj){
        auto componentArr = gameobj->GetComponentsInChildren<T>();
        std::stringstream buff;
        for (int i=0; i<componentArr->Length(); i++){
            auto comp = (*componentArr)[i];
            auto name = comp->get_name();
            auto fullName = comp->GetType()->get_FullName();
            buff  << to_utf8(csstrtostr(fullName)).c_str() << " " << to_utf8(csstrtostr(name)).c_str() << "\n";
        }
        getLogger().info("%s",buff.str().c_str());
    }

    void WriteToLog(std::stringstream& logStream){
        bool success = writefile("sdcard/Android/data/com.beatgames.beatsaber/files/logs/FingerSaber.log", logStream.str());
        if (success)   getLogger().info("Logging to \"FingerSaber.log\" successful.");
        else           getLogger().info("Creating log file failed.");
    }
    void WriteToLog_AllGameObjectsInScene(){
        auto allObject = UnityEngine::Resources::FindObjectsOfTypeAll(csTypeOf(UnityEngine::GameObject*));
        std::stringstream buff;
        for(int i=0; i<allObject->Length(); i++){
            auto go_asObj = (*allObject)[i];
            //getLogger().info("Found items: %i", i);
            if (
                !(go_asObj->get_hideFlags() == UnityEngine::HideFlags::NotEditable || go_asObj->get_hideFlags() == UnityEngine::HideFlags::HideAndDontSave)
                )
            {
                //getLogger().info("Go name: %s", to_utf8(csstrtostr(name)).c_str() );
                // TRYING CASTING 
                UnityEngine::GameObject* go = il2cpp_utils::cast<UnityEngine::GameObject, UnityEngine::Object>(go_asObj);
                
                Il2CppString* name = go->get_name(); // C# strings are pointers to UTF-16 strings
                Il2CppString* rootName = go->get_transform()->GetRoot()->get_name();

                UnityEngine::Transform* parentTf = go->get_transform();

                while(name->CompareTo(rootName) != 0){
                    std::stringstream namestream;
                    namestream << to_utf8(csstrtostr(name)).c_str() << "(" << parentTf->GetInstanceID() << ")";
                    buff << namestream.str() << " -> ";
                    
                    parentTf = parentTf->get_parent();
                    name = parentTf->get_gameObject()->get_name();
                }
                //Name: SaberA, RootName: MenuCore
                //buff << i << "\t" << "Name: " << to_utf8(csstrtostr(name)).c_str();
                buff << to_utf8(csstrtostr(rootName)).c_str() << "(" << go->get_transform()->GetRoot()->GetInstanceID() << ")";
                buff << '\n' << '\n';
            }
        }

        WriteToLog(buff);
    }
}

#include "GlobalNamespace/OVRPlugin.hpp"
#include "GlobalNamespace/OVRPlugin_HandFinger.hpp"
#include "GlobalNamespace/OVRHand.hpp"

#include "GlobalNamespace/FirstPersonFlyingController.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/OculusVRHelper.hpp"
#include "GlobalNamespace/VRPlatformSDK.hpp"
#include "GlobalNamespace/OVRCameraRig.hpp"

#include "GlobalNamespace/OVRSkeleton.hpp"
#include "GlobalNamespace/OVRSkeleton_SkeletonPoseData.hpp"
#include "GlobalNamespace/OVRBone.hpp"
#include "GlobalNamespace/OVRPlugin_Skeleton2.hpp"


#include "GlobalNamespace/OVRSkeletonRenderer.hpp"

#include "UnityEngine/Camera.hpp"

#include "System/Collections/Generic/List_1.hpp"
#include "System/Collections/Generic/IList_1.hpp"
#include "System/Collections/ObjectModel/ReadOnlyCollection_1.hpp"


#include "UnityEngine/LineRenderer.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/LineRenderer.hpp"

#include "UnityEngine/Resources.hpp"

void FingerSaber::_InitializeOculusHands(){
    getLogger().info("Oculus Hand MENU Initialization ..");

    auto old_HandTracking_container = UnityEngine::GameObject::Find(il2cpp_utils::createcsstr("HandTracking_container"));
    if(old_HandTracking_container) UnityEngine::GameObject::Destroy(old_HandTracking_container);

    handTrackingObjectsParent = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("HandTracking_container"));
    UnityEngine::GameObject::DontDestroyOnLoad(handTrackingObjectsParent);

    // ---

    auto rightHandAnchor = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("rightHandAnchor"));
    rightHandAnchor->get_transform()->set_parent(handTrackingObjectsParent->get_transform());

    auto rightHandTrackingGo = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("rightHandTracking"));
    rightHandTrackingGo->get_transform()->set_parent(rightHandAnchor->get_transform());
    
    rightOVRHand = rightHandTrackingGo->AddComponent<GlobalNamespace::OVRHand*>();
    rightOVRHand->HandType = GlobalNamespace::OVRHand::Hand::HandRight;

    rightOVRSkeleton = rightHandTrackingGo->AddComponent<GlobalNamespace::OVRSkeleton*>();
    rightOVRSkeleton->skeletonType = GlobalNamespace::OVRSkeleton::SkeletonType::HandRight;
    rightOVRSkeleton->updateRootPose = true;
    rightOVRSkeleton->Initialize();
    
    auto rightOVRSkeletonRenderer = rightHandTrackingGo->AddComponent<GlobalNamespace::OVRSkeletonRenderer*>();
    rightHandSkeletonMat->SetColor(il2cpp_utils::createcsstr("_Color"), UnityEngine::Color{0.156863, 0.556863, 0.823529, 1.000000});
    rightOVRSkeletonRenderer->skeletonMaterial=rightHandSkeletonMat;
    rightOVRSkeletonRenderer->systemGestureMaterial=rightHandSkeletonMat;
    rightOVRSkeletonRenderer->Initialize();

    getLogger().info("rightHandTracking MENU Stuff Created");

    // ---
   
    auto leftHandAnchor = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("leftHandAnchor"));
    leftHandAnchor->get_transform()->set_parent(handTrackingObjectsParent->get_transform());
    
    auto leftHandTrackingGo = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("leftHandTracking"));
    leftHandTrackingGo->get_transform()->set_parent(leftHandAnchor->get_transform());
    
    leftOVRHand = leftHandTrackingGo->AddComponent<GlobalNamespace::OVRHand*>();
    leftOVRHand->HandType = GlobalNamespace::OVRHand::Hand::HandLeft;

    leftOVRSkeleton = leftHandTrackingGo->AddComponent<GlobalNamespace::OVRSkeleton*>();
    leftOVRSkeleton->skeletonType = GlobalNamespace::OVRSkeleton::SkeletonType::HandLeft;
    leftOVRSkeleton->updateRootPose = true;
    leftOVRSkeleton->Initialize();
    
    auto leftOVRSkeletonRenderer = leftHandTrackingGo->AddComponent<GlobalNamespace::OVRSkeletonRenderer*>();
    leftHandSkeletonMat->SetColor(il2cpp_utils::createcsstr("_Color"), UnityEngine::Color{0.784314, 0.078431, 0.078431, 1.000000});
    leftOVRSkeletonRenderer->skeletonMaterial=leftHandSkeletonMat;
    leftOVRSkeletonRenderer->systemGestureMaterial=leftHandSkeletonMat;

    getLogger().info("leftHandTracking MENU Stuff Created");
    
}

bool FingerSaber::createNewSkeletonMaterials(){
    bool success = false;
    auto Normal = UnityEngine::GameObject::Find(il2cpp_utils::createcsstr("ControllerLeft/MenuHandle/Normal"));
    if( Normal ){
        auto meshRenderer = Normal->GetComponentInChildren<UnityEngine::MeshRenderer*>();
        leftHandSkeletonMat  = UnityEngine::Material::New_ctor(meshRenderer->GetMaterial());
        rightHandSkeletonMat = UnityEngine::Material::New_ctor(meshRenderer->GetMaterial());
        success = true;
    }
    return success;
}

void FingerSaber::ChangeRightSkeletonRendererColor(UnityEngine::Color col){
    if(rightHandSkeletonMat) rightHandSkeletonMat->SetColor(il2cpp_utils::createcsstr("_Color"), col);
}
void FingerSaber::ChangeLeftSkeletonRendererColor(UnityEngine::Color col){
    if(leftHandSkeletonMat) leftHandSkeletonMat->SetColor(il2cpp_utils::createcsstr("_Color"), col);
}

#include "UnityEngine/EventSystems/PointerInputModule.hpp"
#include "VRUIControls/VRInputModule.hpp"
#include "GlobalNamespace/TimeHelper.hpp"

//static bool rButton_prev = false;

void FingerSaber::update_LRHandIsTracked(){
    _oculusRHandIsTracked = rightOVRHand->IsTracked; 
    _oculusLHandIsTracked = leftOVRHand->IsTracked;
}
void FingerSaber::update_LRHandClickRequested(){
    _rHandClickRequested = GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::One, GlobalNamespace::OVRInput::Controller::RHand) 
                ? (_oculusRHandIsTracked) : false;
    _lHandClickRequested = GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::One, GlobalNamespace::OVRInput::Controller::LHand) 
                ? (_oculusLHandIsTracked) : false;
}


//void FingerSaber::FixedUpdate(GlobalNamespace::OculusVRHelper* self){
//
//    /*bool rButton = GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::One, GlobalNamespace::OVRInput::Controller::RTouch) 
//                    ? true : false;
//    if((rButton == true) && (rButton_prev == false)){
//        getLogger().info("R_Button Pressed");
//        //ModUtils::WriteToLog_AllGameObjectsInScene();
//    }
//    rButton_prev = rButton;*/
//
//    /*if(rightOVRHand){
//        update_LRHandIsTracked();
//        update_LRHandClickRequested();
//    }*/
//}
