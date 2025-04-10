#include "caphysicspch.h"

#include "PhysicsModule.h"

#if _WIN32
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        Catalyst::PhysicsModule* module = new Catalyst::PhysicsModule;
    }

    return TRUE;
}
#else
#include <stdio.h>

__attribute__((constructor)) void onLibraryLoad()
{
    Catalyst::PhysicsModule* module = new Catalyst::PhysicsModule;
}
#endif