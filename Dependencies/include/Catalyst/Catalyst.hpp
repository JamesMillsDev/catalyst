#pragma once

#ifdef DLL_EXPORT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

#include <concepts>

template<class T, class U>
concept Derived = std::is_base_of<U, T>::value;

typedef uint8_t ubyte;

typedef uint16_t ushort16;
typedef int16_t short16;

typedef uint32_t uint32;
typedef int32_t int32;