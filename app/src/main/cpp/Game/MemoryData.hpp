#include <list>
#include <thread>
#include <cmath>
#include <map>
#include "Vector3.h"
#include "Color.h"

SKIN_CHANGER skinCgr;


std::map<int, float> oldFireRate = std::map<int, float>();

bool fireRateExists(int id)
{
    for(auto i : oldFireRate)
    {
        if(i.first == id)
            return true;
    }
    return false;
}

void addToFireRateList(int id, float val)
{
    oldFireRate.insert(std::make_pair(id, val));
}

void removeFromFireRateList(int id)
{
    for(auto i : oldFireRate)
    {
        if(i.first == id)
            oldFireRate.erase(id);
    }
}

typedef const char* ADDR;

namespace addresses
{
    inline ADDR Component__get_transform = "0x1DC37D8"; // updated
    inline ADDR Transform__get_position_Injected = "0x1DCF7D8"; // updated
    inline ADDR Transform__set_localScale = "0x1DCFED8";
    inline ADDR Camera__WorldToScreenPoint_Injected = "0x1A41890"; // updated
    inline ADDR Camera__get_main = "0x1A4248C"; // updated
    inline ADDR Character__get_Health = "0x14ECD38"; // updated
    inline ADDR Character__GetBodyPart = "0x14ECD94"; // updated
    inline ADDR PlayerAdapter_ctor = "0x135F13C"; // updated
    inline ADDR PlayerAdapter_getTeamIndex = "0x135F1BC"; // updated
    inline ADDR PlayerAdapter_getUsername = "0x135F304"; // updated
    inline ADDR GameSystem__get_LocalCharacter = "0x105EC90"; // updated
    inline ADDR Character__get_IsCrouched = "0x14ECBC0"; // updated
    inline ADDR LocalCharacter__CheckIfHeadBehindWall = "0x16DD2DC"; // updated
    inline ADDR Character__getIsInitialized = "0x14ECBA8"; // updated
    inline ADDR EGID__ctor = "0x1D7712C"; // updated
    inline ADDR TouchControls__OnInputButton = "0xF0F828"; // updated
    inline ADDR GameSystem__get_LocalID = "0x105C15C"; // updated
    inline ADDR BackendManager__GetSessionToken = "0x1B63C34";
}

int getCharacterID(void* character) { return *(int*)((uint64_t)character + 0x18); }

struct PlayerAdapter
{
    int pad_0[3];
    void* Player; // 0x10
};

namespace POINTERS
{
    void* TouchControls;
    void* gSystem;
    void* localCharacter;
    int localTeamIndex;
    bool localDestroyed = true;

    void* (*Component__get_transform)(void* Component);
    void (*Transform__get_position_Injected)(void* Transform, Vector3& out);
    Vector3 (*Camera__WorldToScreenPoint_Injected)(void* Camera, Vector3 position, int eye);
    void* (*Camera__get_main)();
    int (*Player__get_Health)(void* Player);
    void* (*Character__GetBodyPart)(void* Character, int part);
    void (*PlayerAdapter__ctor)(void* PlayerAdapter, void* Player);
    int (*PlayerAdapter__getTeamIndex)(void* PlayerAdapter);
    monoString* (*PlayerAdapter__getUsername)(void* PlayerAdapter);
    void* (*GameSystem_get_LocalCharacter)(void* GameSystem);
    bool (*Character__get_IsCrouched)(void* Character);
    void (*LocalCharacter__CheckIfHeadBehindWall)(void* LocalCharacter, void* character, int& out);
    bool (*Character__IsInitialized)(void* Character);
    void (*EGID__ctor)(void* instance, int a1, int a2);
    void (*TouchControls__OnInputButton)(void* instance, int input, bool down);
    void (*Transform__set_localScale)(void* instance, Vector3 vector);
    int (*GameSystem__get_LocalID)(void* instance);
    monoString* (*GetSessionToken)(void* instance);

    void initPointers()
    {
        Component__get_transform = (void *(*)(void *))(g_il2cppBaseMap.startAddress + string2Offset(addresses::Component__get_transform));
        Transform__get_position_Injected = (void (*)(void *, Vector3&))(g_il2cppBaseMap.startAddress + string2Offset(addresses::Transform__get_position_Injected));
        Camera__WorldToScreenPoint_Injected = (Vector3 (*)(void *, Vector3, int))(g_il2cppBaseMap.startAddress + string2Offset(addresses::Camera__WorldToScreenPoint_Injected));
        Camera__get_main = (void *(*)())(g_il2cppBaseMap.startAddress + string2Offset(addresses::Camera__get_main));
        Player__get_Health = (int(*)(void *))(g_il2cppBaseMap.startAddress + string2Offset(addresses::Character__get_Health));
        Character__GetBodyPart = (void *(*)(void *, int)) (g_il2cppBaseMap.startAddress + string2Offset(addresses::Character__GetBodyPart));
        PlayerAdapter__ctor = (void (*)(void *, void*)) (g_il2cppBaseMap.startAddress + string2Offset(addresses::PlayerAdapter_ctor));
        PlayerAdapter__getTeamIndex = (int (*)(void *)) (g_il2cppBaseMap.startAddress + string2Offset(addresses::PlayerAdapter_getTeamIndex));
        PlayerAdapter__getUsername = (monoString *(*)(void *)) (g_il2cppBaseMap.startAddress + string2Offset(addresses::PlayerAdapter_getUsername));
        GameSystem_get_LocalCharacter = (void *(*)(void *)) (g_il2cppBaseMap.startAddress + string2Offset(addresses::GameSystem__get_LocalCharacter));
        Character__get_IsCrouched = (bool (*)(void *)) (g_il2cppBaseMap.startAddress + string2Offset(addresses::Character__get_IsCrouched));
        LocalCharacter__CheckIfHeadBehindWall = (void (*)(void *, void *, int&)) (g_il2cppBaseMap.startAddress + string2Offset(addresses::LocalCharacter__CheckIfHeadBehindWall));
        Character__IsInitialized = (bool (*)(void *)) (g_il2cppBaseMap.startAddress + string2Offset(addresses::Character__getIsInitialized));
        EGID__ctor = (void (*)(void *, int, int)) (g_il2cppBaseMap.startAddress + string2Offset(addresses::EGID__ctor));
        TouchControls__OnInputButton = (void (*)(void *, int, bool)) (g_il2cppBaseMap.startAddress + string2Offset(addresses::TouchControls__OnInputButton));
        Transform__set_localScale = (void (*)(void *, Vector3)) (g_il2cppBaseMap.startAddress + string2Offset(addresses::Transform__set_localScale));
        GameSystem__get_LocalID = (int (*)(void *)) (g_il2cppBaseMap.startAddress + string2Offset(addresses::GameSystem__get_LocalID));
        GetSessionToken = (monoString*(*)(void *)) (g_il2cppBaseMap.startAddress + string2Offset(addresses::BackendManager__GetSessionToken));
    }
}
struct player_t
{
    void* obj;
    int teamIndex;
    std::string username;
    std::list<void*> bodyParts = std::list<void*>();
};

class PlayerManager
{
public:
    std::vector<player_t> characters;

    PlayerManager() {
        characters = std::vector<player_t>();
    }

    void addCharacter(player_t p)
    {
        characters.push_back(p);
    }

    void removeCharacter(void* character)
    {
        for(int i = 0; i<characters.size(); i++){
            if((characters)[i].obj == character){
                characters.erase(characters.begin() + i);
                return;
            }
        }
    }
};

PlayerManager* manager = new PlayerManager();

Vector3 GetPosition(void* Transform)
{
    if(!Transform) return Vector3();
    Vector3 position;
    POINTERS::Transform__get_position_Injected(Transform, position);
    return position;
}

Vector3 getBonePosition(void *character, int bone) {
    void *hitSphere = NULL;
    void *transform = NULL;
    if (character != nullptr) {
        void *curBone = POINTERS::Character__GetBodyPart(character, bone);
        if (curBone != nullptr) { hitSphere = *(void **) ((uint64_t) curBone + 0x20); }
        if (hitSphere != nullptr) { transform = *(void **) ((uint64_t) hitSphere + 0x30); }
        if (transform != nullptr) {
            return GetPosition(transform);
        }
        return Vector3(0, 0, 0);
    }
    return Vector3(0, 0, 0);
}

namespace HELPERS
{
    bool IsHeadBehindWall(void* Local, void* Character)
    {
        if(Local == nullptr || Character == nullptr) return true;
        int out; // 17 - Not Behind Wall || 44 - yes
        POINTERS::LocalCharacter__CheckIfHeadBehindWall(Local, Character, out);
        return out == 44; // 44 - Behind Wall
    }
    bool getIsCrouched(void* Character)
    {
        return POINTERS::Character__get_IsCrouched(Character);
    }
    Vector3 WorldToScreenPoint(void* Cam, Vector3 pos) {
        if (!Cam) {
            return Vector3();
        }
        return POINTERS::Camera__WorldToScreenPoint_Injected(Cam, pos, 2);
    }

    Vector3 GetPlayerLocation(void* Player)
    {
        return GetPosition(POINTERS::Component__get_transform(Player));
    }
    int GetPlayerHealth(void* Player)
    {
        if(!Player) return NULL;
        return POINTERS::Player__get_Health(Player);
    }
    bool PlayerAlive(void* Player) {
        return Player != NULL && GetPlayerHealth(Player) > 0;
    }
    bool IsPlayerDead(void* Player) {
        return Player == NULL && GetPlayerHealth(Player) < 1;
    }
    void *getPlayer(void* Character) { return *(void**)((uint64_t)Character + 0x90); }
    int getTeamIndex(void* Character)
    {
        if(getPlayer(Character) == nullptr) return -1;
        PlayerAdapter playerAdapter;
        POINTERS::PlayerAdapter__ctor(&playerAdapter, getPlayer(Character));
        return POINTERS::PlayerAdapter__getTeamIndex(&playerAdapter);
    }
    bool CharacterPlayerExists(void* Character)
    {
        return (*(void**)((uint64_t)Character + 0x90)) != nullptr;
    }
    std::string getUsername(void* Character)
    {
        PlayerAdapter playerAdapter;
        POINTERS::PlayerAdapter__ctor(&playerAdapter, *(void**)((uint64_t)Character + 0x90));
        return (POINTERS::PlayerAdapter__getUsername(&playerAdapter))->getString();
    }
    bool isCharacterVisible(void *character) {
        if(POINTERS::gSystem == nullptr) return 0;
        void *localCharacter = POINTERS::localCharacter;
        if (localCharacter) {
            return !HELPERS::IsHeadBehindWall(localCharacter, character);
        }
        return 0;
    }
    bool isCharacterShooting(void *character) {
        void *characterData = *(void **) ((uint64_t) character + string2Offset(OBFUSCATE("0x98")));
        if (characterData) {
            return *(bool *) ((uint64_t) characterData + string2Offset(OBFUSCATE("0x6C")));
        }
        return 0;
    }
    bool isInFov2(Vector2 rotation, Vector2 newAngle, CFG_AIM cfg) {
        if (!cfg.fovCheck) return true;

        Vector2 difference = newAngle - rotation;
        const auto fov = std::hypot(difference.X, difference.Y);
        if (fov > cfg.fovValue)
            return false;
        return true;
    }
    void *getValidEnt3(CFG_AIM cfg, Vector2 rotation = Vector2()) {
        void *localPlayer = POINTERS::localCharacter;
        if (localPlayer == nullptr) return nullptr;
        if(!POINTERS::Character__IsInitialized(localPlayer)) return nullptr;
        int localTeam = POINTERS::localTeamIndex;
        if(POINTERS::localDestroyed) return nullptr;
        float closestEntDist = 99999.0f;
        void *closestCharacter = nullptr;
        for(auto character : manager->characters) {
            void* currentEnemy;
            currentEnemy = character.obj;
            int curTeam = HELPERS::getTeamIndex(currentEnemy);
            int health = HELPERS::GetPlayerHealth(currentEnemy);
            bool canSet = false;
            Vector2 newAngle;

            if (cfg.aimbot) {
                if (HELPERS::GetPlayerHealth(localPlayer) > 0 && health > 0) {
                    Vector3 localHead = getBonePosition(localPlayer, 10);
                    if (HELPERS::getIsCrouched(localPlayer)) {
                        localHead = localHead - Vector3(0, 0.5, 0);
                    }
                    Vector3 enemyBone = getBonePosition(currentEnemy, cfg.aimBone);
                    Vector3 deltavec = enemyBone - localHead;
                    float deltLength = sqrt(deltavec.X * deltavec.X + deltavec.Y * deltavec.Y +
                                            deltavec.Z * deltavec.Z);

                    newAngle.X = -asin(deltavec.Y / deltLength) * (180.0 / M_PI);
                    newAngle.Y = atan2(deltavec.X, deltavec.Z) * 180.0 / M_PI;
                    if (isInFov2(rotation, newAngle, cfg)) {
                        if (localTeam != curTeam && curTeam != -1) {
                            if(!cfg.visCheck && currentEnemy && isCharacterVisible(currentEnemy))
                                return currentEnemy;
                            if (cfg.visCheck) {
                                if (currentEnemy && isCharacterVisible(currentEnemy)) {
                                    canSet = true;
                                }
                            }
                            void *transform = POINTERS::Component__get_transform(currentEnemy);
                            if (transform) {
                                Vector3 localPosition = GetPosition(transform);
                                Vector3 currentCharacterPosition = GetPosition(
                                        POINTERS::Component__get_transform(localPlayer));
                                Vector3 currentEntDist = Vector3::Distance(localPosition,
                                                                           currentCharacterPosition);
                                if (Vector3::Magnitude(currentEntDist) < closestEntDist) {
                                    if (cfg.visCheck && !canSet) continue;
                                    closestEntDist = Vector3::Magnitude(currentEntDist);
                                    closestCharacter = currentEnemy;
                                }
                            }
                        }
                    }
                }
            }
        }
        return closestCharacter;
    }
}

#include "../Include/ESPRendering.h"

namespace HOOKS {
    CFG_ESP cfg_esp;
    CFG_CHAMS cfg_chams;
    CFG_AIM cfg_aim;
    CFG_VISUALS cfg_visuals;
    CFG_WEAPON cfg_weapon;
    CFG_MISC cfg_misc;
    void* currentAimedCharacter;
    int shootControl = 0;

    void ESP() {
        if(cfg_aim.fovCircle && cfg_aim.fovValue != 0.f && POINTERS::gSystem != nullptr) {
            ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(glWidth / 2, glHeight / 2),
                       (tan(cfg_aim.fovValue * (M_PI / 180) / 2) / tan(90 / 2) *glWidth / 2) * 2,
                       ImColor(cfg_esp.COLOR[0], cfg_esp.COLOR[1],
                                 cfg_esp.COLOR[2], cfg_esp.COLOR[3]),
                       360);
        }
        if (!cfg_esp.ENABLED) return;
        if (POINTERS::gSystem == nullptr) return;
        void *mycam = POINTERS::Camera__get_main();
        if (mycam == nullptr) return;
        for (auto character: manager->characters) {
            if (character.obj != nullptr &&
                HELPERS::PlayerAlive(character.obj) &&
                HELPERS::CharacterPlayerExists(character.obj) &&
                getCharacterID(character.obj) != getCharacterID(POINTERS::localCharacter) &&
                HELPERS::getPlayer(character.obj) != nullptr &&
                POINTERS::Character__IsInitialized(character.obj)
            ) {
                if (POINTERS::localCharacter && character.teamIndex == POINTERS::localTeamIndex) {
                    manager->removeCharacter(character.obj);
                    continue;
                }
                if(cfg_weapon.headRain)
                {
                    for(auto bp : character.bodyParts)
                    {
                        int bodyPart = *(int*)((uint64_t)bp + 0x18);
                        if(bodyPart == 10) // HEAD
                        {
                            if(*(float*)((uint64_t)bp + 0x20 + 0x24) != 20.f)
                                cfg_weapon.hr_old_val = *(float*)((uint64_t)bp + 0x20 + 0x24);
                            *(float*)((uint64_t)bp + 0x20 + 0x24) = 20.f;
                        }
                    }
                } else {
                    if(cfg_weapon.hr_changed)
                    {
                        for(auto bp : character.bodyParts)
                        {
                            int bodyPart = *(int*)((uint64_t)bp + 0x18);
                            if(bodyPart == 10) // HEAD
                            {
                                *(float*)((uint64_t)bp + 0x20 + 0x24) = cfg_weapon.hr_old_val;
                                cfg_weapon.hr_old_val = 0.f;
                                cfg_weapon.hr_changed = false;
                            }
                        }
                    }
                }
                if(cfg_visuals.widePutin)
                {
                    Vector3 vector(cfg_visuals.putinX, cfg_visuals.putinY, cfg_visuals.putinZ);
                    POINTERS::Transform__set_localScale(
                            POINTERS::Component__get_transform(character.obj),
                            vector
                    );
                }
                Vector3 position = HELPERS::GetPlayerLocation(character.obj);
                Vector3 transformPos = HELPERS::WorldToScreenPoint(mycam, position);
                if (transformPos.Z < 0.f) continue;
                transformPos.Y = glHeight - transformPos.Y;

                Vector3 headPos = getBonePosition(character.obj, HEAD);
                Vector3 wsheadPos = HELPERS::WorldToScreenPoint(mycam, headPos);
                if (wsheadPos.Z < 0.f) continue;

                Vector3 aboveHead = headPos + Vector3(0, 0.2, 0);
                Vector3 headEstimate = position + Vector3(0, 1.48, 0);
                Vector3 wsAboveHead = HELPERS::WorldToScreenPoint(mycam, aboveHead);
                Vector3 wsheadEstimate = HELPERS::WorldToScreenPoint(mycam, headEstimate);
                if (wsAboveHead.Z < 0.f || wsheadEstimate.Z < 0.f) continue;

                wsAboveHead.Y = glHeight - wsAboveHead.Y;
                wsheadEstimate.Y = glHeight - wsheadEstimate.Y;

                float height = transformPos.Y - wsAboveHead.Y;
                float horizontalDist = abs(wsAboveHead.X - transformPos.X);
                float width = ((transformPos.Y - wsheadEstimate.Y) / 2) + horizontalDist;

                if (cfg_esp.OUTLINED_BOXES)
                    DrawOutlinedBox2(wsAboveHead.X - width / 2, wsAboveHead.Y, width, height,
                                     ImVec4(cfg_esp.COLOR[0], cfg_esp.COLOR[1], cfg_esp.COLOR[2],
                                            cfg_esp.COLOR[3]),
                                     3, ImGui::GetBackgroundDrawList());

                if (cfg_esp.LINES_FROM_TOP)
                    DrawLine(ImVec2(glWidth / 2, 0.f), ImVec2(transformPos.X, transformPos.Y),
                             ImColor(cfg_esp.COLOR[0], cfg_esp.COLOR[1], cfg_esp.COLOR[2],
                                     cfg_esp.COLOR[3]), 3, ImGui::GetBackgroundDrawList());

                if (cfg_esp.LINES_FROM_BOTTOM)
                    DrawLine(ImVec2(glWidth / 2, glHeight), ImVec2(transformPos.X, transformPos.Y),
                             ImColor(cfg_esp.COLOR[0], cfg_esp.COLOR[1], cfg_esp.COLOR[2],
                                     cfg_esp.COLOR[3]), 3, ImGui::GetBackgroundDrawList());

                if (cfg_esp.USERNAME && HELPERS::CharacterPlayerExists(character.obj)) {
                    std::string username = character.username;
                    float compensation = strlen(username.c_str()) * 4.0f;
                    ImGui::PushFont(cfg_esp.smallerFont);
                    DrawText(ImVec2(wsheadPos.X - compensation, wsAboveHead.Y - 20),
                             ImColor(cfg_esp.COLOR[0], cfg_esp.COLOR[1], cfg_esp.COLOR[2],
                                     cfg_esp.COLOR[3]), username, cfg_esp.smallerFont,
                             ImGui::GetBackgroundDrawList());
                    ImGui::PopFont();
                }
                if(cfg_esp.HEALTHBAR) {
                    int health = *(int*)((uint64_t)character.obj + 0xC0);
                    DrawOutlinedFilledRect(wsAboveHead.X + width / 2 + 12, wsAboveHead.Y + height *
                                                                                           (1 -
                                                                                            (static_cast<float>(health) /
                                                                                             100.0f)),
                                           3, height * (static_cast<float>(health) / 100.0f),
                                           HealthToColor(health), ImGui::GetBackgroundDrawList());
                }
            }
        }
    }

    void * (*old_CreateCharacter)(void *GameSystem, int id, Vector3 position, Quaternion rotation, bool local);
    void* CreateCharacter(void* obj, int id, Vector3 position, Quaternion rotation, bool local){
        if(obj != nullptr){
            void* character = old_CreateCharacter(obj, id, position, rotation, local);
            if(character != nullptr)
            {
                int localID = POINTERS::GameSystem__get_LocalID(obj);
                if(localID && getCharacterID(character) == localID)
                    return character;
                player_t p;
                p.obj = character;
                p.teamIndex = HELPERS::getTeamIndex(character);
                p.username = HELPERS::getUsername(character);
                manager->addCharacter(p);
            }
            return character;
        }
        return old_CreateCharacter(obj, id, position, rotation, local);
    }

    void (*old_DestroyCharacter)(void *obj, void *character, bool teamflip);
    void DestroyCharacter(void* obj, void* character, bool teamflip){
        if(obj != nullptr){
            if(character == POINTERS::localCharacter)
            {
                POINTERS::localCharacter = nullptr;
                POINTERS::localTeamIndex = -1;
            }
            manager->removeCharacter(character);
            return old_DestroyCharacter(obj, character, teamflip);
        }
        return old_DestroyCharacter(obj, character, teamflip);
    }

    int doAimbot = 0;
    void (*oSetRotation)(void* character, Vector2 rotation);
    void setRotation(void *character, Vector2 rotation) {
        void* mycam = POINTERS::Camera__get_main();
        if(mycam == nullptr) return;
        Vector2 newAngle;
        Vector2 difference;
        difference.X = 0;
        difference.Y = 0;
        bool canSet = false;
        void *closestEnt = nullptr;
        if (character != nullptr && POINTERS::localCharacter != nullptr && cfg_aim.aimbot) {
            closestEnt = HELPERS::getValidEnt3(cfg_aim, rotation);
            currentAimedCharacter = closestEnt;
        }

        if (POINTERS::localCharacter != nullptr &&
            HELPERS::GetPlayerHealth(POINTERS::localCharacter) > 0 && closestEnt != nullptr) {
            Vector3 localHead = getBonePosition(POINTERS::localCharacter, 10);
            if (localHead != Vector3(0, 0, 0) && HELPERS::getIsCrouched(POINTERS::localCharacter)) {
                localHead = localHead - Vector3(0, 0.5, 0);
            }

            Vector3 enemyBone = getBonePosition(closestEnt, cfg_aim.aimBone);
            Vector3 wsEnemyBone = HELPERS::WorldToScreenPoint(mycam, enemyBone);
            if(wsEnemyBone.Z > 0.f && cfg_aim.redDot)
                ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(wsEnemyBone.X, wsEnemyBone.Y), 2.f, ImColor(1.f, 0.f, 0.f, 1.f), 360);

            Vector3 deltavec = enemyBone - localHead;
            float deltLength = sqrt(deltavec.X * deltavec.X + deltavec.Y * deltavec.Y +
                                    deltavec.Z * deltavec.Z);

            newAngle.X = -asin(deltavec.Y / deltLength) * (180.0 / M_PI);
            newAngle.Y = atan2(deltavec.X, deltavec.Z) * 180.0 / M_PI;

            if (cfg_aim.aimbot && character == POINTERS::localCharacter) {
                if (cfg_aim.visCheck) {
                    if (HELPERS::isCharacterVisible(closestEnt)) {
                        canSet = true;
                    }
                }
                if (cfg_aim.onShoot) {
                    if (HELPERS::isCharacterShooting(POINTERS::localCharacter)) {
                        if(cfg_aim.visCheck && !canSet) {
                            oSetRotation(character, rotation);
                            return;
                        }
                        if (cfg_aim.fovCheck) {
                            if(doAimbot % 3 == 0) {
                                if (HELPERS::isInFov2(rotation, newAngle, cfg_aim))
                                    difference = newAngle - rotation;
                                doAimbot = 1;
                            } else doAimbot += 1;
                        } else {
                            difference = newAngle - rotation;
                            cfg_aim.fovValue = 360;
                        }
                    }
                } else {
                    if (cfg_aim.fovCheck) {
                        if(doAimbot % 3 == 0) {
                        if(HELPERS::isInFov2(rotation, newAngle, cfg_aim))
                            difference = newAngle - rotation;
                            doAimbot = 1;
                        } else doAimbot += 1;
                    } else {
                        difference = newAngle - rotation;
                        cfg_aim.fovValue = 360;
                    }
                }

                if (cfg_aim.aimbotSmooth) {
                    if(cfg_aim.visCheck && !canSet) {
                        oSetRotation(character, rotation);
                        return;
                    }
                    difference = difference / cfg_aim.smoothAmount;
                }
            }
        }
        oSetRotation(character, rotation + difference);
    }

    void (*oldApplyKickback)(void* characterData, bool& applied);
    void ApplyKickback(void* characterData, bool& applied)
    {
        oldApplyKickback(characterData, applied);
        if(cfg_weapon.noRecoil) {
            *(float*)((uint64_t)characterData + 0x10 + 0xC) = 0.f;
            *(float*)((uint64_t)characterData + 0x10 + 0x10) = 0.f;
        }
    }

    void (*oldSetSpread)(void* OverlayScope, float value);
    void set_Spread(void* OverlayScope, float value)
    {
        if(cfg_visuals.noSniperBlur)
            return;
        return oldSetSpread(OverlayScope, value);
    }

    void (*oldUpdateSpread)(void* CharacterData);
    void UpdateSpread(void* CharacterData)
    {
        if(cfg_weapon.noSpread)
            return;
        return oldUpdateSpread(CharacterData);
    }

    void (*old_CheckCharacterVisibility)(void* CharacterModel, bool& visible);
    void CheckCharacterVisibility(void* CharacterModel, bool& visible)
    {
        old_CheckCharacterVisibility(CharacterModel, visible);
        if(cfg_visuals.teamRadar)
            visible = true;
    }

    bool (*old_Linecast)(Vector3 start, Vector3 end, int layerMask, int interaction);
    bool Linecast(Vector3 start, Vector3 end, int layerMask, int interaction)
    {
        bool ret = old_Linecast(start, end, layerMask, interaction);
        if(cfg_visuals.teamRadar)
            return false;
        return ret;
    }

    void initFont()
    {
        ImGuiIO &io = ImGui::GetIO();
        cfg_esp.smallerFont = io.Fonts->AddFontFromMemoryTTF(Minecraftia, 18, 18.f);
    }

    void initPatches() {
        PATCH_SWITCH("0x1067FF4", "1F2003D5", cfg_weapon.wallShot); // ProcessHitBuffers (0x1067F3C + 0xB8) // updated
    }

    static const char* shaderName = "_PBS_Character_Indirect_Base";

    void EGID__ctor(void* instance, int entityId, int groupId)
    {
        if(groupId == 0)
        {
            if(entityId == skinCgr.old_glove_ct_id ||
                    entityId == skinCgr.default_glove_ct_id)
                entityId = skinCgr.glove_ct_id;
            if(entityId == skinCgr.old_glove_t_id ||
                    entityId == skinCgr.default_glove_t_id)
                entityId = skinCgr.glove_t_id;
        }
        if(groupId == 103)
        {
            if(entityId == skinCgr.default_knife_id)
                entityId = skinCgr.knife_id;
            if(entityId == skinCgr.default_xd45_id)
                entityId = skinCgr.xd45_id;
            if(entityId == skinCgr.default_dualmtx_id)
                entityId = skinCgr.dualmtx_id;
            if(entityId == skinCgr.default_gsr1911_id)
                entityId = skinCgr.gsr1911_id;
            if(entityId == skinCgr.default_mr96_id)
                entityId = skinCgr.mr96_id;
            if(entityId == skinCgr.default_deagle_id)
                entityId = skinCgr.deagle_id;
            if(entityId == skinCgr.default_mpx_id)
                entityId = skinCgr.mpx_id;
            if(entityId == skinCgr.default_mp5_id)
                entityId = skinCgr.mp5_id;
            if(entityId == skinCgr.default_mp7_id)
                entityId = skinCgr.mp7_id;
            if(entityId == skinCgr.default_p90_id)
                entityId = skinCgr.p90_id;
            if(entityId == skinCgr.default_vector_id)
                entityId = skinCgr.vector_id;
            if(entityId == skinCgr.default_fp6_id)
                entityId = skinCgr.fp6_id;
            if(entityId == skinCgr.default_m1887_id)
                entityId = skinCgr.m1887_id;
            if(entityId == skinCgr.default_super90_id)
                entityId = skinCgr.super90_id;
            if(entityId == skinCgr.default_ksg_id)
                entityId = skinCgr.ksg_id;
            if(entityId == skinCgr.default_sa58_id)
                entityId = skinCgr.sa58_id;
            if(entityId == skinCgr.default_m4_id)
                entityId = skinCgr.m4_id;
            if(entityId == skinCgr.default_ar15_id)
                entityId = skinCgr.ar15_id;
            if(entityId == skinCgr.default_ak47_id)
                entityId = skinCgr.ak47_id;
            if(entityId == skinCgr.default_hk417_id)
                entityId = skinCgr.hk417_id;
            if(entityId == skinCgr.default_sg551_id)
                entityId = skinCgr.sg551_id;
            if(entityId == skinCgr.default_aug_id)
                entityId = skinCgr.aug_id;
            if(entityId == skinCgr.default_scarh_id)
                entityId = skinCgr.scarh_id;
            if(entityId == skinCgr.default_trg22_id)
                entityId = skinCgr.trg22_id;
            if(entityId == skinCgr.default_m14_id)
                entityId = skinCgr.m14_id;
            if(entityId == skinCgr.default_svd_id)
                entityId = skinCgr.svd_id;
            if(entityId == skinCgr.default_uratio_id)
                entityId = skinCgr.uratio_id;
        }
        POINTERS::EGID__ctor(instance, entityId, groupId);
    }
    
    void updateSkin(int weaponType, int skin)
    {
        switch(weaponType)
        {
            case Weapons::KNIFE:
                skinCgr.knife_id = skin;
            case Weapons::GLOVE_T:
                skinCgr.old_glove_t_id = skinCgr.glove_t_id;
                skinCgr.glove_t_id = skin;
            case Weapons::GLOVE_CT:
                skinCgr.old_glove_ct_id = skinCgr.glove_ct_id;
                skinCgr.glove_ct_id = skin;
            case Weapons::XD_45:
                skinCgr.xd45_id = skin;
            case Weapons::DUAL_MTX:
                skinCgr.dualmtx_id = skin;
            case Weapons::GSR_1911:
                skinCgr.gsr1911_id = skin;
            case Weapons::MR_96:
                skinCgr.mr96_id = skin;
            case Weapons::DEAGLE:
                skinCgr.deagle_id = skin;
            case Weapons::MPX:
                skinCgr.mpx_id = skin;
            case Weapons::MP5:
                skinCgr.mp5_id = skin;
            case Weapons::MP7:
                skinCgr.mp7_id = skin;
            case Weapons::P90:
                skinCgr.p90_id = skin;
            case Weapons::VECTOR:
                skinCgr.vector_id = skin;
            case Weapons::FP6:
                skinCgr.fp6_id = skin;
            case Weapons::M1887:
                skinCgr.m1887_id = skin;
            case Weapons::SUPER90:
                skinCgr.super90_id = skin;
            case Weapons::KSG:
                skinCgr.ksg_id = skin;
            case Weapons::SA58:
                skinCgr.sa58_id = skin;
            case Weapons::M4:
                skinCgr.m4_id = skin;
            case Weapons::AR15:
                skinCgr.ar15_id = skin;
            case Weapons::AK47:
                skinCgr.ak47_id = skin;
            case Weapons::HK417:
                skinCgr.hk417_id = skin;
            case Weapons::SG551:
                skinCgr.sg551_id = skin;
            case Weapons::AUG:
                skinCgr.aug_id = skin;
            case Weapons::SCAR_H:
                skinCgr.scarh_id = skin;
            case Weapons::TRG_22:
                skinCgr.trg22_id = skin;
            case Weapons::M14:
                skinCgr.m14_id = skin;
            case Weapons::SVD:
                skinCgr.svd_id = skin;
            case Weapons::URATIO:
                skinCgr.uratio_id = skin;
        }
    }

    Vector3 (*old_GetViewAdjustedPos)(void* instance, float aspect);
    Vector3 GetViewAdjustedPos(void* instance, float aspect)
    {
        Vector3 ret = old_GetViewAdjustedPos(instance, aspect);
        if(instance != nullptr) {
            if (cfg_visuals.viewModel) {
                ret.X = cfg_visuals.viewModelXValue;
                ret.Y = cfg_visuals.viewModelYValue;
                ret.Z = cfg_visuals.viewModelZValue;
            } else {
                if (cfg_visuals.viewModelXValue == 0.f &&
                    cfg_visuals.viewModelYValue == 0.f &&
                    cfg_visuals.viewModelZValue == 0.f)
                {
                    cfg_visuals.viewModelXValue = ret.X;
                    cfg_visuals.viewModelYValue = ret.Y;
                    cfg_visuals.viewModelZValue = ret.Z;
                }
            }
        }
        return ret;
    }

    void (*old_GetSpottedStatus)(void* instance, int& out);
    void GetSpottedStatus(void* instance, int& out)
    {
        if(cfg_visuals.radar) {
            old_GetSpottedStatus(instance, out);
            out = 6;
            return;
        }
        old_GetSpottedStatus(instance, out);
    }

    void (*oAddBodyPart)(void* instance, void* bodyPart);
    void AddBodyPart(void* instance, void* bodyPart)
    {
        if(instance != nullptr)
        {
            for(auto c : manager->characters)
            {
                if(c.obj != nullptr)
                {
                    if(c.obj == instance)
                        c.bodyParts.push_back(bodyPart);
                }
            }
        }
        oAddBodyPart(instance, bodyPart);
    }

    // A WAY TO COPS API
//    monoString* (*oGetSessionToken)(void* instance);
//    monoString* GetSessionToken(void* instance)
//    {
//        monoString* a = oGetSessionToken(instance);
//        LOGI("SESSION TOKEN! %s", a->getString().c_str());
//        return a;
//    }

    void initHooks() {
        cfg_esp = CFG_ESP();
        cfg_chams = CFG_CHAMS();
        cfg_aim = CFG_AIM();
        cfg_visuals = CFG_VISUALS();
        cfg_weapon = CFG_WEAPON();
        cfg_misc = CFG_MISC();
        Configs::initDir();
        HOOK("0x1061970", CreateCharacter, old_CreateCharacter);//GameSystem.CreateCharacter // updated
        HOOK("0x10647E8", DestroyCharacter, old_DestroyCharacter);//GameSystem.DestroyCharacter // updated
        HOOK("0x14ED728", setRotation, oSetRotation);//Character.setRotation(Vector2) // updated
        HOOK("0x15D9C80", UpdateSpread, oldUpdateSpread);//CharacterData.UpdateSpread // updated
        HOOK("0x15D9DFC", ApplyKickback, oldApplyKickback);//CharacterData.ApplyKickback // updated
        HOOK("0x1ACEC28", Linecast, old_Linecast);//Physics.Linecast // updated
        HOOK("0x16BFFC4", CheckCharacterVisibility, old_CheckCharacterVisibility);//CharacterModel.CheckCharacterVisibilty // updated
        HOOK("0x1D7712C", EGID__ctor, POINTERS::EGID__ctor);//EGID..ctor // updated
        HOOK("0x1809F28", GetViewAdjustedPos, old_GetViewAdjustedPos);//WeaponViewModel.GetViewAdjustedPos // updated
        HOOK("0x14EED18", GetSpottedStatus, old_GetSpottedStatus);//Character.GetSpottedStatus // updated
        HOOK("0x14EEB64", AddBodyPart, oAddBodyPart);// Character.AddBodyPart
//        HOOK("0x1B63C34", GetSessionToken, oGetSessionToken);
    }

};






