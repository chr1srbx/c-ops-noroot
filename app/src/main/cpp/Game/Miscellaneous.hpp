namespace miscellaneous {

    void init()
    {

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