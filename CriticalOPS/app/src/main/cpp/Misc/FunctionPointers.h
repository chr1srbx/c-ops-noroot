#ifndef IMGUIANDROID_FUNCTIONPOINTERS_H
#define IMGUIANDROID_FUNCTIONPOINTERS_H
#include "GameTypes.h"
namespace Pointers
{

    monoString* (*GetDeviceUniqueIdentifier)();
    void(*OpenURL)(monoString* url);
    void(*ChangeMoney)(float deltaMoney);
    bool *(*SetMasterClient)(void *player);
    bool *(*CloseConnection)(void *player);
    monoArray<void**> *(*PhotonNetwork_playerListothers)();
    void *(*player)();
    int(*get_width)();
    int(*get_height)();
    void LoadPointers()
    {
        GetDeviceUniqueIdentifier = (monoString*(*)()) getRealOffset(string2Offset(OBFUSCATE("0x206444C")));
        OpenURL = (void(*)(monoString*)) getRealOffset(string2Offset(OBFUSCATE("0x1CCB124")));
        ChangeMoney = (void(*)(float)) getRealOffset(string2Offset(OBFUSCATE("0xC8F0E8")));
        PhotonNetwork_playerListothers = (monoArray<void **> *(*)()) getRealOffset(string2Offset(OBFUSCATE("0x2A978F4")));
        CloseConnection = (bool *(*)(void *)) getRealOffset(string2Offset(OBFUSCATE("0x2A9CFFC")));
        SetMasterClient = (bool *(*)(void *)) getRealOffset(string2Offset(OBFUSCATE("0x2A9D360")));
        player = (void *(*)()) getRealOffset(string2Offset(OBFUSCATE("0x2A974E4")));
        get_width = (int (*)()) getRealOffset(string2Offset(OBFUSCATE("0x2059224")));
        get_height = (int (*)()) getRealOffset(string2Offset(OBFUSCATE("0x205926C")));
    }
}
#endif IMGUIANDROID_FUNCTIONPOINTERS_H
