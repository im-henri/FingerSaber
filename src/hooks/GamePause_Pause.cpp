#include "FingerSaber.hpp"
#include "main.hpp"
#include "logging.hpp"
#include "GlobalNamespace/GamePause.hpp"

MAKE_HOOK_MATCH(
    GamePause_Pause, 
    &GlobalNamespace::GamePause::Pause,
    void,
    GlobalNamespace::GamePause* self
) {
    GamePause_Pause(self);

    modManager.is_GamePaused = true;
}

void FingerSaber::_Hook_GamePause_Pause(){
    INSTALL_HOOK(Logger, GamePause_Pause);
}