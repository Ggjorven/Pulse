#pragma once

#include "Pulse/Core/Core.hpp"
#include "Pulse/Core/Defines.hpp"

#include "Pulse/Reflection/Reflective.hpp"

#include <filesystem>

#if defined(PULSE_PLATFORM_WINDOWS)
#include <Windows.h>
// TODO: Additional platforms
#endif

namespace Pulse
{

	class Dll
	{
	public:
		Dll(const std::filesystem::path& path);
		~Dll();

		void Reload();
		void Reload(const std::filesystem::path& path);

		template <typename TFunc>
		TFunc GetCustomFunction(const std::string& cFuncName);

	private:
		std::filesystem::path m_Path;

#if defined(PULSE_PLATFORM_WINDOWS)
		HMODULE m_Handle = {};
// TODO: Additional platforms
#endif
	};

	template <typename TFunc>
	inline TFunc Dll::GetCustomFunction(const std::string& cFuncName)
	{
		#if defined(PULSE_PLATFORM_WINDOWS)
			return (TFunc)GetProcAddress(m_Handle, cFuncName.c_str());
		// TODO: Additional platforms
		#endif
	}


}