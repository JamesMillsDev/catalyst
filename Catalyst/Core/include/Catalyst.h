#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#if DLL_EXPORT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

#include <assert.h>
#include <concepts>

template<class T, class U>
concept derived = std::is_base_of_v<U, T>;

typedef unsigned int uint;
typedef unsigned char ubyte;

namespace Catalyst
{
	extern void MakeApplicationInstance();
}