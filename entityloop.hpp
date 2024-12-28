#pragma once

std::string read_string(std::uintptr_t Address)
{
    char buffer[128];
    readsize(Address, (std::uint8_t*)&buffer, sizeof(buffer));
    return std::string(buffer, strlen(buffer));
};

void GetComponentsInChildren(std::uintptr_t game_object, std::vector<std::uintptr_t>& renderers, int depth = 0)
{
    const auto component_list = read<std::uint64_t>(game_object + 0x30);
    if (!component_list) return;

    const auto component_size = read<int>(game_object + 0x40);
    if (!component_size) return;

    std::string skinnedMeshRendererStr = (std::string)skCrypt("SkinnedMeshRenderer");
    std::string meshRendererStr = (std::string)skCrypt("MeshRenderer");
    std::string transformStr = (std::string)skCrypt("Transform");
    std::string holosightStr = (std::string)skCrypt("holosight");
    std::string simpleStr = (std::string)skCrypt("simple");
    std::string handStr = (std::string)skCrypt("hand");
    std::string newGameObjectStr = (std::string)skCrypt("New Game Object");


    for (int idx = 0; idx < component_size; ++idx)
    {
        const auto component = read<std::uint64_t>(component_list + (0x10 * idx + 0x8));
        if (!component) continue;

        const auto component_ptr = read<std::uint64_t>(component + 0x28);
        if (!component_ptr) continue;

        const auto component_name_ptr = read<std::uint64_t>(component_ptr + 0x0);
        if (!component_name_ptr) continue;

        const auto component_name = read<std::uint64_t>(component_name_ptr + 0x10);
        if (!component_name) continue;

        auto name = read_string(component_name);
        std::string_view name_view = name;

        if (name_view == skinnedMeshRendererStr || name_view == meshRendererStr)
            renderers.push_back(component);

        if (name_view == transformStr)
        {
            const auto child_list = read<std::uint64_t>(component + 0x70);
            if (!child_list) continue;

            const auto child_size = read<int>(component + 0x80);
            if (!child_size) continue;

            for (int i = 0; i < child_size; ++i)
            {
                const auto child_transform = read<std::uint64_t>(child_list + (0x8 * i));
                if (!child_transform) continue;

                const auto child_game_object = read<std::uint64_t>(child_transform + 0x30);
                if (!child_game_object) continue;

                const auto child_object_name = read<std::uint64_t>(child_game_object + 0x60);
                if (!child_object_name) continue;

                auto child_name = read_string(child_object_name);
                std::string_view child_name_view = child_name;

                if (child_name_view.find(holosightStr) != std::string::npos || child_name_view.find(simpleStr) != std::string::npos)
                    continue;

                GetComponentsInChildren(child_game_object, renderers, depth + 1);
            }
        }
    }
}
struct dynamic_array
{
    std::uint64_t base;
    std::uint64_t mem_id;
    std::uint64_t sz;
    std::uint64_t cap;
};
void ProcessSkinnedMeshRenderer(uintptr_t renderer, uintptr_t material)
{
    if (!renderer) return;

    for (std::uint32_t idx = 0; idx < 2; idx++)
    {
        uintptr_t renderEntry = read<uintptr_t>(renderer + 0x20 + (idx * 0x8));
        if (!renderEntry) continue;

        uintptr_t unity_object = read<uintptr_t>(renderEntry + 0x10);
        if (!unity_object) continue;

        dynamic_array mat_list = read<dynamic_array>(unity_object + 0x148);

        if (mat_list.sz < 1 || mat_list.sz > 5) continue;

        for (std::uint32_t mat_idx = 0; mat_idx < mat_list.sz; mat_idx++)
        {
            if (read<unsigned int>(mat_list.base + (mat_idx * 0x4)) != material)
                write<unsigned int>(mat_list.base + (mat_idx * 0x4), material);
        }
    }
}

void EntityLoop() {
    SleepEx(1000, false);
    for (;;) {
        SleepEx(200, false);
        BaseNetworkable* Entity(BaseNetworkable::GetEntity());
        if (!Entity) {
            RefreshDirbase();
            continue;
        }

        Graphicfov = GraphicFov::GetGraphicFov();
        if (!Graphicfov) {
            continue;
        }

        Todsky = TodSky::GetSky();
        if (!Todsky) {
            continue;
        }

        CameraInstance = MainCamera::GetCameraInstance();
        if (!CameraInstance) {
            continue;
        }

        BaseNetworkable* EntityArray = Entity->GetArray();
        if (!EntityArray) {
            continue;
        }

        Local_Player = EntityArray->GetLocalPlayer();
        if (!Local_Player) {
            continue;
        }
        //if (globals::features[HandChams]) {
        //  
        //    unsigned int materiala = 0;
        //    int bChamsmaterial = globals::Handmaterialsa;
        //    if (bChamsmaterial >= 0 && bChamsmaterial <= 11) {
        //        materiala = globals::Handmaterials[bChamsmaterial];
        //    }

        //    BasePlayer* GetHeldItem = Local_Player->GetHeldItem();
        //    if (!GetHeldItem) {
        //        continue;
        //    }

        //    uintptr_t BaseProjectile = (uintptr_t)GetHeldItem->GetBaseProjectile();
        //    if (!BaseProjectile) {
        //        continue;
        //    }

        //    uintptr_t viewModel = read<uintptr_t>(BaseProjectile + 0x208);
        //    if (!viewModel) {
        //        continue;
        //    }

        //    uintptr_t view_model_instance = read<uintptr_t>(viewModel + 0x28); // public BaseViewModel instance; // 0x28
        //    if (!view_model_instance) {
        //        continue;
        //    }

        //    write<bool>(view_model_instance + 0x40, false); // useViewModelCamera
        //    uintptr_t view_model_object = readchain<uintptr_t>(view_model_instance, { 0x10, 0x30 });
        //    if (!view_model_object) {
        //        continue;
        //    }

        //    std::vector<uintptr_t> rendererhand;
        //    GetComponentsInChildren(view_model_object, rendererhand);
        //    for (uintptr_t renderer : rendererhand) {
        //        ProcessSkinnedMeshRenderer(renderer, materiala);
        //    }
        //}
    

        int32_t EntitySize = Entity->GetSize();
        if (EntitySize < 0 || EntitySize > 20000) {
            continue;
        }

        std::vector<sVectorPush> entityplayerpool = { };
        for (int32_t i = 0; i < EntitySize; i++) {
            BaseNetworkable* Entity = 0;
            Entity = EntityArray->GetEntity(i);
            if (!Entity) {
                continue;
            }

            BaseNetworkable* EntityObject = Entity->GetEntityObject();
            if (!EntityObject) {
                continue;
            }

            BaseNetworkable* GameObject = EntityObject->GetGameObject();
            if (!GameObject) {
                continue;
            }

            std::string GameBuff = GameObject->GetGameBuff();
            if (GameBuff.empty()) {
                continue;
            }

            if (globals::features[ScientistEsp] && (
                GameBuff.find(skCrypt("assets/prefabs/npc/bandit")) != std::string::npos ||
                GameBuff.find(skCrypt("tunneldweller")) != std::string::npos ||
                GameBuff.find(skCrypt("scientistnpc")) != std::string::npos)) {
                BasePlayer* Player = EntityObject->GetBasePlayer();

                sBasePlayer BasePlayer;
                BasePlayer.Player = Player;
                BasePlayer.Scientist = true;
                entityplayerpool.push_back({ BasePlayer, sPrefabList(), true });

                if (globals::features[Chams]) {
                    auto SkinnedList = Player->GetPlayerModel()->MultiMesh()->SkinnedRenderersList();
                    SkinnedList->SetChams(SkinnedList->SkinnedList());
                }
            }

            if (globals::features[PlayerEsp] && GameBuff.find(skCrypt("player.prefab")) != std::string::npos) {
                BasePlayer* Player = EntityObject->GetBasePlayer();

                sBasePlayer BasePlayer;
                BasePlayer.Player = Player;
                BasePlayer.Scientist = false;
                entityplayerpool.push_back({ BasePlayer, sPrefabList(), true });

                if (globals::features[Chams]) {

                    bool Sleeper = Player->IsSleeping();
                    if (!globals::features[ShowSleepers] && Sleeper) {
                        continue;
                    }
                    auto SkinnedList = Player->GetPlayerModel()->MultiMesh()->SkinnedRenderersList();
                    SkinnedList->SetChams(SkinnedList->SkinnedList());
                }
            }

            if (GetPrefabNames(GameBuff)) {
                uint64_t EntityPositionAGO = EntityObject->GetEntityPosAgo();
                if (!EntityPositionAGO) {
                    continue;
                }

                sPrefabList BasePrefabs;
                BasePrefabs.PrefabName = prefabMap[GameBuff].PrefabName;
                BasePrefabs.prefab = prefabMap[GameBuff].prefab;
                BasePrefabs.PosAgo = EntityPositionAGO;

                entityplayerpool.push_back({ sBasePlayer(), BasePrefabs, false });
            }
            if (globals::features[DroppedItems] && GameBuff.find(skCrypt("(world)")) != std::string::npos)
            {
                uint64_t EntityPositionAGO = EntityObject->GetEntityPosAgo();
                if (!EntityPositionAGO) {
                    continue;
                }

                std::string PrefabName = FilterDroppedItem(GameBuff);
                if (PrefabName.empty()) {
                    continue;
                }

                sPrefabList BasePrefabs;
                BasePrefabs.PrefabName = PrefabName;
                BasePrefabs.prefab = DroppedItems;
                BasePrefabs.PosAgo = EntityPositionAGO;

                entityplayerpool.push_back({ sBasePlayer(), BasePrefabs, false });
            }
        }
        EntityVectorPool.mutex.lock();
        EntityVectorPool.vec = entityplayerpool;
        EntityVectorPool.mutex.unlock();
    }
}

struct ClosestStruct {
    BasePlayer* TargerEnt;
    Vector3 Bone;
    Vector3 Velocity;
};


void LocalPlayerMiscs() {
    SleepEx(1000, false);
    for (;;) {
        if (!globals::features[AdminFlag] &&
            !globals::features[Spiderman] &&
            !globals::features[TimeChanger] &&
            !globals::features[FovChanger] &&
            !globals::features[SuperEoka] &&
            !globals::features[SuperMeele] &&
            !globals::features[NoRecoil] &&
            !globals::features[NoSpread] &&
            !globals::features[Automatic]) {
            SleepEx(1500, false);
            continue;
        }

        SleepEx(1, false);
        PlayerMovement* Movement = Local_Player->GetPlayerMovement();
        if (!Movement) {
            continue;
        }

        if (globals::features[AdminFlag]) {
            //ConsoleSystemIndex->BlockCommands();
            if (GetAsyncKeyState(globals::keybind[AdminFlag]) & 1) {
                Local_Player->SetFlag(PlayerFlags::IsDeveloper, true);
            }
            else {
                Local_Player->SetFlag(PlayerFlags::IsDeveloper, false);
            }
        }

        if (globals::features[Spiderman]) {
            Movement->Spiderman();
        }

        if (globals::features[TimeChanger]) {
            Todsky->TOD_CycleParameters()->TimeChanger(globals::featuresint[TimeChanger]);
        }

        if (globals::features[FovChanger]) {
            Graphicfov->SetFov(globals::featuresint[FovChanger]);
        }
 
        BasePlayer* GetHeldItem = Local_Player->GetHeldItem();
        if (!GetHeldItem) {
            continue;
        }

        BasePlayer* BaseProjectile = GetHeldItem->GetBaseProjectile();
        if (!BaseProjectile) {
            continue;
        }

        std::string HandWeapon = GetHeldItem->get_item_name();
        if (HandWeapon.find(skCrypt("rocket")) != std::string::npos)
            continue;

        if (globals::features[SuperEoka] && HandWeapon.find(skCrypt("eoka")) != std::string::npos) {
            BaseProjectile->SuperEoka(true);
        }

        if (globals::features[SuperMeele] && Local_Player->is_meele(HandWeapon)) {
            BaseProjectile->SuperMeele(true);
        }

        if (Local_Player->is_weapon(HandWeapon)) {
            BasePlayer* RecoilProperties = BaseProjectile->GetRecoilProperties();
            if (!RecoilProperties) {
                continue;
            }

            BasePlayer* newRecoilOverride = RecoilProperties->GetRecoilOverride();

            if (globals::features[NoRecoil]) {
                RecoilProperties->SetRecoil(0.f);

                newRecoilOverride->SetRecoil(0.f);
            }

            if (globals::features[NoSpread]) {
                BaseProjectile->SetSpread(0.f);

                RecoilProperties->SetAimCone(0.f);

                newRecoilOverride->SetAimCone(0.f);
            }

            if (globals::features[Automatic]) {
                BaseProjectile->SetAutomatic(true);
            }
        }
    }
}
