#include "UnNamedMod.hpp"
#include "main.hpp"

#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Quaternion.hpp"


MAKE_HOOK_MATCH(
    Saber_ManualUpdate, 
    &GlobalNamespace::Saber::ManualUpdate,
    void,
    GlobalNamespace::Saber* self
) {

    //getLogger().info("Saber::ManualUpdate().");
    
    if(self->get_saberType() == GlobalNamespace::SaberType::SaberB){
        //Right saber
        auto xxx = modManager.rightOVRSkeleton->bones->get_Item(GlobalNamespace::OVRSkeleton::BoneId::Hand_IndexTip);
        
        auto targetPos = xxx->get_Transform()->get_position() - modManager.r_saber_TF->get_forward()*0.35;
        auto targetRot = xxx->get_Transform()->get_rotation();
        targetRot = targetRot * UnityEngine::Quaternion::Euler(0, 90+3, 0); // this adds a 90 degrees Y rotation

        self->get_transform()->set_position(targetPos);
        self->get_transform()->set_rotation(targetRot); 
    }
    else{
        // Left saber
        auto yyy = modManager.leftOVRSkeleton->bones->get_Item(GlobalNamespace::OVRSkeleton::BoneId::Hand_IndexTip);
        
        UnityEngine::Vector3    targetPos = yyy->get_Transform()->get_position() - modManager.l_saber_TF->get_forward()*0.35;
        UnityEngine::Quaternion targetRot = yyy->get_Transform()->get_rotation();
        targetRot = targetRot * UnityEngine::Quaternion::Euler(0, -90+3, 180);
        
        self->get_transform()->set_position(targetPos);
        self->get_transform()->set_rotation(targetRot);
    }

    Saber_ManualUpdate(self);
}


void UnNamedMod::_Hook_Saber_ManualUpdate(){
    
    INSTALL_HOOK(getLogger(), Saber_ManualUpdate);
}