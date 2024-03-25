#pragma once

#ifdef DLL_EXPORT
#define CORE_DLL __declspec(dllexport)
#else
#define CORE_DLL __declspec(dllimport)
#endif