namespace cops {
    static bool menuOpen = true;
#include "structs.h"
#include "EntityManager.hpp"
    EntityManager* manager;

#include "Configs.hpp"
#include "Offsets.hpp"
#include "Pointers.hpp"
#include "Helpers.hpp"
#include "Hooks.hpp"
#include "../ESPRendering.h"
#include "ESP.hpp"
#include "Aimbot.hpp"
#include "Weapon.hpp"
#include "Miscellaneous.hpp"
#include "HideSystem.hpp"
#include "SilentAim.hpp"

    void init() {
        manager = new EntityManager();
        menuOpen = true;
        pointers::init();
        hooks::init();
        aimbot::init();
        weapon::init();
        miscellaneous::init();
        hide_system::init();
        silent_aim::init();
    }
}