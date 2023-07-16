#ifndef HOOKS
#define HOOKS

#include "../Misc/FunctionPointers.h"
#include "../Unity/Unity.h"
namespace Hooks {
    bool price, damage, hpcheck, torquecheck, automatic, scope, fuel, masscheck, noshift, reload, range, pickup, sway, headbo,
            jumpheightcheck, unlockall, multiplier, frozen, forcejump, cooldown, noclip, infjump, speedcheck;
    float speed, horse, torque, mass, height, duration, flightspeed;

    float(*oCarPrice )(...);
    float CarPrice(void* obj, int i){
        if(obj != nullptr && price){
            return -1;
        }
        return oCarPrice(obj, i);
    }

    void(*oldCarDamage)(...);
    void CarDamage(void* obj){
        if(obj != nullptr && damage){
            *(bool*)((uint64_t) obj + 0x94) = false;
            *(bool*)((uint64_t) obj + 0x9C) = true;
            *(float*)((uint64_t) obj + 0x1C) = 0;
            *(float*)((uint64_t) obj + 0x20) = 0;
            *(float*)((uint64_t) obj + 0x3C) = 0;
            *(float*)((uint64_t) obj + 0x44) = 0;
        }
        oldCarDamage(obj);
    }

    void(*oldDrivetrain)(...);
    void Drivetrain(void* obj){
        if(obj != nullptr){
            if(hpcheck){
                *(float*)((uint64_t) obj + 0x8C) = horse;
                *(float*)((uint64_t) obj + 0x3C) = horse;
                *(float*)((uint64_t) obj + 0x5C) = horse;
                *(float*)((uint64_t) obj + 0x60) = horse;
                *(float*)((uint64_t) obj + 0x50) = horse;
                *(float*)((uint64_t) obj + 0x58) = horse;
                *(float*)((uint64_t) obj + 0x18C) = horse;
                *(float*)((uint64_t) obj + 0x184) = horse;
            }

            if(torquecheck){
                *(float*)((uint64_t) obj + 0x194) = torque;
                *(float*)((uint64_t) obj + 0x138) = torque;
                *(float*)((uint64_t) obj + 0x11C) = torque;
                *(float*)((uint64_t) obj + 0xDC) = torque;
                *(float*)((uint64_t) obj + 0x64) = torque;
                *(float*)((uint64_t) obj + 0x60) = torque;
            }

            if(masscheck)
                *(float*)((uint64_t) obj + 0x4C) = mass;

            if(noshift){
                *(bool*)((uint64_t) obj + 0x1BD) = true;
                *(bool*)((uint64_t) obj + 0x150) = true;
                *(float*)((uint64_t) obj + 0xD8) = 0;
                *(float*)((uint64_t) obj + 0x14C) = 0;
                *(float*)((uint64_t) obj + 0x154) = 0;
            }


            void* FuelTank = *(void**)((uint64_t) obj + 0x28);
            if(FuelTank != nullptr){
                if(fuel){
                    *(float*)((uint64_t) FuelTank + 0x4C) = MAXFLOAT;
                    *(float*)((uint64_t) FuelTank + 0x48) = MAXFLOAT;
                    *(float*)((uint64_t) FuelTank + 0x24) = MAXFLOAT;
                }

                if(masscheck){
                    *(float*)((uint64_t) FuelTank + 0x2C) = mass;
                }
            }
        }
        oldDrivetrain(obj);
    }

}
#endif HOOKS