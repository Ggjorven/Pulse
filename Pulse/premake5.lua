project "Pulse"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"

	architecture "x86_64"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	-- Note: VS2022/Make only need the pchheader filename
	pchheader "plpch.h"
	pchsource "src/Pulse/plpch.cpp"

	files
	{
		"src/Pulse/**.h",
		"src/Pulse/**.hpp",
		"src/Pulse/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"src",
		"src/Pulse"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "on"

	filter "system:linux"
		systemversion "latest"
		staticruntime "on"

	filter "system:macosx"
		systemversion "14.5"
		staticruntime "on"

		-- Note: XCode only needs the full pchheader path
		pchheader "src/Pulse/plpch.h"

		-- Note: If we don't add the header files to the externalincludedirs
		-- we can't use <angled> brackets to include files.
		externalincludedirs
		{
			"src",
			"src/Pulse"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "APP_DIST"
		runtime "Release"
		optimize "Full"
