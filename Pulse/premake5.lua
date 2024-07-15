project "Pulse"
	kind "StaticLib"
	language "C++"
	cppdialect "C++latest"
	staticruntime "On"

	architecture "x86_64"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

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