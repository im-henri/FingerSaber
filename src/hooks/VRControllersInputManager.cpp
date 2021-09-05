#include "UnNamedMod.hpp"
#include "main.hpp"


#include "GlobalNamespace/OculusVRHelper.hpp"


MAKE_HOOK_MATCH(
    OculusVRHelper_FixedUpdate, 
    &GlobalNamespace::OculusVRHelper::FixedUpdate, 
    void,
    GlobalNamespace::OculusVRHelper* self
) {
    OculusVRHelper_FixedUpdate(self);
    modManager.FixedUpdate(self);
}


void UnNamedMod::_Hook_OculusVRHelper_VRControllersInputManager(){
    INSTALL_HOOK(getLogger(), OculusVRHelper_FixedUpdate);
}

