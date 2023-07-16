
namespace hooks {

    namespace gamesystem {

        void *(*oCreateCharacter)(...);
        void *hkCreateCharacter(void *obj, int id, Vector3 position, Quaternion rotation, bool local) {
            void *character = oCreateCharacter(obj, id, position, rotation, local);
            if (obj != nullptr && character != nullptr && helpers::getPlayer(character) != nullptr) {
                player_t p = player_t();
                auto player_adapter = helpers::CreatePlayerAdapter(character);
                p.obj = character;
                p.player = helpers::getPlayer(character);
                p.teamIndex = helpers::getTeamIndex(player_adapter);
                p.username = helpers::getUsername(player_adapter);
                p.id = id;
                LOGI("Team Index: %i, User Name: %s, ID: %i, Character: %p", p.teamIndex, p.username.c_str(), p.id, character);
                LOGI("Local ID: %i", pointers::GameSystem$$get_LocalId(obj));
                if (pointers::GameSystem$$get_LocalId(obj) == id) {
                    pointers::localEntity = p;
                }
                manager->add(p);
            }
            return character;
        }

        void (*oDestroyCharacter)(...);
        void hkDestroyCharacter(void* obj, void* character, bool teamflip = false) {
            LOGI("DESTROY CHARACTER CALLED");
            if(obj != nullptr && character != nullptr) {
                LOGI("CHARACTER PTR: %p", character);
                manager->remove(character);
            }
            return oDestroyCharacter(obj, character, teamflip);
        }

        void (*oUpdate)(...);
        void hkUpdate(void* obj, float deltaTime)
        {
            pointers::game = obj;
            oUpdate(obj, deltaTime);
        }

        void (*oDestroy)(...);
        void hkDestroy(void* obj) {
            pointers::game = nullptr;
            pointers::localEntity.obj = nullptr;
            pointers::localEntity.player = nullptr;
            pointers::localEntity.teamIndex = -1;
            pointers::localEntity.username = "";
            pointers::localEntity.id = -1;
            oDestroy(obj);
        }
    }

    monoString* (*oGetSessionToken)(void* instance);
    monoString* GetSessionToken(void* instance)
    {
        monoString* a = oGetSessionToken(instance);
        LOGI("SESSION TOKEN! %s", a->getString().c_str());
        return a;
    }

    void init() {
        hook(offsets::gamesystem::CreateCharacter, gamesystem::hkCreateCharacter,
             gamesystem::oCreateCharacter);
        hook(offsets::gamesystem::DestroyCharacter, gamesystem::hkDestroyCharacter,
             gamesystem::oDestroyCharacter);
        hook(offsets::gamesystem::Update, gamesystem::hkUpdate, gamesystem::oUpdate);
        hook(offsets::gamesystem::Destroy, gamesystem::hkDestroy, gamesystem::oDestroy);
        hook(offsets::backend_manager::GetSessionToken, GetSessionToken, oGetSessionToken);
    }

    void patches() {
//        PATCH_SWITCH("0x18A0638", "1F2003D5", configs::visuals::specradar); // FetchFollowedCharacterTeamIndex TBZ
    }
}