#include "UnNamedMod.hpp"
#include "main.hpp"

#include "GlobalNamespace/SaberModelController.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/HideFlags.hpp"
#include "UnityEngine/Resources.hpp"
#include "GlobalNamespace/SaberType.hpp"

#include <sstream>
#include <string>

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

#include "GlobalNamespace/PauseController.hpp"
#include "System/Action.hpp"

#include "UnityEngine/Color.hpp"
#include "UnityEngine/Material.hpp"
#include "GlobalNamespace/ColorManager.hpp"

#include "GlobalNamespace/PauseController.hpp"


static int handInitCount = 0; // Hands always initialized as pair i.e. twice(even when playing one handed mode)
MAKE_HOOK_MATCH(
    SaberModelController_Init, 
    &GlobalNamespace::SaberModelController::Init,
    void,
    GlobalNamespace::SaberModelController* self, 
    UnityEngine::Transform* parent, 
    GlobalNamespace::Saber* saber
) {

    getLogger().info("SaberModelController::Init() Called.");

    SaberModelController_Init(self, parent, saber);

    if(modManager.is_scene_GameCore){
        if(saber->get_saberType() == GlobalNamespace::SaberType::SaberB){
            modManager.ChangeRightSkeletonRendererColor(self->colorManager->ColorForSaberType(GlobalNamespace::SaberType::SaberB));
        }
        
        if(saber->get_saberType() == GlobalNamespace::SaberType::SaberA){
            modManager.ChangeLeftSkeletonRendererColor(self->colorManager->ColorForSaberType(GlobalNamespace::SaberType::SaberA));
        }

        float sScale = 1.0f/6.5f;
        saber->get_transform()->set_localScale(UnityEngine::Vector3{sScale,sScale,sScale});
        
        bool rightsaber_bool = true;
        if (saber->get_saberType() == GlobalNamespace::SaberType::SaberA) rightsaber_bool = false;
        if (rightsaber_bool) modManager.r_saber_TF = saber->get_transform();
        else                 modManager.l_saber_TF = saber->get_transform();

        handInitCount += 1;
        // Figured this is safer way to know when both sabers exist, as opposed to assuming which saber is last to get initialized.
        // (I assume its not always left or right saber that gets initialized last.)
        if (handInitCount%2 == 0){
            auto VRGameCore = UnityEngine::GameObject::Find(il2cpp_utils::createcsstr("Origin/VRGameCore"));
            if(VRGameCore){
                modManager.handTrackingObjectsParent->get_transform()->set_position(VRGameCore->get_transform()->get_position());

                // -- do smthn with this
                auto PauseController_go = UnityEngine::GameObject::Find(il2cpp_utils::createcsstr("PauseController"));
                if(PauseController_go) modManager.pauseController = PauseController_go->GetComponent<GlobalNamespace::PauseController*>();
                // -- do smthn with this
                
                UnityEngine::Vector3 scaler{7.5,7.5,7.5};

                VRGameCore->get_transform()->set_localScale(scaler);
                modManager.handTrackingObjectsParent->get_transform()->set_localScale(scaler);

                auto posBody = VRGameCore->get_transform()->get_position();
                posBody = posBody + UnityEngine::Vector3{0,-5.9,0};
                VRGameCore->get_transform()->set_position(posBody);

                auto posHands = modManager.handTrackingObjectsParent->get_transform()->get_position();
                posHands = posHands + UnityEngine::Vector3{0,-5.9,0};
                modManager.handTrackingObjectsParent->get_transform()->set_position(posHands);

            }
        }
    }

}

void UnNamedMod::_Hook_SaberModelController_Init(){
    
    INSTALL_HOOK(getLogger(), SaberModelController_Init);
}