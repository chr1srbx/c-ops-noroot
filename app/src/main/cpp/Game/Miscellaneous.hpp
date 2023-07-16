namespace miscellaneous {

    void init()
    {

    }

    void UpdateWeapon(void* obj, float deltatime) {
        if (obj != nullptr) {
            void* CharacterData = *(void**)((uint64_t)obj + string2Offset(OBFUSCATE("0x98")));
            if (CharacterData != nullptr) {
                void* CharacterSettingsData = *(void**)((uint64_t)CharacterData + string2Offset(OBFUSCATE("0x78")));
                if (CharacterSettingsData != nullptr) {
                    if (configs::miscellaneous::speed) {
                        *(float*)((uint64_t)CharacterSettingsData + string2Offset(OBFUSCATE("0x14"))) = configs::miscellaneous::speedValue;
                    }

                    if (configs::character::crouchcheck) {
                        *(float*)((uint64_t)CharacterSettingsData + string2Offset(OBFUSCATE("0x1C"))) = configs::character::crouchspeed;
                    }

                    if (configs::character::jumpHeight) {
                        *(float*)((uint64_t)CharacterSettingsData + string2Offset(OBFUSCATE("0x4C"))) = configs::character::jumpVal;
                        *(float*)((uint64_t)CharacterSettingsData + string2Offset(OBFUSCATE("0x50"))) = configs::character::jumpVal;
                    }

                    if (configs::character::noslow) {
                        *(float*)((uint64_t)CharacterSettingsData + string2Offset(OBFUSCATE("0x60"))) = 0;
                        *(float*)((uint64_t)CharacterSettingsData + string2Offset(OBFUSCATE("0x64"))) = 0;
                        *(float*)((uint64_t)CharacterSettingsData + string2Offset(OBFUSCATE("0x5C"))) = 0;
                    }
                }

                void* WeaponDefData = *(void**)((uint64_t)CharacterData + string2Offset(OBFUSCATE("0x80")));
                if (WeaponDefData != nullptr) {
                    //add reload time
                    if (recoil) {
                        *(float*)((uint64_t)WeaponDefData + string2Offset(OBFUSCATE("0xF0"))) = recoilval;
                        *(float*)((uint64_t)WeaponDefData + string2Offset(OBFUSCATE("0x100"))) = recoilval;
                    }

                    if (firerate) {
                        *(float*)((uint64_t)WeaponDefData + string2Offset(OBFUSCATE("0x64"))) = 1600;
                        *(float*)((uint64_t)WeaponDefData + string2Offset(OBFUSCATE("0x8C"))) = 0;
                    }

                    if (burstfire) {
                        *(int*)((uint64_t)WeaponDefData + string2Offset(OBFUSCATE("0x11C"))) = 30;
                        *(float*)((uint64_t)WeaponDefData + string2Offset(OBFUSCATE("0x120"))) = 0;
                    }

                    if (fscope) {
                        *(int *) ((uint64_t) WeaponDefData + string2Offset(OBFUSCATE("0x98"))) = 1;
                    }

                    if (pickup) {
                        *(float *) ((uint64_t) WeaponDefData + string2Offset(OBFUSCATE("0x88"))) = 0;
                        *(float *) ((uint64_t) WeaponDefData + string2Offset(OBFUSCATE("0x8C"))) = 0;
                    }

                    if (shake) {
                        *(float *) ((uint64_t) WeaponDefData + string2Offset(OBFUSCATE("0x110"))) = 0;
                    }
                }
            }
        }
        oldUpdateWeapon(obj, deltatime);
    }

    void patches()
    {

        // PracticeModeBot$$IsValidTarget
        if(configs::miscellaneous::botsNoShoot)
            PATCH("0x1933C84", "00 00 A0 E3 1E FF 2F E1");
        // LocalCharacter$$GetCapsuleParameters
        if(configs::miscellaneous::noclip)
            PATCH("0x16D2860", "00 00 A0 E3 1E FF 2F E1");
    }

}