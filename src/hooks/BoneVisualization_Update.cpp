#include "FingerSaber.hpp"
#include "main.hpp"
#include "logging.hpp"

#include "GlobalNamespace/OVRSkeleton.hpp"
#include "GlobalNamespace/OVRSkeletonRenderer.hpp"
#include "GlobalNamespace/VRControllerTransformOffset.hpp"

#include "UnityEngine/Time.hpp"

#include "ModConfig.hpp"
#include "math.hpp"
MAKE_HOOK_MATCH(
    BoneVisualization_Update,
    &GlobalNamespace::OVRSkeletonRenderer::BoneVisualization::Update,
    void,
    GlobalNamespace::OVRSkeletonRenderer::BoneVisualization* self,

    float scale,
    bool shouldRender,
    bool shouldUseSystemGestureMaterial,
    GlobalNamespace::OVRSkeletonRenderer::ConfidenceBehavior confidenceBehavior,
    GlobalNamespace::OVRSkeletonRenderer::SystemGestureBehavior systemGestureBehavior
) {

    float scale_in;
    if( modManager.is_scene_GameCore == true
        && modManager.multiplayerGameFailed == false
        && getModConfig().HandMode.GetValue() == false
    ){
        scale_in = 7.5f;
    }
    else{
        scale_in = scale;
    }

    BoneVisualization_Update(
        self,
        scale_in,
        shouldRender,
        shouldUseSystemGestureMaterial,
        confidenceBehavior,
        systemGestureBehavior
    );
}


MAKE_HOOK_MATCH(
    OVRSkeletonRenderer_Update,
    &GlobalNamespace::OVRSkeletonRenderer::Update,
    void,
    GlobalNamespace::OVRSkeletonRenderer* self
) {
    DEBUG("OVRSkeletonRenderer_Update");

    OVRSkeletonRenderer_Update(self);

    // Dont move hands in game when mod disabled
    if (getModConfig().ModEnabled.GetValue() == false)
        return;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~ In game ~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if( // Check if we are in game
        (modManager.is_scene_GameCore == false)     ||
        (modManager.is_GamePaused == true)          ||
        (modManager.multiplayerGameFailed == true)  ||
        // Check whether the vrcontroller pointers are valid.
        // They get invalidated after a scene change.
        (modManager.vrcontroller_l == nullptr)      ||
        (modManager.vrcontroller_r == nullptr)
    ){
        return;
    }

    // Target controller
    GlobalNamespace::VRController* vrcntrl;

    GlobalNamespace::OVRBone* targBone;
    UnityEngine::Vector3    targetPos;
    UnityEngine::Quaternion rotOffset;
    UnityEngine::Quaternion targetRot;

    // Left hand / controller
    if (modManager.leftOVRSkeletonRenderer == self){
        DEBUG("Left hand found");
        vrcntrl = modManager.vrcontroller_l;
        if( (getModConfig().HandMode.GetValue() == true) ) {
            targBone  = modManager.leftOVRSkeleton->_bones->get_Item((int) GlobalNamespace::OVRSkeleton::BoneId::Hand_WristRoot);
            targetPos = targBone->get_Transform()->get_position() + modManager.l_saber_TF->get_forward()*0.15;
            rotOffset = UnityEngine::Quaternion::Euler(0, -90, 180);
        }
        else{
            if(modManager.leftHand_isTargetHandRight == false){
                targBone = modManager.leftOVRSkeleton->_bones->get_Item((int)modManager.leftTargetBone);
                rotOffset = UnityEngine::Quaternion::Euler(0, -90, 180);
            } else{
                targBone = modManager.rightOVRSkeleton->_bones->get_Item((int)modManager.leftTargetBone);
                rotOffset = UnityEngine::Quaternion::Euler(0, 90, 0);
            }
            targetPos = targBone->get_Transform()->get_position() - modManager.l_saber_TF->get_forward() * 0.15f;
        }
        targetRot = targBone->get_Transform()->get_rotation() * rotOffset;
        vrcntrl->get_transform()->set_rotation(targetRot);
        vrcntrl->get_transform()->set_position(targetPos);
    }

    // Right hand / controller
    if (modManager.rightOVRSkeletonRenderer == self){
        DEBUG("Right hand found");
        vrcntrl = modManager.vrcontroller_r;
        if( (getModConfig().HandMode.GetValue() == true) ) {
            targBone = modManager.rightOVRSkeleton->_bones->get_Item((int) GlobalNamespace::OVRSkeleton::BoneId::Hand_WristRoot);
            targetPos = targBone->get_Transform()->get_position() + modManager.r_saber_TF->get_forward()*0.15f;
            rotOffset = UnityEngine::Quaternion::Euler(0, 90, 0);
        }
        else{
            if(modManager.rightHand_isTargetHandLeft == false){
                targBone = modManager.rightOVRSkeleton->_bones->get_Item((int)modManager.rightTargetBone);
                rotOffset = UnityEngine::Quaternion::Euler(0, 90, 0);
            } else{
                targBone = modManager.leftOVRSkeleton->_bones->get_Item((int)modManager.rightTargetBone);
                rotOffset = UnityEngine::Quaternion::Euler(0, -90, 180);
            }
            targetPos = targBone->get_Transform()->get_position() - modManager.r_saber_TF->get_forward()*0.15f;
        }
        targetRot = targBone->get_Transform()->get_rotation() * rotOffset;
        vrcntrl->get_transform()->set_rotation(targetRot);
        vrcntrl->get_transform()->set_position(targetPos);
    }

}

void FingerSaber::_Hook_BoneVisualization_Update(){
    INSTALL_HOOK(Logger, BoneVisualization_Update);
    INSTALL_HOOK(Logger, OVRSkeletonRenderer_Update);
}