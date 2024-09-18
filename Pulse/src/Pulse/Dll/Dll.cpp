#include "plpch.h"
#include "Dll.hpp"

#include "Pulse/Core/Logging.hpp"

namespace Pulse
{

	// Anonymous namespace to ensure it can't be accessed through extern
	namespace
	{
		static std::filesystem::path GetCopyPath(const std::filesystem::path& path)
		{
			return path.parent_path() / "Dll-Copy" / path.filename();
		}

		static std::filesystem::path CopyOverDll(const std::filesystem::path& path)
		{
			std::filesystem::path oldDllPath = path;
			std::filesystem::path newDllPath = GetCopyPath(path);

			// Only relevant if the PDB file is found.
			std::filesystem::path oldPdbPath = std::filesystem::path(path).replace_extension(".pdb");
			std::filesystem::path newPdbPath = GetCopyPath(path).replace_extension(".pdb");

			std::filesystem::create_directory(newDllPath.parent_path());

			std::filesystem::copy_file(oldDllPath, newDllPath, std::filesystem::copy_options::overwrite_existing);

			// Only copy if the PDB file is found.
			if (std::filesystem::exists(oldPdbPath))
				std::filesystem::copy_file(oldPdbPath, newPdbPath, std::filesystem::copy_options::overwrite_existing);

			if (!std::filesystem::exists(oldPdbPath) && !std::filesystem::exists(newPdbPath))
				Logger::Log(LogLevel::Warn, "Failed to find PDB file for script ({0}), this is not critical. It only means debugging the script is not possible.", path.string());

			// Note: We delete the old one, since the debugger likes to use that one instead of the copied one and then we can't change the .pdb file while the app is running a.k.a we have to detach before we can compile our script.
			std::filesystem::remove(oldPdbPath);

			return newDllPath;
		}
	}

	Dll::Dll(const std::filesystem::path& path)
		: m_Path(path)
	{
		Reload(path);
	}

	Dll::~Dll()
	{
		#if defined(PULSE_PLATFORM_WINDOWS)
		if (m_Handle)
			FreeLibrary(m_Handle);
		#elif defined(PULSE_PLATFORM_LINUX)
        if (m_Handle)
			dlclose(m_Handle);
		// TODO: Additional platforms
		#endif
	}

	void Dll::Reload()
	{
		Reload(m_Path);
	}

	void Dll::Reload(const std::filesystem::path& path)
	{
		Logger::Assert(std::filesystem::exists(path), "Path doesn't exist.");

		#if defined(PULSE_PLATFORM_WINDOWS)
		if (m_Handle)
			FreeLibrary(m_Handle);

		m_Handle = LoadLibraryA(CopyOverDll(m_Path).string().c_str());
		Logger::Assert((bool)m_Handle, "Failed to load .dll from path: '{0}'", m_Path.string());

        #elif defined(PULSE_PLATFORM_LINUX)
		if (m_Handle)
			dlclose(m_Handle);

		m_Handle = dlopen(CopyOverDll(m_Path).string().c_str(), RTLD_LAZY);
        Logger::Assert((bool)m_Handle, "Failed to load .so from path: '{0}'", m_Path.string());
		// TODO: Additional platforms
		#endif

	}

}
