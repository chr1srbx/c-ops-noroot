namespace helpers {
        struct PlayerAdapter {
            int pad_0[3];
            void *Player; // 0x10
        };

        bool IsInitialized(void *character) {
            return pointers::Character$$get_IsInitialized(character);
        }

        bool IsCrouched(void *character) {
            return pointers::Character$$get_IsCrouched(character);
        }

        Vector3 WorldToScreenPoint(void *cam, Vector3 pos) {
            if (!cam) return Vector3();
            return pointers::Camera$$WorldToScreenPoint(cam, pos, 2);
        }

        Vector3 GetPosition(void *transform) {
            if (!transform) return Vector3();
            Vector3 pos;
            pointers::Transform$$get_position_Injected(transform, pos);
            return pos;
        }

        Vector3 GetPlayerLocation(void *character) {
            return GetPosition(pointers::Component$$get_transform(character));
        }

        int GetPlayerHealth(void *character) {
            if (!character) return 0;
            return pointers::Character$$get_Health(character);
        }

        Vector3 getBonePosition(void *character, int bone) {
            void *hitSphere = NULL;
            void *transform = NULL;
            if (character != nullptr) {
                void *curBone = pointers::Character$$GetBodyPart(character, bone);
                if (curBone != nullptr) { hitSphere = *(void **) ((uint64_t) curBone + 0x20); }
                if (hitSphere != nullptr) { transform = *(void **) ((uint64_t) hitSphere + 0x30); }
                if (transform != nullptr) {
                    return GetPosition(transform);
                }
                return Vector3(0, 0, 0);
            }
            return Vector3(0, 0, 0);
        }

        bool IsPlayerAlive(void *character) {
            return character != nullptr && GetPlayerHealth(character) > 0;
        }

        bool IsPlayerDead(void *character) {
            return character != nullptr && GetPlayerHealth(character) < 1;
        }

        void *getPlayer(void *character) { return *(void **) ((uint64_t) character + 0x90); }

        PlayerAdapter CreatePlayerAdapter(void* character) {
            void* player = getPlayer(character);
            PlayerAdapter adapter;
            pointers::PlayerAdapter$$ctor(&adapter, player);
            return adapter;
        }

        int getTeamIndex(PlayerAdapter adapter) {
            return pointers::PlayerAdapter$$getTeamIndex(&adapter);
        }

        std::string getUsername(PlayerAdapter adapter) {
            return pointers::PlayerAdapter$$getUserName(&adapter)->getString();
        }

        bool isCharacterVisible(void *cam, void *character) {
            if (cam == nullptr || character == nullptr) return false;
            void *transform = pointers::Component$$get_transform(cam);
            void *transform2 = pointers::Component$$get_transform(character);
            if (transform == nullptr || transform2 == nullptr) return false;
            Vector3 position = GetPosition(transform);
            Vector3 position2 = GetPosition(transform2);
            return !pointers::Physics$$Linecast(position, position2, 33792, 0);
        }

        bool isCharacterShooting(void *character) {
            void *characterData = *(void **) ((uint64_t) character + 0x98);
            if (characterData) {
                return *(bool *) ((uint64_t) characterData + 0x6C);
            }
            return 0;
        }

        bool isInFov(Vector2 rotation, Vector2 newAngle) {
            if (!configs::aimbot::fovCheck) return true;

            Vector2 difference = newAngle - rotation;
            const auto fov = hypot(difference.X, difference.Y);
            if (fov > configs::aimbot::fovValue)
                return false;
            return true;
        }

        void *getValidEntity(void* cam, Vector2 rotation = Vector2()) {
            void *localPlayer = pointers::localEntity.obj;
            if (localPlayer == nullptr) return nullptr;
            if (!pointers::Character$$get_IsInitialized(localPlayer)) return nullptr;
            int localTeam = pointers::localEntity.teamIndex;
            float closestEntDist = 99999.0f;
            void *closestCharacter = nullptr;

            for (auto character : manager->get()) {
                void *currentEnemy;
                currentEnemy = character.obj;
                int curTeam = character.teamIndex;
                int health = helpers::GetPlayerHealth(currentEnemy);

                bool canSet = false;
                Vector2 newAngle;

                if (configs::aimbot::enabled) {
                    if (IsPlayerAlive(localPlayer)) {
                        Vector3 localHead = getBonePosition(localPlayer, 10);
                        if (helpers::IsCrouched(localPlayer)) {
                            localHead = localHead - Vector3(0, 0.5, 0);
                        }
                        Vector3 enemyBone = getBonePosition(currentEnemy, configs::aimbot::aimBone);
                        Vector3 deltavec = enemyBone - localHead;
                        float deltLength = sqrt(deltavec.X * deltavec.X + deltavec.Y * deltavec.Y +
                                                deltavec.Z * deltavec.Z);

                        newAngle.X = -asin(deltavec.Y / deltLength) * (180.0 / M_PI);
                        newAngle.Y = atan2(deltavec.X, deltavec.Z) * 180.0 / M_PI;
                        if (isInFov(rotation, newAngle)) {
                            if (localTeam != curTeam && curTeam != -1) {
                                if (!configs::aimbot::visCheck && currentEnemy &&
                                    isCharacterVisible(cam, currentEnemy))
                                    return currentEnemy;

                                if (configs::aimbot::visCheck) {
                                    if (currentEnemy && isCharacterVisible(cam, currentEnemy)) {
                                        canSet = true;
                                    }
                                }

                                void *transform = pointers::Component$$get_transform(currentEnemy);
                                if (transform != nullptr) {
                                    Vector3 localPosition = GetPosition(transform);
                                    Vector3 currentCharacterPosition = GetPosition(pointers::Component$$get_transform(localPlayer));
                                    Vector3 currentEntDist = Vector3::Distance(localPosition, currentCharacterPosition);

                                    if (Vector3::Magnitude(currentEntDist) < closestEntDist) {
                                        if (configs::aimbot::visCheck && !canSet) continue;
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