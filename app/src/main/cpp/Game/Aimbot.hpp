namespace aimbot {

    int doAimbot = 0;
    void* closestEnt = nullptr;
    void (*oSetRotation)(void* character, Vector2 rotation);
    void hkSetRotation(void *character, Vector2 rotation) {
        void* mycam = pointers::Camera$$get_main();
        if(mycam == nullptr) return;
        Vector2 newAngle;
        Vector2 difference;
        difference.X = 0.f;
        difference.Y = 0.f;
        bool canSet = false;
        if (character != nullptr && pointers::localEntity.obj != nullptr && configs::aimbot::enabled) {
            closestEnt = helpers::getValidEntity(mycam, rotation);
        }

        if (pointers::localEntity.obj != nullptr &&
            helpers::IsPlayerAlive(pointers::localEntity.obj) && closestEnt != nullptr) {
            Vector3 localHead = helpers::getBonePosition(pointers::localEntity.obj, 10);
            if (localHead != Vector3(0, 0, 0) && helpers::IsCrouched(pointers::localEntity.obj)) {
                localHead = localHead - Vector3(0, 0.5, 0);
            }

            Vector3 enemyBone = helpers::getBonePosition(closestEnt, configs::aimbot::aimBone);
            Vector3 wsEnemyBone = helpers::WorldToScreenPoint(mycam, enemyBone);
            if(wsEnemyBone.Z > 0.f && configs::aimbot::redDot)
                ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(wsEnemyBone.X, wsEnemyBone.Y), 2.f, ImColor(1.f, 0.f, 0.f, 1.f), 360);

            Vector3 deltavec = enemyBone - localHead;
            float deltLength = sqrt(deltavec.X * deltavec.X + deltavec.Y * deltavec.Y +
                                    deltavec.Z * deltavec.Z);

            newAngle.X = -asin(deltavec.Y / deltLength) * (180.0 / M_PI);
            newAngle.Y = atan2(deltavec.X, deltavec.Z) * 180.0 / M_PI;

            if (configs::aimbot::enabled && character == pointers::localEntity.obj) {
                if (configs::aimbot::visCheck) {
                    if (helpers::isCharacterVisible(mycam, closestEnt)) {
                        canSet = true;
                    }
                }
                if (configs::aimbot::onShoot) {
                    if (helpers::isCharacterShooting(pointers::localEntity.obj)) {
                        if (configs::aimbot::visCheck && !canSet) {
                            oSetRotation(character, rotation);
                            return;
                        }
                        if (configs::aimbot::fovCheck) {
                            if (doAimbot % 6 == 0) {
                                if (helpers::isInFov(rotation, newAngle))
                                    difference = newAngle - rotation;
                                doAimbot = 1;
                            } else doAimbot += 1;
                        } else {
                            difference = newAngle - rotation;
                            configs::aimbot::fovValue = 360;
                        }
                    }
                } else {
                    if (configs::aimbot::fovCheck) {
                        if (doAimbot % 6 == 0) {
                            if (helpers::isInFov(rotation, newAngle))
                                difference = newAngle - rotation;
                            doAimbot = 1;
                        } else doAimbot += 1;
                    } else {
                        difference = newAngle - rotation;
                        configs::aimbot::fovValue = 360;
                    }
                }

                if (configs::aimbot::aimSmooth) {
                    if(configs::aimbot::visCheck && !canSet) {
                        oSetRotation(character, rotation);
                        return;
                    }
                    difference = difference / configs::aimbot::smoothAmount;
                }
            }
        }

        LOGI("NEW AIMBOT ROTATION: %f, %f", (rotation + difference).X, (rotation + difference).Y);
        oSetRotation(character, rotation + (configs::aimbot::silent ? Vector2(0.f, 0.f) : difference));
    }
    void init()
    {
        hook(offsets::character::setRotation, hkSetRotation, oSetRotation);
    }

}