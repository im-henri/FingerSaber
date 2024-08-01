#include "FingerSaber.hpp"

#include "ModConfig.hpp"

#include "GlobalNamespace/OVRInput.hpp"
#include "GlobalNamespace/OVRManager.hpp"


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

#include <sstream>
#include <string>

#include "GlobalNamespace/OVRPlugin.hpp"
#include "GlobalNamespace/OVRHand.hpp"

#include "GlobalNamespace/FirstPersonFlyingController.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/OculusVRHelper.hpp"
#include "GlobalNamespace/VRPlatformSDK.hpp"
#include "GlobalNamespace/OVRCameraRig.hpp"

#include "GlobalNamespace/OVRSkeleton.hpp"
#include "GlobalNamespace/OVRSkeletonRenderer.hpp"
#include "GlobalNamespace/OVRBone.hpp"

#include "UnityEngine/EventSystems/PointerInputModule.hpp"
#include "VRUIControls/VRInputModule.hpp"
#include "GlobalNamespace/TimeHelper.hpp"

#include "UnityEngine/Camera.hpp"

#include "System/Collections/Generic/List_1.hpp"
#include "System/Collections/Generic/IList_1.hpp"
#include "System/Collections/ObjectModel/ReadOnlyCollection_1.hpp"

#include "UnityEngine/LineRenderer.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Color.hpp"

#include "UnityEngine/Resources.hpp"

#include "ModUtils.hpp"
#include "UnityEngine/Resources.hpp"
#include "logging.hpp"

FingerSaber modManager;

void FingerSaber::InstallHooks() {
    _Hook_SceneManager_SetActiveScene();
    _Hook_MultiplayerSpectatorController_SwitchToSpectatingSpot();
    _Hook_SaberModelController_Init();
    _Hook_GamePause_Pause();
    _Hook_GamePause_WillResume();
    _Hook_MultiplayerLocalActivePlayerGameplayManager_PerformPlayerFail();
    _Hook_BoneVisualization_Update();

    _Hook_menu_saber_functionality();
}

const UnityEngine::Color defaultRightColor{0.156863, 0.556863, 0.823529, 1.000000};
const UnityEngine::Color  defaultLeftColor{0.784314, 0.078431, 0.078431, 1.000000};

bool FingerSaber::_Destroy_OculusHands(){
    bool destroyCalled = false;

    auto old_HandTracking_container = UnityEngine::GameObject::Find("HandTracking_container");
    if(old_HandTracking_container) {
        UnityEngine::GameObject::Destroy(old_HandTracking_container);
        destroyCalled = true;
    }

    // Regardless if destroy was initiated or not, we can assure that the handTrackingObject is nullptr,
    // as HandTracking_container is created with DontDestroyOnLoad option -> If they exists they must either exists in scene.
    handTrackingObjectsParent = nullptr;

    return destroyCalled;
}

void FingerSaber::_InitializeOculusHands(){
    INFO("Oculus Hand MENU Initialization ..");

    createNewSkeletonMaterials();

    this->_Destroy_OculusHands();

    handTrackingObjectsParent = UnityEngine::GameObject::New_ctor(("HandTracking_container"));
    UnityEngine::GameObject::DontDestroyOnLoad(handTrackingObjectsParent);

    // ---

    auto rightHandAnchor = UnityEngine::GameObject::New_ctor(("rightHandAnchor"));
    rightHandAnchor->get_transform()->set_parent(handTrackingObjectsParent->get_transform());

    auto rightHandTrackingGo = UnityEngine::GameObject::New_ctor(("rightHandTracking"));
    rightHandTrackingGo->get_transform()->set_parent(rightHandAnchor->get_transform());

    rightOVRHand = rightHandTrackingGo->AddComponent<GlobalNamespace::OVRHand*>();
    rightOVRHand->HandType = GlobalNamespace::OVRHand::Hand::HandRight;

    rightOVRSkeleton = rightHandTrackingGo->AddComponent<GlobalNamespace::OVRSkeleton*>();
    rightOVRSkeleton->_skeletonType = GlobalNamespace::OVRSkeleton::SkeletonType::HandRight;
    rightOVRSkeleton->_updateRootPose = true;
    rightOVRSkeleton->Initialize();

    this->rightOVRSkeletonRenderer = rightHandTrackingGo->AddComponent<GlobalNamespace::OVRSkeletonRenderer*>();
    rightHandSkeletonMat->SetColor("_Color", defaultRightColor);
    rightOVRSkeletonRenderer->_skeletonMaterial=rightHandSkeletonMat;
    rightOVRSkeletonRenderer->_systemGestureMaterial=rightHandSkeletonMat;
    rightOVRSkeletonRenderer->Initialize();

    INFO("Right Handtracking stuff initialized");

    // ---

    UnityEngine::GameObject* leftHandAnchor = UnityEngine::GameObject::New_ctor(("leftHandAnchor"));
    leftHandAnchor->get_transform()->set_parent(handTrackingObjectsParent->get_transform());

    auto leftHandTrackingGo = UnityEngine::GameObject::New_ctor(("leftHandTracking"));
    leftHandTrackingGo->get_transform()->set_parent(leftHandAnchor->get_transform());

    leftOVRHand = leftHandTrackingGo->AddComponent<GlobalNamespace::OVRHand*>();
    leftOVRHand->HandType = GlobalNamespace::OVRHand::Hand::HandLeft;

    leftOVRSkeleton = leftHandTrackingGo->AddComponent<GlobalNamespace::OVRSkeleton*>();
    leftOVRSkeleton->_skeletonType = GlobalNamespace::OVRSkeleton::SkeletonType::HandLeft;
    leftOVRSkeleton->_updateRootPose = true;
    leftOVRSkeleton->Initialize();

    this->leftOVRSkeletonRenderer = leftHandTrackingGo->AddComponent<GlobalNamespace::OVRSkeletonRenderer*>();
    leftHandSkeletonMat->SetColor(("_Color"), defaultLeftColor);
    leftOVRSkeletonRenderer->_skeletonMaterial=leftHandSkeletonMat;
    leftOVRSkeletonRenderer->_systemGestureMaterial=leftHandSkeletonMat;
    leftOVRSkeletonRenderer->Initialize();

    INFO("Left Handtracking stuff initialized");

}

#include "UnityEngine/RenderSettings.hpp"
#include "UnityEngine/Shader.hpp"

void FingerSaber::createNewSkeletonMaterials(){
    std::optional<UnityEngine::Shader*> simpleLit =
      UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::Shader*>().front([&](auto const& e) { return e->get_name() == "Custom/SimpleLit"; });
    if (!simpleLit.has_value()) {
        INFO("SimpleLit shader not found");
        return;
    }
    leftHandSkeletonMat  = UnityEngine::Material::New_ctor(simpleLit.value());
    rightHandSkeletonMat = UnityEngine::Material::New_ctor(simpleLit.value());
}

void FingerSaber::ChangeRightSkeletonRendererColor(UnityEngine::Color col){
    if(rightHandSkeletonMat) rightHandSkeletonMat->SetColor(("_Color"), col);
}
void FingerSaber::ChangeLeftSkeletonRendererColor(UnityEngine::Color col){
    if(leftHandSkeletonMat) leftHandSkeletonMat->SetColor(("_Color"), col);
}

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

void FingerSaber::update_LRTargetBone(){
    /** -----  https://developer.oculus.com/documentation/unity/unity-handtracking/  -----
     * Hand_ThumbTip    = Hand_Start + Hand_MaxSkinnable + 0 // tip of the thumb
     * Hand_IndexTip    = Hand_Start + Hand_MaxSkinnable + 1 // tip of the index finger
     * Hand_MiddleTip   = Hand_Start + Hand_MaxSkinnable + 2 // tip of the middle finger
     * Hand_RingTip     = Hand_Start + Hand_MaxSkinnable + 3 // tip of the ring finger
     * Hand_PinkyTip    = Hand_Start + Hand_MaxSkinnable + 4 // tip of the pinky
     * */
    int tipStart = (int)GlobalNamespace::OVRSkeleton::BoneId::Hand_Start + (int)GlobalNamespace::OVRSkeleton::BoneId::Hand_MaxSkinnable;

    this->leftTargetBone  = tipStart + (getModConfig().LeftHandTargetIdx.GetValue()  % 5);
    this->rightTargetBone = tipStart + (getModConfig().RightHandTargetIdx.GetValue() % 5);

    leftHand_isTargetHandRight = getModConfig().LeftHandTargetIdx.GetValue()  >= 5;
    rightHand_isTargetHandLeft = getModConfig().RightHandTargetIdx.GetValue() >= 5;
}
