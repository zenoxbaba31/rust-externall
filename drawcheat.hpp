#pragma once

void DrawCheat()
{
    Vector3 LocalGamePosition = Local_Player->GetPlayerModel()->GetPosition();
    if (LocalGamePosition.empty()) {
        return;
    }

    Vector3 LocalVAngle = Local_Player->GetPlayerInput()->GetVAngles();
    if (LocalVAngle.empty()) {
        return;
    }

    if (!CameraInstance->GetViewMatrix()) {
        return;
    }

    EntityVectorPool.mutex.lock();
    std::vector<sVectorPush> PrefabPools = EntityVectorPool.vec;
    EntityVectorPool.mutex.unlock();
    for (sVectorPush entitys : PrefabPools)
    {
        if (entitys.Playerbool) {
            continue;
        }

        sPrefabList entity = entitys.Prefab;
        ImColor PrefabColor = globals::EspColor[entity.prefab];

        Vector3 PrefabGamePosition = ((BaseNetworkable*)(entity.PosAgo))->GetEntityPosition();
        if (PrefabGamePosition.empty()) {
            continue;
        }

        int distance = LocalGamePosition.distance(PrefabGamePosition);
        if (distance < 0 || distance > 500) {
            continue;
        }

        Vector3 ScreenPosition;
        if (!WorldToScreen(PrefabGamePosition, ScreenPosition)) {
            continue;
        }

        auto textures = texture_cache.get_texture(entity.PrefabName);
        if (globals::features[DroppedItems] && entity.prefab == DroppedItems && globals::features[DroppedItemsImage] && textures.texture != nullptr) {
            ImGui::GetBackgroundDrawList()->AddImage(reinterpret_cast<void*>(textures.texture), ImVec2(ScreenPosition.x - 15, ScreenPosition.y - 15), ImVec2(ScreenPosition.x + 15, ScreenPosition.y + 15), ImVec2(0, 0), ImVec2(1, 1));
        }
        else {
            char text[0x20];
            snprintf(text, sizeof(text), skCrypt("%s [%d m]"), entity.PrefabName.c_str(), distance);
            DrawString(13, ScreenPosition.x, ScreenPosition.y, PrefabColor, true, true, text);
        }

    }
    if (globals::features[PlayerEsp])
    {
        EntityVectorPool.mutex.lock();
        std::vector<sVectorPush> PrefabPools = EntityVectorPool.vec;
        EntityVectorPool.mutex.unlock();
        BasePlayer* TargerEnt = 0;
        float bestdistance = FLT_MAX;
        for (sVectorPush entitys : PrefabPools)
        {
            if (!entitys.Playerbool) {
                continue;
            }

            sBasePlayer PlayerEnt = entitys.Player;

            if (PlayerEnt.Player->IsDead()) {
                continue;
            }

            bool Sleeper = PlayerEnt.Player->IsSleeping();

            if (!globals::features[ShowSleepers] && Sleeper) {
                continue;
            }

            if (!globals::features[ScientistEsp] && PlayerEnt.Scientist) {
                continue;
            }

            //uintptr_t EntitySID = PlayerEnt.Player->GetSteamIDFLOAT();
            //if (!EntitySID) {
            //    continue;
            //}
            //
            bool TeamIgnoreSKIP = false;
            //for (int i = 0; i < Local_Player->GetTeamSize() && !TeamIgnoreSKIP; i++) {
            //    TeamIgnoreSKIP = bool(EntitySID == Local_Player->IsTeamMate(i));
            //}

            Vector3 Position = PlayerEnt.Player->GetPlayerModel()->GetPosition();
            if (Position.empty()) {
                continue;
            }

            Vector3 HeadPosition = PlayerEnt.Player->GetBoneLocation(BoneList::head);
            if (HeadPosition.empty()) {
                continue;
            }

            std::string PlayerName = PlayerEnt.Player->GetName();

            bool IsWounded = PlayerEnt.Player->IsWounded();

        

            ImColor EspColor = PlayerEnt.Scientist ? globals::EspColor[ScientistEsp] : globals::EspColor[PlayerEsp];

            if (Position.empty() || HeadPosition.empty()) {
                continue;
            }

            int distancee = LocalGamePosition.distance(Position);
            if (distancee < 0 || distancee > 500) {
                continue;
            }

            if (IsWounded) {
                EspColor = globals::EspColor[Wounded];
            }

            else if (Sleeper) {
                EspColor = globals::EspColor[ShowSleepers];
            }

            if (TeamIgnoreSKIP) {
                EspColor = globals::EspColor[TeamEsp];

                if (!globals::features[TeamEsp]) {
                    continue;
                }
            }

            Position.y = Position.y - 0.45f;
            Vector3 ScreenPosition;
            if (!WorldToScreen(Position, ScreenPosition)) {
                if (globals::features[OffScreenArrows]) {
                    float num = atan2f(LocalGamePosition.x - Position.x, LocalGamePosition.z - Position.z) * 57.29578f - 180.f - LocalVAngle.y;
                    Vector2 pos_0 = cos_tan_horizontal(num, 10, globals::ScreenMidSize.right, globals::ScreenMidSize.bottom, 100);
                    Vector2 pos_1 = cos_tan_horizontal(num + 2.f, 10, globals::ScreenMidSize.right, globals::ScreenMidSize.bottom, 90);
                    Vector2 pos_2 = cos_tan_horizontal(num - 2.f, 10, globals::ScreenMidSize.right, globals::ScreenMidSize.bottom, 90);
                    ImGui::GetBackgroundDrawList()->AddTriangleFilled(ImVec2(pos_0.x, pos_0.y), ImVec2(pos_1.x, pos_1.y), ImVec2(pos_2.x + 1, pos_2.y + 1), EspColor);
                }
                continue;
            }

            Vector3 ScreenHeadPosition;
            if (!WorldToScreen(Vector3(HeadPosition.x, HeadPosition.y + 0.45f, HeadPosition.z), ScreenHeadPosition)) {
                continue;
            }

            Vector3 W2SHeadPosition;
            if (!WorldToScreen(HeadPosition, W2SHeadPosition)) {
                continue;
            }

            float distance = Vector2(globals::ScreenMidSize.right, globals::ScreenMidSize.bottom).distance(Vector2(W2SHeadPosition.x, W2SHeadPosition.y));
            if (distance < bestdistance) {
                bestdistance = distance;

                TargerEnt = PlayerEnt.Player;
            }


            float boxHeight = abs(ScreenHeadPosition.y - ScreenPosition.y);
            float boxWeight = boxHeight / 1.5f;
            if (!IsWounded)
            {
                if (globals::features[FillBox]) {
                    ImGui::GetBackgroundDrawList()->AddRectFilledMultiColor(ImVec2(ScreenPosition.x - boxWeight / 2, ScreenHeadPosition.y), ImVec2(ScreenPosition.x + boxWeight / 2, ScreenHeadPosition.y + boxHeight), ImColor(0, 0, 0, 85), ImColor(0, 0, 0, 85), ImColor((int)EspColor.Value.x * 255 / 2.2, (int)EspColor.Value.y * 255 / 2.2, (int)EspColor.Value.z * 255 / 2.2, 85), ImColor((int)EspColor.Value.x * 255, (int)EspColor.Value.y * 255, (int)EspColor.Value.z * 255, 85));
                }

                if (globals::features[CornerBox]) {
                    DrawCorneredBox(ScreenPosition.x - (boxWeight / 2.0f), ScreenPosition.y, boxWeight, -boxHeight, EspColor, 1);
                }
                if (globals::features[NormalBox]) {
                    Draw2DBox(ScreenPosition.x - (boxWeight / 2.0f) - 1, ScreenPosition.y - 1, boxWeight + 2, -boxHeight + 2, EspColor, 1);
                }
            }
            if (globals::features[NameEsp])
            {
                std::string NameTag = PlayerEnt.Scientist ? skCrypt("Scientist") : PlayerName.c_str();
                char text[0x30];  // 48 byte
                snprintf(text, sizeof(text), skCrypt("%s [%d m]%s"), NameTag.c_str(), distancee, IsWounded ? skCrypt(" (WOUNDED)") : skCrypt(""));
                DrawString(13, ScreenPosition.x, ScreenHeadPosition.y, EspColor, true, true, text);
            }
            if (globals::features[SkeletonEsp])
            {
                Vector3 eyePos = PlayerEnt.Player->GetBoneLocation(BoneList::eye);
                if (eyePos.empty()) {
                    continue;
                }

                Vector3 neckPos = PlayerEnt.Player->GetBoneLocation(BoneList::neck);
                if (neckPos.empty()) {
                    continue;
                }

                Vector3 lUpperArmPos = PlayerEnt.Player->GetBoneLocation(BoneList::l_upperarm);
                if (lUpperArmPos.empty()) {
                    continue;
                }

                Vector3 rUpperArmPos = PlayerEnt.Player->GetBoneLocation(BoneList::r_upperarm);
                if (rUpperArmPos.empty()) {
                    continue;
                }

                Vector3 lForearmPos = PlayerEnt.Player->GetBoneLocation(BoneList::l_forearm);
                if (lForearmPos.empty()) {
                    continue;
                }

                Vector3 rForearmPos = PlayerEnt.Player->GetBoneLocation(BoneList::r_forearm);
                if (rForearmPos.empty()) {
                    continue;
                }

                Vector3 lHandPos = PlayerEnt.Player->GetBoneLocation(BoneList::l_hand);
                if (lHandPos.empty()) {
                    continue;
                }

                Vector3 rHandPos = PlayerEnt.Player->GetBoneLocation(BoneList::r_hand);
                if (rHandPos.empty()) {
                    continue;
                }

                Vector3 spine4Pos = PlayerEnt.Player->GetBoneLocation(BoneList::spine4);
                if (spine4Pos.empty()) {
                    continue;
                }

                Vector3 pelvisPos = PlayerEnt.Player->GetBoneLocation(BoneList::pelvis);
                if (pelvisPos.empty()) {
                    continue;
                }

                Vector3 lHipPos = PlayerEnt.Player->GetBoneLocation(BoneList::l_hip);
                if (lHipPos.empty()) {
                    continue;
                }

                Vector3 rHipPos = PlayerEnt.Player->GetBoneLocation(BoneList::r_hip);
                if (rHipPos.empty()) {
                    continue;
                }

                Vector3 lKneePos = PlayerEnt.Player->GetBoneLocation(BoneList::l_knee);
                if (lKneePos.empty()) {
                    continue;
                }

                Vector3 rKneePos = PlayerEnt.Player->GetBoneLocation(BoneList::r_knee);
                if (rKneePos.empty()) {
                    continue;
                }

                Vector3 lFootPos = PlayerEnt.Player->GetBoneLocation(BoneList::l_foot);
                if (lFootPos.empty()) {
                    continue;
                }

                Vector3 rFootPos = PlayerEnt.Player->GetBoneLocation(BoneList::r_foot);
                if (rFootPos.empty()) {
                    continue;
                }

                if (!eyePos.empty() || !neckPos.empty() || !lUpperArmPos.empty() ||
                    !rUpperArmPos.empty() || !lForearmPos.empty() || !rForearmPos.empty() ||
                    !lHandPos.empty() || !rHandPos.empty() || !spine4Pos.empty() ||
                    !pelvisPos.empty() || !lHipPos.empty() || !rHipPos.empty() ||
                    !lKneePos.empty() || !rKneePos.empty() || !lFootPos.empty() ||
                    !rFootPos.empty()) {

                    if (!WorldToScreen(neckPos, neckPos)) {
                        continue;
                    }

                    if (!WorldToScreen(lUpperArmPos, lUpperArmPos)) {
                        continue;
                    }

                    if (!WorldToScreen(rUpperArmPos, rUpperArmPos)) {
                        continue;
                    }

                    if (!WorldToScreen(lForearmPos, lForearmPos)) {
                        continue;
                    }

                    if (!WorldToScreen(rForearmPos, rForearmPos)) {
                        continue;
                    }

                    if (!WorldToScreen(lHandPos, lHandPos)) {
                        continue;
                    }

                    if (!WorldToScreen(rHandPos, rHandPos)) {
                        continue;
                    }

                    if (!WorldToScreen(spine4Pos, spine4Pos)) {
                        continue;
                    }

                    if (!WorldToScreen(pelvisPos, pelvisPos)) {
                        continue;
                    }

                    if (!WorldToScreen(lHipPos, lHipPos)) {
                        continue;
                    }

                    if (!WorldToScreen(rHipPos, rHipPos)) {
                        continue;
                    }

                    if (!WorldToScreen(lKneePos, lKneePos)) {
                        continue;
                    }

                    if (!WorldToScreen(rKneePos, rKneePos)) {
                        continue;
                    }

                    if (!WorldToScreen(lFootPos, lFootPos)) {
                        continue;
                    }

                    if (!WorldToScreen(rFootPos, rFootPos)) {
                        continue;
                    }

                    DrawLine({ W2SHeadPosition.x, W2SHeadPosition.y }, { neckPos.x, neckPos.y }, EspColor, 1.0f);
                    DrawLine({ neckPos.x, neckPos.y }, { spine4Pos.x, spine4Pos.y }, EspColor, 1.0f);
                    DrawLine({ spine4Pos.x, spine4Pos.y }, { pelvisPos.x, pelvisPos.y }, EspColor, 1.0f);
                    DrawLine({ neckPos.x, neckPos.y }, { lUpperArmPos.x, lUpperArmPos.y }, EspColor, 1.0f);
                    DrawLine({ lUpperArmPos.x, lUpperArmPos.y }, { lForearmPos.x, lForearmPos.y }, EspColor, 1.0f);
                    DrawLine({ lForearmPos.x, lForearmPos.y }, { lHandPos.x, lHandPos.y }, EspColor, 1.0f);
                    DrawLine({ neckPos.x, neckPos.y }, { rUpperArmPos.x, rUpperArmPos.y }, EspColor, 1.0f);
                    DrawLine({ rUpperArmPos.x, rUpperArmPos.y }, { rForearmPos.x, rForearmPos.y }, EspColor, 1.0f);
                    DrawLine({ rForearmPos.x, rForearmPos.y }, { rHandPos.x, rHandPos.y }, EspColor, 1.0f);
                    DrawLine({ pelvisPos.x, pelvisPos.y }, { lHipPos.x, lHipPos.y }, EspColor, 1.0f);
                    DrawLine({ lHipPos.x, lHipPos.y }, { lKneePos.x, lKneePos.y }, EspColor, 1.0f);
                    DrawLine({ lKneePos.x, lKneePos.y }, { lFootPos.x, lFootPos.y }, EspColor, 1.0f);
                    DrawLine({ pelvisPos.x, pelvisPos.y }, { rHipPos.x, rHipPos.y }, EspColor, 1.0f);
                    DrawLine({ rHipPos.x, rHipPos.y }, { rKneePos.x, rKneePos.y }, EspColor, 1.0f);
                    DrawLine({ rKneePos.x, rKneePos.y }, { rFootPos.x, rFootPos.y }, EspColor, 1.0f);
                }
            }
        }
    

        if (TargerEnt != 0) {
            Cloth ClothSlotOffset[7];
            Belt BeltSlotOffset[6];
            for (int idX = 0; idX < 7; ++idX) {
                ClothSlotOffset[idX] = TargerEnt->GetClothingItems(TargerEnt->GetClothingItemsList(), idX);
                if (idX != 6) {
                    BeltSlotOffset[idX] = TargerEnt->GetBeltItems(TargerEnt->GetItemsList(), idX);
                }
            }

            if (globals::features[PlayerInfo]) {
                playerinfo(TargerEnt->GetName(), ClothSlotOffset, BeltSlotOffset, "");
            }

            Vector3 Bone;
            if (globals::hitboxtype == 0) Bone = TargerEnt->GetBoneLocation(BoneList::head);
            if (globals::hitboxtype == 1) Bone = TargerEnt->GetBoneLocation(BoneList::spine4);
            if (globals::hitboxtype == 2) Bone = TargerEnt->GetBoneLocation(BoneList::pelvis);
            if (Bone.empty()) {
                return;
            }
            if ((globals::features[Aimbot] || globals::features[Silent] || globals::features[Aimline])) {
                Vector3 screen_pos;
                if (!TargerEnt->IsDead() && WorldToScreen(Bone, screen_pos)) {
                    Vector3 ENEMYHEADPOSS = Bone;
                    float aim_fov = globals::features[AimFov] ? AimFovv(true, screen_pos) : 0;
                    if (!ENEMYHEADPOSS.empty() && aim_fov < globals::featuresint[AimFov]) {
                        if (globals::features[Aimline]) {
                            DrawLine({ (float)globals::ScreenMidSize.right, (float)globals::ScreenMidSize.bottom }, { screen_pos.x, screen_pos.y }, (ImColor)globals::EspColor[Aimline], 1.0f);
                        }
                        if ((globals::features[Aimbot] || globals::features[Silent])) {
                            if (GetAsyncKeyState(globals::keybind[Aimbot]) || GetAsyncKeyState(globals::keybind[Silent])) {
                                Vector3 LocalHeadGamePosition = CameraInstance->GetMatrixPosition();
                                if (globals::features[Predictions]) {
                                    ENEMYHEADPOSS = Prediction(LocalHeadGamePosition, TargerEnt->GetPlayerModel()->GetVelocity(), ENEMYHEADPOSS);
                                }
                                Vector3 TargetAngles = CalcAngle(LocalHeadGamePosition, ENEMYHEADPOSS);
                                Normalize(TargetAngles.y, TargetAngles.x);
                                if (!isnan(TargetAngles.x) || !isnan(TargetAngles.y) && TargetAngles.x != 0 && TargetAngles.y != 0) {
                                    if (globals::features[Aimbot] && GetAsyncKeyState(globals::keybind[Aimbot])) {
                                        PlayerInput* Input = Local_Player->GetPlayerInput();
                                        if (!Input) {
                                            return;
                                        }
                                        Input->SetVAngles(TargetAngles);
                                    }
                                    if (globals::features[Silent] && GetAsyncKeyState(0x01)) {
                                        PlayerEyes* Eyes = Local_Player->GetPlayerEyes();
                                        if (!Eyes) {
                                            return;
                                        }
                                        Eyes->SetSilent(TargetAngles);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    }

}
