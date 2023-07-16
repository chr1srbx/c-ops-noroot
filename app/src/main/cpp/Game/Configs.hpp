    namespace configs {

        namespace esp {
            ImFont* smallerFont;
            bool enabled = false;
            bool usernames = false;
            bool linesTop = false;
            bool linesBottom = false;
            bool boxes = false;
            bool outlined_boxes = false;
            bool corner_boxes = false;
            bool filled_boxes = false;
            bool healthbar = false;
            bool skeletons = false;
            float color[4] = {1.f, 1.f, 1.f, 1.f};
        }

        namespace aimbot {
            bool enabled = false;
            bool silent = false;
            bool fovCheck = false;
            float fovValue = 360.f;
            bool fovCircle = false;
            bool visCheck = false;
            bool redDot = false;
            int aimBone = 10; // HEAD
            bool onShoot = false;
            bool aimSmooth = false;
            float smoothAmount = 0.f;
        }

        namespace chams {
            bool enabled = false;
            bool shading = false;
            bool wireframe = false;
            bool outline = false;
            bool glow = false;
            bool rainbow = false;
            float color[4] = {1.f, 1.f, 1.f, 1.f};
            float width = 1.f;
        }

        namespace weapon {
            bool alwaysCrosshair = false;
            bool noSpread = false;
            bool noRecoil = false;
            bool fireRateActive = false;
            float fireRate = 0.f;
            bool wallShot = false;
            bool headRain = false;
            int hr_old_val = 0.f;
            bool hr_changed = false;
            bool noAimpunch = false;
            bool noDryfire = false;
            bool magicMelee = false;
            bool instantEquipGun = false;
        }

        namespace visuals {
            bool specradar = false;
            bool radar = false;
            bool teamRadar = false;
            bool widePutin = false;
            float putinX = 0.f;
            float putinY = 0.f;
            float putinZ = 0.f;
            bool noFlash = false;
            bool noSmoke = false;
            bool noSniperBlur = false;
            bool viewModel = false;
            float viewModelX = 0.f;
            float viewModelY = 0.f;
            float viewModelZ = 0.f;
        }

        namespace miscellaneous {
            bool speed = false;
            bool noclip = false;
            float speedValue = 20.f;
            bool bombTimer = false;
            bool jumpHeight = false;
            float jumpVal = 0.5f;
            bool botsNoShoot = false;
            bool fly = false;
            float flyVal = 0.f;
            bool skipTut = false;
        }

        namespace skin_changer {
            int default_glove_ct_id = 6714;
            int old_glove_ct_id = 6714;
            int glove_ct_id = 6714;
            int default_glove_t_id = 7093;
            int old_glove_t_id = 7093;
            int glove_t_id = 7093;
            int default_knife_id = 8039;
            int knife_id = 8039;
            int default_xd45_id = 8052;
            int xd45_id = 8052;
            int default_dualmtx_id = 8044;
            int dualmtx_id = 8044;
            int default_gsr1911_id = 4521;
            int gsr1911_id = 4521;
            int default_mr96_id = 8043;
            int mr96_id = 8043;
            int default_deagle_id = 7501;
            int deagle_id = 7501;
            int default_mpx_id = 7029;
            int mpx_id = 7029;
            int default_mp5_id = 8041;
            int mp5_id = 8041;
            int default_mp7_id = 8042;
            int mp7_id = 8042;
            int default_p90_id = 8045;
            int p90_id = 8045;
            int default_vector_id = 6300;
            int vector_id = 6300;
            int default_fp6_id = 4520;
            int fp6_id = 4520;
            int default_m1887_id = 7110;
            int m1887_id = 7110;
            int default_super90_id = 8049;
            int super90_id = 8049;
            int default_ksg_id = 9594;
            int ksg_id = 9594;
            int default_sa58_id = 8047;
            int sa58_id = 8047;
            int default_m4_id = 8040;
            int m4_id = 8040;
            int default_ar15_id = 8985;
            int ar15_id = 8985;
            int default_ak47_id = 4518;
            int ak47_id = 4518;
            int default_hk417_id = 8038;
            int hk417_id = 8038;
            int default_sg551_id = 8048;
            int sg551_id = 8048;
            int default_aug_id = 4519;
            int aug_id = 4519;
            int default_scarh_id = 9453;
            int scarh_id = 9453;
            int default_trg22_id = 8050;
            int trg22_id = 8050;
            int default_m14_id = 4525;
            int m14_id = 4525;
            int default_svd_id = 7174;
            int svd_id = 7174;
            int default_uratio_id = 8051;
            int uratio_id = 8051;
        }

    }