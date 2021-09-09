#include "main.hpp"

#include "FingerSaber.hpp"
#include "Config.hpp"

#include "questui/shared/QuestUI.hpp"

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup


// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getModConfig().Init(modInfo);
    getLogger().info("Completed setup!");
}

#include "ModSettingsViewController.hpp"

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {

    //if (!LoadConfig())
    //    SaveConfig();

    il2cpp_functions::Init();
    QuestUI::Init();

    getLogger().info("Installing hooks...");
    modManager.InstallHooks();
    getLogger().info("Installed all hooks!");

    custom_types::Register::AutoRegister();

    QuestUI::Register::RegisterModSettingsViewController<FingerSaberSettings::ModSettingsViewController*>(modInfo);


}