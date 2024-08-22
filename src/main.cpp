#include "main.hpp"
#include "logging.hpp"
#include "FingerSaber.hpp"
#include "ModConfig.hpp"

#include "ModSettingsViewController.hpp"
#include "ModGameplaySetupMenu.hpp"
#include "bsml/shared/Helpers/delegates.hpp"
#include "bsml/shared/BSML.hpp"

inline modloader::ModInfo modInfo = {MOD_ID, VERSION, 0};  // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Called at the early stages of game loading
extern "C" __attribute__((visibility("default"))) void setup(CModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo.assign(info);

    getModConfig().Init(modInfo);
    INFO("Completed setup!");
}


// Called later on in the game loading - a good time to install function hooks
extern "C" __attribute__((visibility("default"))) void load() {

    il2cpp_functions::Init();
    BSML::Init();

    INFO("Installing hooks...");
    modManager.InstallHooks();
    INFO("Installed all hooks!");

    custom_types::Register::AutoRegister();
    BSML::Register::RegisterGameplaySetupTab<FingerSaberSettings::ModGameplaySetupMenu*>("FingerSaber");
    BSML::Register::RegisterSettingsMenu<FingerSaberSettings::ModSettingsViewController*>("FingerSaber");

    modManager.update_LRTargetBone();
    modManager.update_scoreSubmission();
}