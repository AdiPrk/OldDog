#pragma once

#include "Dog/core.h"

namespace Dog {
	class DOG_API Logger
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define DOG_CORE_TRACE(...)    ::Dog::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define DOG_CORE_INFO(...)     ::Dog::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define DOG_CORE_WARN(...)     ::Dog::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define DOG_CORE_ERROR(...)    ::Dog::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define DOG_CORE_CRITICAL(...) ::Dog::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define DOG_TRACE(...)         ::Dog::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define DOG_INFO(...)          ::Dog::Logger::GetClientLogger()->info(__VA_ARGS__)
#define DOG_WARN(...)          ::Dog::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define DOG_ERROR(...)         ::Dog::Logger::GetClientLogger()->error(__VA_ARGS__)
#define DOG_CRITICAL(...)      ::Dog::Logger::GetClientLogger()->critical(__VA_ARGS__)