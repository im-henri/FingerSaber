
#include "ModUtils.hpp"

template <typename T>
void Log_GetComponents(UnityEngine::GameObject* gameobj, Logger& logger){
    auto componentArr = gameobj->GetComponents<T>();
    std::stringstream buff;
    for (int i=0; i<componentArr->Length(); i++){
        auto comp = componentArr[i];
        auto name = comp->get_name();
        auto fullName = comp->GetType()->get_FullName();
        buff << to_utf8(csstrtostr(fullName)).c_str() << " " << to_utf8(csstrtostr(name)).c_str() << "\n";
    }
    logger.info("%s",buff.str().c_str());
}

template <typename T>
void Log_GetComponentsInChildren(UnityEngine::GameObject* gameobj, Logger& logger){
    auto componentArr = gameobj->GetComponentsInChildren<T>();
    std::stringstream buff;
    for (int i=0; i<componentArr->Length(); i++){
        auto comp = componentArr[i];
        auto name = comp->get_name();
        auto fullName = comp->GetType()->get_FullName();
        buff  << to_utf8(csstrtostr(fullName)).c_str() << " " << to_utf8(csstrtostr(name)).c_str() << "\n";
    }
    logger.info("%s",buff.str().c_str());
}

void Write_To_Log(std::stringstream& logStream, std::string logName, Logger& logger){
    bool success = writefile("sdcard/Android/data/com.beatgames.beatsaber/files/logs/" + logName, logStream.str());
    if (success)   logger.info("%s", ("Logging to \"" + logName + "\" successful.").c_str());
    else           logger.info("Creating log file failed.");
}

void Write_To_Log_AllGameObjectsInScene(std::string logName, Logger& logger){
    auto allObject = UnityEngine::Resources::FindObjectsOfTypeAll(csTypeOf(UnityEngine::GameObject*));

    std::stringstream buff;

    for(int i=0; i<allObject->Length(); i++){
        auto go_asObj = allObject[i];
        //getLogger().info("Found items: %i", i);

        if (
            !(go_asObj->get_hideFlags() == UnityEngine::HideFlags::NotEditable || go_asObj->get_hideFlags() == UnityEngine::HideFlags::HideAndDontSave)
            )
        {
            //getLogger().info("Go name: %s", to_utf8(csstrtostr(name)).c_str() );
            // TRYING CASTING
            UnityEngine::GameObject* go = il2cpp_utils::cast<UnityEngine::GameObject, UnityEngine::Object>(go_asObj);

            Il2CppString* name = go->get_name(); // C# strings are pointers to UTF-16 strings
            Il2CppString* rootName = go->get_transform()->GetRoot()->get_name();

            UnityEngine::Transform* parentTf = go->get_transform();


            while(name->CompareTo(rootName) != 0){
                std::stringstream namestream;
                namestream << to_utf8(csstrtostr(name)).c_str() << "(" << parentTf->GetInstanceID() << ")";
                buff << namestream.str() << " -> ";

                parentTf = parentTf->get_parent();
                name = parentTf->get_gameObject()->get_name();
            }

            //Name: SaberA, RootName: MenuCore
            //buff << i << "\t" << "Name: " << to_utf8(csstrtostr(name)).c_str();
            buff << to_utf8(csstrtostr(rootName)).c_str() << "(" << go->get_transform()->GetRoot()->GetInstanceID() << ")";

            buff << '\n' << '\n';
        }
    }

    Write_To_Log(buff, logName, logger);
}