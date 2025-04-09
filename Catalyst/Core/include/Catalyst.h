#pragma once

#if DLL_EXPORT
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

#include <assert.h>
#include <concepts>
#include <type_traits>

template<class T, class U>
concept derived = std::is_base_of_v<U, T>;

typedef unsigned int uint;
typedef unsigned char ubyte;

// Macro to enable bitwise operators for strongly-typed enums (enum class)
#define ENABLE_ENUM_FLAGS(ENUM_TYPE) \
	inline constexpr ENUM_TYPE operator|(ENUM_TYPE lhs, ENUM_TYPE rhs) { \
		return static_cast<ENUM_TYPE>( \
			static_cast<std::underlying_type_t<ENUM_TYPE>>(lhs) | \
			static_cast<std::underlying_type_t<ENUM_TYPE>>(rhs) \
		); \
	} \
	inline constexpr ENUM_TYPE operator&(ENUM_TYPE lhs, ENUM_TYPE rhs) { \
		return static_cast<ENUM_TYPE>( \
			static_cast<std::underlying_type_t<ENUM_TYPE>>(lhs) & \
			static_cast<std::underlying_type_t<ENUM_TYPE>>(rhs) \
		); \
	} \
	inline constexpr ENUM_TYPE operator^(ENUM_TYPE lhs, ENUM_TYPE rhs) { \
		return static_cast<ENUM_TYPE>( \
			static_cast<std::underlying_type_t<ENUM_TYPE>>(lhs) ^ \
			static_cast<std::underlying_type_t<ENUM_TYPE>>(rhs) \
		); \
	} \
	inline constexpr ENUM_TYPE operator~(ENUM_TYPE lhs) { \
		return static_cast<ENUM_TYPE>( \
			~static_cast<std::underlying_type_t<ENUM_TYPE>>(lhs) \
		); \
	} \
	inline ENUM_TYPE& operator|=(ENUM_TYPE& lhs, ENUM_TYPE rhs) { \
		lhs = lhs | rhs; \
		return lhs; \
	} \
	inline ENUM_TYPE& operator&=(ENUM_TYPE& lhs, ENUM_TYPE rhs) { \
		lhs = lhs & rhs; \
		return lhs; \
	} \
	inline ENUM_TYPE& operator^=(ENUM_TYPE& lhs, ENUM_TYPE rhs) { \
		lhs = lhs ^ rhs; \
		return lhs; \
	}

namespace Catalyst
{
	extern void MakeApplicationInstance();
}