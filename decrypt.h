#pragma once
#define LODWORD(x)  (*((DWORD*)&(x)))
#define TEST_BITD(value, bit) (((value) & (1 << (bit))) != 0)
typedef unsigned int uint32;
#define _DWORD uint32

uint64_t __PAIR64__(uint32_t high, uint32_t low) {
    return (static_cast<uint64_t>(high) << 32) | low;
}

template<class T>
T __ROL__(T value, int count) {
    constexpr uint64_t nbits = sizeof(T) * 8;
    count = count % nbits;

    if (count > 0)
        value = (value << count) | (value >> (nbits - count));
    else
        value = (value >> -count) | (value << (nbits + count));

    return value;
}

uintptr_t Il2cppGetHandle(int32_t ObjectHandleID) {

    uint64_t rdi_1 = ObjectHandleID >> 3;
    uint64_t rcx_1 = (ObjectHandleID & 7) - 1;
    uint64_t baseAddr = GameAssembly + 0xD907AF0 + rcx_1 * 0x28;
    uint32_t limit = read<uint32_t>(baseAddr + 0x10);
    if (rdi_1 < limit) {
        uintptr_t objAddr = read<uintptr_t>(baseAddr);
        uint32_t bitMask = read<uint32_t>(objAddr + ((rdi_1 >> 5) << 2));
        if (TEST_BITD(bitMask, rdi_1 & 0x1f)) {
            uintptr_t ObjectArray = read<uintptr_t>(baseAddr + 0x8) + (rdi_1 << 3);
            return read<BYTE>(baseAddr + 0x14) > 1
                ? read<uintptr_t>(ObjectArray)
                : ~read<uint32_t>(ObjectArray);
        }
    }
    return 0;
}

uint64_t decrypt_ulong_key(uintptr_t encrypted_value)
{
    __m128* v4; // rdx
    int v5; // r8d
    int v6; // eax
    signed __int64 v7; // rcx
    __m128 v23; // [rsp+20h] [rbp-28h] BYREF
    memcpy(&v6, &encrypted_value, sizeof(encrypted_value));
    v4 = &v23;
    v5 = 2;
    memcpy(&v23, &encrypted_value, sizeof(encrypted_value));
    do
    {
        v6 = *(_DWORD*)v4;
        v4 = (__m128*)((char*)v4 + 4);
        *((_DWORD*)v4 - 1) = ((32 * (v6 + 8681491)) | ((unsigned int)(v6 + 8681491) >> 27)) + 2106984722;
        --v5;
    } while (v5);
    signed __int64 v8 = *reinterpret_cast<signed __int64*>(&v23);
    return (v8);
}

std::uint32_t fovdecrypt(float fov)
{
    unsigned int v1; // ecx
    __int64 result; // rax
    signed __int32 v3[8]; // [rsp+0h] [rbp-38h] BYREF
    __int64 v4; // [rsp+50h] [rbp+18h] 
    v1 = _mm_cvtsi128_si32(*(__m128i*) & fov);
    v4 = ((((v1 << 11) | (v1 >> 21)) ^ 0x2F63CE15) + 15988019) ^ 0x5986E25ALL;
    return v4;
}

uint64_t newinventory(uint64_t pointer)
{
    auto il2cpp_object = (std::int32_t)decrypt_ulong_key(read<std::uint64_t>(pointer + 0x18));
    return il2cpp_object;
}

uint64_t decrypt_inventory_pointer(uint64_t pointer)
{
    return Il2cppGetHandle(newinventory(pointer));
}

uint64_t networkable_key(uint64_t pointer) {
    __m128* v1; // rax
    int v2; // r9d
    int v3; // ecx
    int v4; // r8d
    __m128 v6; // [rsp+0h] [rbp-18h] BYREF
    v1 = &v6;
    v2 = 2;
    v6 = read<__m128>(pointer + 0x18);
    do
    {
        v3 = *(_DWORD*)v1;
        v4 = *((unsigned __int16*)v1 + 1);
        v1 = (__m128*)((char*)v1 + 4);
        *((_DWORD*)v1 - 1) = ((4 * (((v3 << 16) | v4) ^ 0xA0A52430)) | ((unsigned __int64)(((v3 << 16) | v4) ^ 0xA0A52430) >> 30))
            + 1215837599;
        --v2;
    } while (v2);
    signed __int64 v8 = *reinterpret_cast<signed __int64*>(&v6);
    return Il2cppGetHandle(v8);
}
