#ifndef MENU
#define MENU

#include "Misc/ESP.h"
namespace Menu
{
    char text_buffer[256] = "";
    bool authenticated = false, addmoney;
    float amount;
    ImVec4 color = ImVec4(1, 1, 1, 1);
    void DrawMenu()
    {
        static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        {
            ImGui::Begin(OBFUSCATE("c-ops"));
            if (ImGui::CollapsingHeader(OBFUSCATE("Car Mods"))) {
            
            }
            ImGui::End();
        }
    }

}
#endif MENU