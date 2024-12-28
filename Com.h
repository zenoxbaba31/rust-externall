#pragma once
#include "plugins.h"
//#define phyread_memory CTL_CODE(FILE_DEVICE_UNKNOWN, 0x193286, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
//#define phywrite_memory CTL_CODE(FILE_DEVICE_UNKNOWN, 0x829823, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define phy_memory CTL_CODE(FILE_DEVICE_UNKNOWN, 0x329823, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define get_module_base CTL_CODE(FILE_DEVICE_UNKNOWN, 0x561419, METHOD_BUFFERED, FILE_SPECIAL_ACCESS) 
#define get_base_address CTL_CODE(FILE_DEVICE_UNKNOWN, 0x431469, METHOD_BUFFERED, FILE_SPECIAL_ACCESS) 
#define get_cr3 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x421469, METHOD_BUFFERED, FILE_SPECIAL_ACCESS) 

typedef struct _k_get_base_module_request {
    ULONG pid;
    ULONGLONG handle;
    const char* name;
} k_get_base_module_request, * pk_get_base_module_request;

typedef struct _k_get_base_address_request {
    ULONG pid;
    ULONGLONG baseaddress;
} k_get_base_address_request, * pk_get_base_address_request;

typedef struct _dtb_invoke
{
    uint32_t pid;
} dtb_invoke, * pdtb_invoke;

typedef struct _k_rw_memoryrequest {
    ULONG pid;
    UINT_PTR address;
    ULONGLONG size;
    void* output;
    bool WRITE;
} k_rw_memoryrequest, * pk_rw_memoryrequest;

//typedef struct _k_rw_readrequest {
//    ULONG pid;
//    UINT_PTR address;
//    ULONGLONG size;
//    void* output;
//} k_rw_readrequest, * pk_rw_readrequest;
//
//typedef struct _k_rw_writerequest {
//    ULONG pid;
//    void* output;
//    UINT_PTR address;
//    ULONGLONG size;
//} k_rw_writerequest, * pk_rw_writerequest;