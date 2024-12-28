#pragma once

static int iTabs = 0;

void DrawMenu(ImGuiStyle& s)
{

    s.WindowRounding = 15.f;
    ImGui::SetNextWindowSize(ImVec2(menu_size.x, menu_size.y));
    ImGui::Begin(skCrypt("General"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);
    {

        anim_speed = ImGui::GetIO().DeltaTime * 12.f;

        auto draw = ImGui::GetWindowDrawList();
        const auto& p = ImGui::GetWindowPos();

        draw->AddRectFilled(p + ImVec2(0, 35), p + ImVec2(menu_size.x, menu_size.y), winbg_color, s.WindowRounding, ImDrawFlags_RoundCornersBottom);

        draw->AddRectFilled(p + ImVec2(0, 35), p + ImVec2(menu_size.x, menu_size.y), winbg_color, s.WindowRounding, ImDrawFlags_RoundCornersBottom);

        draw->AddRectFilled(p + ImVec2(0, 35), p + ImVec2(187, menu_size.y), ImColor(13, 14, 16, 229), s.WindowRounding, ImDrawFlags_RoundCornersBottomLeft);

        draw->AddRectFilled(p, p + ImVec2(menu_size.x, 35), ImColor(5, 5, 5, 240), s.WindowRounding, ImDrawFlags_RoundCornersTop);
        draw->AddRectFilled(p + ImVec2(0, 32), p + ImVec2(menu_size.x, 35), main_color, s.WindowRounding);

        ImGui::PushFont(logo_font);

        draw->AddText(center_text(p, p + ImVec2(menu_size.x, 32), std::string(main_brand).c_str()), ImColor(1.f, 1.f, 1.f, 1.f), std::string(main_brand).c_str());

        ImGui::PopFont();

        ImGui::SetCursorPos(ImVec2(10, 70));

        ImGui::BeginGroup();

        ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.3f), skCrypt("Aimbot"));

        ImGui::Tab(skCrypt("Aimbot"), skCrypt("9"), &iTabs, 0);
        ImGui::Tab(skCrypt("Weapons"), skCrypt("0"), &iTabs, 1);

        ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.3f), skCrypt("Visuals"));

        ImGui::Tab(skCrypt("Player"), skCrypt("8"), &iTabs, 2);
        ImGui::Tab(skCrypt("World"), skCrypt("5"), &iTabs, 3);
        ImGui::Tab(skCrypt("World 2"), skCrypt("5"), &iTabs, 6);

        ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 0.3f), skCrypt("Others"));

        ImGui::Tab(skCrypt("Misc"), skCrypt("1"), &iTabs, 4);
        ImGui::Tab(skCrypt("Config"), skCrypt("2"), &iTabs, 5);

        ImGui::EndGroup();

        ImGui::SetCursorPos(ImVec2(197, 72));
        ImGui::BeginGroup();
        {
            ImGui::BeginChild(skCrypt("Frame##1"), ImVec2(frame_size.x, 680), true);
            {
                if (iTabs == 0)
                {
                    Keybindbox(skCrypt("Aimbot"), &globals::features[Aimbot], &globals::keybind[Aimbot]); if (globals::features[Aimbot]) globals::features[Silent] = false;
                    Keybindbox(skCrypt("Silent"), &globals::features[Silent], &globals::keybind[Silent]);  if (globals::features[Silent]) globals::features[Aimbot] = false;
                    Pickerbox(skCrypt("Aimline"), &globals::features[Aimline], (float*)&globals::EspColor[Aimline]);
                    ImGui::Checkbox(skCrypt("Prediction"), &globals::features[Predictions]);
                    Pickerbox(skCrypt("Aim Fov Circle"), &globals::features[AimFov], (float*)&globals::EspColor[AimFov]);
                    ImGui::SliderInt(skCrypt("Aim Value"), &globals::featuresint[AimFov], 30.f, 360.f);
                    ImGui::Combo(skCrypt("HitBox Select"), &globals::hitboxtype, globals::hitboxtypes, IM_ARRAYSIZE(globals::hitboxtypes));
                }
                if (iTabs == 1)
                {
                    ImGui::Checkbox(skCrypt("No Recoil"), &globals::features[NoRecoil]);
                    ImGui::Checkbox(skCrypt("No Spread"), &globals::features[NoSpread]);
                    ImGui::Checkbox(skCrypt("Automatic"), &globals::features[Automatic]);
                    ImGui::Checkbox(skCrypt("Super Eoka"), &globals::features[SuperEoka]);
                    ImGui::Checkbox(skCrypt("Super Meele"), &globals::features[SuperMeele]);
                }
                if (iTabs == 2)
                {
                    Pickerbox(skCrypt("PlayerEsp"), &globals::features[PlayerEsp], (float*)&globals::EspColor[PlayerEsp]);
                    ImGui::Checkbox(skCrypt("Name Esp"), &globals::features[NameEsp]);
                    ImGui::Checkbox(skCrypt("Corner Box"), &globals::features[CornerBox]);
                    ImGui::Checkbox(skCrypt("2D Box"), &globals::features[NormalBox]);
                    ImGui::Checkbox(skCrypt("Skeleton Esp"), &globals::features[SkeletonEsp]);
                    ImGui::Checkbox(skCrypt("Fill Box"), &globals::features[FillBox]);
                    ImGui::Checkbox(skCrypt("Player Info"), &globals::features[PlayerInfo]);
                    ImGui::Checkbox(skCrypt("Off Screen Arrows"), &globals::features[OffScreenArrows]);
                }
                if (iTabs == 3)
                {
                    DoubleCheckboxAndPicker(skCrypt("Dropped Items"), &globals::features[DroppedItems], &globals::features[DroppedItemsImage], (float*)&globals::EspColor[DroppedItems]);
                    Pickerbox(skCrypt("Stone Esp"), &globals::features[StoneEsp], (float*)&globals::EspColor[StoneEsp]);
                    Pickerbox(skCrypt("Sulfur Esp"), &globals::features[SulfurEsp], (float*)&globals::EspColor[SulfurEsp]);
                    Pickerbox(skCrypt("Metal Esp"), &globals::features[MetalEsp], (float*)&globals::EspColor[MetalEsp]);
                    Pickerbox(skCrypt("Hemp Esp"), &globals::features[HempEsp], (float*)&globals::EspColor[HempEsp]);
                    Pickerbox(skCrypt("Supply Drop Esp"), &globals::features[SupplyDropEsp], (float*)&globals::EspColor[SupplyDropEsp]);
                    Pickerbox(skCrypt("Auto Turret Esp"), &globals::features[AutoTurretEsp], (float*)&globals::EspColor[AutoTurretEsp]);
                    Pickerbox(skCrypt("Shotgun Trap Esp"), &globals::features[ShotgunTrapEsp], (float*)&globals::EspColor[ShotgunTrapEsp]);
                    Pickerbox(skCrypt("Land Mine Esp"), &globals::features[LandMineEsp], (float*)&globals::EspColor[LandMineEsp]);
                    Pickerbox(skCrypt("Bear Trap Esp"), &globals::features[BearTrapEsp], (float*)&globals::EspColor[BearTrapEsp]);
                    Pickerbox(skCrypt("Flame Turret Esp"), &globals::features[FlameTurretEsp], (float*)&globals::EspColor[FlameTurretEsp]);
                    Pickerbox(skCrypt("Tool Cupboard Esp"), &globals::features[ToolCupboardEsp], (float*)&globals::EspColor[ToolCupboardEsp]);
                }
                if (iTabs == 6)
                {
                    Pickerbox(skCrypt("Corpse Esp"), &globals::features[CorpseEsp], (float*)&globals::EspColor[CorpseEsp]);
                    Pickerbox(skCrypt("Collectable Stone Esp"), &globals::features[CollectableStoneEsp], (float*)&globals::EspColor[CollectableStoneEsp]);
                    Pickerbox(skCrypt("Collectable Metal Esp"), &globals::features[CollectableMetalEsp], (float*)&globals::EspColor[CollectableMetalEsp]);
                    Pickerbox(skCrypt("Collectable Sulfur Esp"), &globals::features[CollectableSulfurEsp], (float*)&globals::EspColor[CollectableSulfurEsp]);
                    Pickerbox(skCrypt("Collectable Wood Esp"), &globals::features[CollectableWoodEsp], (float*)&globals::EspColor[CollectableWoodEsp]);
                    Pickerbox(skCrypt("Corn Esp"), &globals::features[CornEsp], (float*)&globals::EspColor[CornEsp]);
                    Pickerbox(skCrypt("Pumpkin Esp"), &globals::features[PumpkinEsp], (float*)&globals::EspColor[PumpkinEsp]);
                    Pickerbox(skCrypt("Mushroom Esp"), &globals::features[MushroomEsp], (float*)&globals::EspColor[MushroomEsp]);
                }
                if (iTabs == 4)
                {
                    Keybindbox(skCrypt("Admin Flag"), &globals::features[AdminFlag], &globals::keybind[AdminFlag]);
                    ImGui::Checkbox(skCrypt("Spiderman"), &globals::features[Spiderman]);
                }

                if (iTabs == 5)
                {
                    ImGui::Combo(skCrypt("Config List"), &globals::Configs, globals::ConfigsC, IM_ARRAYSIZE(globals::ConfigsC), 11);
                    std::string config;
                    if (globals::Configs == 0) config = skCrypt("C:\\Windows\\System\\RUSTEXTERNALCONFIG.json ");
                    if (globals::Configs == 1) config = skCrypt("C:\\Windows\\System\\RUSTEXTERNALCONFIG2.json");
                    if (globals::Configs == 2) config = skCrypt("C:\\Windows\\System\\RUSTEXTERNALCONFIG3.json");
                    if (globals::Configs == 3) config = skCrypt("C:\\Windows\\System\\RUSTEXTERNALCONFIG4.json");
                    if (globals::Configs == 4) config = skCrypt("C:\\Windows\\System\\RUSTEXTERNALCONFIG5.json");
                    if (globals::Configs == 5) config = skCrypt("C:\\Windows\\System\\RUSTEXTERNALCONFIG6.json");
                    if (globals::Configs == 6) config = skCrypt("C:\\Windows\\System\\RUSTEXTERNALCONFIG7.json");
                    if (globals::Configs == 7) config = skCrypt("C:\\Windows\\System\\RUSTEXTERNALCONFIG8.json");
                    if (globals::Configs == 8) config = skCrypt("C:\\Windows\\System\\RUSTEXTERNALCONFIG9.json");
                    if (globals::Configs == 9) config = skCrypt("C:\\Windows\\System\\RUSTEXTERNALCONFIG10.json");

                    if (ImGui::Button("Save", ImVec2(140, 20)))
                        Configsave(config);
                    ImGui::SameLine();
                    if (ImGui::Button("Load", ImVec2(140, 20)))
                        Configload(config);
                }

            }
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild(skCrypt("Frame##2"), ImVec2(frame_size.x, 680), true);
            {
                if (iTabs == 0)
                {
                    ImGui::Checkbox(skCrypt("Ignore Wounded"), &globals::features[IgnoreWounded]);
                    ImGui::Checkbox(skCrypt("Ignore Sleeper"), &globals::features[IgnoreSleeper]);
                    ImGui::Checkbox(skCrypt("Ignore Scientist"), &globals::features[IgnoreNpc]);
                }
                if (iTabs == 2)
                {
                    Pickerbox(skCrypt("Show Sleepers"), &globals::features[ShowSleepers], (float*)&globals::EspColor[ShowSleepers]);
                    Pickerbox(skCrypt("Show Scientist"), &globals::features[ScientistEsp], (float*)&globals::EspColor[ScientistEsp]);
                    Pickerbox(skCrypt("Show Team"), &globals::features[TeamEsp], (float*)&globals::EspColor[TeamEsp]);
                    ImGui::Checkbox(skCrypt("Chams"), &globals::features[Chams]);
                    ImGui::Combo(skCrypt("Chams Select"), &globals::bChamsmaterial, globals::bChamsmaterialItems, IM_ARRAYSIZE(globals::bChamsmaterialItems));
                   // ImGui::Checkbox(skCrypt("Hand & Weapon Chams"), &globals::features[HandChams]);
                   // ImGui::Combo(skCrypt("Hand & Weapon"), &globals::Handmaterialsa, globals::HandChamsmaterialItems, IM_ARRAYSIZE(globals::HandChamsmaterialItems));
                }
                if (iTabs == 3)
                {
                    Pickerbox(skCrypt("Boar Esp"), &globals::features[BoarEsp], (float*)&globals::EspColor[BoarEsp]);
                    Pickerbox(skCrypt("Bear Esp"), &globals::features[BearEsp], (float*)&globals::EspColor[BearEsp]);
                    Pickerbox(skCrypt("Wolf Esp"), &globals::features[WolfEsp], (float*)&globals::EspColor[WolfEsp]);
                    Pickerbox(skCrypt("Chicken Esp"), &globals::features[ChickenEsp], (float*)&globals::EspColor[ChickenEsp]);
                    Pickerbox(skCrypt("Stag Esp"), &globals::features[StagEsp], (float*)&globals::EspColor[StagEsp]);
                    Pickerbox(skCrypt("Horse Esp"), &globals::features[HorseEsp], (float*)&globals::EspColor[HorseEsp]);
                    Pickerbox(skCrypt("Patrol Heli Esp"), &globals::features[PatrolHeli], (float*)&globals::EspColor[PatrolHeli]);
                    Pickerbox(skCrypt("Minicopter Esp"), &globals::features[MinicopterEsp], (float*)&globals::EspColor[MinicopterEsp]);
                    Pickerbox(skCrypt("Car Esp"), &globals::features[CarEsp], (float*)&globals::EspColor[CarEsp]);
                    Pickerbox(skCrypt("Boat Esp"), &globals::features[BoatEsp], (float*)&globals::EspColor[BoatEsp]);
                    Pickerbox(skCrypt("Sam Site Esp"), &globals::features[SamSiteEsp], (float*)&globals::EspColor[SamSiteEsp]);

                }
                if (iTabs == 6)
                {
                    Pickerbox(skCrypt("Shelter Esp"), &globals::features[ShelterEsp], (float*)&globals::EspColor[ShelterEsp]);
                    Pickerbox(skCrypt("Small Box Esp"), &globals::features[SmallBoxEsp], (float*)&globals::EspColor[SmallBoxEsp]);
                    Pickerbox(skCrypt("Large Box Esp"), &globals::features[LargeBoxEsp], (float*)&globals::EspColor[LargeBoxEsp]);
                    Pickerbox(skCrypt("Furnace Esp"), &globals::features[FurnaceEsp], (float*)&globals::EspColor[FurnaceEsp]);
                    Pickerbox(skCrypt("Loot Barrel Esp"), &globals::features[LootBarrelEsp], (float*)&globals::EspColor[LootBarrelEsp]);
                    Pickerbox(skCrypt("Military Crate Esp"), &globals::features[MilitaryCrateEsp], (float*)&globals::EspColor[MilitaryCrateEsp]);
                    Pickerbox(skCrypt("Normal Crate Esp"), &globals::features[NormalCrate], (float*)&globals::EspColor[NormalCrate]);
                    Pickerbox(skCrypt("Elite Crate Esp"), &globals::features[EliteCrate], (float*)&globals::EspColor[EliteCrate]);
                }
                if (iTabs == 4)
                {

                    Pickerbox(skCrypt("Crosshair"), &globals::features[Crosshair], (float*)&globals::EspColor[Crosshair]);
                    ImGui::Checkbox(skCrypt("Time Changer"), &globals::features[TimeChanger]);
                    ImGui::SliderInt(skCrypt("Time Value"), &globals::featuresint[TimeChanger], 0.f, 24.f);
                    ImGui::Checkbox(skCrypt("Fov Changer"), &globals::features[FovChanger]);
                    ImGui::SliderInt(skCrypt("Fov Value"), &globals::featuresint[FovChanger], 90.f, 180.f);

                }
                if (iTabs == 5)
                {
                    ImGui::PushFont(medium_font);
                    time_t expiry_time = string_to_timet(KeyAuthApp.data.expiry);
                    time_t current_time = std::time(nullptr);
                    time_t time_diff = expiry_time - current_time;
                    std::string readable_duration = format_duration(time_diff);
                    draw->AddText(ImVec2(p.x + menu_size.x - 15, p.y + menu_size.y - 15) - ImGui::CalcTextSize(readable_duration.c_str()), ImColor(1.f, 1.f, 1.f, 1.f), std::string(readable_duration).c_str());
                    ImGui::PopFont();
                    //ImGui::ColorEdit4(skCrypt("Main color"), (float*)&main_color.Value, ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
                }


            }
            ImGui::EndChild();
        }
        ImGui::EndGroup();
    }
    ImGui::End();
}
