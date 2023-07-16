namespace silent_aim {

    struct Ray {
        Vector3 m_Origin; // 0x0
        Vector3 m_Direction; // 0xC
    };

    void* (*oTraceShot)(...);
    void* closestEnt = nullptr;
    void* hkTraceShot(void* instance, void* shooter, Ray shootRay)
    {
        void* mycam = pointers::Camera$$get_main();
        if(mycam == nullptr) return oTraceShot(instance, shooter, shootRay);
        Vector2 rotation = pointers::Transform$$get_rotation(pointers::Component$$get_transform(mycam));
        if(rotation == Vector2(0.f, 0.f)) return oTraceShot(instance, shooter, shootRay);
        Vector2 newAngle;
        Vector2 difference;
        difference.X = 0.f;
        difference.Y = 0.f;
        Vector3 enemyBone = Vector3(0.f, 0.f, 0.f);
        bool canSet = false;
        if (shooter != nullptr && pointers::localEntity.obj != nullptr && configs::aimbot::silent) {
            closestEnt = helpers::getValidEntity(mycam, rotation);
        }

        if (pointers::localEntity.obj != nullptr &&
            helpers::IsPlayerAlive(pointers::localEntity.obj) && closestEnt != nullptr) {
            Vector3 localHead = helpers::getBonePosition(pointers::localEntity.obj, 10);
            if (localHead != Vector3(0, 0, 0) && helpers::IsCrouched(pointers::localEntity.obj)) {
                localHead = localHead - Vector3(0, 0.5, 0);
            }

            enemyBone = helpers::getBonePosition(closestEnt, configs::aimbot::aimBone);
            Vector3 wsEnemyBone = helpers::WorldToScreenPoint(mycam, enemyBone);
            if(wsEnemyBone.Z > 0.f && configs::aimbot::redDot)
                ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(wsEnemyBone.X, wsEnemyBone.Y), 2.f, ImColor(1.f, 0.f, 0.f, 1.f), 360);

            Vector3 deltavec = enemyBone - localHead;
            float deltLength = sqrt(deltavec.X * deltavec.X + deltavec.Y * deltavec.Y +
                                    deltavec.Z * deltavec.Z);

            newAngle.X = -asin(deltavec.Y / deltLength) * (180.0 / M_PI);
            newAngle.Y = atan2(deltavec.X, deltavec.Z) * 180.0 / M_PI;

            if (configs::aimbot::enabled && shooter == pointers::localEntity.obj) {
                if (configs::aimbot::visCheck) {
                    if (helpers::isCharacterVisible(mycam, closestEnt)) {
                        canSet = true;
                    }
                }
                if (configs::aimbot::onShoot) {
                    if (helpers::isCharacterShooting(pointers::localEntity.obj)) {
                        if (configs::aimbot::visCheck && !canSet) {
                            return oTraceShot(instance, shooter, shootRay);
                        }
                        if (configs::aimbot::fovCheck) {
                                if (!helpers::isInFov(rotation, newAngle))
                                    enemyBone = Vector3(0.f, 0.f, 0.f);
                        } else {
                            configs::aimbot::fovValue = 360;
                        }
                    }
                } else {
                    if (configs::aimbot::fovCheck) {
                        if (!helpers::isInFov(rotation, newAngle))
                            enemyBone = Vector3(0.f, 0.f, 0.f);
                    } else {
                        configs::aimbot::fovValue = 360;
                    }
                }
            }
        }
        if(enemyBone != Vector3(0.f, 0.f, 0.f))
        {
            shootRay.m_Direction = enemyBone - shootRay.m_Origin;
        }
        return oTraceShot(instance, shooter, shootRay);
    }

    void init() {
        hook("0x1069854", hkTraceShot, oTraceShot); // credits to icemods
    }

}