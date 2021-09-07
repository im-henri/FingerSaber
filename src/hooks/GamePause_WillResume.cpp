#include "UnNamedMod.hpp"
#include "main.hpp"

#include "GlobalNamespace/GamePause.hpp"

MAKE_HOOK_MATCH(
    GamePause_WillResume, 
    &GlobalNamespace::GamePause::WillResume,
    void,
    GlobalNamespace::GamePause* self
) {
    GamePause_WillResume(self);

    modManager.is_GamePaused = false;
}

void UnNamedMod::_Hook_GamePause_WillResume(){
    INSTALL_HOOK(getLogger(), GamePause_WillResume);
}