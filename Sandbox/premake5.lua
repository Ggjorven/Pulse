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

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "APP_RELEASE"
		runtime "Release"
		optimize "on"