namespace weapon {

    void (*oUpdateSpread)(...);
    void hkUpdateSpread(void* instance)
    {
        if(configs::weapon::noSpread)
            return;
        return oUpdateSpread(instance);
    }

    void (*oApplyKickback)(void* instance, bool& applied);
    void hkApplyKickback(void* instance, bool& applied)
    {
        oApplyKickback(instance, applied);
        if(configs::weapon::noRecoil) {
            *(float*)((uint64_t)instance + 0x10 + 0xC) = 0.f;
            *(float*)((uint64_t)instance + 0x10 + 0x10) = 0.f;
        }
    }

    void init()
    {
        hook(offsets::character_data::UpdateSpread, hkUpdateSpread, oUpdateSpread);
        hook(offsets::character_data::ApplyKickback, hkApplyKickback, oApplyKickback);
    }

    void patches()
    {
        // ProcessHitBuffers (0x1067F3C + 0xB8) // updated
        if(configs::weapon::wallShot)
            PATCH(offsets::gamesystem::ProcessHitBuffers, "1F2003D5");
        // CharacterData$$ApplyAimPunch
        if(configs::weapon::noAimpunch)
            PATCH(offsets::character_data::ApplyAimPunch, "0000A0E31EFF2FE1");
        // ImplementerWeaponGameplayBase$$get_Crosshair
        if(configs::weapon::alwaysCrosshair)
            PATCH(offsets::weapon_gameplay::get_Crosshair, "01 00 A0 E3 1E FF 2F E1");
    }

}