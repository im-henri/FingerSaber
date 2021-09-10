#include "FingerSaber.hpp"
#include "main.hpp"

#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Quaternion.hpp"

#include "Config.hpp"

MAKE_HOOK_MATCH(
    Saber_ManualUpdate, 
    &GlobalNamespace::Saber::ManualUpdate,
    void,
    GlobalNamespace::Saber* self
) {

    getLogger().info("Saber::ManualUpdate().");
    
    if(getModConfig().ModEnabled.GetValue() == true){
        if(modManager.getEitherHandIsTracked() == true){
            if(self->get_saberType() == GlobalNamespace::SaberType::SaberB){
                //Right saber
                GlobalNamespace::OVRBone* targBone;
                UnityEngine::Vector3 targetPos;

                UnityEngine::Quaternion rotOffset;

                if( (getModConfig().HandMode.GetValue() == true) || (getModConfig().ModEnabled.GetValue() == false) ) {
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
                    targetPos = targBone->get_Transform()->get_position() - modManager.r_saber_TF->get_forward()*0.35;
                }
                auto targetRot = targBone->get_Transform()->get_rotation();
                targetRot = targetRot * rotOffset; 

                self->get_transform()->set_position(targetPos);
                self->get_transform()->set_rotation(targetRot); 
            }
            else{
                // Left saber
                GlobalNamespace::OVRBone* targBone; 
                UnityEngine::Vector3    targetPos;

                UnityEngine::Quaternion rotOffset;

                if( (getModConfig().HandMode.GetValue() == true) || (getModConfig().ModEnabled.GetValue() == false) ) {
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
                    targetPos = targBone->get_Transform()->get_position() - modManager.l_saber_TF->get_forward()*0.35;
                }
                
                UnityEngine::Quaternion targetRot = targBone->get_Transform()->get_rotation();
                targetRot = targetRot * rotOffset;
                
                self->get_transform()->set_position(targetPos);
                self->get_transform()->set_rotation(targetRot);
            }
        }
    }
    
    Saber_ManualUpdate(self);
}


void FingerSaber::_Hook_Saber_ManualUpdate(){
    
    INSTALL_HOOK(getLogger(), Saber_ManualUpdate);
}