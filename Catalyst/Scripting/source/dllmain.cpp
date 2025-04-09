#include "cascriptingpch.h"

#include "ScriptingModule.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
)
{
    if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	    Catalyst::ScriptingModule* module = new Catalyst::ScriptingModule;

    return TRUE;
}
