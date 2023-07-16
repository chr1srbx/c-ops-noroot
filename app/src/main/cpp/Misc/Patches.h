#ifndef PATCHES
#define PATCHES

namespace Patch{
    void Noclip(){
         if(Hooks::noclip){
                 PATCH("0xBD4C4C", "00F020E3");
                 PATCH("0xBD4568", "00F020E3");
         }
    }
}

#endif
