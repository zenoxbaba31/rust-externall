#pragma once 
#include <ntdef.h>
#include <ntifs.h>
#include <ntddk.h>
#include <minwindef.h>
#include <intrin.h>
#include "API.h"
#include "importes.h"

NTSTATUS ReadPhysicalAddress(uint64_t TargetAddress, PVOID lpBuffer, SIZE_T Size, SIZE_T* BytesRead)
{
    if (!TargetAddress || !Size)
        return STATUS_UNSUCCESSFUL;
    MM_COPY_ADDRESS AddrToRead = { 0 };
    AddrToRead.PhysicalAddress.QuadPart = TargetAddress;
    return MmCopyMemory(lpBuffer, AddrToRead, Size, MM_COPY_MEMORY_PHYSICAL, BytesRead);
}

typedef union _virt_addr_t
{
    void* value;
    struct
    {
        uintptr_t offset : 12;
        uintptr_t pt_index : 9;
        uintptr_t pd_index : 9;
        uintptr_t pdpt_index : 9;
        uintptr_t pml4_index : 9;
        uintptr_t reserved : 16;
    };
} virt_addr_t, * pvirt_addr_t;

namespace pml4
{
    uint64_t savedbaseadd;
    uint32_t pid_save;
    bool cr3_check;

    PVOID split_memory(PVOID SearchBase, SIZE_T SearchSize, const void* Pattern, SIZE_T PatternSize)
    {

        const UCHAR* searchBase = static_cast<const UCHAR*>(SearchBase);
        const UCHAR* pattern = static_cast<const UCHAR*>(Pattern);

        for (SIZE_T i = 0; i <= SearchSize - PatternSize; ++i) {
            SIZE_T j = 0;
            for (; j < PatternSize; ++j) {
                if (searchBase[i + j] != pattern[j])
                    break;
            }

            if (j == PatternSize)
                return const_cast<UCHAR*>(&searchBase[i]);
        }
        return nullptr;
    }
    void* g_mmonp_MmPfnDatabase;

    static NTSTATUS InitializeMmPfnDatabase()
    {
        struct MmPfnDatabaseSearchPattern
        {
            const UCHAR* bytes;
            SIZE_T bytes_size;
            bool hard_coded;
        };

        MmPfnDatabaseSearchPattern patterns;

        static const UCHAR kPatternWin10x64[] = {
            0x48, 0x8B, 0xC1,
            0x48, 0xC1, 0xE8, 0x0C,
            0x48, 0x8D, 0x14, 0x40,
            0x48, 0x03, 0xD2,
            0x48, 0xB8,
        };

        patterns.bytes = kPatternWin10x64;
        patterns.bytes_size = sizeof(kPatternWin10x64);
        patterns.hard_coded = true;

        const auto p_MmGetVirtualForPhysical = reinterpret_cast<UCHAR*>(MmGetVirtualForPhysical);
        if (!p_MmGetVirtualForPhysical) {
            return STATUS_PROCEDURE_NOT_FOUND;
        }

        auto found = reinterpret_cast<UCHAR*>(split_memory(p_MmGetVirtualForPhysical, 0x20, patterns.bytes, patterns.bytes_size));
        if (!found) {
            return STATUS_UNSUCCESSFUL;
        }

        found += patterns.bytes_size;
        if (patterns.hard_coded) {
            g_mmonp_MmPfnDatabase = *reinterpret_cast<void**>(found);
        }
        else {
            const auto mmpfn_address = *reinterpret_cast<ULONG_PTR*>(found);
            g_mmonp_MmPfnDatabase = *reinterpret_cast<void**>(mmpfn_address);
        }

        g_mmonp_MmPfnDatabase = PAGE_ALIGN(g_mmonp_MmPfnDatabase);

        return STATUS_SUCCESS;
    }

    uintptr_t dirbase_from_base_address(void* base)
    {
        if (!NT_SUCCESS(InitializeMmPfnDatabase()))
        	return 0;
        virt_addr_t virt_base{}; virt_base.value = base;
        size_t read{};
        auto ranges = MmGetPhysicalMemoryRanges();
        for (int i = 0;; i++) {

            auto elem = &ranges[i];

            if (!elem->BaseAddress.QuadPart || !elem->NumberOfBytes.QuadPart)
                break;

            uintptr_t current_phys_address = elem->BaseAddress.QuadPart;
            for (int j = 0; j < (elem->NumberOfBytes.QuadPart / 0x1000); j++, current_phys_address += 0x1000) {

                _MMPFN* pnfinfo = (_MMPFN*)((uintptr_t)g_mmonp_MmPfnDatabase + (current_phys_address >> 12) * sizeof(_MMPFN));

                if (pnfinfo->u4.PteFrame == (current_phys_address >> 12)) {
                    MMPTE pml4e{};
                    if (!NT_SUCCESS(ReadPhysicalAddress(current_phys_address + 8 * virt_base.pml4_index, &pml4e, 8, &read)))
                        continue;

                    if (!pml4e.u.Hard.Valid)
                        continue;

                    MMPTE pdpte{};
                    if (!NT_SUCCESS(ReadPhysicalAddress((pml4e.u.Hard.PageFrameNumber << 12) + 8 * virt_base.pdpt_index, &pdpte, 8, &read)))
                        continue;

                    if (!pdpte.u.Hard.Valid)
                        continue;

                    MMPTE pde{};
                    if (!NT_SUCCESS(ReadPhysicalAddress((pdpte.u.Hard.PageFrameNumber << 12) + 8 * virt_base.pd_index, &pde, 8, &read)))
                        continue;

                    if (!pde.u.Hard.Valid)
                        continue;

                    MMPTE pte{};
                    if (!NT_SUCCESS(ReadPhysicalAddress((pde.u.Hard.PageFrameNumber << 12) + 8 * virt_base.pt_index, &pte, 8, &read)))
                        continue;

                    if (!pte.u.Hard.Valid)
                        continue;

                    return current_phys_address;
                }
            }
        }
        return 0;
    }
}

NTSTATUS WritePhysicalAddress(PVOID TargetAddress, PVOID lpBuffer, SIZE_T Size, SIZE_T* BytesWritten)
{
    if (!TargetAddress || !Size)
        return STATUS_UNSUCCESSFUL;
    PHYSICAL_ADDRESS AddrToWrite = { 0 };
    AddrToWrite.QuadPart = (LONGLONG)TargetAddress;
    PVOID pmapped_mem = MmMapIoSpaceEx(AddrToWrite, Size, PAGE_READWRITE);
    if (!pmapped_mem) return STATUS_UNSUCCESSFUL;
    memcpy(pmapped_mem, lpBuffer, Size);
    *BytesWritten = Size;
    MmUnmapIoSpace(pmapped_mem, Size);
    return STATUS_SUCCESS;
}



uintptr_t handle_get_base_address(int processpid)
{
    PEPROCESS process = nullptr;
    NTSTATUS  status = PsLookupProcessByProcessId(HANDLE(processpid), &process);

    if (!NT_SUCCESS(status))
        return 0;

    const auto base_address = uintptr_t(PsGetProcessSectionBaseAddress(process));
    ObDereferenceObject(process);

    return base_address;
}
//
//uint64_t translate_linear(uint64_t directory_base, uint64_t address) {
//
//    directory_base &= ~0xf;
//
//    auto virt_addr = address & ~(~0ul << 12);
//    auto pte = ((address >> 12) & (0x1ffll));
//    auto pt = ((address >> 21) & (0x1ffll));
//    auto pd = ((address >> 30) & (0x1ffll));
//    auto pdp = ((address >> 39) & (0x1ffll));
//    auto p_mask = ((~0xfull << 8) & 0xfffffffffull);
//
//    size_t readsize = 0;
//    uint64_t pdpe = 0;
//    ReadPhysicalAddress(directory_base + 8 * pdp, &pdpe, sizeof(pdpe), &readsize);
//    if (~pdpe & 1) {
//        return 0;
//    }
//
//    uint64_t pde = 0;
//    ReadPhysicalAddress((pdpe & p_mask) + 8 * pd, &pde, sizeof(pde), &readsize);
//    if (~pde & 1) {
//        return 0;
//    }
//
//    if (pde & 0x80)
//        return (pde & (~0ull << 42 >> 12)) + (address & ~(~0ull << 30));
//
//    uint64_t pteAddr = 0;
//    ReadPhysicalAddress((pde & p_mask) + 8 * pt, &pteAddr, sizeof(pteAddr), &readsize);
//    if (~pteAddr & 1) {
//        return 0;
//    }
//
//    if (pteAddr & 0x80) {
//        return (pteAddr & p_mask) + (address & ~(~0ull << 21));
//    }
//
//    address = 0;
//    ReadPhysicalAddress((pteAddr & p_mask) + 8 * pte, &address, sizeof(address), &readsize);
//    address &= p_mask;
//
//    if (!address) {
//        return 0;
//    }
//
//    return address + virt_addr;
//}

uint64_t savedbaseadd;
HANDLE pid_save;
bool cr3_check;

uint64_t translate_linear(uint64_t directory_base, uint64_t address) {
    directory_base &= ~0xfull;

    const uint64_t virt_addr = address & 0xFFF;
    const uint64_t pte = (address >> 12) & 0x1FF;
    const uint64_t pt = (address >> 21) & 0x1FF;
    const uint64_t pd = (address >> 30) & 0x1FF;
    const uint64_t pdp = (address >> 39) & 0x1FF;
    const uint64_t p_mask = (~0xfull << 8) & 0xfffffffffull;

    size_t readsize = 0;
    uint64_t pdpe = 0;
    NTSTATUS status = ReadPhysicalAddress(directory_base + 8 * pdp, &pdpe, sizeof(pdpe), &readsize);
    if (!NT_SUCCESS(status) || !(pdpe & 1)) {
        savedptr = pml4::dirbase_from_base_address((void*)savedbaseadd);
        return 0;
    }

    uint64_t pde = 0;
    status = ReadPhysicalAddress((pdpe & p_mask) + 8 * pd, &pde, sizeof(pde), &readsize);
    if (!NT_SUCCESS(status) || !(pde & 1)) {
        savedptr = pml4::dirbase_from_base_address((void*)savedbaseadd);
        return 0;
    }

    if (pde & 0x80) {
        return (pde & (~0ull << 42 >> 12)) + (address & ~(~0ull << 30));
    }

    uint64_t pteAddr = 0;
    status = ReadPhysicalAddress((pde & p_mask) + 8 * pt, &pteAddr, sizeof(pteAddr), &readsize);
    if (!NT_SUCCESS(status) || !(pteAddr & 1)) {
        savedptr = pml4::dirbase_from_base_address((void*)savedbaseadd);
        return 0;
    }

    if (pteAddr & 0x80) {
        return (pteAddr & p_mask) + (address & ~(~0ull << 21));
    }

    status = ReadPhysicalAddress((pteAddr & p_mask) + 8 * pte, &address, sizeof(address), &readsize);
    if (!NT_SUCCESS(status)) {
        savedptr = pml4::dirbase_from_base_address((void*)savedbaseadd);
        return 0;
    }

    address &= p_mask;
    return address ? (address + virt_addr) : 0;
}

NTSTATUS memory_kernel_memory(HANDLE pid, uint64_t Address, PVOID AllocatedBuffer, SIZE_T size, bool WRITE) {
    NTSTATUS NtRet = STATUS_UNSUCCESSFUL;

    if (!pid || !Address || !size) {
        savedptr = pml4::dirbase_from_base_address((void*)savedbaseadd); 
        return NtRet;
    }

    if (!cr3_check && !pid_save) {
        savedbaseadd = handle_get_base_address((int)pid);
        pml4::InitializeMmPfnDatabase();
        savedptr = pml4::dirbase_from_base_address((void*)savedbaseadd);
        pid_save = pid;
        cr3_check = true;
    }
    else if (cr3_check && pid_save != pid) {
        savedbaseadd = handle_get_base_address((int)pid);
        pml4::InitializeMmPfnDatabase();
        savedptr = pml4::dirbase_from_base_address((void*)savedbaseadd);
        pid_save = pid;
    }

    if (!savedptr) {
        return NtRet;
    }

    SIZE_T CurOffset = 0;
    SIZE_T TotalSize = size;
    while (TotalSize > 0) {
        uint64_t CurPhysAddr = translate_linear(savedptr, (ULONG64)Address + CurOffset);
        if (!CurPhysAddr) {
            savedptr = pml4::dirbase_from_base_address((void*)savedbaseadd);
            return STATUS_UNSUCCESSFUL;
        }

        ULONG64 ReadSize = min(PAGE_SIZE - (CurPhysAddr & 0xFFF), TotalSize);

        if (ReadSize == 0) {
            savedptr = pml4::dirbase_from_base_address((void*)savedbaseadd);
            return STATUS_UNSUCCESSFUL;
        }

        SIZE_T BytesRead = 0;
        if (WRITE) {
            NtRet = WritePhysicalAddress((PVOID)CurPhysAddr, (PVOID)((ULONG64)AllocatedBuffer + CurOffset), ReadSize, &BytesRead);
        }
        else {
            NtRet = ReadPhysicalAddress(CurPhysAddr, (PVOID)((ULONG64)AllocatedBuffer + CurOffset), ReadSize, &BytesRead);
        }

        if (NtRet != STATUS_SUCCESS || BytesRead == 0) {
            savedptr = pml4::dirbase_from_base_address((void*)savedbaseadd);
            return STATUS_UNSUCCESSFUL;
        }

        TotalSize -= BytesRead;
        CurOffset += BytesRead;
    }
    return NtRet;
}


uintptr_t get_module_base_x64(int processpid, PEPROCESS peprocess, UNICODE_STRING module_name)
{
    wchar_t* wStr = module_name.Buffer;
    NTSTATUS status;
    PROCESS_BASIC_INFORMATION ProcInfo;
    ULONG Length = 0;
    HANDLE hProcess;
    status = ObOpenObjectByPointer(peprocess, 0, NULL, 0, 0, KernelMode, &hProcess);
    if (!NT_SUCCESS(status)) return NULL;
    if (!NT_SUCCESS(ZwQueryInformationProcess(hProcess, ProcessBasicInformation, &ProcInfo, sizeof(PROCESS_BASIC_INFORMATION), &Length))) return NULL;
    ZwClose(hProcess);
    PEB procPeb = { 0 };
    status = memory_kernel_memory((HANDLE)processpid, (uint64_t)ProcInfo.PebBaseAddress, &procPeb, sizeof(PEB), false);
    if (!NT_SUCCESS(status)) return NULL;
    PEB_LDR_DATA1 peb_ldr_data = { 0 };
    status = memory_kernel_memory((HANDLE)processpid, (uint64_t)procPeb.Ldr, &peb_ldr_data, sizeof(PEB_LDR_DATA1), false);
    if (!NT_SUCCESS(status)) return NULL;
    LIST_ENTRY* ldr_list_head = (LIST_ENTRY*)peb_ldr_data.InLoadOrderModuleList.Flink;
    LIST_ENTRY* ldr_current_node = peb_ldr_data.InLoadOrderModuleList.Flink;
    do
    {
        LDR_DATA_TABLE_ENTRY1 list_entry = { 0 };
        status = memory_kernel_memory((HANDLE)processpid, (uint64_t)ldr_current_node, &list_entry, sizeof(LDR_DATA_TABLE_ENTRY1), false);
        if (!NT_SUCCESS(status)) return NULL;
        ldr_current_node = list_entry.InLoadOrderLinks.Flink;
        if (list_entry.BaseDllName.Length > 0)
        {
            wchar_t base_name[260] = { 0 };
            status = memory_kernel_memory((HANDLE)processpid, (uint64_t)list_entry.BaseDllName.Buffer, &base_name, list_entry.BaseDllName.Length, false);
            if (NT_SUCCESS(status)) if (!wcscmp(wStr, base_name)) if (list_entry.SizeOfImage != 0) return (uintptr_t)list_entry.DllBase;
        }

    } while (ldr_list_head != ldr_current_node);
    return NULL;
}

uintptr_t get_module_adress_x64(int processpid, PCSZ module_name)
{
    ANSI_STRING stringsa1;
    UNICODE_STRING Module_name;
    RtlInitAnsiString(&stringsa1, module_name);
    RtlAnsiStringToUnicodeString(&Module_name, &stringsa1, TRUE);
    PEPROCESS process = NULL;
    NTSTATUS status = PsLookupProcessByProcessId((HANDLE)processpid, &process);
    if (NT_SUCCESS(status)) {
        ULONG64 moduleaddr = get_module_base_x64(processpid, process, Module_name);
        return moduleaddr;
    }
    else {
        return 0;
    }
    RtlFreeUnicodeString(&Module_name);
}
