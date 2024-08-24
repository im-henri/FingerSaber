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
    GlobalNamespace::OVRSkeletonRenderer::BoneVisualization *self,

    float scale,
    bool shouldRender,
    bool shouldUseSystemGestureMaterial,
    GlobalNamespace::OVRSkeletonRenderer::ConfidenceBehavior confidenceBehavior,
    GlobalNamespace::OVRSkeletonRenderer::SystemGestureBehavior systemGestureBehavior)
{

    float scale_in;
    if (modManager.is_scene_GameCore == true &&
        modManager.multiplayerGameFailed == false &&
        getModConfig().HandMode.GetValue() == false &&
        getModConfig().ModEnabled.GetValue() == true)
    {
        scale_in = 7.5f;
    }
    else
    {
        scale_in = scale;
    }

    BoneVisualization_Update(
        self,
        scale_in,
        shouldRender,
        shouldUseSystemGestureMaterial,
        confidenceBehavior,
        systemGestureBehavior);
}

MAKE_HOOK_MATCH(
    OVRSkeletonRenderer_Update,
    &GlobalNamespace::OVRSkeletonRenderer::Update,
    void,
    GlobalNamespace::OVRSkeletonRenderer *self)

{
    DEBUG("OVRSkeletonRenderer_Update");

    // Exit early if not rendering fingers
    if (getModConfig().handRendering.GetValue() == false)
        return;

    OVRSkeletonRenderer_Update(self);
}

MAKE_HOOK_MATCH(
    OVRSkeleton_UpdateSkeleton,
    &GlobalNamespace::OVRSkeleton::UpdateSkeleton,
    void,
    GlobalNamespace::OVRSkeleton *self)
{
    DEBUG("OVRSkeleton_UpdateSkeleton");

    OVRSkeleton_UpdateSkeleton(self);

    // Dont move controllers in game when mod disabled
    if (getModConfig().ModEnabled.GetValue() == false)
        return;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~ In game ~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if ( // Check if we are in game
        (modManager.is_scene_GameCore == false) ||
        (modManager.is_GamePaused == true) ||
        (modManager.multiplayerGameFailed == true) ||
        // Check whether the vrcontroller pointers are valid.
        // They get invalidated after a scene change.
        (modManager.vrcontroller_l == nullptr) ||
        (modManager.vrcontroller_r == nullptr))
    {
        return;
    }

    // Target controller
    GlobalNamespace::VRController *vrcntrl;

    // Shift sabers forward in hands mode, backwards in normal (finger mode)
    UnityEngine::Vector3 saber_pos_shift;

    // Find position & rotation
    UnityEngine::Transform *targ_bone_tranform;
    bool flip_saber = false;
    if (modManager.rightOVRSkeleton == self)
    {
        vrcntrl = modManager.vrcontroller_r;
        if ((getModConfig().HandMode.GetValue() == true))
        {
            targ_bone_tranform = modManager.rightOVRSkeleton->_bones->get_Item((int)GlobalNamespace::OVRSkeleton::BoneId::Hand_WristRoot)->get_Transform();
            saber_pos_shift = vrcntrl->_viewAnchorTransform->get_forward() * 0.15f;
        }
        else
        {
            if (modManager.rightHand_isTargetHandLeft == false)
                targ_bone_tranform = modManager.rightOVRSkeleton->_bones->get_Item((int)modManager.rightTargetBone)->get_Transform();
            else
            {
                flip_saber = true;
                targ_bone_tranform = modManager.leftOVRSkeleton->_bones->get_Item((int)modManager.rightTargetBone)->get_Transform();
            }
            saber_pos_shift = vrcntrl->_viewAnchorTransform->get_forward() * (-0.15f);
        }
    }
    else
    {
        vrcntrl = modManager.vrcontroller_l;
        flip_saber = true;
        if ((getModConfig().HandMode.GetValue() == true))
        {
            targ_bone_tranform = modManager.leftOVRSkeleton->_bones->get_Item((int)GlobalNamespace::OVRSkeleton::BoneId::Hand_WristRoot)->get_Transform();
            saber_pos_shift = vrcntrl->_viewAnchorTransform->get_forward() * 0.15f;
        }
        else
        {
            if (modManager.leftHand_isTargetHandRight == false)
                targ_bone_tranform = modManager.leftOVRSkeleton->_bones->get_Item((int)modManager.leftTargetBone)->get_Transform();
            else
            {
                flip_saber = false;
                targ_bone_tranform = modManager.rightOVRSkeleton->_bones->get_Item((int)modManager.leftTargetBone)->get_Transform();
            }
            saber_pos_shift = vrcntrl->_viewAnchorTransform->get_forward() * (-0.15f);
        }
    }
    auto targ_rot = targ_bone_tranform->get_rotation() * UnityEngine::Quaternion::Euler(flip_saber ? 180.f : 0.0f, 90.0f, 0.0f);
    auto targ_pos = targ_bone_tranform->get_position() + saber_pos_shift;
    // Apply transform
    vrcntrl->_viewAnchorTransform->set_rotation(targ_rot);
    vrcntrl->_viewAnchorTransform->set_position(targ_pos);
}

void FingerSaber::_Hook_BoneVisualization_Update()
{
    INSTALL_HOOK(Logger, BoneVisualization_Update);
    INSTALL_HOOK(Logger, OVRSkeleton_UpdateSkeleton);
    INSTALL_HOOK(Logger, OVRSkeletonRenderer_Update);
}