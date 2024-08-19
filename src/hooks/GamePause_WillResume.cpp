#include "FingerSaber.hpp"
#include "main.hpp"
#include "logging.hpp"
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

void FingerSaber::_Hook_GamePause_WillResume(){
    INSTALL_HOOK(Logger, GamePause_WillResume);
}