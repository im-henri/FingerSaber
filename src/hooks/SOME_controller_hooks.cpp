#include "UnNamedMod.hpp"
#include "main.hpp"

#include "GlobalNamespace/OVRinput.hpp"
#include "GlobalNamespace/OVRInput_Button.hpp"
#include "GlobalNamespace/OVRInput_Axis1D.hpp"

#include "GlobalNamespace/IVRPlatformHelper.hpp"

#include "UnityEngine/XR/XRNode.hpp"
#include "GlobalNamespace/VRController.hpp"

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"
#include "UnityEngine/Transform.hpp"
#include "GlobalNamespace/IVRPlatformHelper.hpp"
#include "GlobalNamespace/VRControllerTransformOffset.hpp"


MAKE_HOOK_MATCH(
    VRController_Update,
    &GlobalNamespace::VRController::Update,
    void,
    GlobalNamespace::VRController* self
) {

    VRController_Update(self);
    
    // Overriding VR Controller positions when not in game
    if(modManager.is_scene_GameCore == false && modManager.getEitherHandIsTracked() == true){
        UnityEngine::Vector3 rotOffset = self->transformOffset->get_rotationOffset();
        UnityEngine::Transform* hand_bone_tranform;
        const float xRot = 60;
        const float yRot = 0;
        const float zRot = 0;
        if(self->node.value == UnityEngine::XR::XRNode::LeftHand){
            rotOffset = rotOffset + UnityEngine::Vector3( {90+180+xRot, -90+yRot, 0+zRot} );
            hand_bone_tranform = modManager.leftOVRSkeleton->bones->get_Item(GlobalNamespace::OVRSkeleton::BoneId::Hand_WristRoot)->get_Transform();
        }
        else{
            rotOffset = rotOffset + UnityEngine::Vector3( {90+xRot,-90+yRot, 0+zRot} );
            hand_bone_tranform = modManager.rightOVRSkeleton->bones->get_Item(GlobalNamespace::OVRSkeleton::BoneId::Hand_WristRoot)->get_Transform();
        }
        UnityEngine::Quaternion rotOffsetQuaternion = UnityEngine::Quaternion::Euler(rotOffset.x, rotOffset.y, rotOffset.z);
        
        self->get_transform()->set_rotation(hand_bone_tranform->get_rotation() * rotOffsetQuaternion);
        self->get_transform()->set_position(hand_bone_tranform->get_position() + self->get_transform()->get_forward()*+0.175f);
    } 

}

MAKE_HOOK_MATCH(
    OVRInput_Get_Axis1D,
    static_cast< float(*)(GlobalNamespace::OVRInput::Axis1D, GlobalNamespace::OVRInput::Controller)> (&GlobalNamespace::OVRInput::Get),
    float,
    GlobalNamespace::OVRInput::Axis1D virtualMask, GlobalNamespace::OVRInput::Controller controllerMask
) {
    float ret = OVRInput_Get_Axis1D(virtualMask, controllerMask);

    // HERE IS WHERE WE WILL OVERRIDE THE INPUT WHEN NEEDED
    if(controllerMask == GlobalNamespace::OVRInput::Controller::RTouch && modManager.getRHandClickRequested() == true){
        return 1.0f;
    }
    if(controllerMask == GlobalNamespace::OVRInput::Controller::LTouch && modManager.getLHandClickRequested() == true){
        return 1.0f;
    }
    
    return ret;
}

void UnNamedMod::_Hook_SOME_HOOK_METHOD(){
    
    INSTALL_HOOK(getLogger(), OVRInput_Get_Axis1D);
    INSTALL_HOOK(getLogger(), VRController_Update);
    
}

