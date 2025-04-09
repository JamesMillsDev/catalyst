#include "camathspch.h"

#include <iostream>

#include "MathsModule.h"

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		Catalyst::MathsModule* module = new Catalyst::MathsModule;

	return TRUE;
}
