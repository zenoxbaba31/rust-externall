#pragma once
#include <map>
#include <any>
#include <mutex>
#include "Offsets.h"
#include "decrypt.h"

class MainCamera
{
public:
    static MainCamera* GetCameraInstance()
    {
        uint64_t maincamera = read<uint64_t>(GameAssembly + MAINCAMERA);
        if (!maincamera)
            return 0;

        maincamera = read<uint64_t>(maincamera + 0xB8);
        if (!maincamera)
            return 0;

        maincamera = read<uint64_t>(maincamera + 0x78);
        if (!maincamera)
            return 0;

        maincamera = read<uint64_t>(maincamera + 0x10);
        if (!maincamera)
            return 0;

        return (MainCamera*)maincamera;
    }

    bool GetViewMatrix()
    {
        if (!this)
            return false;

        return readsize((uintptr_t)this + 0x30C, pViewMatrix, sizeof(pViewMatrix));
    }

    Vector3 GetMatrixPosition()
    {
        Vector3 MatrixPosition = read<Vector3>((uint64_t)this + 0x454);
        if (MatrixPosition.empty())
            return Vector3();

        return MatrixPosition;
    }
}; MainCamera* CameraInstance = 0;


class PlayerModel
{
public:
    struct dynamic_array
    {
        std::uint64_t base;
        std::uint64_t mem_id;
        std::uint64_t sz;
        std::uint64_t cap;
    };
    PlayerModel* MultiMesh()
    {
        uint64_t MultiMesh = read<uint64_t>((uint64_t)this + MULTIMESH);
        if (!MultiMesh)
            return 0;

        return (PlayerModel*)MultiMesh;
    }
    PlayerModel* SkinnedRenderersList()
    {
        uint64_t MultiMesh = read<uint64_t>((uint64_t)this + SKINNEDRENDERER);
        if (!MultiMesh)
            return 0;

        return (PlayerModel*)MultiMesh;
    }
    PlayerModel* SkinnedList()
    {
        uint64_t Skinnedlist = read<uint64_t>((uint64_t)this + 0x10);
        if (!Skinnedlist)
            return 0;

        return (PlayerModel*)Skinnedlist;
    }
    void SetChams(PlayerModel* SkinnedList)
    {
        unsigned int material = 0;
        int bChamsmaterial = globals::bChamsmaterial;
        if (bChamsmaterial >= 0 && bChamsmaterial <= 11) {
            material = globals::materials[bChamsmaterial];
        }

        int MaterialsCount = read<int>((uint64_t)this + 0x18);

        for (std::uint32_t idx{ 0 }; idx < MaterialsCount; idx++)
        {
            uint64_t renderEntry = read<uint64_t>((uint64_t)SkinnedList + 0x20 + (idx * 0x8));
            if (!renderEntry)
                continue;

            uint64_t untity_object = read<uint64_t>(renderEntry + 0x10);
            if (!untity_object)
                continue;

            dynamic_array mat_list = read<dynamic_array>(untity_object + 0x148);
            if (mat_list.sz < 1 || mat_list.sz > 5)
                continue;

            for (std::uint32_t idx{ 0 }; idx < mat_list.sz; idx++) {
                write<unsigned int>(mat_list.base + (idx * 0x4), material);
            }
        }
    }
    Vector3 GetPosition()
    {
        Vector3 Position = read<Vector3>((uint64_t)this + POSITION);
        if (Position.empty())
            return Vector3();

        return Position;
    }

    Vector3 GetVelocity() {
        Vector3 velocity = read<Vector3>((uint64_t)this + NEWVELOCITY);
        if (velocity.empty())
            return Vector3();

        return velocity;
    }

};

class PlayerMovement
{
public:
    void AlwaysSprint()
    {
        write<float>((uint64_t)this + 0x48, 1.f);
    }
    void Spiderman()
    {
        write<float>((uint64_t)this + 0xCC, -0.54291e+07);
        write<float>((uint64_t)this + 0xD4, -0.54291e+07);
    }
};

class PlayerEyes
{
public:
    void SetSilent(Vector3 TargetAngles)
    {
        Vector4 Quat = ToQuat(Vector3(TargetAngles.x, TargetAngles.y, 0.0f));
        if (Quat.empty())
            return;

         write<Vector4>((uint64_t)this + 0x50, Quat);
    }
};

class PlayerInput
{
public:
    Vector3 GetVAngles()
    {
        Vector3 VAngles = read<Vector3>((uint64_t)this + 0x44);
        if (!this && VAngles.empty())
            return Vector3();

        return VAngles;
    }
    void SetVAngles(Vector3 TargetAngles)
    {
        write<Vector2>((uint64_t)this + 0x44, Vector2(TargetAngles.x, TargetAngles.y)); // private Vector3 bodyAngles;
    }
};

class BasePlayer
{
public:
    PlayerModel* GetPlayerModel()
    {
        uint64_t playermodel = read<uint64_t>((uint64_t)this + PLAYERMODEL);
        if (!playermodel)
            return 0;

        return (PlayerModel*)playermodel;
    }

    struct unity_string
    {
        char pad[0x10];
        int length;
        wchar_t buffer[128 + 1];
    };

    std::string GetName()
    {
        unity_string* t2 = read<unity_string*>((uint64_t)this + DISPLAYNAME);
        if (!t2)
            return std::string();

        unity_string ustr = read<unity_string>((uint64_t)t2);
        if (!ustr.buffer)
            return std::string();

        std::wstring tmpe(ustr.buffer);
        if (tmpe.empty())
            return std::string();

        std::wstring filteredText;
        for (char c : tmpe) {
            if ((c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z') || (c >= L'0' && c <= L'9') || c == L' ' || c == L'_') {
                filteredText += c;
            }
            else {
                filteredText += L'.';
            }
        }

        return std::string(filteredText.begin(), filteredText.end());
    }

    uintptr_t GetFlag()
    {
        uintptr_t Flags = read<uintptr_t>((uint64_t)(uint64_t)this + PLAYERFLAGS);
        if (!Flags)
            return 0;

        return Flags;
    }

    void SetFlag(PlayerFlags flag, bool mode)
    {
        int Flags = this->GetFlag();
        if (!Flags) {
            return;
        }

        if (mode) {
            Flags |= (int)flag;
        }
        else {
            Flags &= ~(int)flag;
        }

        write<uintptr_t>((uint64_t)this + PLAYERFLAGS, Flags);
    }

    bool IsSleeping()
    {
        int Flags = read<int>((uint64_t)(uint64_t)this + PLAYERFLAGS);
        if (!Flags)
            return false;

        return (Flags & 16) != 0;
    }

    bool IsWounded()
    {
        int Flags = read<int>((uint64_t)(uint64_t)this + PLAYERFLAGS);
        if (!Flags)
            return false;

        return (Flags & 64) != 0;
    }

    bool IsDead()
    {
        bool isdead = read<bool>((uint64_t)this + LIFESTATE);

        return isdead;
    }

    uint64_t GetSteamIDFLOAT()
    {
        unity_string* t2 = read<unity_string*>((uint64_t)this + STEAMIDSTRING);
        if (!t2)
            return 0;

        unity_string ustr = read<unity_string>((uint64_t)t2);
        if (!ustr.buffer)
            return 0;

        std::wstring tmp(ustr.buffer);
        if (tmp.empty())
            return 0;

        uint64_t ptrValue;
        std::stringstream ss;
        ss << std::string(tmp.begin(), tmp.end());
        ss >> ptrValue;
        return ptrValue;
    }

    int GetTeamSize()
    {
        uint64_t ClientTeam = read<uint64_t>((uint64_t)this + PLAYERTEAM);
        if (!ClientTeam)
            return 0;

        uint64_t members = read<uint64_t>(ClientTeam + 0x18);
        if (!members)
            return 0;

        uint64_t TeamSize = read<uint64_t>(members + 0x18);
        if (!TeamSize)
            return 0;

        return TeamSize;
    }

    uint64_t IsTeamMate(int Id)
    {
        uint64_t ClientTeam = read<uint64_t>((uint64_t)this + PLAYERTEAM);
        if (!ClientTeam)
            return 0;

        uint64_t members = read<uint64_t>(ClientTeam + 0x18);
        if (!members)
            return 0;

        uint64_t List = read<uint64_t>(members + 0x10);
        if (!List)
            return 0;

        uint64_t players = read<uint64_t>(List + 0x20 + (Id * 0x8));
        if (!players)
            return 0;

        uint64_t TeamMateCheck = read<uint64_t>(players + 0x28);
        if (!TeamMateCheck)
            return 0;

        return TeamMateCheck;
    }

    struct TransformAccessReadOnly {
        uint64_t pTransformData;
        int pIndex;
    };

    struct TransformData {
        uint64_t pTransformArray;
        uint64_t pTransformIndices;
    };

    struct Matrix3x4 {
        float _11, _12, _13, _14;
        float _21, _22, _23, _24;
        float _31, _32, _33, _34;
    };

    struct UnityEngine_TransformAccessReadOnly
    {
        uint64_t TransformData;
        uint32_t Index;
    };

    struct UnityEngine_TransformData
    {
        uint64_t TransformArray;
        uint64_t TransformIndices;
    };

    //Vector3 GetTransformPosition(uint64_t transform)
    //{
    //    uint64_t transform_internal = read<uint64_t>(transform + 0x10);
    //    if (!transform_internal)
    //        return Vector3();

    //    try
    //    {
    //        __m128 result;

    //        const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
    //        const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
    //        const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

    //        UnityEngine_TransformAccessReadOnly pTransformAccessReadOnly = read<UnityEngine_TransformAccessReadOnly>(transform_internal + 0x38);

    //        if (!pTransformAccessReadOnly.TransformData)
    //            return Vector3();

    //        UnityEngine_TransformData transformData = read<UnityEngine_TransformData>(pTransformAccessReadOnly.TransformData + 0x18);

    //        if (!transformData.TransformArray)
    //            return Vector3();

    //        if (!transformData.TransformIndices)
    //            return Vector3();

    //        size_t sizeMatriciesBuf = sizeof(Matrix3x4) * pTransformAccessReadOnly.Index + sizeof(Matrix3x4);
    //        size_t sizeIndicesBuf = sizeof(int) * pTransformAccessReadOnly.Index + sizeof(int);

    //        void* pMatriciesBuf = malloc(sizeMatriciesBuf);
    //        void* pIndicesBuf = malloc(sizeIndicesBuf);

    //        if (pMatriciesBuf && pIndicesBuf)
    //        {
    //            if (transformData.TransformArray == 0 || transformData.TransformIndices == 0)
    //                return Vector3();

    //            readsize(transformData.TransformArray, pMatriciesBuf, sizeMatriciesBuf);
    //            readsize(transformData.TransformIndices, pIndicesBuf, sizeIndicesBuf);

    //            __m128 Result = *reinterpret_cast<__m128*>(reinterpret_cast<uint64_t>(pMatriciesBuf) + (0x30 * pTransformAccessReadOnly.Index));
    //            int TransformIndex = *reinterpret_cast<int*>((reinterpret_cast<uint64_t>(pIndicesBuf) + (0x4 * pTransformAccessReadOnly.Index)));

    //            while (TransformIndex >= 0 && TransformIndex < (sizeMatriciesBuf / 0x30))
    //            {
    //                if (!pMatriciesBuf || !pIndicesBuf || !&result || !&TransformIndex)
    //                    return Vector3();

    //                Matrix3x4 Matrix = *reinterpret_cast<Matrix3x4*>(reinterpret_cast<uint64_t>(pMatriciesBuf) + (0x30 * TransformIndex));
    //                __m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0x00));	// xxxx
    //                __m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0x55));	// yyyy
    //                __m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0x8E));	// zwxy
    //                __m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0xDB));	// wzyw
    //                __m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0xAA));	// zzzz
    //                __m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0x71));	// yxwy
    //                __m128 tmp7 = _mm_mul_ps(*reinterpret_cast<__m128*>(&Matrix._31), Result);

    //                Result = _mm_add_ps(
    //                    _mm_add_ps(
    //                        _mm_add_ps(
    //                            _mm_mul_ps(
    //                                _mm_sub_ps(
    //                                    _mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
    //                                    _mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
    //                                _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
    //                            _mm_mul_ps(
    //                                _mm_sub_ps(
    //                                    _mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
    //                                    _mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
    //                                _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
    //                        _mm_add_ps(
    //                            _mm_mul_ps(
    //                                _mm_sub_ps(
    //                                    _mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
    //                                    _mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
    //                                _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
    //                            tmp7)), *(__m128*)(&Matrix._11));

    //                TransformIndex = *reinterpret_cast<int*>(reinterpret_cast<uint64_t>(pIndicesBuf) + (0x4 * TransformIndex));
    //            }
    //            free(pMatriciesBuf);
    //            free(pIndicesBuf);

    //            return Vector3{ Result.m128_f32[0], Result.m128_f32[1], Result.m128_f32[2] };
    //        }

    //        return {};
    //    }
    //    catch (...)
    //    {
    //        return Vector3();
    //    }
    //}
    Vector3 GetTransformPosition(uint64_t transform)
    {
        uint64_t transform_internal = read<uint64_t>(transform + 0x10);
        if (!transform_internal)
            return Vector3();

        __m128 result;

        __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
        __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
        __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

        UnityEngine_TransformAccessReadOnly pTransformAccessReadOnly = read<UnityEngine_TransformAccessReadOnly>(transform_internal + 0x38);

        if (!pTransformAccessReadOnly.TransformData)
            return Vector3();

        UnityEngine_TransformData transformData = read<UnityEngine_TransformData>(pTransformAccessReadOnly.TransformData + 0x18);

        if (!transformData.TransformArray)
            return Vector3();

        if (!transformData.TransformIndices)
            return Vector3();

        size_t sizeMatriciesBuf = sizeof(Matrix3x4) * pTransformAccessReadOnly.Index + sizeof(Matrix3x4);
        size_t sizeIndicesBuf = sizeof(int) * pTransformAccessReadOnly.Index + sizeof(int);

        void* pMatriciesBuf = malloc(sizeMatriciesBuf);
        void* pIndicesBuf = malloc(sizeIndicesBuf);

        if (pMatriciesBuf && pIndicesBuf)
        {
            if (transformData.TransformArray == 0 || transformData.TransformIndices == 0)
                return Vector3();

            readsize(transformData.TransformArray, pMatriciesBuf, sizeMatriciesBuf);
            readsize(transformData.TransformIndices, pIndicesBuf, sizeIndicesBuf);

            __m128 Result = *reinterpret_cast<__m128*>(reinterpret_cast<uint64_t>(pMatriciesBuf) + (0x30 * pTransformAccessReadOnly.Index));
            int TransformIndex = *reinterpret_cast<int*>((reinterpret_cast<uint64_t>(pIndicesBuf) + (0x4 * pTransformAccessReadOnly.Index)));

            while (TransformIndex >= 0 && TransformIndex < (sizeMatriciesBuf / 0x30))
            {
                Matrix3x4 Matrix = *reinterpret_cast<Matrix3x4*>(reinterpret_cast<uint64_t>(pMatriciesBuf) + (0x30 * TransformIndex));
                __m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0x00));	// xxxx
                __m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0x55));	// yyyy
                __m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0x8E));	// zwxy
                __m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0xDB));	// wzyw
                __m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0xAA));	// zzzz
                __m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0x71));	// yxwy
                __m128 tmp7 = _mm_mul_ps(*reinterpret_cast<__m128*>(&Matrix._31), Result);

                Result = _mm_add_ps(
                    _mm_add_ps(
                        _mm_add_ps(
                            _mm_mul_ps(
                                _mm_sub_ps(
                                    _mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
                                    _mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
                                _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
                            _mm_mul_ps(
                                _mm_sub_ps(
                                    _mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
                                    _mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
                                _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
                        _mm_add_ps(
                            _mm_mul_ps(
                                _mm_sub_ps(
                                    _mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
                                    _mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
                                _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
                            tmp7)), *(__m128*)(&Matrix._11));

                TransformIndex = *reinterpret_cast<int*>(reinterpret_cast<uint64_t>(pIndicesBuf) + (0x4 * TransformIndex));
            }

            return Vector3{ Result.m128_f32[0], Result.m128_f32[1], Result.m128_f32[2] };
        }

        return {};
    }

    Vector3 GetBoneLocation(int boneIndex) {
        uint64_t player_model = read<uint64_t>((uint64_t)this + MODEL);
        if (!player_model)
            return Vector3();

        uint64_t boneTransforms = read<uint64_t>(player_model + BONETRANSFORM);
        if (!boneTransforms)
            return Vector3();

        uint64_t BoneValue = read<uint64_t>((boneTransforms + (0x20 + (boneIndex * 0x8))));
        if (!BoneValue)
            return Vector3();

        return GetTransformPosition(BoneValue);
    }

    PlayerEyes* GetPlayerEyes() {
        uint64_t Playereyes = decrypt_inventory_pointer(read<uint64_t>((uint64_t)this + PLAYEREYES));
        if (!Playereyes)
            return 0;

        return (PlayerEyes*)Playereyes;
    }

    PlayerInput* GetPlayerInput() {
        uint64_t Playerinput = read<uint64_t>((uint64_t)this + PLAYERINPUT); // public PlayerInput input;
        if (!Playerinput)
            return 0;

        return (PlayerInput*)Playerinput;
    }

    PlayerMovement* GetPlayerMovement() {
        uint64_t Movement = read<uint64_t>((uint64_t)this + PLAYERMOVEMENT);
        if (!Movement)
            return 0;

        return (PlayerMovement*)Movement;
    }

    BasePlayer* GetHeldItem() {
        auto player_inventory_encrypted = read<uint64_t>((uint64_t)this + PLAYERINVENTORY);  // offsets::base_player::playerinventory
        if (!player_inventory_encrypted)
            return 0;

        auto player_inventory = decrypt_inventory_pointer(player_inventory_encrypted);
        if (!player_inventory)
            return 0;

        auto inventory_belt = read<uint64_t>(player_inventory + CONTAINERBELT);  // container belt
        if (!inventory_belt)
            return 0;

        auto belt_contents_list = read<uint64_t>(inventory_belt + ITEMLIST);  // offsets::item::ItemList
        if (!belt_contents_list)
            return 0;

        auto belt_contents = read<uint64_t>(belt_contents_list + 0x10);  // offsets::item::Items
        if (!belt_contents)
            return 0;

        auto active_item_id = read<uint64_t>((uint64_t)this + CLACTIVEITEM);  // offsets::base_player::clActiveItem
        if (!active_item_id)
            return 0;

        auto decrypted_item_id = decrypt_ulong_key(active_item_id);
        if (!decrypted_item_id)
            return 0;

        for (int i = 0; i < 6; ++i) {
            auto current_item = read<uint64_t>(belt_contents + 0x20 + (i * 8));
            if (!current_item)
                continue;

            auto current_item_id = read<uint64_t>(current_item + ITEMUID);
            if (!current_item_id)
                continue;

            if (decrypted_item_id == current_item_id) {
                return (BasePlayer*)current_item;
            }
        }

        return 0;
    }
    BasePlayer* GetBaseProjectile() {
        uint64_t unk = read<uint64_t>((uint64_t)this + HELDENTITY); // public HeldItem
        if (!unk)
            return 0;

        return (BasePlayer*)unk;
    }
    uint64_t GetPrimaryMagazine() {
        uint64_t unk = read<uint64_t>((uint64_t)this + PRIMARYMAGAZINE); // public HeldItem
        if (!unk)
            return 0;

        return unk;
    }
    BasePlayer* GetRecoilProperties() {
        uint64_t RecoilProperties = read<uint64_t>((uint64_t)this + RECOILPROPERTIES);
        if (!RecoilProperties)
            return 0;

        return (BasePlayer*)RecoilProperties;
    }
    BasePlayer* GetRecoilOverride() {
        uint64_t RecoilOverride = read<uint64_t>((uint64_t)this + NEWRECOILOVERIDE);
        if (!RecoilOverride)
            return 0;

        return (BasePlayer*)RecoilOverride;
    }

    std::string get_item_name()
    {
        uint64_t unk = read<uint64_t>((uint64_t)this + ITEMDEFINITION); // public ItemDefinition info;
        if (!unk)
            return std::string();

        unk = read<uint64_t>(unk + 0x28); //  public string shortname
        if (!unk)
            return std::string();

        std::wstring item_name = read_wstring(unk + 0x14, 64);
        return std::string(item_name.begin(), item_name.end());
    }

    bool is_weapon(std::string ItemName) {
        static const std::vector<std::string> weaponitems = {
            (std::string)skCrypt("minigun"),
            (std::string)skCrypt("rifle"),
            (std::string)skCrypt("pistol"),
            (std::string)skCrypt("bow"),
            (std::string)skCrypt("lmg"),
            (std::string)skCrypt("shotgun"),
            (std::string)skCrypt("smg")
        };
        for (const auto& item : weaponitems) {
            if (ItemName.find(skCrypt("rocket")) != std::string::npos)
                return false;
            if (ItemName.find(item) != std::string::npos)
                return true;
        }
        return false;
    }

    bool is_meele(std::string ItemName) {
        static const std::vector<std::string> meele_items = {
            (std::string)skCrypt("rock"),
            (std::string)skCrypt("hatchet"),
            (std::string)skCrypt("stone.pickaxe"),
            (std::string)skCrypt("stonehatchet"),
            (std::string)skCrypt("pickaxe"),
            (std::string)skCrypt("hammer.salvage"),
            (std::string)skCrypt("icepick.salvag"),
            (std::string)skCrypt("axe.salvaged"),
            (std::string)skCrypt("pitchfork"),
            (std::string)skCrypt("mace"),
            (std::string)skCrypt("spear.stone"),
            (std::string)skCrypt("spear.wooden"),
            (std::string)skCrypt("machete"),
            (std::string)skCrypt("bone.club"),
            (std::string)skCrypt("paddle"),
            (std::string)skCrypt("salvaged.sword"),
            (std::string)skCrypt("salvaged.cleav"),
            (std::string)skCrypt("knife.combat"),
            (std::string)skCrypt("knife.butcher"),
            (std::string)skCrypt("knife.bone"),
            (std::string)skCrypt("hammer"),
            (std::string)skCrypt("torch"),
            (std::string)skCrypt("sickle")
        };

        for (const auto& item : meele_items) {
            if (ItemName.find(item) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    uint64_t GetItemsList() {
        uint64_t Inventory = decrypt_inventory_pointer(read<uint64_t>((uint64_t)this + PLAYERINVENTORY));
        if (!Inventory)
            return 0;

        uint64_t Belt = read<uint64_t>(Inventory + CONTAINERBELT);
        if (!Belt)
            return 0;

        uint64_t ItemList = read<uint64_t>(Belt + ITEMLIST);
        if (!ItemList)
            return 0;

        uint64_t LastItemList = read<uint64_t>(ItemList + 0x10);
        if (!LastItemList)
            return 0;

        return LastItemList;
    }

    uint64_t GetClothingItemsList() {
        uint64_t Inventory = decrypt_inventory_pointer(read<uint64_t>((uint64_t)this + PLAYERINVENTORY));
        if (!Inventory)
            return 0;

        uint64_t Belt = read<uint64_t>(Inventory + CONTAINERWEAR);
        if (!Belt)
            return 0;

        uint64_t ItemList = read<uint64_t>(Belt + ITEMLIST);
        if (!ItemList)
            return 0;

        uint64_t LastItemList = read<uint64_t>(ItemList + 0x10);
        if (!LastItemList)
            return 0;

        return LastItemList;
    }

    Belt GetBeltItems(uint64_t ItemsList, int Slot) {
        if (!ItemsList)
            return Belt();

        uint64_t SlotItem = read<uint64_t>(ItemsList + ((Slot) * 8) + 0x20);
        if (!SlotItem)
            return Belt();

        uint64_t Item1Info = read<uint64_t>(SlotItem + ITEMDEFINITION);
        if (!Item1Info)
            return Belt();

        uint64_t ItemDisplayName = read<uint64_t>(Item1Info + 0x28);
        if (!ItemDisplayName)
            return Belt();

        int ItemAmmo = read<int>(SlotItem + ITEMCOUNT);
        int WeaponAmmo = read<int>(SlotItem + ITEMWEAPONAMMO);
        float health = read<float>(SlotItem + ITEMHEALTH);
        float MAXHEALTH = read<float>(SlotItem + ITEMMAXHEALTH);

        std::wstring Name = read_wstring(ItemDisplayName + 0x14, 128);
        std::string output(Name.begin(), Name.end());

        return { output, ItemAmmo, WeaponAmmo, health, MAXHEALTH };
    }

    Cloth GetClothingItems(uint64_t ItemsList, int Slot) {
        if (!ItemsList)
            return Cloth();

        constexpr uint64_t SlotOffsets[] = { 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50 };
        if ((Slot + 1) < 1 || (Slot + 1) > 7)
            return Cloth();

        uint64_t SlotItem = read<uint64_t>(ItemsList + SlotOffsets[(Slot + 1) - 1]);
        if (!SlotItem)
            return Cloth();

        uint64_t Item1Info = read<uint64_t>(SlotItem + ITEMDEFINITION);
        if (!Item1Info)
            return Cloth();

        uint64_t ItemDisplayName = read<uint64_t>(Item1Info + 0x28);
        if (!ItemDisplayName)
            return Cloth();

        float health = read<float>(SlotItem + ITEMHEALTH);
        float MAXHEALTH = read<float>(SlotItem + ITEMMAXHEALTH);

        std::wstring Name = read_wstring(ItemDisplayName + 0x14, 128);
        std::string output(Name.begin(), Name.end());

        return { output, health, MAXHEALTH };
    }

    void SetAimCone(float value)
    {
        write<float>((uint64_t)this + 0x60, value); // public float aimconeCurveScale;
    }
    void SetAutomatic(bool value)
    {
        write<bool>((uint64_t)this + 0x2C8, value); // public bool automatic;
    }
    void SuperEoka(bool value)
    {
         write<float>((uint64_t)this + 0x3D0, 100.f); // successFraction
         write<bool>((uint64_t)this + 0x3E0, value); // didSparkThisFrame // private bool %96b303fba5fdfba285c48d09d0d57dd9fa2132e7; // 0x3B9
    }
    void SuperMeele(bool value)
    {
        write<float>((uint64_t)this + 0x2D0, 3.0f); // public float maxDistance;
        write<float>((uint64_t)this + 0x2D4, 1.0f); // public float attackRadius;
        write<bool>((uint64_t)this + 0x2D9, false); // public bool blockSprintOnAttack;
        write<bool>((uint64_t)this + 0x313, false); // public bool onlyThrowAsProjectile;
        write<bool>((uint64_t)this + 0x310, true); // private bool throwReady; //private bool %21ea6d3ef7643807643d828691159ff09b6eeadd; // 0x2F8
    }
    void SetSpread(float value)
    {
        write<float>((uint64_t)this + 0x318, value); // aimSway
        write<float>((uint64_t)this + 0x334, value); // hpaimCone
        write<float>((uint64_t)this + 0x330, value); // aimCone
        write<float>((uint64_t)this + 0x348, value); // stancePenaltyScale
        write<float>((uint64_t)this + 0x33C, value); // aimConePenaltyMax; 
        write<float>((uint64_t)this + 0x338, value); // aimconePenaltyPerShot;
    }
    void SetRecoil(float value)
    {
        write<float>((uint64_t)this + 0x18, value);
        write<float>((uint64_t)this + 0x1C, value);
        write<float>((uint64_t)this + 0x20, value);
        write<float>((uint64_t)this + 0x24, value);
    }
};


enum BoneList : int {
    head = 47,
    eye = 49,
    neck = 46,
    l_upperarm = 24,
    r_upperarm = 55,
    l_forearm = 25,
    r_forearm = 56,
    l_hand = 26,
    r_hand = 57,
    spine4 = 22,
    pelvis = 19,
    l_hip = 1,
    r_hip = 13,
    l_knee = 2,
    r_knee = 14,
    l_foot = 3,
    r_foot = 15,
};

struct sBasePlayer
{
    BasePlayer* Player;
    bool Scientist;
};

struct sPlayerDraw
{
    Vector3 head;
    Vector3 eye;
    Vector3 neck;
    Vector3 l_upperarm;
    Vector3 r_upperarm;
    Vector3 l_forearm;
    Vector3 r_forearm;
    Vector3 l_hand;
    Vector3 r_hand;
    Vector3 spine4;
    Vector3 pelvis;
    Vector3 l_hip;
    Vector3 r_hip;
    Vector3 l_knee;
    Vector3 r_knee;
    Vector3 l_foot;
    Vector3 r_foot;
};

template <typename Type>
struct ThreadVec
{
    std::vector<Type> vec;
    std::mutex mutex;
};

struct sVectorPush
{
    sBasePlayer Player;
    sPrefabList Prefab;
    bool Playerbool;
};
ThreadVec<sVectorPush> EntityVectorPool = {};

class BaseNetworkable
{
public:
    static BaseNetworkable* GetEntity()
    {
        uint64_t basenetworkable = read<uint64_t>(GameAssembly + BASENETWORKABLE);
        if (!basenetworkable)
            return 0;

        basenetworkable = read<uint64_t>(basenetworkable + 0xB8);
        if (!basenetworkable)
            return 0;

        basenetworkable = read<uint64_t>(basenetworkable + 0x20);
        if (!basenetworkable)
            return 0;

        basenetworkable = networkable_key(basenetworkable);
        if (!basenetworkable)
            return 0;   

        basenetworkable = read<uint64_t>(basenetworkable + 0x10);
        if (!basenetworkable)
            return 0;

        basenetworkable = networkable_key(basenetworkable);
        if (!basenetworkable)
            return 0;

        basenetworkable = read<uint64_t>(basenetworkable + 0x10);
        if (!basenetworkable)
            return 0;

        return (BaseNetworkable*)basenetworkable;
    }
    int32_t GetSize()
    {
        int32_t entity_size = read<int32_t>((uint64_t)this + 0x18);
        if (!entity_size)
            return 0;

        return entity_size;
    }
    BaseNetworkable* GetArray()
    {
        uint64_t entity_array = read<uint64_t>((uint64_t)this + 0x10);
        if (!entity_array)
            return 0;

        return (BaseNetworkable*)entity_array;
    }
    BaseNetworkable* GetEntity(int i)
    {
        uint64_t entity = read<uint64_t>((uint64_t)this + 0x20 + (i * 0x08));
        if (!entity)
            return 0;

        return (BaseNetworkable*)entity;
    }
    BaseNetworkable* GetEntityObject()
    {
        uint64_t EntityObject = read<uint64_t>((uint64_t)this + 0x10);
        if (!EntityObject)
            return 0;

        return (BaseNetworkable*)EntityObject;
    }
    uint64_t GetEntityPosAgo()
    {
        uint64_t EntityPosAgo = readchain<uint64_t>((uint64_t)this, { 0x30, 0x30, 0x8, 0x38 });
        if (!EntityPosAgo)
            return 0;

        return EntityPosAgo;
    }
    Vector3 GetEntityPosition()
    {
        Vector3 EntityPos = read<Vector3>((uint64_t)this + 0x90);
        if (EntityPos.empty())
            return Vector3();

        return (Vector3)EntityPos;
    }
    BaseNetworkable* GetGameObject()
    {
        uint64_t GameObject = read<uint64_t>((uint64_t)this + 0x30);
        if (!GameObject)
            return 0;

        return (BaseNetworkable*)GameObject;
    }
    std::string GetGameBuff()
    {
        uint64_t GameBuff = read<uint64_t>((uint64_t)this + 0x60);
        if (!GameBuff)
            return std::string();

        return ReadChar(GameBuff);
    }
    BasePlayer* GetBasePlayer()
    {
        uint64_t Player = read<uint64_t>((uint64_t)this + 0x28);
        if (!Player)
            return 0;

        return (BasePlayer*)Player;
    }
    BasePlayer* GetLocalPlayer()
    {
        uint64_t localplayer = read<uint64_t>((uint64_t)this + 0x20 + (0 * 0x8));
        if (!localplayer)
            return 0;

        return (BasePlayer*)localplayer;
    }

}; BasePlayer* Local_Player = 0;


std::map<std::wstring, float> bullets =
{
    {  std::wstring(skCrypt(L"ammo.rifle")), 1.0f },
    {  std::wstring(skCrypt(L"ammo.rifle.hv")), 1.2f },
    {  std::wstring(skCrypt(L"ammo.rifle.explosive")), 0.49f },
    {  std::wstring(skCrypt(L"ammo.rifle.incendiary")), 0.55f },
    {  std::wstring(skCrypt(L"ammo.pistol")), 1.0f },
    {  std::wstring(skCrypt(L"ammo.pistol.hv")), 1.33333f },
    {  std::wstring(skCrypt(L"ammo.pistol.fire")), 0.75f },
    {  std::wstring(skCrypt(L"arrow.wooden")), 1.0f },
    {  std::wstring(skCrypt(L"arrow.hv")), 1.6f },
    {  std::wstring(skCrypt(L"arrow.fire")), 0.8f },
    {  std::wstring(skCrypt(L"arrow.bone")), 0.9f },
    {  std::wstring(skCrypt(L"ammo.handmade.shell")), 1.0f },
    {  std::wstring(skCrypt(L"ammo.shotgun.slug")), 2.25f },
    {  std::wstring(skCrypt(L"ammo.shotgun.fire")), 1.0f },
    {  std::wstring(skCrypt(L"ammo.shotgun")), 2.25f },
    {  std::wstring(skCrypt(L"ammo.nailgun.nails")), 1.f }
};

float ProjectileSpeed_Normal(float dist)
{
    BasePlayer* HELD = Local_Player->GetHeldItem();
    const float default_speed = 300.0f;
    std::string HandWeapon = HELD->get_item_name();
    if (HandWeapon.empty())
        return default_speed;

    uint64_t primary_magazine = HELD->GetBaseProjectile()->GetPrimaryMagazine();
    if (!primary_magazine)
        return default_speed;

    uint64_t Ammo = read<uint64_t>(primary_magazine + 0x20);
    if (!Ammo)
        return default_speed;

    uint64_t Unk3 = read<uint64_t>(Ammo + 0x28);
    if (!Unk3)
        return default_speed;

    std::wstring magazine_shortname = read_wstring(Unk3 + 0x14, 64);
    if (magazine_shortname.empty())
        return default_speed;

    const float bullet_multiplier = bullets[magazine_shortname];

    if (HandWeapon == std::string(skCrypt("rifle.ak")) || HandWeapon == std::string(skCrypt("rifle.ak.ice")) || HandWeapon == std::string(skCrypt("rifle.ak.diver")))
    {

        if (dist <= 0.f)
        {
            return 375.9f * bullet_multiplier;
        }
        else
            if (dist <= 150.f)
            {
                return 386.f * bullet_multiplier; //AK
            }
            else
                if (dist <= 200.f)
                {
                    return 390.f * bullet_multiplier; //AK
                }
                else
                    if (dist <= 250.f)
                    {
                        return 400.f * bullet_multiplier; //AK
                    }
                    else
                        if (dist <= 300.0f)
                        {
                            return 410.f * bullet_multiplier; //AK
                        }
    }
    if (HandWeapon == std::string(skCrypt("hmlmg"))) return 500 * bullet_multiplier; //500 d 
    if (HandWeapon == std::string(skCrypt("rifle.lr300"))) return 340.f * bullet_multiplier;	   //FULL FIXLEDIM
    if (HandWeapon == std::string(skCrypt("rifle.bolt"))) return 579.f * bullet_multiplier;		   //FULL FIXLEDIM
    if (HandWeapon == std::string(skCrypt("rifle.l96"))) return 1126.0f * bullet_multiplier;	   //FULL FIXLEDIM
    if (HandWeapon == std::string(skCrypt("rifle.m39"))) return 445.f * bullet_multiplier;		   //FULL FIXLEDIM
    if (HandWeapon == std::string(skCrypt("rifle.semiauto")) || HandWeapon == std::string(skCrypt("rifle.sks"))) return 358.0f * bullet_multiplier;   //FULL FIXLEDIM
    if (HandWeapon == std::string(skCrypt("lmg.m249"))) return 450.f * bullet_multiplier;		   //FULL FIXLEDIM
    if (HandWeapon == std::string(skCrypt("smg.thompson")))
    {
        if (dist <= 180.f)
        {
            return 270.f * bullet_multiplier;
        }
        else
            if (dist <= 204.f)
            {
                return 250.f * bullet_multiplier;
            }
            else
                if (dist <= 250.f)
                {
                    return 245.f * bullet_multiplier;
                }
                else
                    if (dist <= 350.f)
                    {
                        return 230.f * bullet_multiplier;
                    }
    }							                              //FULL FIXLEDIM
    if (HandWeapon == std::string(skCrypt("smg.2")))
    {
        if (dist <= 160.f)
        {
            return 220.f * bullet_multiplier;
        }
        else
            if (dist <= 200.f)
            {
                return 195.f * bullet_multiplier;
            }
    }
    if (HandWeapon == std::string(skCrypt("smg.mp5"))) return 240.f * bullet_multiplier;  //220
    if (HandWeapon == std::string(skCrypt("pistol.prototype17"))) //FULL FIXLEDIM
    {
        if (dist <= 110.f)
        {
            return 300.f * bullet_multiplier;
        }
        else
            if (dist <= 169.f)
            {
                return 290.f * bullet_multiplier;
            }
            else
                if (dist <= 170.f)
                {
                    return 280.f * bullet_multiplier;
                }
                else
                    if (dist <= 180.f)
                    {
                        return 275.f * bullet_multiplier;
                    }
                    else
                        if (dist <= 190.f)
                        {
                            return 270.f * bullet_multiplier;
                        }
                        else
                            if (dist <= 200.f)
                            {
                                return 265.f * bullet_multiplier;
                            }
    }
    if (HandWeapon == std::string(skCrypt("pistol.python"))) return 300.f * bullet_multiplier;
    if (HandWeapon == std::string(skCrypt("pistol.semiauto"))) return 300.f * bullet_multiplier;
    if (HandWeapon == std::string(skCrypt("pistol.revolver"))) return 270.f * bullet_multiplier;
    if (HandWeapon == std::string(skCrypt("pistol.m92"))) return 300.f * bullet_multiplier;
    if (HandWeapon == std::string(skCrypt("pistol.eoka"))) return 90.f * bullet_multiplier;
    if (HandWeapon == std::string(skCrypt("pistol.nailgun")))
    {
        if (dist <= 60.f)
        {
            return 59.f * bullet_multiplier;
        }
        else
            if (dist <= 85.f)
            {
                return 58.3f * bullet_multiplier;
            }
            else
                return 57.8f * bullet_multiplier;
    }
    if (HandWeapon == std::string(skCrypt("crossbow"))) {
        if (dist <= 83.f)
        {
            return 90.f * bullet_multiplier;
        }
        else
            if (dist <= 100.f)
            {
                return 88.f * bullet_multiplier;
            }
            else
                if (dist <= 150.f)
                {
                    return 86.f * bullet_multiplier;
                }
                else
                    return 86.f * bullet_multiplier;
    }
    if (HandWeapon == std::string(skCrypt("bow.compound")))
    {
        if (dist <= 90.f)
        {
            return 120.f * bullet_multiplier;
        }
        else
            if (dist <= 150.F)
            {
                return  115.76f * bullet_multiplier;
            }
            else
                return  115.5f * bullet_multiplier;

    }
    if (HandWeapon == std::string(skCrypt("bow.hunting")) || HandWeapon == std::string(skCrypt("legacy bow"))) {
        if (dist <= 41.f)
        {
            return 60.f * bullet_multiplier;
        }
        else
            if (dist <= 82.f)
            {
                return  58.f * bullet_multiplier;
            }
            else
                if (dist <= 102.f)
                {
                    return  57.5f * bullet_multiplier;
                }
                else
                    if (dist <= 112.f)
                    {
                        return  57.3f * bullet_multiplier;
                    }
                    else
                        if (dist <= 127.f)
                        {
                            return  57.f * bullet_multiplier;
                        }
                        else
                            if (dist <= 146.f)
                            {
                                return  56.5f * bullet_multiplier;
                            }
                            else
                                if (dist <= 153.f)
                                {
                                    return  56.3f * bullet_multiplier;
                                }
                                else
                                    if (dist <= 163.f)
                                    {
                                        return  56.f * bullet_multiplier;
                                    }
                                    else
                                        if (dist <= 172.f)
                                        {
                                            return  55.7f * bullet_multiplier;
                                        }
                                        else
                                            if (dist <= 178.f)
                                            {
                                                return  55.5f * bullet_multiplier;
                                            }
                                            else
                                                if (dist <= 184.f)
                                                {
                                                    return  55.3f * bullet_multiplier;
                                                }
                                                else
                                                    if (dist <= 189.f)
                                                    {
                                                        return  55.1f * bullet_multiplier;
                                                    }
                                                    else
                                                        if (dist <= 196.f)
                                                        {
                                                            return  54.9f * bullet_multiplier;
                                                        }
                                                        else
                                                            if (dist <= 201.f)
                                                            {
                                                                return  54.7f * bullet_multiplier;
                                                            }
                                                            else
                                                                if (dist <= 206.f)
                                                                {
                                                                    return  54.5f * bullet_multiplier;
                                                                }
                                                                else
                                                                    if (dist <= 210.f)
                                                                    {
                                                                        return  54.3f * bullet_multiplier;
                                                                    }
                                                                    else
                                                                        if (dist <= 215.f)
                                                                        {
                                                                            return 54.1f * bullet_multiplier;
                                                                        }
                                                                        else
                                                                            if (dist <= 220.f)
                                                                            {
                                                                                return  53.9f * bullet_multiplier;
                                                                            }
                                                                            else
                                                                                if (dist <= 225.1f)
                                                                                {
                                                                                    return 53.7f * bullet_multiplier;
                                                                                }
                                                                                else
                                                                                    if (dist <= 230.1f)
                                                                                    {
                                                                                        return  53.5f * bullet_multiplier;
                                                                                    }
                                                                                    else
                                                                                        if (dist <= 233.1f)
                                                                                        {
                                                                                            return  53.3f * bullet_multiplier;
                                                                                        }
                                                                                        else
                                                                                            if (dist <= 237.1f)
                                                                                            {
                                                                                                return  53.1f * bullet_multiplier;
                                                                                            }
                                                                                            else
                                                                                                if (dist <= 241.1f)
                                                                                                {
                                                                                                    return  52.9f * bullet_multiplier;
                                                                                                }
                                                                                                else
                                                                                                    if (dist <= 244.1f)
                                                                                                    {
                                                                                                        return  52.7f * bullet_multiplier;
                                                                                                    }
                                                                                                    else
                                                                                                        if (dist <= 248.1f)
                                                                                                        {
                                                                                                            return  52.5f * bullet_multiplier;
                                                                                                        }
                                                                                                        else
                                                                                                            if (dist <= 252.1f)
                                                                                                            {
                                                                                                                return  52.3f * bullet_multiplier;
                                                                                                            }
                                                                                                            else
                                                                                                                if (dist <= 255.1f)
                                                                                                                {
                                                                                                                    return  52.1f * bullet_multiplier;
                                                                                                                }
                                                                                                                else
                                                                                                                    if (dist <= 500.f)
                                                                                                                    {
                                                                                                                        return 50.f * bullet_multiplier;
                                                                                                                    }
    }
    if (HandWeapon == std::string(skCrypt("shotgun.pump"))) return 100.0f * bullet_multiplier;
    if (HandWeapon == std::string(skCrypt("shotgun.spas12"))) return 100.0f * bullet_multiplier;
    if (HandWeapon == std::string(skCrypt("shotgun.waterpipe"))) return 100.0f * bullet_multiplier;
    if (HandWeapon == std::string(skCrypt("shotgun.double"))) return 100.0f * bullet_multiplier;
    return default_speed;
}
Vector3 Prediction(Vector3 LP_Pos, Vector3 Velocity, Vector3 BonePos) {
    const float Dist = LP_Pos.distance(BonePos);
    if (Dist > 0.001f) {
        const float BulletTime = Dist / ProjectileSpeed_Normal(Dist);
        const Vector3 vel = Velocity;
        Vector3 PredictVel;
        PredictVel.x = vel.x * BulletTime * 0.75f;
        PredictVel.y = vel.y * BulletTime * 0.75f;
        PredictVel.z = vel.z * BulletTime * 0.75f;

        BonePos.x += PredictVel.x;
        BonePos.y += PredictVel.y;
        BonePos.z += PredictVel.z;
        BonePos.y += (4.905f * BulletTime * BulletTime);
    } return BonePos;
}

class GraphicFov
{
public:
    static GraphicFov* GetGraphicFov()
    {
        uint64_t Graphicfov = read<uint64_t>(GameAssembly + GRAPHICCONVAR);
        if (!Graphicfov)
            return 0;

        uint64_t field = read<uint64_t>(Graphicfov + 0xb8);
        if (!field)
            return 0;

        return (GraphicFov*)field;
    }
    void SetFov(float fov)
    {
        std::uint32_t decryptedfov = fovdecrypt(fov);
        if (!decryptedfov)
            return;

         write<std::uint32_t>((uint64_t)this + 0x490LL, decryptedfov);
    }
};  GraphicFov* Graphicfov = 0;

class TodSky
{
public:
    static TodSky* GetSky()
    {
        uint64_t todsky = read<uint64_t>(GameAssembly + TODSKY);
        if (!todsky)
            return 0;

        todsky = read<uint64_t>(todsky + 0xB8);
        if (!todsky)
            return 0;

        todsky = read<uint64_t>(todsky + 0x30);
        if (!todsky)
            return 0;
        
        todsky = read<uint64_t>(todsky + 0x10);
        if (!todsky)
            return 0;
        
        todsky = read<uint64_t>(todsky + 0x20);
        if (!todsky)
            return 0;

        return (TodSky*)todsky;
    }

    TodSky* TOD_CycleParameters()
    {
        uint64_t CycleParameters = read<uint64_t>((uint64_t)this + 0x40); // public TOD_CycleParameters Cycle;
        if (!CycleParameters)
            return 0;

        return (TodSky*)CycleParameters;
    }

    void TimeChanger(float value)
    {
            write<float>((uint64_t)this + 0x10, value);
    }
}; TodSky* Todsky = 0;

bool WorldToScreen(const Vector3& EntityPos, Vector3& ScreenPos)
{
    if (!pViewMatrix) {
        return false;
    }

    float w = EntityPos.x * pViewMatrix[3] + EntityPos.y * pViewMatrix[7] + EntityPos.z * pViewMatrix[11] + pViewMatrix[15];
    if (w < 0.098f) {
        return false;
    }

    float x = EntityPos.x * pViewMatrix[0] + EntityPos.y * pViewMatrix[4] + EntityPos.z * pViewMatrix[8] + pViewMatrix[12];
    float y = EntityPos.x * pViewMatrix[1] + EntityPos.y * pViewMatrix[5] + EntityPos.z * pViewMatrix[9] + pViewMatrix[13];

    ScreenPos.x = globals::ScreenMidSize.right * (1.f + x / w);
    ScreenPos.y = globals::ScreenMidSize.bottom * (1.f - y / w);
    return true;
}
