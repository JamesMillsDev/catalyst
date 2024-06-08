#include "cainputpch.h"

#include <iostream>

#include "InputModule.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	    Catalyst::InputModule* module = new Catalyst::InputModule;

    return TRUE;
}
