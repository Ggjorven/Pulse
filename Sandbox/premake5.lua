project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "off"
	
	architecture "x86_64"
	
	-- debugdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}") 
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp",

		"vendor/**.h",
		"vendor/**.hpp",
		"vendor/**.cpp"
	}

	includedirs
	{
		"src",
		"%{wks.location}/vendor",

		"%{wks.location}/Pulse/src"
	}

	links
	{
		"Pulse"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "on"

	filter "system:linux"
		systemversion "latest"
		staticruntime "on"

	filter "system:macosx"
		systemversion "latest"
		staticruntime "on"

		-- Note: If we don't add the header files to the sysincluddirs
		-- we can't use <angled> brackets to include files.
		sysincludedirs
		{
			"src",
			"%{wks.location}/vendor",
	
			"%{wks.location}/Pulse/src"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "APP_RELEASE"
		runtime "Release"
		optimize "on"