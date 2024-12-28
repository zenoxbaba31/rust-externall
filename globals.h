#pragma once
const std::string compilation_date = (std::string)skCrypt(__DATE__);
const std::string compilation_time = (std::string)skCrypt(__TIME__);


static inline char* memdup(const char* s, size_t n)
{
    char* t = (char*)malloc(n);
    memcpy(t, s, n);
    return t;
}
#define _memdup(object) memdup(object, sizeof(object))

using namespace KeyAuth;
std::string filecode; // Default color
std::string name; // Application Name
std::string ownerid; // Owner ID
std::string secret; // Application Secret
std::string version; // Application Version
std::string url; // change if you're self-hosting
api KeyAuthApp(name, ownerid, secret, version, url);

#define RGBA(r, g, b, a) ImVec4((r) / 255.f, (g) / 255.f, (b) / 255.f, (a) / 255.f)

enum features
{
    Spiderman,
    TimeChanger,
    FovChanger,
    NoRecoil,
    NoSpread,
    Automatic,
    SuperEoka,
    SuperMeele,
    AdminFlag,
    Aimbot,
    Silent,
    Aimline,
    AimFov,
    Crosshair,
    Predictions,
    HandChams,
    IgnoreVisible,
    IgnoreWounded,
    IgnoreSleeper,
    IgnoreNpc,

    NameEsp,
    PlayerInfo,
    PlayerEsp,
    Chams,
    CornerBox,
    NormalBox,
    FillBox,
    SkeletonEsp,
    ScientistEsp,
    ShowSleepers,
    TeamEsp,
    Wounded,
    StoneEsp,
    SulfurEsp,
    MetalEsp,
    HempEsp,
    MilitaryCrateEsp,
    DroppedItems,
    DroppedItemsImage,
    SupplyDropEsp,
    AutoTurretEsp,
    ShotgunTrapEsp,
    FlameTurretEsp,
    BearTrapEsp,
    LandMineEsp,
    SamSiteEsp,
    ToolCupboardEsp,
    CarEsp,
    BoatEsp,
    BoarEsp,
    BearEsp,
    ChickenEsp,
    StagEsp,
    HorseEsp,
    WolfEsp,
    NormalCrate,
    EliteCrate,
    PatrolHeli,
    MinicopterEsp,
    ShelterEsp,
    SmallBoxEsp,
    LargeBoxEsp,
    FurnaceEsp,
    MushroomEsp,
    CollectableStoneEsp,
    CollectableMetalEsp,
    CollectableSulfurEsp,
    CollectableWoodEsp,
    CornEsp,
    PumpkinEsp,
    LootBarrelEsp,
    CorpseEsp,
    OffScreenArrows,
};

namespace globals {
    RECT ScreenSize, ScreenMidSize;
    const int MaxFeatures = 100;
    bool showmenu = true;
    bool features[MaxFeatures];
    int keybind[MaxFeatures];
    int featuresint[MaxFeatures];
    float featuresfloat[MaxFeatures];
    ImVec4 EspColor[MaxFeatures];

    int hitboxtype = 0;
    const char* hitboxtypes[] = { _memdup(skCrypt("Head")), _memdup(skCrypt("Spine")), _memdup(skCrypt("Pelvis")) };

    int Configs;
    static const char* ConfigsC[] = {
        _memdup(skCrypt("Config 1")),
        _memdup(skCrypt("Config 2")),
        _memdup(skCrypt("Config 3")),
        _memdup(skCrypt("Config 4")),
        _memdup(skCrypt("Config 5")),
        _memdup(skCrypt("Config 6")),
        _memdup(skCrypt("Config 7")),
        _memdup(skCrypt("Config 8")),
        _memdup(skCrypt("Config 9")),
        _memdup(skCrypt("Config 10")) };

    static int bChamsmaterial = 2;
    unsigned int materials[] = { /*4209254*/4476236, /*162164*/164326, /*876350*/931524, /*2599726*/2747052, /*3435770*/3647078, /*1124958*/1195608, /*78674*/79928, /*1293024*/1370622, /*1086174*/1151512, /*850144*/898510, /*395976*/425420, /*2023020*/2144238 };
    static const char* bChamsmaterialItems[] = {
               _memdup(skCrypt("Chromatic")),
               _memdup(skCrypt("Gold")),
               _memdup(skCrypt("Flash")),
               _memdup(skCrypt("White (Wall)")),
               _memdup(skCrypt("Gray (Wall)")),
               _memdup(skCrypt("Blue")),
               _memdup(skCrypt("Green")),
               _memdup(skCrypt("Orange")),
               _memdup(skCrypt("Alarm")),
               _memdup(skCrypt("Bright Green")),
               _memdup(skCrypt("Bright Red")),
               _memdup(skCrypt("Bright White"))
    };

    //static int Handmaterialsa = 2;
    //unsigned int Handmaterials[] = { /*4209254*/4476236, /*162164*/164326, /*876350*/931524, /*2599726*/2747052, /*3435770*/3647078, /*1124958*/1195608, /*78674*/79928, /*1293024*/1370622, /*1086174*/1151512, /*850144*/898510, /*395976*/425420, /*2023020*/2144238 };
    //static const char* HandChamsmaterialItems[] = {
    //           _memdup(skCrypt("Chromatic")),
    //           _memdup(skCrypt("Gold")),
    //           _memdup(skCrypt("Flash")),
    //           _memdup(skCrypt("White (Wall)")),
    //           _memdup(skCrypt("Gray (Wall)")),
    //           _memdup(skCrypt("Blue")),
    //           _memdup(skCrypt("Green")),
    //           _memdup(skCrypt("Orange")),
    //           _memdup(skCrypt("Alarm")),
    //           _memdup(skCrypt("Bright Green")),
    //           _memdup(skCrypt("Bright Red")),
    //           _memdup(skCrypt("Bright White"))
    //};
}

struct Belt
{
    std::string Name;
    int Count;
    int WeaponAmmo;
    float Health;
    float MaxHealth;
};

struct Cloth
{
    std::string Name;
    float Health;
    float MaxHealth;
};

struct sPrefabList
{
    std::string PrefabName;
    features prefab;
    uintptr_t PosAgo;
};

enum class PlayerFlags
{
    Unused1 = 1,
    Unused2 = 2,
    IsAdmin = 4,
    ReceivingSnapshot = 8,
    Sleeping = 16,
    Spectating = 32,
    Wounded = 64,
    IsDeveloper = 128,
    Connected = 256,
    ThirdPersonViewmode = 1024,
    EyesViewmode = 2048,
    ChatMute = 4096,
    NoSprint = 8192,
    Aiming = 16384,
    DisplaySash = 32768,
    Relaxed = 65536,
    SafeZone = 131072,
    ServerFall = 262144,
    Incapacitated = 524288,
    Workbench1 = 1048576,
    Workbench2 = 2097152,
    Workbench3 = 4194304
};

std::unordered_map<std::string, sPrefabList> prefabMap
{
    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/resource/ores/stone-ore.prefab"), {(std::string)skCrypt("Stone Ore"), StoneEsp, 0 }},
    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/resource/ores_sand/stone-ore.prefab"), {(std::string)skCrypt("Stone Ore"), StoneEsp, 0 }},
    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/resource/ores_snow/stone-ore.prefab"), {(std::string)skCrypt("Stone Ore"), StoneEsp, 0 }},

    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/resource/ores/sulfur-ore.prefab"), {(std::string)skCrypt("Sulfur Ore"), SulfurEsp, 0 }},
    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/resource/ores_sand/sulfur-ore.prefab"), {(std::string)skCrypt("Sulfur Ore"), SulfurEsp, 0 }},
    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/resource/ores_snow/sulfur-ore.prefab"), {(std::string)skCrypt("Sulfur Ore"), SulfurEsp, 0 }},

    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/resource/ores/metal-ore.prefab"), {(std::string)skCrypt("Metal Ore"), MetalEsp, 0 }},
    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/resource/ores_sand/metal-ore.prefab"), {(std::string)skCrypt("Metal Ore"), MetalEsp, 0 }},
    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/resource/ores_snow/metal-ore.prefab"), {(std::string)skCrypt("Metal Ore"), MetalEsp, 0 }},

    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/collectable/hemp/hemp-collectable.prefab"), {(std::string)skCrypt("Hemp"), HempEsp, 0 }},

    {(std::string)skCrypt("assets/bundled/prefabs/radtown/crate_normal.prefab"), {(std::string)skCrypt("Military Chest"), MilitaryCrateEsp, 0 }},

    {(std::string)skCrypt("assets/prefabs/misc/supply drop/supply_drop.prefab"), {(std::string)skCrypt("Supply Drop"), SupplyDropEsp, 0 }},

    {(std::string)skCrypt("assets/prefabs/npc/autoturret/autoturret_deployed.prefab"), {(std::string)skCrypt("Auto Turret"), AutoTurretEsp, 0 }},
        {(std::string)skCrypt("assets/prefabs/deployable/single shot trap/guntrap.deployed.prefab"), {(std::string)skCrypt("ShotGun Trap"), ShotgunTrapEsp, 0 }},
    {(std::string)skCrypt("assets/prefabs/npc/flame turret/flameturret.deployed.prefab"), {(std::string)skCrypt("Flame Turret"), FlameTurretEsp, 0 }},
    {(std::string)skCrypt("assets/prefabs/deployable/bear trap/beartrap.prefab"), {(std::string)skCrypt("Bear Trap"), BearTrapEsp, 0 }},
    {(std::string)skCrypt("assets/prefabs/deployable/landmine/landmine.prefab"), {(std::string)skCrypt("Landmine"), LandMineEsp, 0 }},
    {(std::string)skCrypt("assets/prefabs/npc/sam_site_turret/sam_site_turret_deployed.prefab"), {(std::string)skCrypt("Sam Site"), SamSiteEsp, 0 }},

    {(std::string)skCrypt("assets/prefabs/deployable/tool cupboard/cupboard.tool.deployed.prefab"), {(std::string)skCrypt("Tool Cupboard"), ToolCupboardEsp, 0 }},

       {(std::string)skCrypt("assets/content/vehicles/modularcar/2module_car_spawned.entity.prefab"), {(std::string)skCrypt("2 Slot Car"), CarEsp, 0 }},
    {(std::string)skCrypt("assets/content/vehicles/modularcar/3module_car_spawned.entity.prefab"), {(std::string)skCrypt("3 Slot Car"), CarEsp, 0 }},
    {(std::string)skCrypt("assets/content/vehicles/modularcar/4module_car_spawned.entity.prefab"), {(std::string)skCrypt("4 Slot Car"), CarEsp, 0 }},
    {(std::string)skCrypt("assets/content/vehicles/boats/rowboat/rowboat.prefab"), {(std::string)skCrypt("Boat"), BoatEsp, 0 }},

    {(std::string)skCrypt("assets/rust.ai/agents/boar/boar.prefab"), {(std::string)skCrypt("Boar"), BoarEsp, 0 }},
    {(std::string)skCrypt("assets/rust.ai/agents/bear/bear.prefab"), {(std::string)skCrypt("Bear"), BearEsp, 0 }},
    {(std::string)skCrypt("assets/rust.ai/agents/chicken/chicken.prefab"), {(std::string)skCrypt("Chicken"), ChickenEsp, 0 }},
    {(std::string)skCrypt("assets/rust.ai/agents/stag/stag.prefab"), {(std::string)skCrypt("Stag"), StagEsp, 0 }},
    {(std::string)skCrypt("assets/rust.ai/agents/bear/polarbear.prefab"), {(std::string)skCrypt("Bear"), BearEsp, 0 }},
    {(std::string)skCrypt("assets/rust.ai/agents/wolf/wolf.prefab"), {(std::string)skCrypt("Wolf"), WolfEsp, 0 }},
    {(std::string)skCrypt("assets/rust.ai/agents/horse/horse.prefab"), {(std::string)skCrypt("Horse"), HorseEsp, 0 }},

    {(std::string)skCrypt("assets/bundled/prefabs/radtown/crate_normal_2.prefab"), {(std::string)skCrypt("Normal Crate"), NormalCrate, 0 }},
    {(std::string)skCrypt("assets/bundled/prefabs/radtown/crate_elite.prefab"), {(std::string)skCrypt("Elite Crate"), EliteCrate, 0 }},

    {(std::string)skCrypt("assets/prefabs/npc/patrol helicopter/patrolhelicopter.prefab"), {(std::string)skCrypt("Patrol Helicopter"), PatrolHeli, 0 }},
    {(std::string)skCrypt("assets/content/vehicles/minicopter/minicopter.entity.prefab"), {(std::string)skCrypt("Minicopter"), MinicopterEsp, 0 }},

    {(std::string)skCrypt("assets/prefabs/building/legacy.shelter.wood/legacy.shelter.wood.deployed.prefab"), {(std::string)skCrypt("Shelter"), ShelterEsp, 0 }},
    {(std::string)skCrypt("assets/prefabs/deployable/woodenbox/woodbox_deployed.prefab"), {(std::string)skCrypt("Small Box"), SmallBoxEsp, 0 }},
    {(std::string)skCrypt("assets/prefabs/deployable/large wood storage/box.wooden.large.prefab"), {(std::string)skCrypt("Large Box"), LargeBoxEsp, 0 }},
    {(std::string)skCrypt("assets/prefabs/deployable/furnace/furnace.prefab"), {(std::string)skCrypt("Furnace"), FurnaceEsp, 0 }},

    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/collectable/mushrooms/mushroom-cluster-5.prefab"), {(std::string)skCrypt("Mushroom"), MushroomEsp, 0 }},
    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/collectable/mushrooms/mushroom-cluster-6.prefab"), {(std::string)skCrypt("Mushroom"), MushroomEsp, 0 }},

    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/collectable/stone/stone-collectable.prefab"), {(std::string)skCrypt("Stone"), CollectableStoneEsp, 0 }},
    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/collectable/stone/sulfur-collectable.prefab"), {(std::string)skCrypt("Sulfur"), CollectableSulfurEsp, 0 }},
    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/collectable/stone/metal-collectable.prefab"), {(std::string)skCrypt("Metal"), CollectableMetalEsp, 0 }},
    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/collectable/wood/wood-collectable.prefab"), {(std::string)skCrypt("Wood"), CollectableWoodEsp, 0 }},

    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/collectable/corn/corn-collectable.prefab"), {(std::string)skCrypt("Corn"), CornEsp, 0 }},
    {(std::string)skCrypt("assets/bundled/prefabs/autospawn/collectable/pumpkin/pumpkin-collectable.prefab"), {(std::string)skCrypt("Pumpkin"), PumpkinEsp, 0 }},

    {(std::string)skCrypt("assets/bundled/prefabs/radtown/loot_barrel_1.prefab"), {(std::string)skCrypt("Loot Barrel"), LootBarrelEsp, 0 }},
    {(std::string)skCrypt("assets/bundled/prefabs/radtown/loot_barrel_2.prefab"), {(std::string)skCrypt("Loot Barrel"), LootBarrelEsp, 0 }},

    {(std::string)skCrypt("assets/prefabs/player/player_corpse.prefab"), {(std::string)skCrypt("Corpse"), CorpseEsp, 0 }},
    {(std::string)skCrypt("assets/prefabs/misc/item drop/item_drop_backpack.prefab"), {(std::string)skCrypt("Backpack"), CorpseEsp, 0 }},
};

bool GetPrefabNames(std::string GameBuff)
{
    if (prefabMap.find(GameBuff) != prefabMap.end()) {
        if (globals::features[prefabMap[GameBuff].prefab]) {
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}
