
    namespace pointers {
        void* game; // GameSystem
        player_t localEntity;

        void *(*Component$$get_transform)(void *instance);
        void (*Transform$$get_position_Injected)(void *instance, Vector3 &out);
        Vector3 (*Camera$$WorldToScreenPoint)(void *instance, Vector3 position, int eye);
        void *(*Camera$$get_main)();
        int (*Character$$get_Health)(void *instance);
        void *(*Character$$GetBodyPart)(void *instance, int bodyPart);
        void (*PlayerAdapter$$ctor)(void *instance, void *player);
        int (*PlayerAdapter$$getTeamIndex)(void *instance);
        monoString *(*PlayerAdapter$$getUserName)(void *instance);
        bool (*Character$$get_IsCrouched)(void *instance);
        bool (*Character$$get_IsInitialized)(void *instance);
        void (*EGID$$ctor)(void *instance, int a1, int a2);
        void (*Transform$$set_localScale)(void *instance, Vector3 vector);
        bool (*GameSystem$$TryGetCharacter)(void* instance, int id, void*& character);
        int (*GameSystem$$get_LocalId)(void* instance);
        bool (*Physics$$Linecast)(Vector3 start, Vector3 end, int layermask, int trigger);
        Vector2 (*Transform$$get_rotation)(void* instance);

        void init() {
            game = nullptr;
            Component$$get_transform = (void *(*)(void *)) (getRealOffset(offsets::component::get_transform));
            Transform$$get_position_Injected = (void (*)(void *, Vector3 &out)) (getRealOffset(offsets::transform::get_position_Injected));
            Transform$$set_localScale = (void (*)(void *, Vector3)) (getRealOffset(offsets::transform::set_localScale));
            Camera$$WorldToScreenPoint = (Vector3 (*)(void *, Vector3 position, int eye)) (getRealOffset(offsets::camera::WorldToScreenPoint));
            Camera$$get_main = (void *(*)()) (getRealOffset(offsets::camera::get_main));
            Character$$get_Health = (int (*)(void *)) (getRealOffset(offsets::character::get_Health));
            Character$$GetBodyPart = (void *(*)(void *, int)) (getRealOffset(offsets::character::GetBodyPart));
            Character$$get_IsCrouched = (bool (*)(void *)) (getRealOffset(offsets::character::get_IsCrouched));
            Character$$get_IsInitialized = (bool (*)(void *)) (getRealOffset(offsets::character::get_IsInitialized));
            PlayerAdapter$$ctor = (void (*)(void *, void *)) (getRealOffset(offsets::player_adapter::ctor));
            PlayerAdapter$$getTeamIndex = (int (*)(void *)) (getRealOffset(offsets::player_adapter::getTeamIndex));
            GameSystem$$get_LocalId = (int (*)(void *)) (getRealOffset(offsets::gamesystem::get_LocalID));
            PlayerAdapter$$getUserName = (monoString *(*)(void *)) (getRealOffset(offsets::player_adapter::getUserName));
            EGID$$ctor = (void (*)(void *, int, int)) (getRealOffset(offsets::egid::ctor));
            Physics$$Linecast = (bool (*)(Vector3, Vector3, int, int)) (getRealOffset(offsets::physics::Linecast2));
            Transform$$get_rotation = (Vector2 (*)(void*))(getRealOffset(offsets::transform::get_rotation));
        }
    }

