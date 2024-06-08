#include "caphysicspch.h"

#include "PhysicsModule.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
)
{
    if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	    Catalyst::PhysicsModule* module = new Catalyst::PhysicsModule;

    return TRUE;
}
