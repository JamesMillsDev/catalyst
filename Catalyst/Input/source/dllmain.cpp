#include "cainputpch.h"

#include <iostream>

#include "InputModule.h"

namespace Catalyst
{
	void MakeInputModuleInstance()
	{
		InputModule* module = new InputModule;
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		Catalyst::MakeInputModuleInstance();

	return TRUE;
}
