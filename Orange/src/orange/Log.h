#pragma once

#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Orange {

	class ORANGE_API Log {

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}


// Macros for logging

// Core log macros
#define OG_CORE_TRACE(...) ::Orange::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define OG_CORE_INFO(...) ::Orange::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OG_CORE_WARN(...) ::Orange::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OG_CORE_ERROR(...) ::Orange::Log::GetCoreLogger()->error(__VA_ARGS__)
#define OG_CORE_FATAL(...) ::Orange::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define OG_TRACE(...) ::Orange::Log::GetClientLogger()->trace(__VA_ARGS__)
#define OG_INFO(...) ::Orange::Log::GetClientLogger()->info(__VA_ARGS__)
#define OG_WARN(...) ::Orange::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OG_ERROR(...) ::Orange::Log::GetClientLogger()->error(__VA_ARGS__)
#define OG_FATAL(...) ::Orange::Log::GetClientLogger()->fatal(__VA_ARGS__)
