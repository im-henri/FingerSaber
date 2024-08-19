#include "FingerSaber.hpp"
#include "main.hpp"

#include "ModConfig.hpp"
#include "logging.hpp"
#include "GlobalNamespace/OVRInput.hpp"
#include "GlobalNamespace/IVRPlatformHelper.hpp"

#include "UnityEngine/XR/XRNode.hpp"
#include "GlobalNamespace/VRController.hpp"

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Transform.hpp"
#include "GlobalNamespace/IVRPlatformHelper.hpp"
#include "GlobalNamespace/VRControllerTransformOffset.hpp"

#include "GlobalNamespace/IGamePause.hpp"
#include "GlobalNamespace/VRControllerTransformOffset.hpp"
#include "UnityEngine/Time.hpp"
#include "ModUtils.hpp"
#include "math.hpp"

MAKE_HOOK_MATCH(
    VRController_Update,
    &GlobalNamespace::VRController::Update,
    void,
    GlobalNamespace::VRController *self)
{
    bool isEitherHandTracked = modManager.getEitherHandIsTracked();

    // Only call original function when hands are not tracked and mod is not enabled
    if (isEitherHandTracked == false)
    {
        VRController_Update(self);
        return;
    }

    // Assign vrcontrollers here so that they can be moved inside BoneVisualization Update
    // That way we can be sure that the controller positions are updated more efficiently.
    if (!modManager.vrcontroller_r && (self->node.value__ == (int)UnityEngine::XR::XRNode::RightHand))
        modManager.vrcontroller_r = self;
    else if (!modManager.vrcontroller_l && (self->node.value__ == (int)UnityEngine::XR::XRNode::LeftHand))
        modManager.vrcontroller_l = self;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~  Any Menu Saber Movement  ~~~~~~~~~~~~~~~~~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Updating menu saber movement here, to keep BoneVisualization_Update
    // cleaner. It also seems like this update is called less frequently.
    if (false == ((modManager.is_scene_GameCore == false) ||
                  (modManager.is_GamePaused == true) ||
                  (modManager.multiplayerGameFailed == true)))
    {
        return;
    }

    // Find position & rotation
    UnityEngine::Transform *hand_bone_tranform;
    bool flip_saber = false;
    if (self->node == UnityEngine::XR::XRNode::RightHand)
    {
        hand_bone_tranform = modManager.rightOVRSkeleton->_bones->get_Item((int)GlobalNamespace::OVRSkeleton::BoneId::Hand_WristRoot)->get_Transform();
    }
    else
    {
        hand_bone_tranform = modManager.leftOVRSkeleton->_bones->get_Item((int)GlobalNamespace::OVRSkeleton::BoneId::Hand_WristRoot)->get_Transform();
        flip_saber = true;
    }
    auto targ_rot = hand_bone_tranform->get_rotation() * UnityEngine::Quaternion::Euler(flip_saber ? 180.f : 0.0f, 90.0f, 0.0f);
    auto targ_pos = hand_bone_tranform->get_position() + self->_viewAnchorTransform->get_forward() * +0.175f;

    // Apply transform
    if (getModConfig().SmoothedMenuHands.GetValue()) // Smoothed in menu
    {
        UnityEngine::Quaternion *oldRot;
        UnityEngine::Vector3 *oldPos;
        if (self->node == UnityEngine::XR::XRNode::LeftHand)
        {
            oldRot = &modManager.menu_l_quaternion;
            oldPos = &modManager.menu_l_vector3;
        }
        else
        {
            oldRot = &modManager.menu_r_quaternion;
            oldPos = &modManager.menu_r_vector3;
        }
        // Smooth angles
        float anglediff = UnityEngine::Quaternion::Angle(*oldRot, targ_rot) / 40.0f;
        *oldRot = UnityEngine::Quaternion::Lerp(*oldRot, targ_rot, UnityEngine::Time::get_deltaTime() * 10.0f * anglediff);
        self->_viewAnchorTransform->set_rotation(*oldRot);
        // Smooth position
        *oldPos = Vector3_Lerp(*oldPos, targ_pos, UnityEngine::Time::get_deltaTime() * 35.0f);
        self->_viewAnchorTransform->set_position(*oldPos);
    }
    else // Apply normally
    {
        self->_viewAnchorTransform->set_rotation(targ_rot);
        self->_viewAnchorTransform->set_position(targ_pos);
    }
}

MAKE_HOOK_MATCH(
    OVRInput_Update,
    &GlobalNamespace::OVRInput::Update,
    void)
{
    OVRInput_Update();

    // Polling the input after Update ensures that no input is lost
    if (modManager.rightOVRHand)
    {
        modManager.update_LRHandIsTracked();
        modManager.update_LRHandClickRequested();
    }

    // Checking any of the hand is tracked. Simply checking if mod is makes it impossible to quit the game
    // without controllers.
    if (modManager.getEitherHandIsTracked() == false)
        return;

    if (modManager.is_scene_GameCore == true)
    {
        auto getButton = static_cast<bool (*)(GlobalNamespace::OVRInput::Button, GlobalNamespace::OVRInput::Controller)>(&GlobalNamespace::OVRInput::Get);
        if (getButton(GlobalNamespace::OVRInput::Button::Start, GlobalNamespace::OVRInput::Controller::Hands))
        {
            if (modManager.pauseController)
            {
                if (modManager.is_GamePaused)
                    modManager.pauseController->HandlePauseMenuManagerDidPressContinueButton();
                else
                    modManager.pauseController->Pause();
            }
        }
    }

    if (modManager.rightOVRHand)
    {
        if ((modManager.is_scene_GameCore == true) && (modManager.is_GamePaused == false) &&
            (modManager.pauseController != nullptr) && (modManager.getEitherHandIsTracked() == false) &&
            (getModConfig().AutoPause.GetValue() == true))
        {
            modManager.pauseController->Pause();
        }
    }
}

MAKE_HOOK_MATCH(
    OVRInput_Get_Axis1D,
    static_cast<float (*)(GlobalNamespace::OVRInput::Axis1D, GlobalNamespace::OVRInput::Controller)>(&GlobalNamespace::OVRInput::Get),
    float,
    GlobalNamespace::OVRInput::Axis1D virtualMask, GlobalNamespace::OVRInput::Controller controllerMask)
{
    float ret = OVRInput_Get_Axis1D(virtualMask, controllerMask);

    // Overriding controller trigger presses when hand-tracking requested.
    if (modManager.is_scene_GameCore == false || modManager.is_GamePaused || (modManager.multiplayerGameFailed == true))
    {
        if (controllerMask == GlobalNamespace::OVRInput::Controller::RTouch &&
            (getModConfig().OtherHandClicks.GetValue() ? modManager.getLHandClickRequested() : modManager.getRHandClickRequested()) == true)
        {
            ret = 1.0f;
        }
        if (controllerMask == GlobalNamespace::OVRInput::Controller::LTouch &&
            (getModConfig().OtherHandClicks.GetValue() ? modManager.getRHandClickRequested() : modManager.getLHandClickRequested()) == true)
        {
            ret = 1.0f;
        }
    }

    return ret;
}

void FingerSaber::_Hook_menu_saber_functionality()
{
    INSTALL_HOOK(Logger, OVRInput_Get_Axis1D);
    INSTALL_HOOK(Logger, VRController_Update);
    INSTALL_HOOK(Logger, OVRInput_Update);
}
