#include "../Data/Fonts/Minecraftia.h"

namespace esp {

    void initFont()
    {
        ImGuiIO &io = ImGui::GetIO();
        configs::esp::smallerFont = io.Fonts->AddFontFromMemoryTTF(Minecraftia, 18, 18.f);
    }

    void run()
    {
        if(!configs::esp::enabled) return;

        void* mycam = pointers::Camera$$get_main();
        if(mycam == nullptr) return;
        if(pointers::game == nullptr) return;

        if(manager->get().size() == 0) return;

        for(auto character : manager->get()) {
            if(character.obj != nullptr &&
                pointers::localEntity.obj != nullptr &&
                pointers::localEntity.teamIndex != character.teamIndex &&
                helpers::IsPlayerAlive(character.obj) &&
                helpers::IsInitialized(character.obj)
            ) {
                if(configs::visuals::widePutin)
                {
                    Vector3 vector(configs::visuals::putinX, configs::visuals::putinY, configs::visuals::putinZ);
                    pointers::Transform$$set_localScale(
                        pointers::Component$$get_transform(character.obj),
                        vector
                    );
                }
                Vector3 position = helpers::GetPlayerLocation(character.obj);
                Vector3 transformPos = helpers::WorldToScreenPoint(mycam, position);
                if (transformPos.Z < 0.f) continue;
                transformPos.Y = glHeight - transformPos.Y;

                Vector3 headPos = helpers::getBonePosition(character.obj, 10);
                Vector3 wsheadPos = helpers::WorldToScreenPoint(mycam, headPos);
                if (wsheadPos.Z < 0.f) continue;

                Vector3 aboveHead = headPos + Vector3(0, 0.2, 0);
                Vector3 headEstimate = position + Vector3(0, 1.48, 0);
                Vector3 wsAboveHead = helpers::WorldToScreenPoint(mycam, aboveHead);
                Vector3 wsheadEstimate = helpers::WorldToScreenPoint(mycam, headEstimate);
                if (wsAboveHead.Z < 0.f || wsheadEstimate.Z < 0.f) continue;

                wsAboveHead.Y = glHeight - wsAboveHead.Y;
                wsheadEstimate.Y = glHeight - wsheadEstimate.Y;

                float height = transformPos.Y - wsAboveHead.Y;
                float horizontalDist = abs(wsAboveHead.X - transformPos.X);
                float width = ((transformPos.Y - wsheadEstimate.Y) / 2) + horizontalDist;

                if (configs::esp::outlined_boxes)
                    DrawOutlinedBox2(wsAboveHead.X - width / 2, wsAboveHead.Y, width, height,
                                     ImVec4(configs::esp::color[0], configs::esp::color[1], configs::esp::color[2],
                                            configs::esp::color[3]),
                                     3, ImGui::GetBackgroundDrawList());

                if (configs::esp::linesTop)
                    DrawLine(ImVec2(glWidth / 2, 0.f), ImVec2(transformPos.X, transformPos.Y),
                             ImColor(configs::esp::color[0], configs::esp::color[1], configs::esp::color[2],
                                     configs::esp::color[3]), 3, ImGui::GetBackgroundDrawList());

                if (configs::esp::linesBottom)
                    DrawLine(ImVec2(glWidth / 2, glHeight), ImVec2(transformPos.X, transformPos.Y),
                             ImColor(configs::esp::color[0], configs::esp::color[1], configs::esp::color[2],
                                     configs::esp::color[3]), 3, ImGui::GetBackgroundDrawList());

                if (configs::esp::usernames && helpers::getPlayer(character.obj) != nullptr) {
                    std::string username = character.username;
                    float compensation = strlen(username.c_str()) * 4.0f;
                    ImGui::PushFont(configs::esp::smallerFont);
                    DrawText(ImVec2(wsheadPos.X - compensation, wsAboveHead.Y - 20),
                             ImColor(configs::esp::color[0], configs::esp::color[1], configs::esp::color[2],
                                     configs::esp::color[3]), username, configs::esp::smallerFont,
                             ImGui::GetBackgroundDrawList());
                    ImGui::PopFont();
                }
                if(configs::esp::healthbar) {
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

}