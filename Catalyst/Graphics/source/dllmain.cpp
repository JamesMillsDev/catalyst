#include "cagraphicspch.h"

#include <iostream>

#include "GraphicsModule.h"

#if _WIN32
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        Catalyst::GraphicsModule* module = new Catalyst::GraphicsModule;
    }

    return TRUE;
}
#else
#include <stdio.h>

__attribute__((constructor)) void onLibraryLoad()
{
    Catalyst::GraphicsModule* module = new Catalyst::GraphicsModule;
}
#endif