#pragma once

#include <memory>


#ifdef OG_PLATFORM_WINDOWS
#if OG_DYNAMIC_LINK
#ifdef OG_BUILD_DLL
#define ORANGE_API __declspec(dllexport)
#else
#define ORANGE_API __declspec(dllimport)
#endif
#else
#define ORANGE_API
#endif
#else
#error Orange only supports Windows!
#endif


#ifdef OG_DEBUG
#define OG_ENABLE_ASSERTS
#endif


#ifdef OG_ENABLE_ASSERTS
#define OG_ASSERT(x, ...) { if(!(x)) { OG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define OG_CORE_ASSERT(x, ...) { if(!(x)) { OG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define OG_ASSERT(x, ...)
#define OG_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)


#define OG_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)


namespace Orange {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
