
    namespace offsets {
        namespace gamesystem {
            uint64_t get_LocalID = 0x105F700;
            auto Update = OBFUSCATE("0x1067314");
            auto Destroy = OBFUSCATE("0x1063408");
            auto CreateCharacter = OBFUSCATE("0x1064F14");
            auto DestroyCharacter = OBFUSCATE("0x1067D8C");
            auto ProcessHitBuffers = OBFUSCATE("0x106B4E0");
        }

        namespace character {
            uint64_t get_Health = 0x14F01F0;
            uint64_t GetBodyPart = 0x14F024C;
            uint64_t get_IsCrouched = 0x14F0078;
            uint64_t get_IsInitialized = 0x14F0060;
            auto setRotation = OBFUSCATE("0x14F0BE0"); //Vector2
            auto GetSpottedStatus = OBFUSCATE("0x14F21D0");
            auto AddBodyPart = OBFUSCATE("0x14F201C");
        }

        namespace character_data {
            auto UpdateSpread = OBFUSCATE("0x15D8478");
            auto ApplyAimPunch = OBFUSCATE("0x15D8718");
            auto ApplyKickback = OBFUSCATE("0x15D85F4");
        }

        namespace weapon_gameplay {
            auto get_Crosshair = OBFUSCATE("0x110C1F0");
        }

        namespace character_model {
            auto CheckCharacterVisibility = OBFUSCATE("0x16C1788");
        }

        namespace weapon_view_model {
            auto GetViewAdjustedPos = OBFUSCATE("0x18086EC");
        }

        namespace physics {
            uint64_t Linecast2 = 0x1AC73B4;
            uint64_t Linecast = 0x1AC72A0; // trigger
        }

        namespace player_adapter {
            uint64_t ctor = 0x135C260;
            uint64_t getTeamIndex = 0x135C2E0;
            uint64_t getUserName = 0x135C428;
        }

        namespace transform {
            uint64_t get_position_Injected = 0x1DC9834;
            uint64_t set_localScale = 0x1DC9F34;
            uint64_t get_rotation = 0x1DC9A70;
        }

        namespace component {
            uint64_t get_transform = 0x1DBD834;
        }

        namespace camera {
            uint64_t WorldToScreenPoint = 0x1A3CF10;
            uint64_t get_main = 0x1A3DB0C;
        }

        namespace backend_manager {
            const char* GetSessionToken = "0x1B5E2AC";
        }

        namespace egid {
            uint64_t ctor = 0x1D70188;
        }
    }
