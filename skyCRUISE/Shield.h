#pragma once
#include <Windows.h>
#include "offsets.h"
#include "UGameData.h"

typedef __int64(WINAPI* fnShield_Decrypt)(__int64, __int64);

fnShield_Decrypt pShield_Decrypt;
BYTE Decrypt_Function_container[0x1000];
DWORD64 decrypt_start_rax;


BOOL Shield_Initialize(uint64_t ImageBase)
{
    DWORD dwOld;
    VirtualProtect(Decrypt_Function_container, 0x1000, PAGE_EXECUTE_READWRITE, &dwOld);
    DWORD64 P = mem->RVM<uint64_t>(ImageBase + __OFFSET__Decrypt);

    KernelDriver::DriverReadMemory(P, Decrypt_Function_container, 0x1000);

    decrypt_start_rax = *(PDWORD)(Decrypt_Function_container + 0x3) + P + 0x7;

    pShield_Decrypt = (fnShield_Decrypt)(Decrypt_Function_container + 0x7);

    return TRUE;
}

__declspec(noinline)
uint64_t getShieldStart()
{
    return decrypt_start_rax;
}

__int64 Decrypt_Shield(__int64 value, __int64 temp = 0)
{
    if (temp == 0)
        temp = getShieldStart();
    return pShield_Decrypt(temp, value);
}
