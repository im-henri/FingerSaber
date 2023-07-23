#include "FingerSaber.hpp"
#include "main.hpp"

#include "ModConfig.hpp"

#include "GlobalNamespace/OVRInput.hpp"
#include "GlobalNamespace/OVRInput_Button.hpp"
#include "GlobalNamespace/OVRInput_Axis1D.hpp"
#include "GlobalNamespace/OVRInput_RawButton.hpp"

#include "GlobalNamespace/IVRPlatformHelper.hpp"

#include "UnityEngine/XR/XRNode.hpp"
#include "GlobalNamespace/VRController.hpp"

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Transform.hpp"
#include "GlobalNamespace/IVRPlatformHelper.hpp"
#include "GlobalNamespace/VRControllerTransformOffset.hpp"

#include "GlobalNamespace/IGamePause.hpp"

#include "UnityEngine/Time.hpp"
#include "ModUtils.hpp"

MAKE_HOOK_MATCH(
    VRController_Update,
    &GlobalNamespace::VRController::Update,
    void,
    GlobalNamespace::VRController* self
) {

    VRController_Update(self);

    // Early exit when mod disabled OR hands are not tracked
    if (getModConfig().ModEnabled.GetValue() == false || modManager.getEitherHandIsTracked() == false)
        return;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~  Smoothed saber movement in the menu ~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if( (modManager.is_scene_GameCore == false) ||
        (modManager.is_GamePaused == true)      ||
        (modManager.multiplayerGameFailed == true)
    ) {
        UnityEngine::Vector3 rotOffset = self->transformOffset->get_rotationOffset();
        UnityEngine::Transform* hand_bone_tranform;
        UnityEngine::Quaternion* oldRot;
        UnityEngine::Vector3*    oldPos;

        const float xRot = 60;
        const float yRot = 0;
        const float zRot = 180;
        if(self->node.value == UnityEngine::XR::XRNode::LeftHand){
            rotOffset = rotOffset + UnityEngine::Vector3( {90+180+xRot, -90+yRot, 0+zRot} );
            hand_bone_tranform = modManager.leftOVRSkeleton->bones->get_Item(GlobalNamespace::OVRSkeleton::BoneId::Hand_WristRoot)->get_Transform();
            oldRot = &modManager.menu_l_quaternion;
            oldPos = &modManager.menu_l_vector3;
        }
        else{
            rotOffset = rotOffset + UnityEngine::Vector3( {90+xRot,-90+yRot, 0+zRot} );
            hand_bone_tranform = modManager.rightOVRSkeleton->bones->get_Item(GlobalNamespace::OVRSkeleton::BoneId::Hand_WristRoot)->get_Transform();
            oldRot = &modManager.menu_r_quaternion;
            oldPos = &modManager.menu_r_vector3;
        }
        UnityEngine::Quaternion rotOffsetQuaternion = UnityEngine::Quaternion::Euler(rotOffset.x, rotOffset.y, rotOffset.z);

        UnityEngine::Quaternion newRot = hand_bone_tranform->get_rotation() * rotOffsetQuaternion;
        float anglediff =  UnityEngine::Quaternion::Angle(*oldRot, newRot)/40.0f;
        *oldRot = UnityEngine::Quaternion::Lerp(*oldRot, newRot, UnityEngine::Time::get_deltaTime() * 10.0f * anglediff);
        self->get_transform()->set_rotation(*oldRot);

        UnityEngine::Vector3    newPos = hand_bone_tranform->get_position() + self->get_transform()->get_forward()*+0.175f;
        *oldPos = UnityEngine::Vector3::Lerp(*oldPos, newPos, UnityEngine::Time::get_deltaTime() * 35.0f);
        self->get_transform()->set_position(*oldPos);
    }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~ In game ~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    else if (self->node.value == UnityEngine::XR::XRNode::LeftHand){
        GlobalNamespace::OVRBone* targBone;
        UnityEngine::Vector3    targetPos;
        UnityEngine::Quaternion rotOffset;
        if( (getModConfig().HandMode.GetValue() == true) ) {
            targBone  = modManager.leftOVRSkeleton->bones->get_Item(GlobalNamespace::OVRSkeleton::BoneId::Hand_WristRoot);
            targetPos = targBone->get_Transform()->get_position() + modManager.l_saber_TF->get_forward()*0.15;
            rotOffset = UnityEngine::Quaternion::Euler(0, -90, 180);
        }
        else{
            if(modManager.leftHand_isTargetHandRight == false){
                targBone = modManager.leftOVRSkeleton->bones->get_Item(modManager.leftTargetBone);
                rotOffset = UnityEngine::Quaternion::Euler(0, -90, 180);
            } else{
                targBone = modManager.rightOVRSkeleton->bones->get_Item(modManager.leftTargetBone);
                rotOffset = UnityEngine::Quaternion::Euler(0, 90, 0);
            }
            targetPos = targBone->get_Transform()->get_position();// - modManager.l_saber_TF->get_forward()*0.35;
        }
        UnityEngine::Quaternion targetRot = targBone->get_Transform()->get_rotation();
        targetRot = targetRot * rotOffset;
        self->get_transform()->set_position(targetPos);
        self->get_transform()->set_rotation(targetRot);
    }
    else {
        GlobalNamespace::OVRBone* targBone;
        UnityEngine::Vector3 targetPos;
        UnityEngine::Quaternion rotOffset;
        if( (getModConfig().HandMode.GetValue() == true) ) {
            targBone = modManager.rightOVRSkeleton->bones->get_Item(GlobalNamespace::OVRSkeleton::BoneId::Hand_WristRoot);
            targetPos = targBone->get_Transform()->get_position() + modManager.r_saber_TF->get_forward()*0.15;
            rotOffset = UnityEngine::Quaternion::Euler(0, 90, 0);
        }
        else{
            if(modManager.rightHand_isTargetHandLeft == false){
                targBone = modManager.rightOVRSkeleton->bones->get_Item(modManager.rightTargetBone);
                rotOffset = UnityEngine::Quaternion::Euler(0, 90, 0);
            } else{
                targBone = modManager.leftOVRSkeleton->bones->get_Item(modManager.rightTargetBone);
                rotOffset = UnityEngine::Quaternion::Euler(0, -90, 180);
            }
            targetPos = targBone->get_Transform()->get_position(); //  - modManager.r_saber_TF->get_forward()*0.35;
        }
        auto targetRot = targBone->get_Transform()->get_rotation();
        targetRot = targetRot * rotOffset;
        self->get_transform()->set_rotation(targetRot);
        self->get_transform()->set_position(targetPos);
    }


}

MAKE_HOOK_MATCH(
    OVRInput_Update,
    &GlobalNamespace::OVRInput::Update,
    void
) {
    OVRInput_Update();

    // Checking if mod is enabled to exit early if it is not.
    if(getModConfig().ModEnabled.GetValue() == false)
        return;

    // Polling the input after Update ensures that no input is lost
    if(modManager.rightOVRHand){
        modManager.update_LRHandIsTracked();
        modManager.update_LRHandClickRequested();
    }

    if(modManager.is_scene_GameCore == true ){
        auto getButton = static_cast< bool(*)(GlobalNamespace::OVRInput::Button, GlobalNamespace::OVRInput::Controller)> (&GlobalNamespace::OVRInput::Get);
        if( getButton(GlobalNamespace::OVRInput::Button::Start, GlobalNamespace::OVRInput::Controller::Hands) ){
            if(modManager.pauseController){
                if(modManager.is_GamePaused)
                    modManager.pauseController->HandlePauseMenuManagerDidPressContinueButton();
                else
                    modManager.pauseController->Pause();
            }
        }
    }

    if(modManager.rightOVRHand){
        if( (modManager.is_scene_GameCore == true) &&  (modManager.is_GamePaused == false) &&
            (modManager.pauseController != nullptr) && (modManager.getEitherHandIsTracked() == false) &&
            (getModConfig().AutoPause.GetValue() == true)
        ){
            modManager.pauseController->Pause();
        }
    }

}

MAKE_HOOK_MATCH(
    OVRInput_Get_Axis1D,
    static_cast< float(*)(GlobalNamespace::OVRInput::Axis1D, GlobalNamespace::OVRInput::Controller)> (&GlobalNamespace::OVRInput::Get),
    float,
    GlobalNamespace::OVRInput::Axis1D virtualMask, GlobalNamespace::OVRInput::Controller controllerMask
) {
    float ret = OVRInput_Get_Axis1D(virtualMask, controllerMask);

    // Overriding controller trigger presses when hand-tracking requested.
    if(modManager.is_scene_GameCore == false || modManager.is_GamePaused || (modManager.multiplayerGameFailed == true)){
        if(controllerMask == GlobalNamespace::OVRInput::Controller::RTouch &&
            (getModConfig().OtherHandClicks.GetValue() ? modManager.getLHandClickRequested() : modManager.getRHandClickRequested()) == true)
        {
            ret = 1.0f;
        }
        if(controllerMask == GlobalNamespace::OVRInput::Controller::LTouch &&
            (getModConfig().OtherHandClicks.GetValue() ? modManager.getRHandClickRequested() : modManager.getLHandClickRequested()) == true){
            ret = 1.0f;
        }
    }

    return ret;
}

void FingerSaber::_Hook_menu_saber_functionality(){
    INSTALL_HOOK(getLogger(), OVRInput_Get_Axis1D);
    INSTALL_HOOK(getLogger(), VRController_Update);
    INSTALL_HOOK(getLogger(), OVRInput_Update);
}

