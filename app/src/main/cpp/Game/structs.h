struct Ray {
    Vector3 origin;
    Vector3 direction;
};

struct TransformData
{
    Vector3 position;
    Vector3 velocity;
    Vector2 rotation;
};

enum BodyPart
{
    LOWERLEG_LEFT,
    LOWERLEG_RIGHT,
    UPPERLEG_LEFT,
    UPPERLEG_RIGHT,
    STOMACH,
    CHEST,
    UPPERARM_LEFT,
    UPPERARM_RIGHT,
    LOWERARM_LEFT,
    LOWERARM_RIGHT,
    HEAD,
    NECK    // its custom
};

struct HitSphere
{
    int pad_0[6];
    Vector3 m_center;
    float m_radius;
    void* m_placementData;
    void* m_transform;
    float m_radiusModifier;
};

struct CharacterBodyPart
{
    int pad_0[6];
    BodyPart m_bodyPart;
    HitSphere* m_hitSphere;
};

struct PlayerAdapter
{
    int pad_0[4];
    void* Player; // 0x10
};

struct HitData
{
    bool hitCharacter;
    bool traced;
    Vector3 hitWorldPos;
    int victimID;
    BodyPart hitBodyPart;
    Vector3 hitLocalPos;
    int hitAngle;
    Vector3 hitWorldNormal;
    int hitMaterialDef;
};

enum ChatMessageType
{
    PUBLIC_CHAT,
    TEAM_CHAT,
    RADIO,
    NOTIFICATION,
    LOCAL_NOTIFICATION,
    BROADCAST,
    FRIENDS,
    PARTY,
    CLAN
};

struct AimingData
{
    float aimSpread;
    float rawSpread;
    float rawRecoil;
    float recoilY;
    float recoilX;
    float inaccuracy;
};

struct StateData
{
    bool onGround;
    bool crouched;
    bool aiming;
    bool walking;
    bool spotted;
    bool reset;
};

struct InputFlags
{
    bool crouch;
    bool walk;
    bool jump;
};

struct InputData
{
    Vector2 moveInput;
    TransformData transform;
    InputFlags inputFlags;
    StateData state;
};

struct PlayerInput
{
    TransformData Transform;
    StateData State;
    Vector2 Input;
    int Tick;
};

struct MoveRequest
{
    PlayerInput playerInput;
};

enum WeaponType
{
    _Automatic,
    _SemiAutomatic,
    _Manual,
    _Grenade,
    _Bomb,
    _BurstFire
};

enum WeaponSlot
{
    _Primary,
    _Secondary,
    _Melee,
    _Grenade_1,
    _Utility,
    _Grenade_2,
    _Grenade_3
};

enum WeaponCategory
{
    Pistol,
    AssaultRifle,
    SubmachineGun,
    Shotgun,
    SniperRifle,
    Melee,
    Grenade,
    Utility
};


struct WeaponDefData
{
    int pad_0[4];
    monoString* Name;
    int ID;
    int pad_1[1];
    monoString* LocalizationKey;
    monoString* LocalizationGroup;
    WeaponType WeaponType;
    WeaponSlot WeaponSlot;
    WeaponCategory weaponCategory;
};

struct CharacterData
{
    int pad_0[4];
    AimingData aim;
    TransformData transform;
    Vector2 m_moveInput;
    StateData state;
    float m_aimPunchY;
    float m_aimPunchX;
    float slowDown;
    float m_speedFactor;
    int m_ticksRecovered;
    bool m_firing;
    uint8_t pad_1[3];
    void* m_weaponSettings;
    void* m_characterSettings;
    WeaponDefData* m_wpn;
    WeaponDefData* m_lastShotWpn;
    float m_armorMoveSpeedReductionRates;
};

struct Player
{
    int pad_0[8];
    int ID;
    int Ping;
    int RecentChatMessageCount;
    int64_t LastKillTime;
    int ShortTimeKills;
    int Rank;
    int NumConnects;
    int64_t LastDisconnectTime;
    bool Ready;
    uint8_t pad[3];
    int InvalidRequestCount;
    void* Weapons;
    void* CasualMMRs;
    void* VictoryWeapons;
};

struct Character
{
    int pad_0[6];
    int m_id;
    int pad_1;
    void* m_assets;
    void* m_game;
    void* m_inputSystem;
    void* m_gameSystem;
    void* m_collider;
    void* m_rigidbody;
    void* m_pivotX;
    void* m_pivoty;
    void* m_aimPivot;
    void* m_spine;
    void* m_transform;
    void* m_bodyParts;
    void* m_model;
    void* m_boundingSphere;
    Player* m_player;
    CharacterData* m_data;
    void* m_stepHeight;
    int pad_2;
    float m_crouchPhase;
    float m_lastSpotTime;
    int64_t m_invulnerabilityTime;
    int m_health;
    void* m_selectedWeapon;
    void* m_quickSwitchWeapon;
    bool m_initialized;
    uint8_t pad_3[7];
    void* m_weapons;
    void* m_characterArmors;
};

struct Weapon
{
    int pad_0[10];
    int m_ID;
    int m_skinID;
    void* m_carrier; // Old Character;
    void* m_shotTickData;
    WeaponDefData* m_weaponDefData;
    Vector3 m_position;
    Vector3 m_rotation;
    bool m_stop;
    int pad_1[3];
    Vector3 m_velocity;
    Vector3 m_angularVelocity;
};

enum WeaponDefId
{
    InvalidDefId = -1,
    Deagle = 7519,
    GSR1911 = 105,
    MR96 = 8,
    MTX = 14,
    P250 = 21,
    XD45 = 16,
    MP5 = 15,
    MP7 = 106,
    MPX = 6712,
    P90 = 20,
    Vector = 4009,
    AK47 = 4,
    AR15 = 13707,
    AUG = 19,
    HK417 = 9,
    M4 = 6,
    SA58 = 7,
    SG551 = 10,
    SCARH = 14680,
    FP6 = 17,
    Super90 = 18,
    Winchester = 6525,
    KSG = 15079,
    M14 = 5,
    SVD = 7073,
    TRG = 3078,
    URatio = 11,
    Knife = 103,
    Flashbang = 13,
    Frag = 22,
    SmokeGrenade = 12
};

struct Quaternion
{
    float x,y,z,w;
};

struct BurstData
{
    Ray currentRay;
    void* weaponEgid;
    int damagePercent;
    bool trace;
    float maxRange;
};

struct ShootData
{
    int pad_0[4];
    StateData stateData;
    uint8_t pad_1[2];
    TransformData transform;
    Vector2 moveInput;
    AimingData aimingBefore;
    uint8_t ticksRecovered;
    uint8_t pad_2[3];
    Vector3 aimPivot;
    uint8_t  m_rayCount;
};
