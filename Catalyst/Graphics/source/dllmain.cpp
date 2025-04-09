#include "cagraphicspch.h"

#include <iostream>

#include "GraphicsModule.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	    Catalyst::GraphicsModule* module = new Catalyst::GraphicsModule;

    return TRUE;
}
