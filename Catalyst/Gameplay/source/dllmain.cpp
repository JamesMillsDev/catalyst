#include "cagameplaypch.h"

#include <iostream>

#include "GameplayModule.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	    Catalyst::GameplayModule* module = new Catalyst::GameplayModule;

    return TRUE;
}
